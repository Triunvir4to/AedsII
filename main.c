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
    return (*(int*)key) % INITIAL_TABLE_SIZE;
}

/**
 * @brief Example hash function for characters.
 *
 * @param key Pointer to the key.
 * @param key_size Size of the key.
 * @return Hash value.
 */
unsigned int hash_char(const void *key, size_t key_size) {
    return (*(char*)key) % INITIAL_TABLE_SIZE;
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
    HashTable *int_table = newHash(sizeof(int), sizeof(int), hash_int, compare_int);

    int key1 = 5;
    int value1 = 10;
    HTinsert(int_table, &key1, &value1);

    int key2 = 15;
    int value2 = 20;
    HTinsert(int_table, &key2, &value2);

    HashTableIterator iterator = newHTIterator(int_table);

    while (nextHTI(&iterator))
        printf("Key: %d, Value: %d\n", *(int*)iterator.key, *(int*)iterator.value);

    free_table(int_table);

    return 0;
}