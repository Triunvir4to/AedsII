//
// Created by rafa on 27/06/24.
//

#ifndef AEDSII_HASHTABLE_H
#define AEDSII_HASHTABLE_H

#include <stdbool.h>


#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * @brief Initial size of the hash table.
 */
#define INITIAL_TABLE_SIZE 16

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
    unsigned int (*hash_function)(const void *key, size_t key_size); /**< Just must transform datatype in int Pointer to the hash function */
    int (*key_compare)(const void *key1, const void *key2, size_t key_size); /**< Pointer to the key comparison function */
    size_t capacity;    /**< size of _entries array */
    size_t length;      /**< number of items in hash table  */
} HashTable;

typedef struct {
    char *movie_id;
    char *movie_name;
    int year;
    char *certificate;
    int run_time;
    char *genre;
    float rating;
    char *description;
    char *director;
    char *director_id;
    char *star;
    char *star_id;
    float gross;
    int votes;
} Movie;

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

void free_entries(HashEntry **entries, size_t capacity);

/**
 * @brief Inserts a key-value pair into the hash table.
 *
 * @param table Pointer to the hash table.
 * @param key Pointer to the key.
 * @param value Pointer to the value.
 * @return true if the insertion was successful, false otherwise.
 */
bool HTinsert(HashTable *table, void *key, void *value);

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


#endif //AEDSII_HASHTABLE_H
