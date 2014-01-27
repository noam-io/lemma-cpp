#include "utils.h"
#include <errno.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <iostream>
#include <algorithm>
#include <strstream>
#include <unistd.h>
#include <stdio.h>

using namespace std;

const int kBufferSize = 1024;


static struct ErrorEntry {
	int nID;
	const char* pcMessage;

	ErrorEntry(int id, const char* pc=0):
		nID(id), pcMessage(pc)
	{
	}

	bool operator<(const ErrorEntry& rhs) const
	{
		return nID < rhs.nID;
	}
} gaErrorList[] = {
		ErrorEntry(0,					"No Error")
};

const int kNumMessages = sizeof(gaErrorList) / sizeof(ErrorEntry);


const char* GetLastErrorMessage(const char* pcMessagePrefix,
		int nErrorID /* =0 */)
{
	static char acErrorBuffer[256];
	ostrstream outs(acErrorBuffer, sizeof(acErrorBuffer));
	outs << pcMessagePrefix << ": ";

	ErrorEntry* pEnd = gaErrorList + kNumMessages;
	ErrorEntry Target(nErrorID ? nErrorID : errno);
	ErrorEntry* it = lower_bound(gaErrorList, pEnd, Target);
	if (( it != pEnd) && (it->nID == Target.nID)){
		outs << it->pcMessage;
	} else {
		outs << "unknown error";
	}
	outs << " (" << Target.nID << ")";
	outs << ends;
	acErrorBuffer[sizeof(acErrorBuffer) - 1] = '\0';
	return acErrorBuffer;
}


int getSO_ERROR(SOCKET sd)
{
	int err = 1;
	socklen_t len = sizeof err;
	if( -1 == getsockopt(sd, SOL_SOCKET, SO_ERROR, (char*)&err, &len)){
		perror("getSO_ERROR");
	}
	if(err){
		errno = err;
	}
	return err;
}


bool ShutdownConnection(SOCKET sd)
{
	if(sd < 0){
		return false;
	}

	// clear any errors
	getSO_ERROR(sd);

	//terminate the reliable' delivery
	if(shutdown(sd, 2) == SOCKET_ERROR){
		return false;
	}

	// Read any access data
	char acReadBuffer[kBufferSize];
	while(1){
		int nNewBytes = recv(sd, acReadBuffer, kBufferSize, 0);
		if(nNewBytes == SOCKET_ERROR){
			return false;
		} else if(nNewBytes != 0){
			cerr << endl << "FYI, received " << nNewBytes <<
					" unexpected bytes during shutdown." << endl;
		} else {
			// Okay!
			break;
		}
	}

	// close connection
	if(close(sd) == SOCKET_ERROR){
		return false;
	}
	return true;
}











