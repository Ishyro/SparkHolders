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

#include "Values.h"

class Socket {
	public :
		Socket();
		Socket(int fd):fd(fd){}
		void connect(const std::string & host, int port);
		void connect(in_addr ipv4, int port);
		std::string read();
		void write(std::string msg);
		Character * getUser();
		int getFD();
		bool isOpen() const;
		void close();
	private:
		int fd;
		Character * user;
};

std::ostream & operator<< (std::ostream & os, struct sockaddr_in * addr);

#endif /* _SOCKET_H_ */
