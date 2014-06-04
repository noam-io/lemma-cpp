//Copyright (c) 2014, IDEO 

#include <stdio.h>
#include <unistd.h>
#include "LemmaApi.h"


void event_handle(CEvent const * event)
{
	printf("Received message.\n");
}

int main(int argc, const char* argv[])
{
	LemmaApi lemma("CPP Test", "");
	lemma.hear("CPPHear", event_handle);
	lemma.begin();
	printf("Hello world.\n");

	int count = 0;
	while(1){
		lemma.run();
		if(count == 1000){
			lemma.speak("CPPSend", 1);
			count = 0;
		}
		count++;
		usleep(1000);
	}
}



