#include <string.h>
#include <stdlib.h>
#include "LemmaUtil.h"
char * CLemma_BuyBuf(char const* buffer, int length)
{
	char * purchase = NULL;
	if (buffer == NULL)
	{
		return NULL;
	}

	purchase = (char*)malloc(length+1);
	strncpy(purchase, buffer, length);
	purchase[length] = 0;
	return purchase;
}
char * CLemma_BuyString(char const* string)
{
	if (string == NULL)
		return NULL;
	return CLemma_BuyBuf(string, (int)strlen(string));
}

int CLemma_MapToIntFrom(MapStringInt* map, const char* name)
{
	MapStringInt* p = map;
	while (p->string != NULL && 0 != strcmp(name, p->string))
	{
		p++;
	}

	return p->n;
}

const char* CLemma_MapToStringFrom(MapStringInt* map, int n)
{
	MapStringInt* p = map;
	while (p->string != NULL && n != p->n)
	{
		p++;
	}

	return p->string;
}

int CLemma_IsCharacter(unsigned char const* byte)
{
  if ((*byte < (unsigned char const)0x80) || (*byte > (unsigned char const)0xBF))
    return 1;
  return 0;
}
