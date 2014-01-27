#ifndef LemmaUTILS_H
#define LemmaUTILS_H
char * CLemma_BuyBuf(char const*, int);
char * CLemma_BuyString(char const*);
int CLemma_IsCharacter(unsigned char const* byte);

typedef struct MapStringInt
{
	const char* string;
	int n;
} MapStringInt;

int CLemma_MapToIntFrom(MapStringInt*, const char*);
const char* CLemma_MapToStringFrom(MapStringInt*, int);
#endif
