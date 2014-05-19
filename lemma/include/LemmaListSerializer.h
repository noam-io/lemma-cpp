//Copyright (c) 2014, IDEO 

#ifndef LemmaListSerializer_H
#define LemmaListSerializer_H

char* LemmaList_Serialize(struct LemmaList*);
void LemmaList_Release(char *serializedResults);
struct json_t * LemmaList_Jsonify(struct LemmaList * self);

#endif
