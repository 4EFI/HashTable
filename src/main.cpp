
#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "hash_table.hpp"

int main()
{
    HashTable hash_table = {};
    HashTableCtor( &hash_table, 97 ); 

    int i = HashTablePushWord( &hash_table, "Vlad" );

    ListDump( &hash_table.arr[i], GRAPH_VIZ, "%d: ", i );

    HashTableDtor( &hash_table );
}