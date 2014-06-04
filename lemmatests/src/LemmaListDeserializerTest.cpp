//Copyright (c) 2014, IDEO

#include <stdlib.h>
#include <string.h>

extern "C"
{
#include "LemmaList.h"
#include "LemmaListDeserializer.h"
#include "LemmaListSerializer.h"
}

#include "CppUTest/TestHarness.h"
#include "CppUTest/TestHarness_c.h"

TEST_GROUP(LemmaListDeserializer)
{
    LemmaList* lemmaList;
	LemmaList* deserializedList;
	char* serializedList;

    void setup()
    {
		lemmaList  = LemmaList_Create();
		serializedList = 0;
		deserializedList = 0;
    }

    void teardown()
    {
		LemmaList_Destroy(lemmaList);

		if (deserializedList)
			LemmaList_Destroy(deserializedList);

		if (serializedList != 0)
			LemmaList_Release(serializedList);
    }

	void check_lists_equal(LemmaList* expected, LemmaList* actual) {
		CHECK(LemmaList_Equals(expected, actual));
	}

};


TEST(LemmaListDeserializer, deserializeEmptyList)
{
	deserializedList = LemmaList_Deserialize("[]");
	CHECK(deserializedList != 0);
	LONGS_EQUAL(0, LemmaList_GetLength(deserializedList));
}

TEST(LemmaListDeserializer, deserializeNull)
{
	LemmaList* list = LemmaList_Deserialize(0);
	POINTERS_EQUAL(0, list);
}

TEST(LemmaListDeserializer, deserializeEmptyString)
{
	LemmaList* list = LemmaList_Deserialize("");
	POINTERS_EQUAL(0, list);
}

TEST(LemmaListDeserializer, MissingOpenBracketReturnsNull)
{
	LemmaList* list = LemmaList_Deserialize("hello");
	POINTERS_EQUAL(0, list);
}

TEST(LemmaListDeserializer, MissingClosingBracketReturnsNull)
{
	LemmaList* list = LemmaList_Deserialize("[");
	POINTERS_EQUAL(0, list);
}

TEST(LemmaListDeserializer, canDeserializeCanonicalListWithOneElement)
{
	char const* canonicalList = "[\"Hi doug.\"]";
	LemmaList* deserializedList = LemmaList_Deserialize(canonicalList);
	CHECK(deserializedList != NULL);
	LONGS_EQUAL(1, LemmaList_GetLength(deserializedList));
	STRCMP_EQUAL("Hi doug.", LemmaList_GetStringAt(deserializedList, 0));
	LemmaList_Destroy(deserializedList);
}

TEST(LemmaListDeserializer, canDeserializeWithMultibyteCharacters)
{
  char const* canonicalList = "[\"Hi JRÜ€©\"]";
  LemmaList* deserializedList = LemmaList_Deserialize(canonicalList);
  CHECK(deserializedList != NULL);
  LONGS_EQUAL(1, LemmaList_GetLength(deserializedList));
  STRCMP_EQUAL("Hi JRÜ€©", LemmaList_GetStringAt(deserializedList, 0));
  LemmaList_Destroy(deserializedList);
}

TEST(LemmaListDeserializer, canDeSerializeListWithOneElement)
{
	LemmaList_AddString(lemmaList, "hello");
	serializedList = LemmaList_Serialize(lemmaList);
	deserializedList = LemmaList_Deserialize(serializedList);
	CHECK(deserializedList != 0);
	check_lists_equal(lemmaList, deserializedList);
}

TEST(LemmaListDeserializer, canDeSerializeListWithTwoElements)
{
	LemmaList_AddString(lemmaList, "hello");
	LemmaList_AddString(lemmaList, "bob");
	serializedList = LemmaList_Serialize(lemmaList);
	deserializedList = LemmaList_Deserialize(serializedList);
	CHECK(deserializedList != 0);
	check_lists_equal(lemmaList, deserializedList);
}

TEST(LemmaListDeserializer, canAddSubList)
{
	LemmaList* embeddedList;
	embeddedList = LemmaList_Create();
	LemmaList_AddString(embeddedList, "element");
	LemmaList_AddList(lemmaList, embeddedList);
	serializedList = LemmaList_Serialize(lemmaList);
	deserializedList = LemmaList_Deserialize(serializedList);
	LemmaList * subList = LemmaList_GetListAt(deserializedList, 0);
	subList = LemmaList_GetListAt(deserializedList, 0);
	check_lists_equal(embeddedList, subList);
}
