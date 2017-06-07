#include "unorderedmap.h"
//#include <unordered_map>
#include <iostream>
//#include <cstdio>
#include <cstdlib>

using namespace std;

string conv_char_to_str(const char *cstr) {
    if(cstr == NULL) {
        cerr << "Erreur conv(char*, str) : input NULL" << endl;
        exit(EXIT_FAILURE);
        //return ""; //juste pour compilateur
    } else {
        return string(cstr);
    }
}

const char* conv_str_to_char(/*const*/ string &sstr) {
    return sstr.c_str();
}


extern "C" {
    void HashMap_set(HashMap *hashmap, const char *key, const char* value) {
        (*hashmap)[conv_char_to_str(key)] = conv_char_to_str(value);
    }

    bool HashMap_keyExist(const HashMap *hashmap, const char *key) {
        return (*hashmap).find(conv_char_to_str(key)) != (*hashmap).end();
    }

    const char* HashMap_get(/*const*/ HashMap *hashmap, const char *key) {
        return conv_str_to_char((*hashmap)[conv_char_to_str(key)]);
        /*
        auto res = (*hashmap).find(conv_char_to_str(key));
        if(res != (*hashmap).end())
            return conv_str_to_char(res->second);
        else
            return NULL;
        */
    }

    HashMap* HashMap_new() {
        return new unordered_map<string, string>();
    }

    void HashMap_free(const HashMap *hm) {
        delete hm;
    }

}
