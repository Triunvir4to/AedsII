cmake_minimum_required(VERSION 3.22)

project(AEDSII)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_executable(AEDSII
        HashTable.c
        HTFacilities.c
        HashTableIterator.c
        main.c
)

target_include_directories(AEDSII PRIVATE .)
