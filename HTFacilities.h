//
// Created by rafa on 28/06/24.
//

#ifndef AEDSII_HTFACILITIES_H
#define AEDSII_HTFACILITIES_H

#include <stdlib.h>

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
 * @brief Example hash function for Strings.
 *
 * @param key Pointer to the key.
 * @param key_size Size of the key.
 * @return Hash value.
 */
unsigned int hash_string(const void *key, size_t key_size);

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

/**
 * @brief Example key comparison function for strings.
 *
 * @param key1 Pointer to the first key.
 * @param key2 Pointer to the second key.
 * @param key_size Size of the keys.
 * @return Comparison result.
 */
int compare_string(const void *key1, const void *key2, size_t key_size);

void debug();

#endif //AEDSII_HTFACILITIES_H
