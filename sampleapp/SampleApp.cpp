//Copyright (c) 2014, IDEO 

#include <stdio.h>
#include <unistd.h>
#include <csignal>
#include "LemmaApi.h"

bool run = true;

void signal_handle(int signum)
{
	printf("Captured interupt signal.\n");
	run = false;
}	

void event_handle(CEvent const * event)
{
	printf("Received message.\n");
}

int main(int argc, const char* argv[])
{
	signal(SIGINT, signal_handle);
	LemmaApi lemma("CPP Test", "");
	lemma.hear("CPPHear", event_handle);
	lemma.begin();
	printf("Hello world.\n");

	int count = 0;
	while(run){
		lemma.run();
		if(count == 1000){
			lemma.speak("CPPSend", 1);
			count = 0;
		}
		count++;
		usleep(1000);
	}
	printf("Exiting...\n");
	return 0;
}



