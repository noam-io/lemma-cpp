//Copyright (c) 2014, IDEO 

#include "CppUTest/TestHarness.h"
#include "HostLocator.h"
#include "udp.h"
#include "MessageBuilder.h"
#include "Testticker.h"

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
  TestUdp * udp;
  TestTicker * ticker;
  HostLocator * locator;

  void setup()
  {
    udp = new TestUdp();
    ticker = new TestTicker();
    locator = new HostLocator(*udp, *ticker, "guest123", "room5");
  }

  void teardown()
  {
    delete locator;
    delete ticker;
    delete udp;
  }
};

TEST(HostLocator, SendsMarco)
{
  locator->tryLocate();
  ticker->time = true;
  MessageBuilder builder("guest123");
  char* marco = builder.buildMarco("room5");
  STRCMP_EQUAL(marco, udp->broadcastedMessage);
  CHECK_FALSE(locator->isFound());
  free(marco);
}

TEST(HostLocator, WaitsToSendMarco)
{
  ticker->time = false;
  locator->tryLocate();
  STRCMP_EQUAL("", udp->broadcastedMessage);
  CHECK_FALSE(locator->isFound());
}

TEST(HostLocator, ReceivesPolo)
{
  udp->readResult = true;
  strcpy(udp->message, "[\"polo\", \"guest123\", 8839]");
  locator->tryLocate();
  CHECK_TRUE(locator->isFound());
  STRCMP_EQUAL("172.32.22.11", locator->ipAddress());
  CHECK_EQUAL(8839, locator->port());
}

TEST(HostLocator, ReceivesJunk)
{
  udp->readResult = true;
  strcpy(udp->message, "[JUNK]");
  locator->tryLocate();
  CHECK_FALSE(locator->isFound());
  STRCMP_EQUAL("", locator->ipAddress());
  CHECK_EQUAL(-1, locator->port());
}
