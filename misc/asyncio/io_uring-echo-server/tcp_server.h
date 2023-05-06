
#pragma once

#include <arpa/inet.h>
#include <asm-generic/int-ll64.h>
#include <atomic>
#include <cassert>
#include <iostream>
#include <liburing.h>
#include <memory>
#include <netinet/in.h>
#include <stdint.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

constexpr int kMaxReadLen = 2048;

struct Request {
  enum class Type {
    Accept = 0,
    Read,
    Write,
    Close,
  };

  // mandantory
  Type type;
  int fd{-1};

  // optional
  unsigned char *buffer{nullptr};
  std::vector<unsigned char> buffer_storage;
};

class TcpServer final {
public:
  TcpServer() {
    listen_sock_ = socket(AF_INET, SOCK_STREAM, 0);
    io_uring_queue_init(100, &ring_, 0);

    pipe2(pipe_fds_, 0);
  }
  TcpServer(const TcpServer &) = delete;
  TcpServer(TcpServer &&) = delete;
  ~TcpServer() {
    close(listen_sock_);
    io_uring_queue_exit(&ring_); // TODO: need to free all requests first to avoid leak

    close(pipe_fds_[0]);
    close(pipe_fds_[1]);
  }

  int StartListen(uint16_t port) noexcept;

  void EchoLoopForever() noexcept;
  void QuitEchoLoop() noexcept;

public:
  // io_uring
  void AddWaitForCloseRequest() noexcept; // wait for pipe readable
  void AddAcceptRequest() noexcept;
  void AddReadRequest(int sockfd, int max_read_len) noexcept;
  void AddWriteRequest(int sockfd, unsigned char *buff, int len) noexcept;
  int Submit() noexcept;

private:
  struct io_uring ring_;
  int listen_sock_;
  int pipe_fds_[2]; // pipe_fds_[0] readable means expect to close

  struct sockaddr_in peer_addr_;
  socklen_t peer_addr_len_{sizeof(peer_addr_)};
};
