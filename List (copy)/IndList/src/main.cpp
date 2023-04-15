
#include <stdio.h>
#include "list.h"
#include "LOG.h"

int main()
{
    List      list = {0};
    ListCtor( &list, 5 );

    int typeListDump = TypeListDump::GRAPH_VIZ; 

    ListPushBack( &list, 15 );
    ListDump( &list, typeListDump, "PushBack( 15 )" ); 

    ListPushBack( &list, 25 );
    ListDump( &list, typeListDump, "PushBack( 25 )" );

    ListRemove( &list, 1 );
    ListDump( &list, typeListDump, "Remove( 1 )" ); 

    ListInsert( &list, 2, 50 );
    ListDump( &list, typeListDump, "Insert( 2, 50 )" );

    ListInsert( &list, 1, 100 );
    ListDump( &list, typeListDump, "Insert( 1, 100 )" );

    ListInsert( &list, 1, 300 );
    ListDump( &list, typeListDump, "Insert( 1, 300 )" );

    ListLinearize( &list );
    ListDump( &list, typeListDump, "ListLinearize()" );

    ListPopBack( &list, 1000 );
    ListDump( &list, typeListDump, "ListPopBack( 1000 )" );

    ListLinearize( &list );
    ListDump( &list, typeListDump, "ListLinearize()" );

    ListDtor( &list );
}