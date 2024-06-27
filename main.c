#include "HashTable.h"
#include <stdio.h>

int main() {
    HashTable *int_table = newHash(sizeof(int), sizeof(int), hash_int, compare_int);

    int key1 = 5;
    int value1 = 10;
    HTinsert(int_table, &key1, &value1);

    int search_key = 5;
    int *found_value = search(int_table, &search_key);
    if (found_value)
        printf("Found value: %d\n", *found_value);
    else
        printf("Value not found\n");


    free_table(int_table);

    // Create a hash table for char to int
    HashTable *char_table = newHash(sizeof(char), sizeof(int), hash_char, compare_char);

    char key2 = 'a';
    int value2 = 20;
    HTinsert(char_table, &key2, &value2);

    char search_key2 = 'a';
    int *found_value2 = search(char_table, &search_key2);
    if (found_value2)
        printf("Found value: %d\n", *found_value2);
    else
        printf("Value not found\n");


    free_table(char_table);

    return 0;
}