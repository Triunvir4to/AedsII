#include "HashTable.h"
#include <stdlib.h>
#include <memory.h>

static bool HashExpand(HashTable *table){
    size_t new_capacity = table->capacity * 2;
    if (new_capacity < table->capacity)
        return false;

    HashEntry **new_entries = calloc(new_capacity, sizeof(HashEntry*));
    if (new_entries == NULL)
        return false;

    // Rehash all existing entries into the new table
    for (size_t i = 0; i < table->capacity; i++) {
        HashEntry *entry = table->entries[i];
        if (entry != NULL && entry->key != NULL) {
            unsigned int new_slot = table->hash_function(entry->key, table->key_size) % new_capacity;
            new_entries[new_slot] = entry;
        }
    }

    // Free old entries array and update the table's details
    free(table->entries);
    table->entries = new_entries;
    table->capacity = new_capacity;
    return true;
}

/**
 * @brief Default hash function to calculate the index for a given key.
 *
 * @param key Pointer to the key.
 * @param key_size Size of the key.
 * @return Hash value.
 */
static unsigned int default_hash_function(const void *key, size_t key_size) {
    const unsigned char *ptr = key;
    unsigned long int value = 0;
    for (size_t i = 0; i < key_size; ++i) {
        value = value * 37 + ptr[i];
    }
    return value % INITIAL_TABLE_SIZE;
}

/**
 * @brief Default key comparison function.
 *
 * @param key1 Pointer to the first key.
 * @param key2 Pointer to the second key.
 * @param key_size Size of the keys.
 * @return Comparison result.
 */
static int default_key_compare(const void *key1, const void *key2, size_t key_size) {
    return memcmp(key1, key2, key_size);
}

/**
 * @brief Creates a new hash entry.
 *
 * @param key Pointer to the key.
 * @param value Pointer to the value.
 * @param key_size Size of the key.
 * @param value_size Size of the value.
 * @return Pointer to the created hash entry.
 */
static HashEntry* create_entry(const void *key, const void *value, size_t key_size, size_t value_size) {
    HashEntry *entry = malloc(sizeof(HashEntry));
    entry->key = malloc(key_size);
    entry->value = malloc(value_size);

    memcpy(entry->key, key, key_size);
    memcpy(entry->value, value, value_size);

    return entry;
}

HashTable* newHash(size_t key_size, size_t value_size,
                   unsigned int (*hash_function)(const void *key, size_t key_size),
                   int (*key_compare)(const void *key1, const void *key2, size_t key_size)){
    HashTable *table = malloc(sizeof(HashTable));
    table->entries = calloc(INITIAL_TABLE_SIZE, sizeof(HashEntry*));
    table->key_size = key_size;
    table->value_size = value_size;
    table->hash_function = hash_function ? hash_function : default_hash_function;
    table->key_compare = key_compare ? key_compare : default_key_compare;
    table->capacity = INITIAL_TABLE_SIZE;
    table->length = 0;

    return table;
}

void free_table(HashTable *table) {
    for (size_t i = 0; i < table->capacity; i++) {
        if (table->entries[i] != NULL) {
            free(table->entries[i]->key);
            free(table->entries[i]->value);
            free(table->entries[i]);
        }
    }

    free(table->entries);
    free(table);
}

bool HTinsert(HashTable *table, const void *key, const void *value) {
    unsigned int slot = table->hash_function(key, table->key_size) % table->capacity;
    unsigned int original_slot = slot;

    while (table->entries[slot] != NULL) {
        if (table->key_compare(table->entries[slot]->key, key, table->key_size) == 0) {
            memcpy(table->entries[slot]->value, value, table->value_size);
            return true;
        }

        slot = (slot + 1) % table->capacity;
        if (slot == original_slot)
            return false;
    }

    table->entries[slot] = create_entry(key, value, table->key_size, table->value_size);
    table->length++;

    if ((float)table->length / table->capacity > 0.7)
        HashExpand(table);

    return true;
}

void *search(HashTable *table, const void *key) {
    unsigned int slot = table->hash_function(key, table->key_size);

    if (table->entries[slot] != NULL && table->key_compare(table->entries[slot]->key, key, table->key_size) == 0)
        return table->entries[slot]->value;


    return NULL;
}

bool delete(HashTable *table, const void *key) {
    unsigned int slot = table->hash_function(key, table->key_size);

    if (table->entries[slot] != NULL && table->key_compare(table->entries[slot]->key, key, table->key_size) == 0) {
        free(table->entries[slot]->key);
        free(table->entries[slot]->value);
        free(table->entries[slot]);
        table->entries[slot] = NULL;
        return true;
    }

    return false;
}