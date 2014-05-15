#include "LemmaApi.h"
#include <iostream>

LemmaApi lemma("cpp_verification");

void echoHandler(CEvent const * e)
{
  std::cout << e->name << " : " << e->stringValue << std::endl;
  lemma.sendEvent("echoVerify", e->stringValue);
}

void plusOneHandler(CEvent const * e)
{
  std::cout << e->name << " : " << e->stringValue << std::endl;
  lemma.sendEvent("plusOneVerify", (int)(e->intValue + 1));
}

int main(int argc, const char* argv[])
{
  lemma.hear("Echo", echoHandler);
  lemma.hear("PlusOne", plusOneHandler);
  lemma.begin("127.0.0.1", 7733);

  int runs = 0;
  while(1)
  {
    runs++;
    lemma.run();
  }
  return 0;
}

