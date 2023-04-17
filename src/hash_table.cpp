
#include "config.h"

#include <stdio.h>
#include <stdlib.h>

#include "hash_table.hpp"
#include "list.h"

//-----------------------------------------------------------------------------

int HashTableCtor( HashTable* hash_table, int size )
{
    if( size <= 0 ) size = 1;

    hash_table->size = size;
    hash_table->arr  = ( List* )calloc( size, sizeof( List ) );

    if( !hash_table ) return 0; 
  
    for( int i = 0; i < size; i++ )
    {
        ListCtor( &hash_table->arr[i], 1 );
    }      
    
    return 1;
}

//-----------------------------------------------------------------------------

int HashTableDtor( HashTable* hash_table )
{
    hash_table->size = 0;
    free( hash_table->arr ); 
    hash_table->arr = NULL;

    return 1;
}

//-----------------------------------------------------------------------------

int HashTablePushWord( HashTable* hash_table, Elem_t elem, size_t (*hash_function)( Elem_t ) )
{
    size_t hash = hash_function( elem );
    size_t size = hash_table->size;

    int i = hash % size;

    ListPushBack( &hash_table->arr[ i ], elem );
    
    return i;
}

//-----------------------------------------------------------------------------

size_t GetHash( Elem_t elem )
{    
    return 1;
}

//-----------------------------------------------------------------------------