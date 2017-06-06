#ifndef UNORDEREDMAP_H_INCLUDED
#define UNORDEREDMAP_H_INCLUDED

#include <stdbool.h>

#ifdef __cplusplus
#include <unordered_map>
typedef std::unordered_map<std::string, std::string> HashMap;
#else
typedef struct unordered_map unordered_map;
typedef unordered_map HashMap;
#endif

//#if defined(__STDC__) || defined(__cplusplus)
#ifdef __cplusplus
extern "C" {
#endif

void HashMap_set(HashMap *hashmap, const char *key, const char* value);

bool HashMap_keyExist(const HashMap *hashmap, const char *key);

const char* HashMap_get(/*const*/ HashMap *hashmap, const char *key);

#ifdef __cplusplus
}
#endif

#endif // UNORDEREDMAP_H_INCLUDED
