#include "communication/Socket.h"

#include "util/Logger.h"

#include <cerrno>
#include <cstring>

#ifdef LOG
void Socket::connection(const std::string host, int32_t port) {
  if((fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
    #ifndef _WIN32_WINNT
      LOGGER_FATAL("Socket creation: " + std::to_string(errno) + std::string(" - ") + std::string(std::strerror(errno)));
      log << "socket: " << errno << std::endl;
    #else
      LOGGER_FATAL("Socket creation: " + std::to_string(WSAGetLastError()));
      log << "socket: " << WSAGetLastError() << std::endl;
    #endif
    close();
    exit(EXIT_FAILURE);
  }
  sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  inet_pton(addr.sin_family, host.c_str(), &addr.sin_addr.s_addr);
  if(connect(fd, (sockaddr *) &addr, sizeof(addr)) == -1) {
    #ifndef _WIN32_WINNT
      LOGGER_FATAL("Socket connect: " + std::to_string(errno) + std::string(" - ") + std::string(std::strerror(errno)));
      log << "connect: " << errno << std::endl;
    #else
      LOGGER_FATAL("Socket connect: " + std::to_string(WSAGetLastError()));
      log << "connect: " << WSAGetLastError() << std::endl;
    #endif
    close();
    throw CloseException();
  }
}

void Socket::write(std::string msg) {
  log << "write(" << msg << ")" << std::endl;
  std::string length = std::to_string(msg.length());
  log << "length: " << length << std::endl;
  do {
    #ifndef _WIN32_WINNT
      int32_t sent = send(fd, (void *) length.c_str(), (ssize_t) length.length() + 1, 0);
    #else
      int32_t sent = send(fd, length.c_str(), (ssize_t) length.length() + 1, 0);
    #endif
    if (sent == -1) {
        log << "Error number: " << errno << std::endl;
        LOGGER_FATAL("Socket write: " + std::to_string(errno) + std::string(" - ") + std::string(std::strerror(errno)));
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
    log << "Error number: " << errno << std::endl;
    LOGGER_FATAL("Socket write: " + std::to_string(errno) + std::string(" - ") + std::string(std::strerror(errno)));
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
    log << "msg_part: " << part << std::endl;
    int32_t size = std::min(SOCKET_MESSAGE_SIZE, (int32_t) part.length() + 1);
    #ifndef _WIN32_WINNT
      int32_t sent = send(fd, (void *) part.c_str(), (ssize_t) size, 0);
    #else
      int32_t sent = send(fd, part.c_str(), (ssize_t) size, 0);
    #endif
    if (sent == -1) {
      log << "Error number: " << errno << std::endl;
      LOGGER_FATAL("Socket write: " + std::to_string(errno) + std::string(" - ") + std::string(std::strerror(errno)));
      close();
      throw CloseException();
    }
    // Send may have not sent all the data
    if(msg.length() > (uint64_t) sent - 1) {
      msg = msg.substr(sent - 1, msg.length());
    }
    // it just works
    usleep(1);
  } while(msg != "");
}

std::string Socket::read() {
  log << "read()" << std::endl;
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
      LOGGER_FATAL("Socket read: " + std::to_string(errno) + std::string(" - ") + std::string(std::strerror(errno)));
      close();
      delete msg;
      throw CloseException();
    }
    number[1] = '\0';
    len += std::string(number);
  } while(number[0] != '$');
  delete number;
  // last character is '$'
  int32_t length_left = stoi(len.substr(0, len.length() - 1));
  do {
    log << "left: " << length_left << std::endl;
    int32_t size = std::min(SOCKET_MESSAGE_SIZE, length_left + 1);
    std::fill(msg, msg+SOCKET_MESSAGE_SIZE, '\0');
    #ifndef _WIN32_WINNT
      bool received = (int32_t) recv(fd, (void *) msg, (ssize_t) size, 0) != -1;
    #else
      bool received = (int32_t) recv(fd, msg, (ssize_t) size, 0) != -1;
    #endif
    if(!received) {
      LOGGER_FATAL("Socket read: " + std::to_string(errno) + std::string(" - ") + std::string(std::strerror(errno)));
      close();
      delete msg;
      throw CloseException();
    }
    std::string part = std::string(msg);
    log << "msg_part: " << part << std::endl;
    length_left -= part.length();
    result += part;
    // it just works
    usleep(1);
  } while(length_left > 0);
  delete msg;
  return result;
}
#else
void Socket::connection(const std::string host, int32_t port) {
  if((fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
    #ifndef _WIN32_WINNT
      perror("socket");
    #else
      std::cerr << "socket: " << WSAGetLastError() << std::endl;
    #endif
    LOGGER_FATAL("Socket creation: " + std::to_string(errno) + std::string(" - ") + std::string(std::strerror(errno)));
    close();
    exit(EXIT_FAILURE);
  }
  sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  inet_pton(addr.sin_family, host.c_str(), &addr.sin_addr.s_addr);
  if(connect(fd, (sockaddr *) &addr, sizeof(addr)) == -1) {
    #ifndef _WIN32_WINNT
      perror("connect");
    #else
      std::cerr << "connect: " << WSAGetLastError() << std::endl;
    #endif
    close();
    throw CloseException();
    LOGGER_FATAL("Socket connect: " + std::to_string(errno) + std::string(" - ") + std::string(std::strerror(errno)));
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
        LOGGER_FATAL("Socket write: " + std::to_string(errno) + std::string(" - ") + std::string(std::strerror(errno)));
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
    LOGGER_FATAL("Socket write: " + std::to_string(errno) + std::string(" - ") + std::string(std::strerror(errno)));
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
    if (sent == -1) {
      LOGGER_FATAL("Socket write: " + std::to_string(errno) + std::string(" - ") + std::string(std::strerror(errno)));
      close();
      throw CloseException();
    }
    // Send may have not sent all the data
    if(msg.length() > (uint64_t) sent - 1) {
      msg = msg.substr(sent - 1, msg.length());
    }
    // it just works
    usleep(1);
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
      LOGGER_FATAL("Socket read: " + std::to_string(errno) + std::string(" - ") + std::string(std::strerror(errno)));
      close();
      delete msg;
      throw CloseException();
    }
    number[1] = '\0';
    len += std::string(number);
  } while(number[0] != '$');
  delete number;
  // last character is '$'
  int32_t length_left = stoi(len.substr(0, len.length() - 1));
  do {
    int32_t size = std::min(SOCKET_MESSAGE_SIZE, length_left + 1);
    std::fill(msg, msg+SOCKET_MESSAGE_SIZE, '\0');
    #ifndef _WIN32_WINNT
      bool received = (int32_t) recv(fd, (void *) msg, (ssize_t) size, 0) != -1;
    #else
      bool received = (int32_t) recv(fd, msg, (ssize_t) size, 0) != -1;
    #endif
    if(!received) {
      LOGGER_FATAL("Socket read: " + std::to_string(errno) + std::string(" - ") + std::string(std::strerror(errno)));
      close();
      delete msg;
      throw CloseException();
    }
    std::string part = std::string(msg);
    length_left -= part.length();
    result += part;
    // it just works
    usleep(1);
  } while(length_left > 0);
  delete msg;
  return result;
}
#endif

int32_t Socket::getFD() { return fd; }
bool Socket::isOpen() const { return fd != -1; }

void Socket::close() {
  #ifdef LOG
    log.close();
  #endif
  if(isOpen()) {
		#ifdef _WIN32_WINNT
      shutdown(fd, SD_BOTH);
      ::closesocket(fd);
		#else
      shutdown(fd, SHUT_RDWR);
      ::close(fd);
		#endif
    fd = -1;
  }
}
