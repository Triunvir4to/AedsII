#include "HashTable.h"
#include "HashTableIterator.h"
#include <stdio.h>

/**
 * @brief Example hash function for integers.
 *
 * @param key Pointer to the key.
 * @param key_size Size of the key.
 * @return Hash value.
 */
unsigned int hash_int(const void *key, size_t key_size) {
    return (*(int*)key);
}

/**
 * @brief Example hash function for characters.
 *
 * @param key Pointer to the key.
 * @param key_size Size of the key.
 * @return Hash value.
 */
unsigned int hash_char(const void *key, size_t key_size) {
    return (*(char*)key);
}

/**
 * @brief Example key comparison function for integers.
 *
 * @param key1 Pointer to the first key.
 * @param key2 Pointer to the second key.
 * @param key_size Size of the keys.
 * @return Comparison result.
 */
int compare_int(const void *key1, const void *key2, size_t key_size) {
    return (*(int*)key1) - (*(int*)key2);
}

/**
 * @brief Example key comparison function for characters.
 *
 * @param key1 Pointer to the first key.
 * @param key2 Pointer to the second key.
 * @param key_size Size of the keys.
 * @return Comparison result.
 */
int compare_char(const void *key1, const void *key2, size_t key_size) {
    return (*(char*)key1) - (*(char*)key2);
}

int main() {
    HashTable *char_table = newHash(sizeof(char), sizeof(int), hash_char, compare_char);

    for (char c = 'a'; c <= 'z'; ++c) {
        int value = c - 'a' + 1; // Just a sample value
        HTinsert(char_table, &c, &value);
    }

    HashTableIterator iterator = newHTIterator(char_table);

    while (nextHTI(&iterator))
        printf("Key: %c, Value: %d\n", *(char*)iterator.key, *(int*)iterator.value);


    printf("\ntable size: %zu\n", char_table->capacity);

    free_table(char_table);

    return 0;
}