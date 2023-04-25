
#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/stat.h>
#include <immintrin.h>

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
    for( size_t i = 0; i < hash_table->size; i++ )
    {
        ListDtor( &hash_table->arr[i] );
    }      

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
        char* word = ( char* )calloc( 32, sizeof( char ) );

        // skip not letter
        int num_skip = 0;
        sscanf( text, "%[^a-zA-Z]%n", skip, &num_skip );
        
        text += num_skip;

        // scanf word from text
        int num_read = 0;
        sscanf( text, "%[a-zA-Z]%n", word, &num_read );

        if( num_read == 0 ) return 1;

        text += num_read;
        
        HashTablePushWord( hash_table, word );
    }
    
    return 1;
}

int HashTableLoadTextAVX( HashTable* hash_table, const char* text )
{
    while( true )
    {
        if( *text == '\0' ) break;

        HashTablePushWordAVX( hash_table, ( char* )text );

        text += 32;
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
    if( HashTableFindWord( hash_table, elem ) ) return -1;
    
    size_t hash = hash_table->hash_function( elem );
    size_t size = hash_table->size;

    size_t i = hash % size;

    ListPushBack( &hash_table->arr[ i ], elem );

    return i;
}

size_t HashTablePushWordAVX( HashTable* hash_table, Elem_t elem )
{
    if( HashTableFindWordAVX( hash_table, elem ) ) return -1;
    
    size_t hash = hash_table->hash_function( elem );
    size_t size = hash_table->size;

    size_t i = hash % size;

    ListPushBack( &hash_table->arr[ i ], elem );

    return i;
}

//-----------------------------------------------------------------------------

size_t HashTableFindWord( HashTable* hash_table, Elem_t elem )
{
    size_t hash      = hash_table->hash_function( elem );
    size_t curr_list = hash % hash_table->size;

    size_t list_size = hash_table->arr[curr_list].size;

    for( size_t i = 1; i <= list_size; i++ )
    {
        Elem_t curr_elem = hash_table->arr[curr_list].nodes[i].elem;

        if( !strcmp( curr_elem, elem) ) 
        {
            return 1;       
        }
    }
    
    return 0;
}

inline int StrCmpAsm( const char* str_1, const char* str_2 )
{
    int res = 0;
    asm \
    (
        ".intel_syntax noprefix;"
        ".loop:"
            "mov r11b, [rsi];"
            "mov r10b, [rdi];"
    	    "cmp r10b, 0;"
    	    "je .done;"
    	    "cmp r11b, 0;"
    	    "je .done;"
    	    "cmp r11b, r10b;"
    	    "jne .done;"
    	    "inc rdi;"  
    	    "inc rsi;"
    	    "jmp .loop;"
        
        ".done:"
        "movzx rax, r10b;"
        "movzx rbx, r11b;"
    	"sub   rax, rbx;"
        
        ".att_syntax prefix;"
        : "=a" ( res )
    );
    
    return res;
}

inline size_t StrCmpAVX( const char* str_1, const char* str_2 )
{
    __m256i str_1_vec = _mm256_load_si256( ( __m256i* )str_1 );
    __m256i str_2_vec = _mm256_load_si256( ( __m256i* )str_2 );
    __m256i cmp       = _mm256_cmpeq_epi8( str_1_vec, str_2_vec );

    int mask = _mm256_movemask_epi8( cmp );
    if( mask == 0xffffffff ) return 0;
    else                     return 1;
}

size_t HashTableFindWordAVX( HashTable* hash_table, Elem_t elem )
{
    size_t hash      = hash_table->hash_function( elem );
    size_t curr_list = hash % hash_table->size;

    size_t list_size = hash_table->arr[curr_list].size;

    for( size_t i = 1; i <= list_size; i++ )
    {
        Elem_t curr_elem = hash_table->arr[curr_list].nodes[i].elem;

        if( !StrCmpAVX( curr_elem, elem) ) 
        {
            return 1;       
        }     
    }
    
    return 0;
}

//-----------------------------------------------------------------------------

//  This function searches all the words that are 
//  in the table for counting the search time

size_t HashTableFindAllWords( HashTable* hash_table )
{
    for( size_t curr_list = 0; curr_list < hash_table->size; curr_list++ )
    {
        for( int i = 1;   i <= hash_table->arr[curr_list].size; i++ )
        {
            Elem_t curr_elem = hash_table->arr[curr_list].nodes[i].elem;

            HashTableFindWordAVX( hash_table, curr_elem );
        }
    }

    return 1;
}

//-----------------------------------------------------------------------------


// Hash functions
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

size_t GetRolHash( Elem_t elem )
{
    size_t hash  = 0;

    for( size_t i = 0; ; i++ )
    {
        if( !elem[i] ) break;

        hash = ROL( hash, 1 ) ^ elem[i];
    }

    return hash;
}

//-----------------------------------------------------------------------------

size_t GetRorHash( Elem_t elem )
{
    size_t hash  = 0;

    for( size_t i = 0; ; i++ )
    {
        if( !elem[i] ) break;

        hash = ROR( hash, 1 ) ^ elem[i];
    }

    return hash;
}

//-----------------------------------------------------------------------------

size_t GetBkdrHash( Elem_t elem )
{
   size_t seed = 31; /* 31 131 1313 13131 131313 etc.. */
   size_t hash = 0;

   for( size_t i = 0; ; i++ )
   {
        if( !elem[i] ) break;
        
        hash = ( hash * seed ) + ( elem[i] );
   }

   return hash;
}

//-----------------------------------------------------------------------------

size_t GetCrc32Hash( Elem_t elem ) 
{
    size_t crc  = 0xFFFFFFFF; 
    size_t mask = 0;

    for( size_t i = 0; ; i++ )
    {
        if( !elem[i] ) break;
        
        crc = crc ^ elem[i];

        for( size_t j = 0; j < 8; j++ )     // Do 8 times
        {    
            mask = -( crc &  1 );
            crc  =  ( crc >> 1 ) ^ ( 0xEDB88320 & mask );
        }
    }
    return ~crc;
}

//-----------------------------------------------------------------------------


// Other functions
//-----------------------------------------------------------------------------

size_t ROL( size_t num, size_t shift )
{
    return ( num << shift ) | ( num >> ( 8 * sizeof( size_t ) - shift ) );
}

//-----------------------------------------------------------------------------

size_t ROR( size_t num, size_t shift )
{
    return ( num >> shift ) | ( num << ( 8 * sizeof( size_t ) - shift ) );
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
    
    *str = ( char* )aligned_alloc( 32, sizeof( char ) * (fileSize + 1) );

    long int rightRead = fread( *str, sizeof( char ), fileSize, file );

    #pragma GCC diagnostic ignored "-Wunused-result"

    if( rightRead < fileSize )
        realloc( str, sizeof( char ) * ( rightRead + 1 ) ); // Windows specific, \r remove

    #pragma GCC diagnostic ignored "-Wunused-result"

    (*str)[rightRead] = '\0';

    return rightRead;
}

//-----------------------------------------------------------------------------