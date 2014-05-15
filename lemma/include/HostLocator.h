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
  const char * ipAddress();
  int port();

private:
  const char * lemmaId;
  const char * roomName;
  Udp & udp;
  bool found;
  char hostIpAddress[24];
  int hostPort;
};
#endif
