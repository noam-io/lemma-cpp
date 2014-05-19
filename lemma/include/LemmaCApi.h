//Copyright (c) 2014, IDEO 

#ifndef LEMMA_C_API_H
#define LEMMA_C_API_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

  #include "EventHandler.h"
  #include "LemmaList.h"

  void * lemmaCreate(char * lemmaId);
  void lemmaDestroy(void * self);
  void lemmaHear(void * self, const char * eventName, event_handler_t handler);
  void lemmaBegin(void * self, const char * maestroIpAddress, int maestroPort);
  void lemmaBeginWithDiscovery(void * self, int broadcastPort);
  void lemmaRun(void * self);
  void sendStringEvent(void * self, char const * name, const char* value);
  void sendIntEvent(void * self, char const * name, int value);
  void sendFloatEvent(void * self, char const * name, double value);
  void sendListEvent(void * self, char const * name, LemmaList * list);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
