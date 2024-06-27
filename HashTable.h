//
// Created by rafa on 27/06/24.
//

#ifndef AEDSII_HASHTABLE_H
#define AEDSII_HASHTABLE_H

#include <stdbool.h>


#include <stdbool.h>
#include <stdlib.h>

/**
 * @brief Initial size of the hash table.
 */
#define INITIAL_TABLE_SIZE 100

/**
 * @brief Structure for a hash table entry.
 */
typedef struct {
    void *key;   /**< Pointer to the key */
    void *value; /**< Pointer to the value */
} HashEntry;

/**
 * @brief Structure for the hash table.
 */
typedef struct {
    HashEntry **entries;                                         /**< Array of pointers to hash entries */
    size_t key_size;                                             /**< Size of the key */
    size_t value_size;                                           /**< Size of the value */
    unsigned int (*hash_function)(const void *key, size_t key_size); /**< Pointer to the hash function */
    int (*key_compare)(const void *key1, const void *key2, size_t key_size); /**< Pointer to the key comparison function */
    size_t capacity;    /**< size of _entries array */
    size_t length;      /**< number of items in hash table/**< Size of the hash table */
} HashTable;

/**
 * @brief Creates a new hash table.
 *
 * @param key_size Size of the key.
 * @param value_size Size of the value.
 * @param hash_function Pointer to the hash function.
 * @param key_compare Pointer to the key comparison function.
 * @return Pointer to the created hash table.
 */
HashTable* newHash(size_t key_size, size_t value_size,
                        unsigned int (*hash_function)(const void *key, size_t key_size),
                        int (*key_compare)(const void *key1, const void *key2, size_t key_size));

/**
 * @brief Frees the memory allocated for the hash table.
 *
 * @param table Pointer to the hash table.
 */
void free_table(HashTable *table);

/**
 * @brief Inserts a key-value pair into the hash table.
 *
 * @param table Pointer to the hash table.
 * @param key Pointer to the key.
 * @param value Pointer to the value.
 * @return true if the insertion was successful, false otherwise.
 */
bool insert(HashTable *table, const void *key, const void *value);

/**
 * @brief Searches for a key in the hash table.
 *
 * @param table Pointer to the hash table.
 * @param key Pointer to the key.
 * @return Pointer to the value if the key is found, NULL otherwise.
 */
void* search(HashTable *table, const void *key);

/**
 * @brief Deletes a key-value pair from the hash table.
 *
 * @param table Pointer to the hash table.
 * @param key Pointer to the key.
 * @return true if the deletion was successful, false otherwise.
 */
bool delete(HashTable *table, const void *key);

/**
 * @brief Resizes the hash table.
 *
 * @param table Pointer to the hash table.
 * @param new_size New size for the hash table.
 * @return true if the resize was successful, false otherwise.
 */
bool resize_table(HashTable *table, size_t new_size);

/**
 * @brief Example hash function for integers.
 *
 * @param key Pointer to the key.
 * @param key_size Size of the key.
 * @return Hash value.
 */
unsigned int hash_int(const void *key, size_t key_size);

/**
 * @brief Example hash function for characters.
 *
 * @param key Pointer to the key.
 * @param key_size Size of the key.
 * @return Hash value.
 */
unsigned int hash_char(const void *key, size_t key_size);

/**
 * @brief Example key comparison function for integers.
 *
 * @param key1 Pointer to the first key.
 * @param key2 Pointer to the second key.
 * @param key_size Size of the keys.
 * @return Comparison result.
 */
int compare_int(const void *key1, const void *key2, size_t key_size);

/**
 * @brief Example key comparison function for characters.
 *
 * @param key1 Pointer to the first key.
 * @param key2 Pointer to the second key.
 * @param key_size Size of the keys.
 * @return Comparison result.
 */
int compare_char(const void *key1, const void *key2, size_t key_size);
#endif //AEDSII_HASHTABLE_H
