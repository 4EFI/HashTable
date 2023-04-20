
#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/stat.h>

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

int HashTableAddText()
{
    return 1;
}

//-----------------------------------------------------------------------------

int HashTableSetHashFunction( HashTable* hash_table, size_t (*hash_function)( Elem_t ) )
{
    hash_table->hash_function = hash_function;

    return 1;
}

//-----------------------------------------------------------------------------

size_t HashTablePushWord( HashTable* hash_table, Elem_t elem )
{
    size_t hash = hash_table->hash_function( elem );
    size_t size = hash_table->size;

    size_t i = hash % size;

    ListPushBack( &hash_table->arr[ i ], elem );
    
    return i;
}

//-----------------------------------------------------------------------------

size_t HashTableFindWord( HashTable* hash_table, Elem_t elem )
{
    size_t hash = hash_table->hash_function( elem );
    size_t ind  = hash % hash_table->size;

    size_t list_size = hash_table->arr[ind].size;

    for( size_t i = 1; i <= list_size; i++ )
    {
        if( !strcmp( hash_table->arr[ind].nodes[i].elem, elem) ) 
        {
            return 1;
        }
    }
    
    return 0;
}

//-----------------------------------------------------------------------------

#pragma GCC diagnostic ignored "-Wunused-parameter"

size_t GetConstHash( Elem_t elem )
{        
    return 1;
}

#pragma GCC diagnostic warning "-Wunused-parameter"

//-----------------------------------------------------------------------------


// File Functions

long int GetFileSizeFromStat( FILE* file );
long int ReadAllFile        ( FILE* file, char** str );

//-----------------------------------------------------------------------------

long int GetFileSizeFromStat( FILE* file ) 
{
    //{ ASSERT
    assert( file != NULL );
    //}

    struct stat fileInfo = {};

    fstat( fileno( file ), &fileInfo );

    long int fileSize = fileInfo.st_size;

    return fileSize;
} 

//-----------------------------------------------------------------------------

long int ReadAllFile( FILE* file, char** str )
{
    // ASSERT
    assert (file != NULL);
    assert (str  != NULL);
    //

    long int fileSize = GetFileSizeFromStat( file );
    
    *str = ( char* )calloc( sizeof( char ), fileSize + 1 );

    long int rightRead = fread( *str, sizeof( char ), fileSize, file );

#pragma GCC diagnostic ignored "-Wunused-result"

    if( rightRead < fileSize )
        realloc( str, sizeof( char ) * ( rightRead + 1 ) ); // Windows specific, \r remove

#pragma GCC diagnostic ignored "-Wunused-result"

    (*str)[rightRead] = '\0';

    return rightRead;
}

//-----------------------------------------------------------------------------