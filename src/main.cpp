
#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <immintrin.h>

#include "list.h"
#include "hash_table.hpp"

//-----------------------------------------------------------------------------

extern "C" size_t GetBkdrHashAsm( Elem_t elem );

//-----------------------------------------------------------------------------

int main()
{    
    HashTable hash_table = {};
    HashTableCtor( &hash_table, 1001 ); 

    HashTableSetHashFunction( &hash_table, GetCrc32HashAVX );

    FILE* input_file = fopen( "res/hamlet_avx.txt", "rb" );
    if(  !input_file ) { printf( "Error open file...\n" ); return -1; } 
    
    char* text = NULL;
    ReadAllFile( input_file, &text );

    HashTableLoadTextAVX( &hash_table, text );

    HashTableFindAllWords( &hash_table );

    FILE* output_file = fopen( "res/output_crc32_avx.csv", "w" );
    HashTableMakeCSV( &hash_table, output_file );

    HashTableDtor( &hash_table );
}

//-----------------------------------------------------------------------------

