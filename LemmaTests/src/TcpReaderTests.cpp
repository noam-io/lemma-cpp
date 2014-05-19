//Copyright (c) 2014, IDEO 

#include "CppUTest/TestHarness.h"
#include "TcpReader.h"
#include "Connection.h"
#include <string>

class TestTcpReader : public TcpReader
{
public:
  char msgReceived[1024];
  int messagesReceived;
  TestTcpReader() : messagesReceived(0) { }
  virtual bool messageReceived(const char* msg, size_t length)  {
    memset(msgReceived, 0, 1024);
    messagesReceived++;
    strncpy(msgReceived, msg, length);
    return true;
  }
};

TEST_GROUP(TcpReader)
{
  TestTcpReader reader;
  Connection conn;

};

TEST(TcpReader, HandlesMessage){
  CHECK_EQUAL(8, reader.handle( "000002ab", 8, conn ));
  CHECK_EQUAL( 1, reader.messagesReceived );
  STRCMP_EQUAL( "ab", reader.msgReceived );
}


TEST(TcpReader, HandlesLongerMessage){
  CHECK_EQUAL(22, reader.handle("000016abcdefghijklmnop", 22, conn));
  CHECK_EQUAL(1, reader.messagesReceived);

  STRCMP_EQUAL("abcdefghijklmnop", reader.msgReceived);
}

TEST(TcpReader, HandlesMultipleMessages)
{
  CHECK_EQUAL(19, reader.handle("000002ab000005derek", 19, conn));
  CHECK_EQUAL(2, reader.messagesReceived);
  STRCMP_EQUAL("derek", reader.msgReceived);
}

TEST(TcpReader, HandlesMultipleMessagesWithDifferentSizes)
{
  CHECK_EQUAL(18, reader.handle("000004abcd000002ef", 18, conn));
  CHECK_EQUAL(2, reader.messagesReceived);
  STRCMP_EQUAL("ef", reader.msgReceived);
}

TEST(TcpReader, HandlesIncompleteMessage)
{
  CHECK_EQUAL(0, reader.handle("000004ab", 8, conn));
  CHECK_EQUAL(0, reader.messagesReceived);
}

TEST(TcpReader, HandlesIncompleteLength)
{
  CHECK_EQUAL(0, reader.handle("000", 3, conn));
  CHECK_EQUAL(0, reader.messagesReceived);
}

TEST(TcpReader, HandlesIncompleteLengthInSecondMessage)
{
  char msg[] = "000002ab001";
  char after[] = "00abcds";
  CHECK_EQUAL(8, reader.handle(msg, 11, conn));
  CHECK_EQUAL(1, reader.messagesReceived);
  STRCMP_EQUAL("ab", reader.msgReceived);
}

TEST(TcpReader, HandlesShorterSecondMessageThanFirstMessage)
{
  char msg[] = "000010abcdefghij000002ef";
  CHECK_EQUAL(24, reader.handle(msg, 24, conn));
  CHECK_EQUAL(2, reader.messagesReceived);
  STRCMP_EQUAL("ef", reader.msgReceived);
}
