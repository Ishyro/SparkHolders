#include "communication/ServerSocket.h"

#include <cerrno>
#include <cstring>

#ifdef LOG
  #include <ostream>
  #include <fstream>
#endif

#include "util/Logger.h"

ServerSocket::ServerSocket(int32_t port, int32_t maxPlayers, bool multiplayer) {
  struct sockaddr_in sin;
  if((socketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
    #ifndef _WIN32_WINNT
      LOGGER_FATAL("ServerSocket socket: " + std::to_string(errno) + std::string(" - ") + std::string(std::strerror(errno)));
    #else
      LOGGER_FATAL("ServerSocket socket: " + std::to_string(WSAGetLastError()));
    #endif
    close();
    exit(EXIT_FAILURE);
  }
  if(multiplayer) {
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
  }
  else {
    sin.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
  }
  sin.sin_port = htons(port);
  sin.sin_family = AF_INET;

  if(bind(socketfd,(struct sockaddr *)& sin, sizeof(sin)) < 0) {
    #ifndef _WIN32_WINNT
      LOGGER_FATAL("ServerSocket bind: " + std::to_string(errno) + std::string(" - ") + std::string(std::strerror(errno)));
    #else
      LOGGER_FATAL("ServerSocket bind: " + std::to_string(WSAGetLastError()));
    #endif
    close();
    exit(EXIT_FAILURE);
  }
  listen(socketfd, maxPlayers);
}

Socket * ServerSocket::accept() {
  int32_t fd;
  struct sockaddr_in addr;;
  #ifndef _WIN32_WINNT
    uint32_t len = (sizeof(addr));
  #else
    int32_t len = (sizeof(addr));
  #endif
  if((fd = ::accept(socketfd, (struct sockaddr *)& addr, &len)) == -1) {
    #ifndef _WIN32_WINNT
      LOGGER_FATAL("ServerSocket accept: " + std::to_string(errno) + std::string(" - ") + std::string(std::strerror(errno)));
    #else
      LOGGER_FATAL("ServerSocket accept: " + std::to_string(WSAGetLastError()));
    #endif
    close();
    throw CloseException();
  }
  #ifdef LOG
    return new Socket(fd, std::string("socket_server_") + std::to_string(fd) + std::string(".log"));
  #else
    return new Socket(fd);
  #endif
}

void ServerSocket::close() {
  if(isOpen()) {
		#ifdef _WIN32_WINNT
      shutdown(socketfd, SD_BOTH);
      ::closesocket(socketfd);
		#else
      shutdown(socketfd, SHUT_RDWR);
      ::close(socketfd);
		#endif
    socketfd = -1;
  }
}
