#include <string.h>
#include <iostream>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <netinet/in.h>
#include <errno.h>


#include "UdpListener.h"
#include "utils.h"


UdpListener::UdpListener()
{
	memset(lastAddressBuf, 0, 24);
}

bool UdpListener::startup()
{
	return true;
}

bool UdpListener::createSocket()
{
	RecvSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(RecvSocket == -1 /* INVALID_SOCKET */){
		printf("socket failed with error %d\n", errno);
		return false;
	}

	u_long nNoBlock = 1;
	ioctl(RecvSocket, FIONBIO, &nNoBlock);

	bool bOptVal = true;
	setsockopt(RecvSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&bOptVal, sizeof(bool));

	return true;
}


bool UdpListener::bindTo(int port)
{
	int iResult = 0;
	sockaddr_in RecvAddr;
	RecvAddr.sin_family = AF_INET;
	RecvAddr.sin_port = htons(port);
	RecvAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	iResult = bind(RecvSocket, (struct sockaddr*)&RecvAddr, sizeof(RecvAddr));
	if(iResult != 0){
		printf("bind failed with error %d\n", errno);
		return false;
	}
	return true;
}


bool UdpListener::attemptRead()
{
	unsigned int SenderAddrSize = sizeof(SenderAddr);
	int iResult = 0;
	timeval timeout = {0, 0};
	fd_set ReadFDs, ExceptFDs;

	FD_ZERO(&ReadFDs);
	FD_ZERO(&ExceptFDs);
	FD_SET(RecvSocket, &ReadFDs);
	FD_SET(RecvSocket, &ExceptFDs);

	if(select(0, &ReadFDs, 0, &ExceptFDs, &timeout) > 0){
		if(FD_ISSET(RecvSocket, &ReadFDs)){
			messageLength = recvfrom(RecvSocket, message, MAX_BUFFER_SIZE, 0, (struct sockaddr *)&SenderAddr, &SenderAddrSize);
			if(messageLength == -1 /* SOCKET_ERROR */){
				printf("recvfrom filed with error %\n", errno);
			} else {
				message[messageLength] = 0;
				return true;
			}
		} else if(FD_ISSET(RecvSocket, &ReadFDs)){
			printf("Socket error");
		}
	}
	return false;
}


char* UdpListener::lastAddress()
{
	return inet_ntoa(SenderAddr.sin_addr);
}

bool UdpListener::close()
{
	int iResult = 0;
	iResult = ::close(RecvSocket);
	if(iResult == -1 /* SOCKET_ERROR */){
		printf("closesocket failed with error %d\n", errno);
		return false;
	}
	return true;
}


