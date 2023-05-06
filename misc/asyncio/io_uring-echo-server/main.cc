
#include "tcp_server.h"
#include <cassert>
#include <iostream>
#include <thread>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cout << "Usage: " << argv[0] << " [port]" << std::endl;
    return -1;
  }
  auto port = std::strtol(argv[1], NULL, 10);

  auto tcpServer = TcpServer();
  auto ret = tcpServer.StartListen(static_cast<uint16_t>(port));
  if (ret != 0) {
    return -1;
  }

  // initialize for first accept
  tcpServer.AddWaitForCloseRequest();
  tcpServer.AddAcceptRequest();
  tcpServer.Submit();

  std::thread t([&tcpServer] {
    tcpServer.EchoLoop(); // until quit or error
  });

  while (true) {
    std::cout << "press 'quit' to exit..." << std::endl;
    std::string s;
    std::cin >> s;
    if (s == "quit") {
      break;
    }
  }

  tcpServer.Stop();
  t.join();
  return 0;
}
