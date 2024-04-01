#ifndef _SERVERSOCKET_H_
#define _SERVERSOCKET_H_

#include "communication/Socket.h"

class ServerSocket {

	public :
		ServerSocket(int32_t port, int32_t maxPlayers, bool local);
		int32_t getFD() { return socketfd; }
		bool isOpen() const { return socketfd != -1; }
		Socket accept();
		void close();
	private:
		int32_t socketfd;
};

#endif /* _SERVERSOCKET_H_ */
