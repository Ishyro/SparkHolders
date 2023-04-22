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

class CloseException : public std::exception {

};

class Socket {
	public :
		Socket();
		Socket(int fd):fd(fd) {
			#ifdef _WIN32_WINNT
				WSADATA d;
				if (WSAStartup(MAKEWORD(2, 2), &d)) {
					fprintf(stderr, "Failed to initialize.\n");
				}
			#endif
		}
		void connect(const std::string & host, int port);
		void connect(in_addr ipv4, int port);
		std::string read();
		void write(std::string msg);
		int getFD();
		bool isOpen() const;
		void close();
	private:
		int fd;
  		static const char final_delimiter = '%';
  		static const char delimiter = '&';
};

#endif /* _SOCKET_H_ */
