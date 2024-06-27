cmake_minimum_required(VERSION 3.22)

project(AEDSII)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY /build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY /build)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY /build)

add_executable(AEDSII
        HashTable.c
        HashTableIterator.c
        main.c
)