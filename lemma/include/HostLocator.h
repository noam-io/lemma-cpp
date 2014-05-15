#ifndef HOST_LOCATOR_H
#define HOST_LOCATOR_H
class Udp;

class HostLocator
{
public:
  HostLocator(Udp& udp, const char * lemmaId, const char * roomName);
  void begin();
  void tryLocate();
  bool isFound();

private:
  const char * lemmaId;
  const char * roomName;
  Udp & udp;
  bool found;
};
#endif
