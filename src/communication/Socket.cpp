#include "communication/Socket.h"

#ifdef _WIN32_WINNT
  #pragma comment(lib, "Ws2_32.lib")
#endif

Socket::Socket(){
  if((fd = socket(AF_INET,SOCK_STREAM,0)) == -1) {
    #ifndef _WIN32_WINNT
      perror("socket");
    #else
      std::cout << "accept: " << WSAGetLastError() << std::endl;
    #endif
    exit(1);
  }
  #ifdef _WIN32_WINNT
		WSADATA d;
		if (WSAStartup(MAKEWORD(2, 2), &d)) {
	  	fprintf(stderr, "Failed to initialize.\n");
		}
	#endif
}

// tente de se connecter à l'hôte fourni
void Socket::connect(const std::string & host, int port) {
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  inet_pton(addr.sin_family, host.c_str(), &addr.sin_addr.s_addr);
  if (::connect(fd, (struct sockaddr *) &addr, sizeof(addr)) == -1) {
    #ifndef _WIN32_WINNT
      perror("connect");
    #else
      std::cerr << "accept: " << WSAGetLastError() << std::endl;
    #endif
    exit(EXIT_FAILURE);
  }
}

void Socket::connect(in_addr ipv4, int port) {
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr = ipv4;
  if (::connect(fd, (struct sockaddr *) &addr, sizeof(addr)) == -1) {
    #ifndef _WIN32_WINNT
      perror("connect");
    #else
      std::cerr << "connect: " << WSAGetLastError() << std::endl;
    #endif
    exit(EXIT_FAILURE);
  }
}

std::string Socket::read() {
  std::string result = std::string("");
  while(true) {
    char * msg = new char [1024];
    if(recv(fd, (void *) msg, (ssize_t) 1024, 0) == 1) {
      close();
      throw CloseException();
    }
    else {
      std::string part = std::string(msg);
      delete msg;
      if(part.find(delimiter) != std::string::npos) {
        result += part.substr(0, part.find(delimiter));
      }
      else {
        return result += part.substr(0, part.find(final_delimiter));
      }
    }
  }
}

void Socket::write(std::string msg) {
  do {
    std::string tosend;
    if(msg.length() > 1023) {
      tosend = msg.substr(0, 1023) + delimiter;
      msg = msg.substr(1023, msg.length());
    }
    else {
      tosend = msg + final_delimiter;
      msg = "";
    }
    if(send(fd, (void *) tosend.c_str(), (ssize_t) 1024, 0) == -1) {
      close();
      throw CloseException();
    }
  } while(msg != "");
}

int Socket::getFD() { return fd; }
bool Socket::isOpen() const { return fd != -1; }

void Socket::close() {
  if(isOpen()) {
    shutdown(fd,2);
		#ifdef _WIN32_WINNT
      ::closesocket(fd);
		#else
      ::close(fd);
		#endif
  }
}
