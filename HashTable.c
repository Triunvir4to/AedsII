#include "HashTable.h"
#include <stdlib.h>
#include <memory.h>
#include <stdint-gcc.h>
#include <stdio.h>
#include "HTFacilities.h"

#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL


void print_movie(Movie *movie) {
    printf("Movie ID: %s\n", movie->movie_id);
    printf("Movie Name: %s\n", movie->movie_name);
    printf("Year: %d\n", movie->year);
    printf("Certificate: %s\n", movie->certificate);
    printf("Run Time: %d\n", movie->run_time);
    printf("Genre: %s\n", movie->genre);
    printf("Rating: %.1f\n", movie->rating);
    printf("Description: %s\n", movie->description);
    printf("Director: %s\n", movie->director);
    printf("Director ID: %s\n", movie->director_id);
    printf("Star: %s\n", movie->star);
    printf("Star ID: %s\n", movie->star_id);
    printf("Gross: %.2f\n", movie->gross);
    printf("Votes: %d\n", movie->votes);
}

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
    for (size_t i = 0; i < key_size; ++i)
        value = value * 37 + ptr[i];

    return value;
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
    HashEntry *entry = allocateMemory(sizeof(HashEntry));

    entry->key = allocateMemory(key_size);
    entry->value = allocateMemory(value_size);

    memcpy(entry->key, key, key_size);
    memcpy(entry->value, value, value_size);

    return entry;
}

size_t hash_function_wraper(HashTable *table, const void *key, size_t key_size) {
    const unsigned char* ptr = (const unsigned char*) key;
    uint64_t hash = FNV_OFFSET;

    for (size_t i = 0; i < key_size; ++i) {
        hash ^= (uint64_t) ptr[i];
        hash *= FNV_PRIME;
    }

    return (size_t)(hash % table->capacity);
}

bool HashExpand(HashTable *table) {
    size_t new_capacity = table->capacity * 2;
    if (new_capacity <= table->capacity) {  // Check for integer overflow
        return false;
    }

    HashEntry **new_entries = (HashEntry **) allocateMemory(new_capacity * sizeof(HashEntry*));
    if (new_entries == NULL) {
        return false;
    }

    for(size_t i = 0; i < new_capacity; ++i) {
        new_entries[i] = NULL;
    }

    size_t old_capacity = table->capacity;
    HashEntry **old_entries = table->entries;

    table->entries = new_entries;
    table->capacity = new_capacity;

    for (size_t i = 0; i < old_capacity; i++) {
        HashEntry *entry = old_entries[i];
        if (entry != NULL) {
            // Re-insert into new table using new hash values based on new capacity
            size_t new_slot = hash_function_wraper(table, entry->key, table->key_size);
            while (new_entries[new_slot] != NULL) {  // Use linear probing to find an empty slot
                new_slot = (new_slot + 1) % new_capacity;
            }
            new_entries[new_slot] = entry;  // Place entry in new table
        }
    }

    free(old_entries);  // Free the old table array
    return true;
}

HashTable* newHash(size_t key_size, size_t value_size,
                   unsigned int (*hash_function)(const void *key, size_t key_size),
                   int (*key_compare)(const void *key1, const void *key2, size_t key_size)){
    HashTable *table = allocateMemory(sizeof(HashTable));
    table->capacity = INITIAL_TABLE_SIZE;
    table->entries = (HashEntry **)allocateMemory(table->capacity * sizeof(HashEntry*));
    table->key_size = key_size;
    table->value_size = value_size;
    table->hash_function = hash_function ? hash_function : default_hash_function;
    table->key_compare = key_compare ? key_compare : default_key_compare;
    table->length = 0;

    for (int i = 0; i < table->capacity; ++i)
        table->entries[i] = NULL;


    return table;
}

void free_entries(HashEntry **entries, size_t capacity){
    for (size_t i = 0; i < capacity; i++) {
        if (entries[i] != NULL) {
            free(entries[i]->key);
            free(entries[i]->value);
            free(entries[i]);
        }
    }
    free(entries);
}

void free_table(HashTable *table) {
    free_entries(table->entries, table->capacity);
    free(table);
}

bool HTinsert(HashTable *table, void *key, void *value) {
    if ((float)table->length / table->capacity > 0.8) {
        if (!HashExpand(table))
            return false;
    }

    size_t slot = hash_function_wraper(table, key, table->key_size);
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
    return true;
}

void *search(HashTable *table, const void *key) {
    size_t hash_index = hash_function_wraper(table, key, strlen(key));
    size_t index = hash_index;
    int i = 0;

    while (table->entries[index] != NULL) {
        if (table->key_compare(table->entries[index]->key, key, table->key_size) == 0) {
            return table->entries[index]->value;
        }
        i++;
        index = (hash_index + i * i) % table->capacity; // Quadratic probing
        if (index == hash_index) { // Went through all slots
            break;
        }
    }
    return NULL;
}


bool delete(HashTable *table, const void *key) {
    size_t slot = hash_function_wraper(table, key, table->key_size);

    if (table->entries[slot] != NULL && table->key_compare(table->entries[slot]->key, key, table->key_size) == 0) {
        free(table->entries[slot]->key);
        free(table->entries[slot]->value);
        free(table->entries[slot]);
        table->entries[slot] = NULL;
        return true;
    }

    return false;
}