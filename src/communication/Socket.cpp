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
  char final_delimiter = '%';
  char delimiter = '&';
  std::string result = std::string("");
  while(true) {
    char * msg = new char [1024];
    if(::read(fd, (void *) msg, (ssize_t) 1024) != 0) {
      std::string part = std::string(msg);
      free(msg);
      if(part.find(delimiter) != std::string::npos) {
        result += part.substr(0, part.find(delimiter));
      } else {
        return result += part.substr(0, part.find(final_delimiter));
      }
    } else {
      delete msg;
      close();
      throw CloseException();
    }
  }
}

void Socket::write(std::string msg) {
  char final_delimiter = '%';
  char delimiter = '&';
  do {
    std::string tosend;
    if(msg.length() > 1023) {
      tosend = msg.substr(0, 1023) + delimiter;
      msg = msg.substr(1023, msg.length());
    } else {
      tosend = msg + final_delimiter;
      msg = "";
    }
    if(::write(fd, (void *) tosend.c_str(), (ssize_t) 1024) == -1) {
      perror("connect");
      exit(EXIT_FAILURE);
    }
  } while(msg != "");
}

int Socket::getFD() { return fd; }
bool Socket::isOpen() const { return fd != -1; }

void Socket::close() {
  if(isOpen()) {
    shutdown(fd,2);
    ::close(fd);
  }
}
