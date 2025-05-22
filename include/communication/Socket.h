#ifndef _SOCKET_H_
#define _SOCKET_H_

#include <unistd.h>

#ifndef _WIN32_WINNT
	#include <netinet/ip.h>
	#include <sys/socket.h>
	#include <arpa/inet.h>
	#include <netdb.h>
#else
	#include <winsock2.h>
	#include <ws2tcpip.h>
#endif

#ifdef LOG
  #include <ostream>
  #include <fstream>
#endif

#include <string>
#include <iosfwd>
#include <iostream>
#include <exception>

#include "Values.h"

#define SOCKET_MESSAGE_SIZE 256

class CloseException : public std::exception {

};

class Socket {
	public :
		#ifdef LOG
			std::ofstream log;
		#endif
		Socket() {}
	#ifdef LOG
		Socket(std::string log_path) { log.open(log_path); }
		Socket(int32_t arg_fd, std::string log_path):fd(arg_fd) {
			log.open(log_path);
			#ifdef _WIN32_WINNT
				WSADATA d;
				if (WSAStartup(MAKEWORD(2, 2), &d)) {
	  				std::cerr << "Failed to initialize." << std::endl;
				}
			#endif
		}
	#else
		Socket(int32_t arg_fd):fd(arg_fd) {
			#ifdef _WIN32_WINNT
				WSADATA d;
				if (WSAStartup(MAKEWORD(2, 2), &d)) {
	  				std::cerr << "Failed to initialize." << std::endl;
				}
			#endif
		}
	#endif
		void connection(const std::string host, int32_t port);
		void write(std::string msg);
		std::string read();
		int32_t getFD();
		bool isOpen() const;
		void close();
	private:
		int32_t fd;
};

#endif /* _SOCKET_H_ */
