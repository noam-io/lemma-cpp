#ifndef TCP_SERVER_H
#define TCP_SERVER_H


#include <vector>
#include "DataHandler.h"
#include <sys/select.h>
#include <sys/types.h>
#include <unistd.h>

//struct fd_set;
typedef std::vector<Connection*> ConnectionList;

class TcpServer {

private:
	int mListeningSocket;
	ConnectionList mConnections;
	DataHandler& handler;

public:
	TcpServer(DataHandler& handler);
	virtual ~TcpServer();

	int start(const char* pcAddress = "0.0.0.0", int nPort = 0);
	int run();
	int listeningPort();

private:
	void acceptConnections();
	int highestFD();
	void setupFDSets(fd_set& ReadFDs, fd_set& WriteFDs, fd_set& ExceptFDs, int ListeningSocket);
	bool readData(Connection& conn);
};

#endif
