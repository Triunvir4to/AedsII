//
// Created by rafa on 27/06/24.
//

#ifndef AEDSII_HASHTABLEITERATOR_H
#define AEDSII_HASHTABLEITERATOR_H

#include "HashTable.h"

/**
 * @brief Structure for a hash table iterator.
 *
 * This structure allows iteration over the entries of a hash table.
 */
typedef struct {
    const void* key;  /**< Pointer to the current key */
    void* value;      /**< Pointer to the current value */

    // PRIVATE fields -> Don't use these fields directly.
    HashTable *_table; /**<  **PRIVATE field Don't use** Reference to the hash table being iterated */
    size_t _index;    /**< **PRIVATE field Don't use** Current index into the hash table entries */
} HashTableIterator;

// Return new hash table iterator (for use with ht_next).
HashTableIterator ht_iterator(HashTable * table);

// Move iterator to next item in hash table, update iterator's key
// and value to current item, and return true. If there are no more
// items, return false. Don't call ht_set during iteration.
bool ht_next(HashTable *it);


#endif //AEDSII_HASHTABLEITERATOR_H
