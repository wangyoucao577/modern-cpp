
#include "tcp_server.h"
#include <cassert>
#include <iostream>

int main() {

  auto tcpServer = TcpServer(8888);
  tcpServer.StartListen();

  while (true) {
    struct io_uring_cqe *cqe{nullptr};
    ret = io_uring_wait_cqe(&ring, &cqe);
    if (ret != 0) {
      std::cout << "io_uring_wait_cqe failed, err " << ret << std::endl;
      break;
    }

    auto req = static_cast<IORequest *>(io_uring_cqe_get_data(cqe));

    if (cqe->res < 0) {
      // handle error
      std::cout << "cqe result err " << cqe->res << std::endl;
      if (req) {
        delete req;
      }
      continue;
    }

    // handle response

    if (req) {
      delete req;
    }

    io_uring_cqe_seen(&ring, cqe);
  }

  return -1;
}