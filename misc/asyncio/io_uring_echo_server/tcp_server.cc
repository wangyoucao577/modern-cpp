
#include "tcp_server.h"
#include <liburing.h>

int TcpServer::StartListen() noexcept {
  auto ret = 0;

  int reuse = 1;
  ret =
      setsockopt(sock_, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(int));
  if (ret != 0) {
    std::cout << "setsockopt SO_REUSEADDR failed, err " << ret << std::endl;
    return ret;
  }

  struct sockaddr_in listen_addr;
  listen_addr.sin_family = AF_INET;
  listen_addr.sin_port = htons(port_);
  listen_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  ret = bind(sock_, (struct sockaddr *)&listen_addr, sizeof(listen_addr));
  if (ret != 0) {
    std::cout << "bind failed, err " << ret << std::endl;
    return ret;
  }

  ret = listen(sock_, 100);
  if (ret != 0) {
    std::cout << "listen failed, err " << ret << std::endl;
    return ret;
  }

  ret = io_uring_queue_init(100, &ring_, 0);
  if (ret != 0) {
    std::cout << "io_uring_queue_init failed, err " << ret << std::endl;
    return ret;
  }

  return ret;
}

void TcpServer::AddAcceptRequest() {

  struct io_uring_sqe *sqe = io_uring_get_sqe(&ring_);
  struct sockaddr_in peer_addr;
  socklen_t peer_addr_len = sizeof(peer_addr);
  io_uring_prep_accept(sqe, sock_, (struct sockaddr *)&peer_addr,
                       &peer_addr_len, 0);

  auto req = IORequest{.fields = {.type = RequestType::Accept}};
  io_uring_sqe_set_data(sqe, (void *)req.raw);
}

void TcpServer::AddReadRequest() {

  struct io_uring_sqe *sqe = io_uring_get_sqe(&ring_);

  io_uring_prep_read(sqe, sock_, (struct sockaddr *)&peer_addr, &peer_addr_len,
                     0);

  auto req = IORequest{.fields = {.type = RequestType::Accept}};
  io_uring_sqe_set_data(sqe, (void *)req.raw);
}