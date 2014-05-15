#include "CppUTest/TestHarness.h"
#include "HostLocator.h"
#include "Udp.h"
#include "MessageBuilder.h"

class TestUdp : public Udp
{
public:
  char broadcastedMessage[128];
  bool readResult;

  TestUdp()
    : readResult(false)
  {
    memset(broadcastedMessage, 0, 128);
  }

	virtual bool attemptRead(){
    return readResult;
  }

	virtual const char* lastAddress() {
    return "172.32.22.11";
  }

	virtual bool close() {
    return false;
  }

  virtual bool broadcast(const char * message)
  {
    memset(broadcastedMessage, 0, 128);
    strcpy(broadcastedMessage, message);
    return true;
  }
};

TEST_GROUP(HostLocator)
{
};

TEST(HostLocator, SendsMarco)
{
  TestUdp udp;
  HostLocator locator(udp, "guest123", "room5");
  locator.tryLocate();
  MessageBuilder builder("guest123");
  char* marco = builder.buildMarco("room5");
  CHECK_EQUAL(0, strcmp(marco, udp.broadcastedMessage));
  CHECK_FALSE(locator.isFound());
  free(marco);
}

TEST(HostLocator, ReceivesPolo)
{
  TestUdp udp;
  udp.readResult = true;
  strcpy(udp.message, "[\"polo\", \"guest123\", 8839]");
  HostLocator locator(udp, "guest123", "room5");
  locator.tryLocate();
  CHECK_TRUE(locator.isFound());

}
