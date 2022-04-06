#ifndef _SOCKET_H_
#define _SOCKET_H_

#include <unistd.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string>
#include <iosfwd>
#include <iostream>

class Socket {
	public :
		Socket();
		Socket(int fd):fd(fd){}
		void connect(const std::string & host, int port);
		void connect(in_addr ipv4, int port);
		std::string read();
		void write(std::string msg);
		void close();
		int getFD() { return fd ;}
		bool isOpen() const {return fd != -1;}
	private:
		int fd;
};

std::ostream & operator<< (std::ostream & os, struct sockaddr_in * addr);

#endif /* _SOCKET_H_ */
