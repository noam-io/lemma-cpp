#include <errno.h>
#include <iostream>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>


#include "UnixUdp.h"


UnixUdp::UnixUdp( int broadcastPort )
: broadcastPort(broadcastPort)
{
}

bool UnixUdp::createSocket()
{
	RecvSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if( RecvSocket == -1 ){
		perror("socket creation failed");
		return false;
	}

  //set non-blocking socket
	u_long nNoBlock = 1;
	ioctl(RecvSocket, FIONBIO, &nNoBlock);

  //Reuse address (probably not needed anymore)
	bool bOptVal = true;
	setsockopt(RecvSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&bOptVal, sizeof(bool));

  //allow broadcasting on this socket
  int broadcastPermission = 1;
  setsockopt(RecvSocket, SOL_SOCKET, SO_BROADCAST, (void *) &broadcastPermission, sizeof(broadcastPermission));

	return true;
}


bool UnixUdp::bind(int port)
{
	int iResult = 0;
	sockaddr_in RecvAddr;
	RecvAddr.sin_family = AF_INET;
	RecvAddr.sin_port = htons(port);
	RecvAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	iResult = ::bind(RecvSocket, (struct sockaddr*)&RecvAddr, sizeof(RecvAddr));
	if(iResult != 0){
		perror("failed to bind udp");
		return false;
	}
	return true;
}


bool UnixUdp::attemptRead()
{
	unsigned int SenderAddrSize = sizeof(SenderAddr);
	int iResult = 0;
	timeval timeout = {0, 0};
	fd_set ReadFDs, ExceptFDs;

	FD_ZERO(&ReadFDs);
	FD_ZERO(&ExceptFDs);
	FD_SET(RecvSocket, &ReadFDs);
	FD_SET(RecvSocket, &ExceptFDs);

	if(select(RecvSocket + 1, &ReadFDs, 0, &ExceptFDs, &timeout) > 0){
		if(FD_ISSET(RecvSocket, &ReadFDs)){
			messageLength = recvfrom(RecvSocket, message, MAX_BUFFER_SIZE, 0, (struct sockaddr *)&SenderAddr, &SenderAddrSize);
			if(messageLength == -1 /* SOCKET_ERROR */){
				perror("recvfrom failed");
			} else {
				message[messageLength] = 0;
				return true;
			}
		} else if(FD_ISSET(RecvSocket, &ReadFDs)){
			perror("Socket error");
		}
	}
	return false;
}


char* UnixUdp::lastAddress()
{
	return inet_ntoa(SenderAddr.sin_addr);
}

bool UnixUdp::close()
{
	int iResult = 0;
	iResult = ::close( RecvSocket );
	if( iResult == -1 ){
		perror("closesocket failed");
		return false;
	}
	return true;
}


bool UnixUdp::broadcast(const char * message)
{
  struct sockaddr_in broadcastAddr;

  memset(&broadcastAddr, 0, sizeof(broadcastAddr));
  broadcastAddr.sin_family = AF_INET;
  //TODO loop over all broadcast addresses on all interfaces if ifaddrs
  //http://man7.org/linux/man-pages/man3/getifaddrs.3.html
  broadcastAddr.sin_addr.s_addr = inet_addr("255.255.255.255");
  broadcastAddr.sin_port = htons(broadcastPort);

  if (sendto(RecvSocket, message, strlen(message), 0, (struct sockaddr *)
        &broadcastAddr, sizeof(broadcastAddr)) != strlen(message))
  {
    perror("sendto() sent a different number of bytes than expected");
    return false;
  }

  return true;
}
