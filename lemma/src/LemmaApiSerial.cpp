/*
 * LemmaApiSerial.cpp
 *
 *  Created on: Feb 4, 2014
 *      Author: dbrody
 */

#include "LemmaApiSerial.h"
#include <iostream>

LemmaApiSerial::LemmaApiSerial(const char* lemmaId, const char* device, speed_t baud) : LemmaApi(lemmaId){
	Serial = new SerialCommunication(device, baud);
	Serial->start();
}

LemmaApiSerial::~LemmaApiSerial() {
	if(Serial){
		delete Serial;
	}
}

void LemmaApiSerial::hear(char* eventName)
{
	// No callback since it is just passed to Serial
	LemmaApi::hear(eventName, NULL);
}

bool LemmaApiSerial::messageReceived(char* msg, size_t length)
{
	char lengthStr[7];
	snprintf(lengthStr, 7, "%06lu", strlen(msg));
	std::cout << "Received (" << length << "): " << lengthStr << msg << std::endl;
	Serial->writeMessage(6, lengthStr);
	Serial->writeMessage(length, msg);
	Serial->writeMessage(1, "\n");
	return true;
}

