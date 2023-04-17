
#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "hash_table.hpp"

int main()
{
    HashTable hash_table = {};
    HashTableCtor( &hash_table, 10 );  

    

    HashTableDtor( &hash_table );
}