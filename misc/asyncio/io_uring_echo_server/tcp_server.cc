
#include "tcp_server.h"
#include <cstdio>
#include <iostream>
#include <liburing.h>
#include <utility>

int TcpServer::StartListen(uint16_t port) noexcept {

  auto ret = 0;

  int reuse = 1;
  ret = setsockopt(listen_sock_, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(int));
  if (ret != 0) {
    std::cout << "setsockopt SO_REUSEADDR failed, err " << errno << std::endl;
    return ret;
  }

  struct sockaddr_in listen_addr;
  listen_addr.sin_family = AF_INET;
  listen_addr.sin_port = htons(port);
  listen_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  ret = bind(listen_sock_, (struct sockaddr *)&listen_addr, sizeof(listen_addr));
  if (ret != 0) {
    std::cout << "bind failed, err " << errno << std::endl;
    return ret;
  }

  ret = listen(listen_sock_, 100);
  if (ret != 0) {
    std::cout << "listen failed, err " << errno << std::endl;
    return ret;
  }
  std::cout << "tcp listening on :" << port << std::endl;

  return ret;
}

void TcpServer::Stop() noexcept { write(pipe_fds_[1], "1", 1); }

void TcpServer::AddWaitForCloseRequest() noexcept {
  struct io_uring_sqe *sqe = io_uring_get_sqe(&ring_);

  auto req = new Request{.type = Request::Type::Close, .fd = pipe_fds_[0]};
  req->buffer_storage.resize(1024);
  req->buffer = req->buffer_storage.data();

  io_uring_prep_read(sqe, pipe_fds_[0], req->buffer, req->buffer_storage.size(), 0);
  io_uring_sqe_set_data(sqe, static_cast<void *>(req));
}

void TcpServer::AddAcceptRequest() noexcept {

  struct io_uring_sqe *sqe = io_uring_get_sqe(&ring_);
  io_uring_prep_accept(sqe, listen_sock_, (struct sockaddr *)&peer_addr_, &peer_addr_len_, 0);

  auto req = new Request{.type = Request::Type::Accept, .fd = listen_sock_};
  io_uring_sqe_set_data(sqe, static_cast<void *>(req));
}

void TcpServer::AddReadRequest(int sockfd, int max_read_len) noexcept {

  struct io_uring_sqe *sqe = io_uring_get_sqe(&ring_);

  auto req = new Request{.type = Request::Type::Read, .fd = sockfd};
  req->buffer_storage.resize(max_read_len);
  req->buffer = req->buffer_storage.data();

  io_uring_prep_recv(sqe, sockfd, req->buffer, req->buffer_storage.size(), 0);
  io_uring_sqe_set_data(sqe, static_cast<void *>(req));
}

void TcpServer::AddWriteRequest(int sockfd, unsigned char *buff, int len) noexcept {
  struct io_uring_sqe *sqe = io_uring_get_sqe(&ring_);

  auto req = new Request{.type = Request::Type::Write, .fd = sockfd};
  req->buffer_storage.insert(req->buffer_storage.end(), buff, buff + len);
  req->buffer = req->buffer_storage.data();

  io_uring_prep_send(sqe, sockfd, req->buffer, req->buffer_storage.size(), 0);
  io_uring_sqe_set_data(sqe, static_cast<void *>(req));
}

int TcpServer::Submit() noexcept { return io_uring_submit(&ring_); }

void TcpServer::EchoLoop() noexcept {
  std::cout << "echo server started" << std::endl;

  while (true) {
    struct io_uring_cqe *cqe{nullptr};
    auto ret = io_uring_wait_cqe(&ring_, &cqe);
    if (ret != 0) {
      std::cout << "io_uring_wait_cqe failed, err " << ret << std::endl;
      break;
    }

    auto raw_req = static_cast<Request *>(io_uring_cqe_get_data(cqe));
    auto req = std::unique_ptr<Request>(raw_req);

    // handle response
    bool to_exit = false;
    switch (req->type) {
    case Request::Type::Close: {
      if (cqe->res <= 0) {
        std::cout << "close res " << cqe->res << std::endl;
      }
      to_exit = true;
      break;
    }
    case Request::Type::Accept: {
      if (cqe->res <= 0) {
        std::cout << "accept res " << cqe->res << std::endl;
      } else {
        auto client_sock_fd = cqe->res;
        AddReadRequest(client_sock_fd, kMaxReadLen);
      }

      AddAcceptRequest(); // trigger another accept
      Submit();
      break;
    }
    case Request::Type::Read: {
      if (cqe->res <= 0) {
        std::cout << "read res " << cqe->res << " fd " << req->fd << std::endl;
        close(req->fd);
        break;
      }

      auto read_bytes = cqe->res;
      AddWriteRequest(req->fd, req->buffer, read_bytes);
      Submit();
      break;
    }
    case Request::Type::Write: {
      if (cqe->res <= 0) {
        std::cout << "write res " << cqe->res << " fd " << req->fd << std::endl;
        close(req->fd);
        break;
      }

      AddReadRequest(req->fd, kMaxReadLen);
      Submit();
      break;
    }
    default:
      assert(0);
      break;
    }

    io_uring_cqe_seen(&ring_, cqe);

    if (to_exit) {
      break;
    }
  }

  std::cout << "echo server ended" << std::endl;
}