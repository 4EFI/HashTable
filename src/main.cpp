
#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "hash_table.hpp"

//-----------------------------------------------------------------------------

int main()
{
    HashTable hash_table = {};
    HashTableCtor( &hash_table, 103 ); 

    HashTableSetHashFunction( &hash_table, GetConstHash );

    HashTablePushWord( &hash_table, "Vlad" );

    printf( "%d\n", HashTableFindWord( &hash_table, "Vlad" ) );

    // ListDump( &hash_table.arr[1], GRAPH_VIZ, "%d: ", 1 );

    HashTableDtor( &hash_table );
}

//-----------------------------------------------------------------------------
