#ifndef _SERVERSOCKET_H_
#define _SERVERSOCKET_H_

#include "communication/Socket.h"

class ServerSocket {

	public :
		ServerSocket(int port, int maxPlayers, bool local);
		int getFD() { return socketfd; }
		bool isOpen() const { return socketfd != -1; }
		Socket accept();
		void close();
	private:
		int socketfd;
};

#endif /* _SERVERSOCKET_H_ */
