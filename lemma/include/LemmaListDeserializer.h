#ifndef LemmaList_Deserialize_H
#define LemmaList_Deserialize_H

struct LemmaList* LemmaList_Deserialize(char const *);
struct LemmaList* LemmaList_Dejsonify(struct json_t* json);

#endif
