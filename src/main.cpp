
#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "hash_table.hpp"

int main()
{
    HashTable hash_table = {};
    HashTableCtor( &hash_table, 1003 ); 

    // ListDump( &hash_table.arr[1], GRAPH_VIZ, "%d: ", 1 );

    HashTableDtor( &hash_table );
}