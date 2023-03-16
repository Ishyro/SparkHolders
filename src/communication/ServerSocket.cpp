#include "communication/ServerSocket.h"

ServerSocket::ServerSocket(int port, int maxPlayers, bool local) {
  struct sockaddr_in sin;
  if((socketfd = socket(AF_INET,SOCK_STREAM,0)) < 0) {
    perror("socket");
    exit(EXIT_FAILURE);
  }
  if(local) {
    sin.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
  }
  else {
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
  }
  sin.sin_port = htons(port);
  sin.sin_family = AF_INET;

  if(bind(socketfd,(struct sockaddr *)& sin,sizeof(sin)) < 0) {
    perror("bind");
    exit(EXIT_FAILURE);
  }
  listen(socketfd, maxPlayers);
}

Socket ServerSocket::accept() {
  int fd;
  struct sockaddr_in addr;;
  unsigned int len = (sizeof(addr));
  if((fd = ::accept(socketfd, (struct sockaddr *)& addr, &len)) == -1) {
    perror("accept");
    exit(EXIT_FAILURE);
  }
  return Socket(fd);
}

void ServerSocket::close() {
  if(isOpen()) {
    shutdown(socketfd,2);
    ::close(socketfd);
  }
}
