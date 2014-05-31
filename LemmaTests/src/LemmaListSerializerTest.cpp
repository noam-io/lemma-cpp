//Copyright (c) 2014, IDEO

#include <stdlib.h>
#include <string.h>


extern "C"
{
#include "LemmaList.h"
#include "LemmaListSerializer.h"
}

#include "CppUTest/TestHarness.h"
#include "CppUTest/TestHarness_c.h"

TEST_GROUP(LemmaListSerializer)
{
  LemmaList* lemmaList;
  char* serializedList;

  void setup()
  {
    lemmaList  = LemmaList_Create();
    serializedList = 0;
  }

  void teardown()
  {
    LemmaList_Destroy(lemmaList);

    LemmaList_Release(serializedList);
  }
};

TEST(LemmaListSerializer, SerializeAListWithNoElements)
{
	serializedList = LemmaList_Serialize(lemmaList);
	STRCMP_EQUAL("[]", serializedList);
}

TEST(LemmaListSerializer, SerializeAListWithOneElements)
{
	LemmaList_AddString(lemmaList, "hello");
	serializedList = LemmaList_Serialize(lemmaList);
	STRCMP_EQUAL("[\"hello\"]", serializedList);
}

TEST(LemmaListSerializer, SerializeAListWithTwoElements)
{
	LemmaList_AddString(lemmaList, "hello");
	LemmaList_AddString(lemmaList, "world");

	serializedList = LemmaList_Serialize(lemmaList);

	STRCMP_EQUAL("[\"hello\", \"world\"]", serializedList);
}

TEST(LemmaListSerializer, ListCopysItsString)
{
	char string[12] = "Hello";
	LemmaList_AddString(lemmaList, string);
	strcpy(string, "Goodbye");
	serializedList = LemmaList_Serialize(lemmaList);
	STRCMP_EQUAL("[\"Hello\"]", serializedList);
}

TEST(LemmaListSerializer, canCopyAList)
{
	LemmaList_AddString(lemmaList, "123456");
	LemmaList_AddString(lemmaList, "987654");

	LemmaList* copy = LemmaList_Create();
	int i;
	for (i=0; i<LemmaList_GetLength(lemmaList); i++) {
		char* string = LemmaList_GetStringAt(lemmaList, i);
		LemmaList_AddString(copy, string);
	}
	char * serialCopy = LemmaList_Serialize(copy);
	char * serialLemmaList = LemmaList_Serialize(lemmaList);
	STRCMP_EQUAL(serialCopy, serialLemmaList);

	LemmaList_Destroy(copy);
	free(serialLemmaList);
	free(serialCopy);

}

TEST(LemmaListSerializer, SerializeNestedList)
{
	LemmaList* embeddedList;
	embeddedList = LemmaList_Create();
	LemmaList_AddString(embeddedList, "element");
	LemmaList_AddList(lemmaList, embeddedList);
	serializedList = LemmaList_Serialize(lemmaList);
	STRCMP_EQUAL("[[\"element\"]]", serializedList);
}

TEST(LemmaListSerializer, serializeNull)
{
  LemmaList_AddString(lemmaList, NULL);
	serializedList = LemmaList_Serialize(lemmaList);
	STRCMP_EQUAL("[]", serializedList);

}

TEST(LemmaListSerializer, serializeMultibyteCharacters)
{
  LemmaList_AddString(lemmaList, "Ü€©phewÜ€©");
  serializedList = LemmaList_Serialize(lemmaList);
  STRCMP_EQUAL("[\"Ü€©phewÜ€©\"]", serializedList);
}

