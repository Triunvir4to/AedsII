#include "HashTableIterator.h"

HashTableIterator newHTIterator(HashTable *table) {
    HashTableIterator it;
    it._table = table;
    it._index = 0;
    return it;
}

bool nextHTI(HashTableIterator *it) {
    HashTable *table = it->_table;
    while (it->_index < table->capacity) {
        size_t i = it->_index;
        it->_index++;
        if (table->entries[i] != NULL && table->entries[i]->key != NULL) {
            // Found next non-empty item, update iterator key and value.
            HashEntry *entry = table->entries[i];
            it->key = entry->key;
            it->value = entry->value;
            return true;
        }
    }
    return false;
}