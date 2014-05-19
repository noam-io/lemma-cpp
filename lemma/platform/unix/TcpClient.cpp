//Copyright (c) 2014, IDEO 


#include "TcpClient.h"
#include <iostream>
#include <string.h>
#include <stdio.h>

using namespace std;

const char* LookupAddress(const char* pcHost);
SOCKET EstablishConnection(const char* nRemoteAddr, u_short nPort);

TcpClient::TcpClient()
{
}

TcpClient::~TcpClient()
{
	cout << "Shutting connection down..." << flush;
	if(ShutdownConnection(mSocket))
	{
		cout << "Connection is down." << endl;
	}
	else
	{
		cout << endl << GetLastErrorMessage("Shutdown connection") << endl;
	}
}

int TcpClient::connect(const char* pcHost, int nPort)
{
	// Find Server's Address
	//cout << "Looking up address...\n" << flush;
	const char* nRemoteAddress = LookupAddress(pcHost);
	if(nRemoteAddress == NULL){
		cerr << endl << GetLastErrorMessage("looking up address") << endl;
		return 3;
	}

	// Connect to the Server
	//printf("Connecting to remote host %s...\n", nRemoteAddress);
	mSocket = EstablishConnection(nRemoteAddress, nPort);

	if ( mSocket == -1 /* INVALID_SOCKET */) {
		cerr << endl << GetLastErrorMessage("connect to server") << endl;
		return 3;
	}

	//cout << "connected, socket " << mSocket << "." << endl;
	return 0;
}


int TcpClient::sendMessage(const char* message, size_t messageLength){
	int result = send(mSocket, message, messageLength, 0);
	if(result == SOCKET_ERROR)
	{
		cerr << endl << GetLastErrorMessage("send packet") << endl;
		return 3;
	}
	return 0;
}

const char* LookupAddress(const char* pcHost)
{
	if(inet_addr(pcHost) == INADDR_NONE)
	{
		hostent* pHE = gethostbyname(pcHost);
		if(pHE == 0){
			return NULL;
		}
		return pHE->h_addr_list[0];
	}
	return pcHost;
}


SOCKET EstablishConnection(const char* nRemoteAddr, u_short nPort)
{
	//printf("Establishing Connection: %s:%d\n", nRemoteAddr, nPort);
	SOCKET sd = socket(AF_INET, SOCK_STREAM, 0);
	if(sd != -1 /* INVALID_SOCKET */ ){
		sockaddr_in sinRemote;
		sinRemote.sin_family = AF_INET;
		sinRemote.sin_addr.s_addr = inet_addr(nRemoteAddr);
		sinRemote.sin_port = htons(nPort);
		if(connect(sd, (struct sockaddr*)&sinRemote, sizeof(struct sockaddr)) == SOCKET_ERROR){
			sd = -1;
		}
	}
	return sd;
}


int TcpClient::read(char* buffer, size_t expectedSize)
{
	unsigned int nTotalBytes = 0;
	while(nTotalBytes < expectedSize){
		char* currentLocation = buffer + nTotalBytes;
		int bufferRemaining = expectedSize - nTotalBytes;
		int nNewBytes = recv(mSocket, currentLocation, bufferRemaining, 0);
		if(nNewBytes == SOCKET_ERROR){
			return -1;
		} else if(nNewBytes == 0){
			cerr << "Connection closed by peer." << endl;
			return 0;
		}
		nTotalBytes += nNewBytes;
	}
	return nTotalBytes;
}

