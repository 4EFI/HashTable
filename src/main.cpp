
#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "hash_table.hpp"

//-----------------------------------------------------------------------------

int main()
{
    HashTable hash_table = {};
    HashTableCtor( &hash_table, 1001 ); 

    HashTableSetHashFunction( &hash_table, GetConstHash );

    FILE* input_file = fopen( "res/hamlet.txt", "rb" );
    if(  !input_file ) { printf( "Error open file...\n" ); return -1; } 
    
    char* text = NULL;
    ReadAllFile( input_file, &text );

    HashTableAddText( &hash_table, text );

    printf( "%ld\n", HashTableFindWord( &hash_table, "Vlad" ) );

    // ListDump( &hash_table.arr[1], GRAPH_VIZ, "%d: ", 1 );

    HashTableDtor( &hash_table );
}

//-----------------------------------------------------------------------------
