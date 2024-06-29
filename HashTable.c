#include "HashTable.h"
#include <stdlib.h>
#include <stdint-gcc.h>
#include <stdio.h>
#include <string.h>

#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL


/**
 * @brief Prints an allocation failed message and exits the program.
 *
 */
static void allocation_failed() {
    fprintf(stderr, "Memory allocation failed.\n");
    exit(-1);
}

/**
 * @brief Allocates memory and returns the pointer to the allocated memory.
 *
 * @param size Size of the memory block to be allocated.
 * @return Pointer to the allocated memory.
 */
static void* allocateMemory(size_t size) {
    void *ptr = malloc(size);
    if (!ptr)
        allocation_failed();

    return ptr;
}

static uint64_t hash_key(const char* key) {
    uint64_t hash = FNV_OFFSET;
    for (const char* p = key; *p; p++) {
        hash ^= (uint64_t)(unsigned char)(*p);
        hash *= FNV_PRIME;
    }
    return hash;
}

/**
 * @brief Creates a new hash entry.
 *
 * @param key Pointer to the key.
 * @param value Pointer to the value.
 * @return Pointer to the created hash entry.
 */
static const char* create_entry(HashEntry **entries, size_t capacity,
                                const char* key, void* value, size_t* plength) {
    uint64_t hash = hash_key(key);
    size_t index = (size_t)(hash & (uint64_t)(capacity - 1));

    // Loop through to handle collisions
    while (entries[index] != NULL && entries[index]->key != NULL) {
        if (strcmp(key, entries[index]->key) == 0) {
            entries[index]->value = value;
            return entries[index]->key;
        }
        index = (index + 1) % capacity;
    }

    // Allocate memory for new entry if necessary
    if (entries[index] == NULL) {
        entries[index] = malloc(sizeof(HashEntry));
        if (entries[index] == NULL) {
            return NULL; // handle failed memory allocation
        }
        entries[index]->key = NULL; // initialize key
        entries[index]->value = NULL; // initialize value
    }

    // Duplicate key and increment length if not NULL
    if (plength != NULL) {
        key = strdup(key);
        if (key == NULL)
            return NULL;

        (*plength)++;
    }

    entries[index]->key = (char *) key;
    entries[index]->value = value;
    return key;
}

static bool HashExpand(HashTable *table) {
    size_t new_capacity = table->capacity * 2;
    if (new_capacity < table->capacity)
        return false;

    HashEntry **new_entries = calloc(new_capacity, sizeof(HashEntry));
    if (new_entries == NULL)
        return false;

    for (size_t i = 0; i < table->capacity; i++) {
        HashEntry *entry = table->entries[i];
        if (entry != NULL && entry->key != NULL) {
            create_entry(new_entries, new_capacity, entry->key,
                         entry->value, NULL);
        }
    }

    free(table->entries);
    table->entries = new_entries;
    table->capacity = new_capacity;
    return true;
}

HashTable* newHash() {
    HashTable *table = allocateMemory(sizeof(HashTable));
    table->capacity = INITIAL_TABLE_SIZE;
    table->entries = (HashEntry **)allocateMemory(table->capacity * sizeof(HashEntry*));
    table->length = 0;

    for (int i = 0; i < table->capacity; ++i)
        table->entries[i] = NULL;


    return table;
}

void free_entries(HashEntry **entries, size_t capacity){
    for (size_t i = 0; i < capacity; i++) {
        if (entries[i] != NULL) {
            free((void *)entries[i]->key);
            free(entries[i]);
        }
    }
    free(entries);
}

void free_table(HashTable *table) {
    free_entries(table->entries, table->capacity);
    free(table);
}

bool HTSet(HashTable *table, const char *key, void *value) {
    if (value == NULL)
        return NULL;

    if (table->length >= table->capacity / 2) {
        if (!HashExpand(table))
            return NULL;
    }
    return create_entry(table->entries, table->capacity, key, value,
                        &table->length);
}

void *HTget(HashTable *table, const char *key, int *access_count) {
    (*access_count) = 0;
    uint64_t hash = hash_key(key);
    size_t index = (size_t)(hash & (uint64_t)(table->capacity - 1));

    while (table->entries[index] != NULL && table->entries[index]->key != NULL) {
        ++(*access_count);
        if (strcmp(key, table->entries[index]->key) == 0)
            return table->entries[index]->value;

        index = (index + 1) % table->capacity;
    }
    return NULL;
}