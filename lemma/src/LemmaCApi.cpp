

  #include "LemmaApi.h"
  #include <iostream>

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

  
  void lemmaHear(void * self, char * eventName, event_handler_t handler)
  {
    LemmaApi* lemma = (LemmaApi*)self;
    lemma->hear(eventName, handler);
  }

  void lemmaBegin(void * self, char* maestroIpAddress, int maestroPort)
  {
    LemmaApi* lemma = (LemmaApi*)self;
    lemma->begin(maestroIpAddress, maestroPort);
  }

  void lemmaBeginWithDiscovery(void * self, int broadcastPort)
  {
    LemmaApi* lemma = (LemmaApi*)self;
    lemma->begin(broadcastPort);
  }

  void lemmaRun(void * self)
  {
    LemmaApi* lemma = (LemmaApi*)self;
    lemma->run();
  }

  void sendStringEvent(void * self, char const * name, char* value)
  {
    LemmaApi* lemma = (LemmaApi*)self;
    lemma->sendEvent(name, value);
  }

  void sendIntEvent(void * self, char const * name, int value)
  {
    LemmaApi* lemma = (LemmaApi*)self;
    lemma->sendEvent(name, value);
  }

  void sendDoubleEvent(void * self, char const * name, double value)
  {
    LemmaApi* lemma = (LemmaApi*)self;
    lemma->sendEvent(name, value);
  }

  void sendListEvent(void * self, char const * name, LemmaList * list)
  {
    LemmaApi* lemma = (LemmaApi*)self;
	lemma->sendEvent(name, list);
  }
  }
