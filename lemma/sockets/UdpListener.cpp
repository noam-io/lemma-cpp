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


#include "UdpListener.h"


UdpListener::UdpListener( int broadcastPort )
: broadcastPort(broadcastPort)
{
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
				printf("recvfrom filed with error %d\n", errno);
			} else {
				message[messageLength] = 0;
				return true;
			}
		} else if(FD_ISSET(RecvSocket, &ReadFDs)){
			printf("Socket error");
		}
	}
  else
  {
    printf(".");
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


bool UdpListener::broadcast(const char * message)
{
  int sock = RecvSocket ;              /* Socket */
  struct sockaddr_in broadcastAddr; /* Broadcast address */
  int broadcastPermission;          /* Socket opt to set permission to broadcast */
  unsigned int localIPLen;       /* Length of string to broadcast */


  /* Set socket to allow broadcast */
  broadcastPermission = 1;
  if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (void *) &broadcastPermission, sizeof(broadcastPermission)) < 0)
  {
    perror("setsockopt() failed");
    return false;
  }

  /* Construct local address structure */
  memset(&broadcastAddr, 0, sizeof(broadcastAddr));   /* Zero out structure */
  broadcastAddr.sin_family = AF_INET;                 /* Internet address family */
  broadcastAddr.sin_addr.s_addr = inet_addr("255.255.255.255");   /* Broadcast IP address */
  broadcastAddr.sin_port = htons(broadcastPort);      /* Broadcast port */

  /* Broadcast localIP in datagram to clients */
  if (sendto(sock, message, strlen(message), 0, (struct sockaddr *)
        &broadcastAddr, sizeof(broadcastAddr)) != strlen(message))
  {
    perror("sendto() sent a different number of bytes than expected");
    return false;
  }

  return true;
}
