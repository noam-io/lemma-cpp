#include <stdlib.h>
#include <string.h>
#include <iostream>

extern "C"
{
#include "LemmaList.h"
}

#include "CppUTest/TestHarness.h"
#include "CppUTest/TestHarness_c.h"

TEST_GROUP(LemmaList)
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
			cpputest_free(serializedList);
    }

	void check_lists_equal(LemmaList* expected, LemmaList* actual) {
		CHECK(LemmaList_Equals(expected, actual));
	}

};

TEST(LemmaList, twoEmptyListsAreEqual)
{
	LemmaList* list = LemmaList_Create();
	check_lists_equal(lemmaList, list);
	LemmaList_Destroy(list);
}

TEST(LemmaList, twoDifferentLenghtListsAreNotEqual)
{
	LemmaList* list = LemmaList_Create();
	LemmaList_AddString(lemmaList, "hello");
	CHECK(!LemmaList_Equals(lemmaList, list));
	LemmaList_Destroy(list);
}

TEST(LemmaList, twoSingleElementListsWithDifferentElmementsAreNotEqual)
{
	LemmaList* list = LemmaList_Create();
	LemmaList_AddString(lemmaList, "hello");
	LemmaList_AddString(list, "goodbye");
	CHECK(!LemmaList_Equals(lemmaList, list));
	LemmaList_Destroy(list);
}


TEST(LemmaList, twoIdenticalMultipleElementListsElmementsAreEqual)
{
	LemmaList* list = LemmaList_Create();
	LemmaList_AddString(lemmaList, "hello");
	LemmaList_AddString(lemmaList, "goodbye");
	LemmaList_AddString(list, "hello");
	LemmaList_AddString(list, "goodbye");
	CHECK(LemmaList_Equals(lemmaList, list));
	LemmaList_Destroy(list);
}


TEST(LemmaList, twoNonIdenticalMultipleElementListsElmementsAreNotEqual)
{
	LemmaList* list = LemmaList_Create();
	LemmaList_AddString(lemmaList, "hello");
	LemmaList_AddString(lemmaList, "hello");
	LemmaList_AddString(list, "hello");
	LemmaList_AddString(list, "goodbye");
	CHECK(!LemmaList_Equals(lemmaList, list));
	LemmaList_Destroy(list);
}


TEST(LemmaList, canGetElements)
{
	LemmaList_AddString(lemmaList, "element1");
	LemmaList_AddString(lemmaList, "element2");
	STRCMP_EQUAL("element1", LemmaList_GetStringAt(lemmaList, 0));
	STRCMP_EQUAL("element2", LemmaList_GetStringAt(lemmaList, 1));
}


TEST(LemmaList, canGetHashWithOneElement)
{
	LemmaList_AddString(lemmaList, "<table><tr><td>name</td><td>bob</td></tr></table>");

	LemmaList * hash = LemmaList_GetHashAt(lemmaList, 0);
     LemmaList * twoElementList = LemmaList_GetListAt(hash, 0);

     STRCMP_EQUAL("name", LemmaList_GetStringAt(twoElementList, 0));
     STRCMP_EQUAL("bob", LemmaList_GetStringAt(twoElementList, 1));
     LemmaList_Destroy(hash);
}

TEST(LemmaList, canGetHashWithMultipleElements)
{
     LemmaList_AddString(lemmaList, "<table><tr><td>name</td><td>dough</td></tr><tr><td>addr</td><td>here</td></tr></table>");

     LemmaList * hash = LemmaList_GetHashAt(lemmaList, 0);
     LemmaList * twoElementList = LemmaList_GetListAt(hash,1);

     STRCMP_EQUAL("addr", LemmaList_GetStringAt(twoElementList, 0));
     STRCMP_EQUAL("here", LemmaList_GetStringAt(twoElementList, 1));
     LemmaList_Destroy(hash);
}


TEST(LemmaList, cannotGetElementThatAreNotThere)
{
	LemmaList_AddString(lemmaList, "element1");
	LemmaList_AddString(lemmaList, "element2");
	POINTERS_EQUAL(0, LemmaList_GetStringAt(lemmaList, 3));
}

TEST(LemmaList, canReplaceString)
{
	LemmaList_AddString(lemmaList, "replaceMe");
	LemmaList_ReplaceAt(lemmaList, 0, "WithMe");

	STRCMP_EQUAL("WithMe", LemmaList_GetStringAt(lemmaList, 0));
}

TEST(LemmaList, canGetTail)
{
  LemmaList_AddString(lemmaList, "1");
  LemmaList_AddString(lemmaList, "2");
  LemmaList_AddString(lemmaList, "3");
  LemmaList_AddString(lemmaList, "4");

  LemmaList* expected = LemmaList_Create();
  LemmaList_AddString(expected, "3");
  LemmaList_AddString(expected, "4");

  LemmaList* tail = LemmaList_GetTailAt(lemmaList, 2);
  CHECK(LemmaList_Equals(expected, tail));
  LemmaList_Destroy(tail);
  LemmaList_Destroy(expected);
}

TEST(LemmaList, recursiveToString)
{
 	LemmaList_AddString(lemmaList, "a");
	LemmaList_AddString(lemmaList, "b");

  LemmaList* sublist = LemmaList_Create();
  LemmaList_AddString(sublist, "3");
  LemmaList_AddString(sublist, "4");

  LemmaList_AddList(lemmaList, sublist);

  STRCMP_EQUAL("[\"a\", \"b\", [\"3\", \"4\"]]", LemmaList_ToString(lemmaList));

}

TEST(LemmaList, getDouble)
{
  LemmaList_AddString(lemmaList, "2.3");
  DOUBLES_EQUAL(2.3, LemmaList_GetDoubleAt(lemmaList, 0), 0.1);
}
