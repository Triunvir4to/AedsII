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

/**
 * @brief Creates a new hash table iterator.
 *
 * @param table Pointer to the hash table.
 * @return Initialized hash table iterator.
 */
HashTableIterator newHTIterator(HashTable * table);

/**
 * @brief Moves the iterator to the next item in the hash table.
 *
 * Updates the iterator's key and value to the current item.
 *
 * @param it Pointer to the hash table iterator.
 * @return true if the iterator was advanced to the next entry, false if there are no more entries.
 *
 * @note Do not call `insert` or `delete` during iteration.
 */
bool nextHTI(HashTableIterator *it);


#endif //AEDSII_HASHTABLEITERATOR_H
