#ifndef D_LemmaList_H
#define D_LemmaList_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct LemmaList LemmaList;

LemmaList* LemmaList_Create(void);
void LemmaList_Destroy(LemmaList*);
extern void LemmaList_AddString(LemmaList*, char const *);
void LemmaList_AddList(LemmaList* self, LemmaList* element);
int LemmaList_GetLength(LemmaList*);
int LemmaList_Equals(LemmaList* self, LemmaList* other);
LemmaList * LemmaList_GetListAt(LemmaList* self, int index);
char * LemmaList_GetStringAt(LemmaList* self, int index);
double LemmaList_GetDoubleAt(LemmaList* self, int index);
LemmaList* LemmaList_GetHashAt(LemmaList* self, int index);
void LemmaList_ReplaceAt(LemmaList* self, int index, char const * replacementString);
void LemmaList_AddBuffer(LemmaList* self, char const* buffer, int length);
LemmaList* LemmaList_GetTailAt(LemmaList*, int index);
char* LemmaList_ToString(LemmaList*);
#ifdef __cplusplus
}
#endif


#endif
