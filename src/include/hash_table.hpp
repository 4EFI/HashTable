
#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "config.h"

struct List;

//-----------------------------------------------------------------------------

struct HashTable
{
    List* arr;
    int   size;
};

//-----------------------------------------------------------------------------

int HashTableCtor( HashTable* hash_table, int size );
int HashTableDtor( HashTable* hash_table );

//-----------------------------------------------------------------------------

#endif
