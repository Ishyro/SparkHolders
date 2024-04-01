#ifndef _SOCKET_H_
#define _SOCKET_H_

#include <unistd.h>

#ifndef _WIN32_WINNT
	#include <netinet/ip.h>
	#include <sys/socket.h>
	#include <arpa/inet.h>
	#include <netdb.h>
#else
	#include <ws2tcpip.h>
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
		Socket();
		Socket(int32_t fd):fd(fd) {
			#ifdef _WIN32_WINNT
				WSADATA d;
				if (WSAStartup(MAKEWORD(2, 2), &d)) {
	  				std::cerr << "Failed to initialize." << std::endl;
				}
			#endif
		}
		void connect(const std::string & host, int32_t port);
		void connect(in_addr ipv4, int32_t port);
		void write(std::string msg);
		std::string read();
		int32_t getFD();
		bool isOpen() const;
		void close();
	private:
		int32_t fd;
};

#endif /* _SOCKET_H_ */
