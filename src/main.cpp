
#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <immintrin.h>

#include "list.h"
#include "hash_table.hpp"

//-----------------------------------------------------------------------------

extern "C" size_t GetBkdrHashAsm( Elem_t elem );

int MakeAllHashFunctionsCSV( const HashFunction* hash_functions, int size, const char* text );

//-----------------------------------------------------------------------------

int main()
{    
    const char* input_file_name = "res/hamlet_avx.txt";
    FILE*       input_file      = fopen( input_file_name, "rb" );

    if( !input_file ) { printf( "Error open file \"%s\"...\n", input_file_name ); return -1; } 
    
    char* text = NULL;
    ReadAllFile( input_file, &text );
    
    MakeAllHashFunctionsCSV( HashFunctions, NumHashFunctions, text );

    HashTable hash_table = {};
    HashTableCtor( &hash_table, HashTableSize, GetCrc32HashAVX ); 

    HashTableLoadTextAVX( &hash_table, text );

    HashTableFindAllWords( &hash_table );

    HashTableDtor( &hash_table );
}

//-----------------------------------------------------------------------------

int MakeAllHashFunctionsCSV( const HashFunction* hash_functions, int size, const char* text )
{
    for( int i = 0; i < size; i++ )
    {
        HashTable hash_table = {};
        HashTableCtor( &hash_table, 
                        hash_functions[i].hash_table_size, 
                        hash_functions[i].function ); 
        
        HashTableLoadTextAVX( &hash_table, text );

        FILE* output_file = fopen( hash_functions[i].name_output_file, "w" );
        HashTableMakeCSV( &hash_table, output_file );

        HashTableDtor( &hash_table );
    }

    return 1;
}

//-----------------------------------------------------------------------------

