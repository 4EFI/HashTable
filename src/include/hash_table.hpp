
#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "config.h"

struct List;

//-----------------------------------------------------------------------------

struct HashTable
{
    List*  arr;
    size_t size;

    size_t (*hash_function)( Elem_t );
};

//-----------------------------------------------------------------------------

size_t GetConstHash( Elem_t elem );

int HashTableCtor( HashTable* hash_table, int size );
int HashTableDtor( HashTable* hash_table );

int HashTableAddText( HashTable* hash_table, const char* text );

int HashTableSetHashFunction( HashTable* hash_table, size_t (*hash_function)( Elem_t ) );

size_t HashTablePushWord( HashTable* hash_table, Elem_t elem );
size_t HashTableFindWord( HashTable* hash_table, Elem_t elem );


//-----------------------------------------------------------------------------

#endif
