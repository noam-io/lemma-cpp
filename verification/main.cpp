#include "LemmaApi.h"
#include <iostream>

void speedHandler(CEvent const * e)
{
  std::cout << e->name << " : " << e->stringValue << std::endl;
}

int main(int argc, const char* argv[])
{
  LemmaApi lemma("testwindowsApp");
  lemma.hear("orthography", speedHandler);
  lemma.hear("ptt", speedHandler);
  lemma.begin(8833);

  int runs = 0;
  while(1)
  {
    runs++;
    lemma.run();

    if (runs % 10000 == 0)
    {
      lemma.sendEvent("speed", runs / 10000);
      lemma.sendEvent("doug", "boo");
    }
  }
  return 0;
}

