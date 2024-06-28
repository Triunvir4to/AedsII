#include "HashTable.h"
#include "HashTableIterator.h"
#include "HTFacilities.h"
#include <stdio.h>

int main() {
    HashTable *string_table = newHash(sizeof(char*), sizeof(int), hash_string, compare_string);

    const char *keys[] = {"apple", "banana", "cherry", "date", "elderberry", "fig", "grape", "honeydew"};
    int values[] = {1, 2, 3, 4, 5, 6, 7, 8};

    for (int i = 0; i < 8; ++i) {
        HTinsert(string_table, keys[i], &values[i]);
    }

    HashTableIterator iterator = newHTIterator(string_table);

    while (nextHTI(&iterator))
        printf("Key: %s, Value: %d\n", (char*)iterator.key, *(int*)iterator.value);

    printf("\ntable size: %zu\n", string_table->capacity);

    free_table(string_table);

    return 0;
}