//Copyright (c) 2014, IDEO 

#include "LemmaApi.h"
#include <iostream>

LemmaApi lemma("cpp_verification", "lemma_verification");

void echoHandler(CEvent const * e)
{
  std::cout << e->name << " : " << e->stringValue << std::endl;
  lemma.speak("EchoVerify", e->stringValue);
}

void plusOneHandler(CEvent const * e)
{
  std::cout << e->name << " : " << e->stringValue << std::endl;
  lemma.speak("PlusOneVerify", (int)(e->intValue + 1));
}

int main(int argc, const char* argv[])
{
  lemma.hear("Echo", echoHandler);
  lemma.hear("PlusOne", plusOneHandler);
  lemma.begin();

  int runs = 0;
  while(1)
  {
    runs++;
    lemma.run();
  }
  return 0;
}

