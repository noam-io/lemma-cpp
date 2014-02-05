/*
 * LemmaApiSerial.h
 *
 *  Created on: Feb 4, 2014
 *      Author: dbrody
 */

#ifndef LEMMAAPISERIAL_H_
#define LEMMAAPISERIAL_H_

#include "LemmaApi.h"
#include "SerialCommunication.h"

class LemmaApiSerial: public LemmaApi {
public:
	LemmaApiSerial(const char* lemmaId, const char* device, speed_t baud);
	virtual ~LemmaApiSerial();

	void hear(char* eventName);
	bool messageReceived(char* msg, size_t length);
	SerialCommunication* Serial;

};

#endif /* LEMMAAPISERIAL_H_ */
