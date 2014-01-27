#include "../include/LemmaList.h"
#include "../include/LemmaUtil.h"
#include "../include/LemmaListDeserializer.h"
#include "../include/LemmaListSerializer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//static local variables
typedef struct Node Node;

struct Node {
	Node* next;
	char* string;
	LemmaList* list;
};

struct LemmaList {
	int length;
	Node* head; 
	Node* tail;
};

static void insertNode(LemmaList* self, Node* node);

LemmaList* LemmaList_Create(void)
{
	 LemmaList* self = (LemmaList*)malloc(sizeof(LemmaList));
	 memset(self, 0, sizeof(LemmaList));
	 return self;
}

void LemmaList_Destroy(LemmaList* self)
{
	Node * node;
	Node * next;
	for(node = self->head; node; node = next)
	{
		if (node->string) 
			free(node->string);
		
		if (node->list)
			LemmaList_Destroy(node->list);
			
		next = node->next;
		free(node);
	}
	free(self);
}

void LemmaList_AddBuffer(LemmaList* self, char const* buffer, int length)
{
	Node* newNode = (Node*)malloc(sizeof(Node));
	newNode->next = 0;
	newNode->list = 0;

	insertNode(self, newNode);

	newNode->string = CLemma_BuyBuf(buffer, length);
}


void LemmaList_AddString(LemmaList* self, char const* string)
{
	LemmaList_AddBuffer(self, string, string ? (int)strlen(string) : 0);
}

void LemmaList_AddList(LemmaList* self, LemmaList* element)
{
	Node* newNode = (Node*)malloc(sizeof(Node));
	newNode->next = 0;
	newNode->list = element;
	newNode->string = 0;

	insertNode(self, newNode);
}

int LemmaList_GetLength(LemmaList* self)
{
	return self->length;
}


int LemmaList_Equals(LemmaList* self, LemmaList* other){
	Node *p, *q;
	if (self->length != other->length)
		return 0;

	for (p = self->head, q=other->head; p; p=p->next, q=q->next)
	{
		if (strcmp(p->string, q->string) != 0)
			return 0;
	}

	return 1;
}

Node * LemmaList_GetNodeAt(LemmaList* self, int index)
{
	int i;
	Node* node = self->head;

	if (index >= self->length)
		return 0;

	for (i = 0; i < index; i++)
	{
		node = node->next;
	}
	return node;
}

LemmaList * LemmaList_GetListAt(LemmaList* self, int index)
{
	Node * node = LemmaList_GetNodeAt(self, index);
	if (node == 0)
		return 0;

	return node->list;
}

char * LemmaList_GetStringAt(LemmaList* self, int index)
{
	Node* node = LemmaList_GetNodeAt(self, index);
	if(node == 0)
		return 0;
	return node->string;
}

double LemmaList_GetDoubleAt(LemmaList* self, int index)
{
  char* speed_s = LemmaList_GetStringAt(self, index);
  return atof(speed_s);
}

static char * parseHashCell(char ** cellStart)
{
	char * cellValue = *cellStart + strlen("<td>");
	char * cellStop = strstr(cellValue, "</td>");

	int length = (int)(cellStop - cellValue);
	char * buf = (char*)malloc(length + 1);
	strncpy(buf, cellValue, length);
	buf[length] = 0;
	
	*cellStart = strstr(cellStop + strlen("<td>"), "<td>");
	
	return buf;
}

static LemmaList* parseHashEntry(char * row)
{
		char * hashKey;
		char * hashValue; 
		char * cellStart;

		LemmaList * element = LemmaList_Create();

		cellStart = strstr(row, "<td>");

		hashKey = parseHashCell(&cellStart);
		LemmaList_AddString(element, hashKey);
		free(hashKey);

		hashValue = parseHashCell(&cellStart);
		LemmaList_AddString(element, hashValue);
		free(hashValue);
		
		return element;
}

static LemmaList* LemmaList_deserializeHash(char * serializedHash)
{	
  LemmaList *element;
  LemmaList *hash = LemmaList_Create();
  
  char * row = strstr(serializedHash, "<tr>");
  while (row != NULL)
	{
		element = parseHashEntry(row);
		LemmaList_AddList(hash, element);
		row = strstr(row + strlen("<tr>"), "<tr>");
	}
  return hash;	
}

LemmaList* LemmaList_GetHashAt(LemmaList* self, int index)
{
	return LemmaList_deserializeHash(LemmaList_GetStringAt(self, 0));
}

void LemmaList_ReplaceAt(LemmaList* self, int index, char const * replacementString)
{
	char * newString;
	Node* node = LemmaList_GetNodeAt(self, index);
	if(node->list != 0){
		LemmaList_Destroy(node->list);
		node->list = 0;
	}
	newString = CLemma_BuyString(replacementString);
	free(node->string);
	node->string = newString;
}

static void insertNode(LemmaList* self, Node* node)
{
	if (self->length == 0)
	{
		self->head = node;
	}
	else
	{
		self->tail->next = node;
	}
	self->tail = node;	
	self->length++;
}

LemmaList* LemmaList_GetTailAt(LemmaList* self, int index)
{
	LemmaList * tail = LemmaList_Create();
	int length = LemmaList_GetLength(self);
	for(;index < length; index++) {
		LemmaList_AddString(tail, LemmaList_GetStringAt(self, index));
	}
	return tail;
}

char* LemmaList_ToString(LemmaList* self) {
	static char string[128];
	char buf[128];
	int i;
	int length;

	buf[0] = '\0';
	strncat(buf, "[", 128);
	length = LemmaList_GetLength(self);
	for (i = 0; i<length; i++) {
		LemmaList* sublist = LemmaList_GetListAt(self, i);
		if (sublist != NULL) {
			strncat(buf, LemmaList_ToString(sublist), 128);
		} else {
			strncat(buf, "\"", 128);
			strncat(buf, LemmaList_GetStringAt(self, i), 128);
			strncat(buf, "\"", 128);
		}
		if (i != (length-1)) {
			strncat(buf, ", ", 128);
		}	
	}
	strncat(buf, "]", 128);
	strncpy(string, buf, 128);
	return string;	
}
