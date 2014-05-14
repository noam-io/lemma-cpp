#include "MessageBuilder.h"
#include <String.h>
#include "CppUTest/TestHarness.h"
#include "LemmaList.h"


TEST_GROUP(MessageBuilder)
{
};

TEST(MessageBuilder, createsStringMessage) {
  MessageBuilder builder("lemmaId");

  char const * expected = "[\"event\", \"lemmaId\", \"speed\", \"65\"]";
  char * actual = builder.buildEvent("speed", "65");

  CHECK(strcmp(actual, expected) == 0);
  free(actual);
}

TEST(MessageBuilder, createsIntMessage) {
  MessageBuilder builder("lemmaId");

  char const * expected = "[\"event\", \"lemmaId\", \"speed\", 65]";
  char * actual = builder.buildEvent("speed", 65);

  CHECK(strcmp(actual, expected) == 0);
  free(actual);
}

TEST(MessageBuilder, createsFloatMessage) {
  MessageBuilder builder("lemmaId");

  char const * expected = "[\"event\", \"lemmaId\", \"speed\", 23.25]";
  char * actual = builder.buildEvent("speed", 23.25);

  CHECK(strcmp(actual, expected) == 0);
  free(actual);
}

TEST(MessageBuilder, createsListMessage) {
  MessageBuilder builder("lemmaId");
  LemmaList * list = LemmaList_Create();
  LemmaList_AddString(list, "event in list");

  char const * expected = "[\"event\", \"lemmaId\", \"speed\", [\"event in list\"]]";
  char * actual = builder.buildEvent("speed", list);

  CHECK(strcmp(actual, expected) == 0);
  free(actual);
}

TEST(MessageBuilder, createRegistrationMessage) {
  MessageBuilder builder("lemmaId");

  char const* hears[2] = {"event1", "event2"};
  char const* plays[3] = {"event3", "event4", "event5"};

  char const* expected = "[\"register\", \"lemmaId\", 123, [\"event1\", \"event2\"], [\"event3\", \"event4\", \"event5\"]]";
  char* actual = builder.buildRegister(123, hears, 2, plays, 3);

  CHECK_EQUAL(0, strcmp(actual, expected));
  free(actual);
}

