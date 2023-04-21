
#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "hash_table.hpp"

//-----------------------------------------------------------------------------

int main()
{
    HashTable hash_table = {};
    HashTableCtor( &hash_table, 1000 ); 

    HashTableSetHashFunction( &hash_table, GetSumAsciiHash );

    FILE* input_file = fopen( "res/hamlet.txt", "rb" );
    if(  !input_file ) { printf( "Error open file...\n" ); return -1; } 
    
    char* text = NULL;
    ReadAllFile( input_file, &text );

    HashTableLoadText( &hash_table, text );

    FILE* output_file = fopen( "res/output_sum_ascii.csv", "w" );
    HashTableMakeCSV( &hash_table, output_file );

    // ListDump( &hash_table.arr[0], GRAPH_VIZ, "%d: ", 1 );

    HashTableDtor( &hash_table );
}

//-----------------------------------------------------------------------------
