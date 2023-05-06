
#pragma once

#include <arpa/inet.h>
#include <asm-generic/int-ll64.h>
#include <iostream>
#include <liburing.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

enum class RequestType : unsigned char {
  Accept = 0,
  Read,
  Write,
  Close,
};

union IORequest { // align to 64 bits, the tricky to avoid memory allocation
  struct {
    int fd;           // 4 bytes
    RequestType type; // 1 byte
    int buff_id : 24; // 3 bytes
  } fields;
  __u64 raw;
};
static_assert(sizeof(IORequest) == sizeof(__u64),
              "sizeof(IORequest) == sizeof(__u64) failed");

class TcpServer {
public:
  TcpServer(unsigned short port) : port_(port) {
    sock_ = socket(AF_INET, SOCK_STREAM, 0);
  }
  TcpServer(const TcpServer &) = delete;
  TcpServer(TcpServer &&) = delete;
  ~TcpServer() {
    close(sock_);
    io_uring_queue_exit(&ring_);
  }

  int sock() const noexcept { return sock_; }

  int StartListen() noexcept;

  // io_uring
  void AddAcceptRequest();
  void AddReadRequest();
  void AddWriteRequest();
  int Submit();

private:
  unsigned short port_{0};
  int sock_{-1};
  struct io_uring ring_;
};
