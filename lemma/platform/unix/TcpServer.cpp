//Copyright (c) 2014, IDEO 



#include "utils.h"
#include <iostream>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdio.h>


#include "Connection.h"
#include "TcpServer.h"


using namespace std;


TcpServer::TcpServer(DataHandler& handler) : handler(handler)
{
}

TcpServer::~TcpServer()
{
	ConnectionList::iterator it = mConnections.begin();
	while(it != mConnections.end()){
		delete *it;
		++it;
	}
}

SOCKET SetUpListener(const char* pcAddress, int nPort);

int TcpServer::start(const char* pcAddress, int nPort)
{
	mListeningSocket = SetUpListener(pcAddress, htons(nPort));
	if(mListeningSocket == -1 /* INVALID_SOCKET */ ){
		cout << endl << GetLastErrorMessage("establish listener") << endl;
		return 3;
	}
	return 0;
}


int TcpServer::listeningPort()
{
	sockaddr_in name;
	socklen_t length = sizeof(sockaddr);
	if( 0 == getsockname(mListeningSocket, (struct sockaddr*) &name, &length))
		return ntohs(name.sin_port);
	return -1;
}


int TcpServer::run()
{
	acceptConnections();
	return 0;
}


SOCKET SetUpListener(const char* pcAddress, int nPort){
	u_long nInterfaceAddr = inet_addr(pcAddress);
	if(nInterfaceAddr != INADDR_NONE){
		SOCKET sd = socket(AF_INET, SOCK_STREAM, 0);
		if(sd != -1 /* INVALID_SOCKET */ ){
			sockaddr_in sinInterface;
			sinInterface.sin_family = AF_INET;
			sinInterface.sin_addr.s_addr = nInterfaceAddr;
			sinInterface.sin_port = nPort;
			if(bind(sd, (sockaddr*)&sinInterface, sizeof(sockaddr_in)) != SOCKET_ERROR){
				listen(sd, SOMAXCONN);
				return sd;
			} else {
				cerr << GetLastErrorMessage("bind() failed") << endl;
			}
		}
	}
	return -1 /* INVALID_SOCKET */ ;
}


int TcpServer::highestFD()
{
	int largest = 0;
	ConnectionList::iterator it = mConnections.begin();
	while(it != mConnections.end()){
		int fd = (*it)->sd;
		largest = std::max(largest, fd);
		++it;
	}
	return largest;
}


void TcpServer::setupFDSets(fd_set& ReadFDs, fd_set& WriteFDs, fd_set& ExceptFDs, int ListeningSocket)
{
	FD_ZERO(&ReadFDs);
	FD_ZERO(&WriteFDs);
	FD_ZERO(&ExceptFDs);

	if(ListeningSocket != -1 /* INVALID_SOCKET */){
		FD_SET(ListeningSocket, &ReadFDs);
		FD_SET(ListeningSocket, &ExceptFDs);
	}

	ConnectionList::iterator it = mConnections.begin();
	while(it != mConnections.end()){
		if((*it)->nCharsInBuffer < kBufferSize){
			// Space in buffer so pay attention
			FD_SET((*it)->sd, &ReadFDs);
		}

		if((*it)->nCharsInWriteBuffer > 0){
			// Data to be sent so signal
			FD_SET((*it)->sd, &WriteFDs);
		}

		FD_SET((*it)->sd, &ExceptFDs);
		++it;
	}
}


bool TcpServer::readData(Connection& conn)
{
	int nBytes = recv(conn.sd, conn.readBuffer + conn.nCharsInBuffer,
			kBufferSize - conn.nCharsInBuffer, 0);
	if(nBytes == 0){
		cout << "Socket " << conn.sd << " was closed by the client. Shutting down." << endl;
		return false;
	} else if(nBytes == SOCKET_ERROR) {
		int err;
		unsigned int errlen = sizeof(err);
		getsockopt(conn.sd, SOL_SOCKET, SO_ERROR, (char*)&err, &errlen);
		return (err == EWOULDBLOCK);
	}

	conn.nCharsInBuffer += nBytes;
	int bytesHandled = handler.handle(conn.readBuffer, conn.nCharsInBuffer, conn);
	conn.nCharsInBuffer -= bytesHandled;
	if(conn.nCharsInBuffer > 0){
		memcpy(&conn.readBuffer[0], &conn.readBuffer[bytesHandled], conn.nCharsInBuffer);
	}
	return true;
}


bool WriteData(Connection& conn)
{
	int nBytes = send(conn.sd, conn.writeBuffer, conn.nCharsInWriteBuffer, 0);
	if(nBytes == SOCKET_ERROR){
		int err;
		unsigned int errlen = sizeof(err);
		getsockopt(conn.sd, SOL_SOCKET, SO_ERROR, (char*)&err, &errlen);
		return (err == EWOULDBLOCK);
	}

	if(nBytes == conn.nCharsInWriteBuffer) {
		conn.nCharsInWriteBuffer = 0;
	} else {
		conn.nCharsInWriteBuffer -= nBytes;
		memmove(conn.writeBuffer, conn.writeBuffer + nBytes, conn.nCharsInWriteBuffer);
	}
	return true;
}


void TcpServer::acceptConnections()
{
	struct sockaddr_in sinRemote;
	unsigned int nAddrSize = sizeof(sinRemote);

	fd_set ReadFDs, WriteFDs, ExceptFDs;
	setupFDSets(ReadFDs, WriteFDs, ExceptFDs, mListeningSocket);
	timeval t = {0, 0};
	int fdMax = std::max(mListeningSocket, highestFD()) + 1;
	if(select(fdMax, &ReadFDs, &WriteFDs, &ExceptFDs, &t) > 0){
		if(FD_ISSET(mListeningSocket, &ReadFDs)){
			SOCKET sd = accept(mListeningSocket, (sockaddr*)&sinRemote, &nAddrSize);
			if(sd != -1 /* INVALID_SOCKET */){
				cout << "Accepted connection from " <<
						inet_ntoa(sinRemote.sin_addr) << ":" <<
						ntohs(sinRemote.sin_port) <<
						", socket " << sd << "." << endl;
				mConnections.push_back(new Connection(sd));
				if((mConnections.size() + 1) > 64){
					cout << "WARNING: More than 63 client connections accepted."
							"This will not work reliably on some stacks!" << endl;
				}
				u_long nNoBlock = 1;
				ioctl(sd, FIONBIO, &nNoBlock);
			} else {
				cerr << GetLastErrorMessage("accept() failed") << endl;
				return;
			}
		} else if(FD_ISSET(mListeningSocket, &ExceptFDs)){
			int err;
			unsigned int errlen = sizeof(err);
			getsockopt(mListeningSocket, SOL_SOCKET, SO_ERROR, (char*)&err, &errlen);
			cerr << GetLastErrorMessage("Exception on listening socket: ", err) << endl;
			return;
		}

		ConnectionList::iterator it = mConnections.begin();
		while(it != mConnections.end()){
			bool bOK = true;
			const char* pcErrorType = 0;

			if(FD_ISSET((*it)->sd, &ExceptFDs)){
				bOK = false;
				pcErrorType = "General socket error";
				FD_CLR((*it)->sd, &ExceptFDs);
			} else{
				if(FD_ISSET((*it)->sd, &ReadFDs)){
					bOK = readData(**it);
					pcErrorType = "Read error";
					FD_CLR((*it)->sd, &ReadFDs);
				}
				if(FD_ISSET((*it)->sd, &WriteFDs)){
					bOK = WriteData(**it);
					pcErrorType = "Write error";
					FD_CLR((*it)->sd, &WriteFDs);
				}
			}
			if(!bOK){
				// Something bad happened on socket
				int err;
				unsigned int errlen = sizeof(err);
				getsockopt((*it)->sd, SOL_SOCKET, SO_ERROR, (char*)&err, &errlen);
				if(err != NO_ERROR){
					cerr << GetLastErrorMessage(pcErrorType, err) << endl;
				}
				ShutdownConnection((*it)->sd);
				mConnections.erase(it);
				it = mConnections.begin();
			} else {
				++it;
			}
		}
	} else {
	}
}









