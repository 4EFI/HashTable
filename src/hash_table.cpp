
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
        ListCtor( &hash_table->arr[i], 2 );
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

int HashTableLoadText( HashTable* hash_table, const char* text )
{
    while( true )
    {
        char skip[256] = "";
        char* word = ( char* )calloc( 256, sizeof( char ) );

        // skip not letter
        int num_skip = 0;
        sscanf( text, "%[^a-zA-Z]%n", skip, &num_skip );
        
        text += num_skip;

        // scanf word from text
        int num_read = 0;
        sscanf( text, "%[a-zA-Z]%n", word, &num_read );

        if( num_read == 0 ) return 1;

        text += num_read;
        if( !HashTableFindWord( hash_table, word ) )
        {
            HashTablePushWord( hash_table, word );
        }
    }
    
    return 1;
}

//-----------------------------------------------------------------------------

int HashTableMakeCSV( HashTable* hash_table, FILE* output_file )
{
    for( size_t i = 0; i < hash_table->size; i++ )
    {
        fprintf( output_file, "%d ", hash_table->arr[i].size );
    }
    
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

size_t GetFirstAsciiHash( Elem_t elem )
{
    return elem[0];
}

//-----------------------------------------------------------------------------

size_t GetLenWordHash( Elem_t elem )
{
    return strlen( elem );
}

//-----------------------------------------------------------------------------

size_t GetSumAsciiHash( Elem_t elem )
{
    size_t sum = 0;
    
    for( size_t i = 0; ; i++ )
    {
        if( !elem[i] ) break;
        
        sum += elem[i];
    }
    
    return sum;
}

//-----------------------------------------------------------------------------


// File Functions
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