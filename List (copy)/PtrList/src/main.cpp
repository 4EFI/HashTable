
#include <stdio.h>
#include "list_ptr.h"
#include "LOG.h"

int main()
{
    List      list = {0};
    ListCtor( &list );

    ListNode* node1 = ListPushBack( &list, 15 );
    ListDump( &list, "PushBack( 15 )" ); 

    ListNode* node2 = ListPushBack( &list, 25 );
    ListDump( &list, "PushBack( 25 )" ); 

    ListRemove( &list, node2 );
    ListDump( &list, "ListRemove( %p )", node2 );

    ListNode* node3 = ListPushBack( &list, 50 );
    ListDump( &list, "PushBack( 50 )" );

    ListDtor( &list );
}