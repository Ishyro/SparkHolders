#include "communication/ServerSocket.h"

#ifdef _WIN32_WINNT
  #pragma comment(lib, "Ws2_32.lib")
#endif

ServerSocket::ServerSocket(int32_t port, int32_t maxPlayers, bool multiplayer) {
  #ifdef _WIN32_WINNT
		WSADATA d;
	  if (WSAStartup(MAKEWORD(2, 2), &d)) {
			fprintf(stderr, "Failed to initialize.\n");
		}
	#endif
  struct sockaddr_in sin;
  if((socketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
    #ifndef _WIN32_WINNT
      perror("socket");
    #else
      std::cerr << "socket: " << WSAGetLastError() << std::endl;
    #endif
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
      perror("bind");
    #else
      std::cerr << "bind: " << WSAGetLastError() << std::endl;
    #endif
    exit(EXIT_FAILURE);
  }
  listen(socketfd, maxPlayers);
}

Socket ServerSocket::accept() {
  int32_t fd;
  struct sockaddr_in addr;;
  #ifndef _WIN32_WINNT
    uint32_t len = (sizeof(addr));
  #else
    int32_t len = (sizeof(addr));
  #endif
  if((fd = ::accept(socketfd, (struct sockaddr *)& addr, &len)) == -1) {
    #ifndef _WIN32_WINNT
      perror("accept");
    #else
      std::cerr << "accept: " << WSAGetLastError() << std::endl;
    #endif
    exit(EXIT_FAILURE);
  }
  return Socket(fd);
}

void ServerSocket::close() {
  if(isOpen()) {
    shutdown(socketfd,2);
		#ifdef _WIN32_WINNT
      ::closesocket(socketfd);
		#else
      ::close(socketfd);
		#endif
  }
}
