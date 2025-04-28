#include "communication/Socket.h"

#ifdef _WIN32_WINNT
  #pragma comment(lib, "Ws2_32.lib")
#endif

Socket::Socket(){
  if((fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
    #ifndef _WIN32_WINNT
      perror("socket");
    #else
      std::cerr << "socket: " << WSAGetLastError() << std::endl;
    #endif
    exit(1);
  }
  #ifdef _WIN32_WINNT
		WSADATA d;
		if (WSAStartup(MAKEWORD(2, 2), &d)) {
	  	std::cerr << "Failed to initialize." << std::endl;
		}
	#endif
}

// tente de se connecter à l'hôte fourni
void Socket::connect(const std::string & host, int32_t port) {
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  inet_pton(addr.sin_family, host.c_str(), &addr.sin_addr.s_addr);
  if (::connect(fd, (struct sockaddr *) &addr, sizeof(addr)) == -1) {
    #ifndef _WIN32_WINNT
      perror("connect");
    #else
      std::cerr << "connect: " << WSAGetLastError() << std::endl;
    #endif
    exit(EXIT_FAILURE);
  }
}

void Socket::connect(in_addr ipv4, int32_t port) {
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

void Socket::write(std::string msg) {
  std::string length = std::to_string(msg.length());
  do {
    #ifndef _WIN32_WINNT
      int32_t sent = send(fd, (void *) length.c_str(), (ssize_t) length.length() + 1, 0);
    #else
      int32_t sent = send(fd, length.c_str(), (ssize_t) length.length() + 1, 0);
    #endif
    if (sent == -1) {
        std::cerr << "Error number: " << errno << std::endl;
        close();
        throw CloseException();
      }
    // always unsigned or -1
    if(length.length() > (unsigned int32_t) sent) {
      length = length.substr(sent - 1, length.length());
    }
    else {
      length = "";
    }
  } while(length != "");
  char * end_number = new char [1];
  end_number[0] = '$';
  #ifndef _WIN32_WINNT
    if(send(fd, (void *) end_number, (ssize_t) 1, 0) == -1)
  #else
    if(send(fd, end_number, (ssize_t) 1, 0) == -1)
  #endif
  {
    std::cerr << "Error number: " << errno << std::endl;
    close();
    throw CloseException();
  }
  delete end_number;
  do {
    std::string part;
    if(msg.length() > SOCKET_MESSAGE_SIZE - 1) {
      part = msg.substr(0, SOCKET_MESSAGE_SIZE - 1);
    }
    else {
      part = msg;
      msg = "";
    }
    int32_t size = std::min(SOCKET_MESSAGE_SIZE, (int32_t) part.length() + 1);
    #ifndef _WIN32_WINNT
      int32_t sent = send(fd, (void *) part.c_str(), (ssize_t) size, 0);
    #else
      int32_t sent = send(fd, part.c_str(), (ssize_t) size, 0);
    #endif
    // it just works
    usleep(1);
    if (sent == -1) {
      std::cerr << "Error number: " << errno << std::endl;
      close();
      throw CloseException();
    }
    // Send may have not sent all the data
    if(msg.length() > (uint64_t) sent - 1) {
      msg = msg.substr(sent - 1, msg.length());
    }
  } while(msg != "");
}

std::string Socket::read() {
  std::string result = std::string("");
  char * msg = new char [SOCKET_MESSAGE_SIZE];
  char * number = new char [2];
  std::string len = std::string("");
  do {
    #ifndef _WIN32_WINNT
      if(recv(fd, (void *) number, (ssize_t) 1, 0) == -1)
    #else
      if(recv(fd, number, (ssize_t) 1, 0) == -1)
    #endif
    {
      close();
      delete msg;
      throw CloseException();
    }
    number[1] = '\0';
    len += std::string(number);
  } while(number[0] != '$');
  delete number;
  int32_t length = stoi(len.substr(0, len.length() - 1));
  int32_t left = length;
  do {
    int32_t size = std::min(SOCKET_MESSAGE_SIZE, left + 1);
    std::fill(msg, msg+SOCKET_MESSAGE_SIZE, '\0');
    #ifndef _WIN32_WINNT
      int32_t received = (int32_t) recv(fd, (void *) msg, (ssize_t) size, 0) == -1;
    #else
      int32_t received = recv(fd, msg, (ssize_t) size, 0) == -1;
    #endif
    if(received == -1) {
      close();
      delete msg;
      throw CloseException();
    }
    // it just works
    usleep(1);
    std::string part = std::string(msg);
    left -= part.length();
    result += part;
  } while(left > 0);
  delete msg;
  return result;
}

int32_t Socket::getFD() { return fd; }
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
