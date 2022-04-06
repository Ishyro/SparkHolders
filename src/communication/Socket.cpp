#include "communication/Socket.h"

Socket::Socket(){
  if((fd = socket(AF_INET,SOCK_STREAM,0)) == -1) {
    perror("socket");
    exit(1);
  }
}

// tente de se connecter à l'hôte fourni
void Socket::connect(const std::string & host, int port) {
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  inet_aton(host.c_str(), &addr.sin_addr);
  if (::connect(fd, (struct sockaddr *) &addr, sizeof(addr)) == -1) {
    perror("connect");
    exit(EXIT_FAILURE);
  }
}

void Socket::connect(in_addr ipv4, int port) {
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr = ipv4;
  if (::connect(fd, (struct sockaddr *) &addr, sizeof(addr)) == -1) {
    perror("connect");
    exit(EXIT_FAILURE);
  }
}

std::string Socket::read() {
  char delimiter = '%';
  char * msg = (char *) malloc(sizeof(char) * 1024);
  if(::read(fd, (void *) msg, (ssize_t) 1024) != 0) {
    std::string result = std::string(msg);
    return result.substr(0, result.find(delimiter));
  }
  return "CLOSED";
}

void Socket::write(std::string msg) {
  std::string tosend = msg + "%";
  if(::write(fd, (void *) tosend.c_str(), (ssize_t) 1024) == -1) {
    perror("connect");
    exit(EXIT_FAILURE);
  }
}

void Socket::close() {
  if(isOpen()) {
    shutdown(fd,2);
    ::close(fd);
  }
}
