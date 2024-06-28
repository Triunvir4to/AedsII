#include <string.h>
#include "HTFacilities.h"

unsigned int hash_int(const void *key, size_t key_size) {
    return (*(int*)key);
}

unsigned int hash_char(const void *key, size_t key_size) {
    return (*(char*)key);
}

unsigned int hash_string(const void *key, size_t key_size) {
    const char *str = (const char *)key;
    unsigned int hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;  // hash * 33 + c


    return hash;
}

int compare_int(const void *key1, const void *key2, size_t key_size) {
    return (*(int*)key1) - (*(int*)key2);
}

int compare_char(const void *key1, const void *key2, size_t key_size) {
    return (*(char*)key1) - (*(char*)key2);
}

int compare_string(const void *key1, const void *key2, size_t key_size) {
    return strcmp((const char *)key1, (const char *)key2);
}