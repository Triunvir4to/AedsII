#include "HashTable.h"
#include <stdlib.h>

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

static bool HashExpand(HashTable *table){
    size_t new_capacity = table->capacity * 2;
    if (new_capacity < table->capacity)
        return false;

    HashEntry *new_entries = calloc(new_capacity, sizeof(HashEntry));
    if (new_entries == NULL)
        return false;

    for (size_t i = 0; i < table->capacity; i++) {
        HashEntry *entry = table->entries[i];
        if (entry.key != NULL) {
            ht_set_entry(new_entries, new_capacity, entry.key,
                         entry.value, NULL);
        }
    }

    // Free old entries array and update this table's details.
    free(table->entries);
    table->entries = new_entries;
    table->capacity = new_capacity;
    return true;
}