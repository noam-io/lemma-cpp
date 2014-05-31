//Copyright (c) 2014, IDEO
#include "LemmaApi.h"

extern "C" {
  #include "LemmaCApi.h"

  void * lemmaCreate(char * lemmaId)
  {
    return (void*) new LemmaApi("Voice Engine");
  }

  void lemmaDestroy(void * self)
  {
    delete (LemmaApi*)self;
  }


  void lemmaHear(void * self, const char * eventName, event_handler_t handler)
  {
    LemmaApi* lemma = (LemmaApi*)self;
    lemma->hear(eventName, handler);
  }

  void lemmaBegin(void * self, const char * maestroIpAddress, int maestroPort)
  {
    LemmaApi* lemma = (LemmaApi*)self;
    lemma->begin(maestroIpAddress, maestroPort);
  }

  void lemmaBeginWithDiscovery(void * self, int broadcastPort)
  {
    LemmaApi* lemma = (LemmaApi*)self;
    lemma->begin();
  }

  void lemmaRun(void * self)
  {
    LemmaApi* lemma = (LemmaApi*)self;
    lemma->run();
  }

  void sendStringEvent(void * self, char const * name, const char* value)
  {
    LemmaApi* lemma = (LemmaApi*)self;
    lemma->speak(name, value);
  }

  void sendIntEvent(void * self, char const * name, int value)
  {
    LemmaApi* lemma = (LemmaApi*)self;
    lemma->speak(name, value);
  }

  void sendDoubleEvent(void * self, char const * name, double value)
  {
    LemmaApi* lemma = (LemmaApi*)self;
    lemma->speak(name, value);
  }

  void sendListEvent(void * self, char const * name, LemmaList * list)
  {
    LemmaApi* lemma = (LemmaApi*)self;
	lemma->speak(name, list);
  }
  }
