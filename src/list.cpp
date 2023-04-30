
#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "list.h" 

int StrCmpAVX( const char* str_1, const char* str_2 );

//-----------------------------------------------------------------------------

const char* FileListDumpName = "list_dump.html";
FILE* FileListDump = fopen( FileListDumpName, "w" );

//-----------------------------------------------------------------------------

int ListResize( List* list ); 

int FillListNodeArr( ListNode arr[], int iBegin, int iEnd );

//  List functions
//-----------------------------------------------------------------------------

int ListCtor( List* list, int size )
{
    if( list == NULL ) return 0;

    if(size < 1) size = 1;

    list->coeffResize = 2;
    list->size = 0;
    list->free = 1;

    list->isSorted = true;

    list->nodes = ( ListNode* )calloc( size , sizeof( ListNode ) );

    if( list->nodes != NULL ) list->capacity = size;
    else return -1;

    PrepareListNodeArr( list->nodes, list->capacity );

    return 1;
}

//-----------------------------------------------------------------------------

int ListDtor( List* list )
{
    if( list == NULL ) return 0;

    list->coeffResize = 0;
    list->free        = 0;

    free( list->nodes );
    list->nodes = NULL;

    list->size = 0;

    return 1;
}

//-----------------------------------------------------------------------------

int PrepareListNodeArr( ListNode arr[], int size )
{
    if( arr == NULL ) return 0;

    0[arr].prev = 0;
    0[arr].elem = ListPoison;
    0[arr].next = 0;
    
    FillListNodeArr( arr, 1, size - 1 );

    return 1;
}

//-----------------------------------------------------------------------------

int FillListNodeArr( ListNode arr[], int iBegin, int iEnd )
{
    if( arr == NULL ) return 0;

    for( int i = iBegin; i <= iEnd; i++ )
    {
        arr[i].prev = -1;
        arr[i].elem = 0;
        arr[i].next = i + 1;
    }

    return 1;
}

//-----------------------------------------------------------------------------

int PrintListNodeArr( ListNode arr[], int size )
{
    if( arr == NULL ) return 0;

    printf( "+-----+-------------+------+------+\n" );
    printf( "|   i |     element | next | prev |\n" );
    printf( "+-----+-------------+------+------+\n" );

    for( int i = 0; i < size; i++ )
    {
        printf( "| %3d | %11s | %4d | %4d |\n", i, arr[i].elem, arr[i].next, arr[i].prev );    
    }

    printf( "+-----+-------------+------+------+\n" );

    return 1;  
}

//-----------------------------------------------------------------------------

int GraphVizListInfo( List* list, FILE* tempFile )
{
    if( list == NULL || tempFile == NULL ) return 0;
    
    fprintf( tempFile, "info[ shape = record, style = \"filled\", fillcolor = \"lightyellow\", "
                       "label = \" head = %d | tail = %d | free = %d | capacity = %d | size = %d | isSorted = %d \" ];\n",  
                        ListHead( list ), ListTail( list ), list->free, list->capacity, list->size, list->isSorted );

#undef DEF_INFO

    return 1;
}

//-----------------------------------------------------------------------------

int GraphVizListNodeArr( ListNode arr[], int size, FILE* tempFile )
{
    if( arr == NULL ) return 0;
    
    // Create null-poison node
    fprintf( tempFile, "node0[ shape = record, style = \"filled\", fillcolor = \"#FF9090\", "
                       "label = \"<p> prev = %d | <d> data[0]\\nPOISON | <n> next = %d\" ];\n", 
                        arr[0].prev, arr[0].next );
    
    for( int i = 1; i < size; i++ )
    {
        // Create node 
        fprintf( tempFile, "node%d[ shape = record, style = \"filled\", fillcolor = \"%s\", " 
                           "label = \"<p> prev = %d | <d> data[%d] \\n %s | <n> next = %d\" ];\n", 
                            i, arr[i].prev == -1 ? "lightgreen" : "#B0F0F0", 
                               arr[i].prev, i, arr[i].elem, arr[i].next );
    }

    
    // Connect nodes with invis edges
    fprintf( tempFile, "edge[ style = invis ]" );
    for( int i = 1; i < size; i++ )
    {
        // Connect nodes empty edges 
        fprintf( tempFile, "node%d -> node%d [ style = invis ];", i - 1, i );
    }

    // Connect prev/next to data 
    fprintf( tempFile, "graph[ nodesep = 1, splines = ortho ];\n" );
    for( int i = 1; i < size; i++ )
    {
        if( arr[i].prev == -1 || arr[i].next == -1 ) continue;
        
        if( arr[i].prev != 0 )
        {
            fprintf( tempFile, "node%d -> node%d [ style = dashed, constraint = false ];", i, arr[i].prev );
        }
        if( arr[i].next != 0 )
        {
            fprintf( tempFile, "node%d -> node%d [ style = solid,  constraint = false ];", i, arr[i].next );
        }
    }

    return 1;
}

//-----------------------------------------------------------------------------

FILE* CreateListDumpDotFile( List* list, const char* fileName )
{
    if( fileName == NULL ) return NULL;
    
    FILE* tempDotFile = fopen( fileName, "w" );

    fprintf( tempDotFile, "digraph ListDump\n" );
    fprintf( tempDotFile, "{\n" );
    {
        fprintf( tempDotFile, "rankdir = LR;\n" );

        GraphVizListNodeArr( list->nodes, list->capacity, tempDotFile );
        GraphVizListInfo   ( list, /*                  */ tempDotFile );
    }
    fprintf( tempDotFile, "}\n" );

    return tempDotFile;
}

//-----------------------------------------------------------------------------

int CreateGraphVizImg( const char* dotFileName, const char* fileName, const char* fileType )
{
    if(dotFileName == NULL || fileName == NULL || fileType == NULL) return 0; 

    char cmd[MaxStrLen] = "";

    sprintf( cmd, "dot %s -T %s -o %s", dotFileName, fileType, fileName );
    system(  cmd  );

    return 1;
}

//-----------------------------------------------------------------------------

int ListDump( List* list, int typeDump, const char* str, ... )
{
    if( list == NULL ) return 0;

    va_list   arg = {};
    va_start( arg, str );

    if/* */( typeDump == TypeListDump::CONSOLE )
    {
        /*             */  fprintf( stdout, "%*sListDump%*s\n", 13, "", 13, "" );
        /*             */ vfprintf( stdout, str, arg );
        if( str != NULL )  fprintf( stdout, "\n" );

        PrintListNodeArr( list->nodes, list->capacity );
    }
    else if( typeDump == TypeListDump::GRAPH_VIZ )
    {
        // Create dot file
        const char tempDotFileName[] = "graphVizTemp.dot";
        FILE*      tempDotFile = CreateListDumpDotFile( list, tempDotFileName );  
        fclose(    tempDotFile    );


        static int graphNum = 0;
        char       graphName[MaxStrLen] = "";
        sprintf(   graphName, "img/graph%d.png", graphNum++ );

        CreateGraphVizImg( tempDotFileName, graphName, "png" );


        // Delete temp file
        remove( tempDotFileName );

        // Create html file
        /*            */  fprintf( FileListDump, "<pre>" );

        /*            */  fprintf( FileListDump, "<font size = 4>" );
        /*            */ vfprintf( FileListDump, str, arg );
        /*            */  fprintf( FileListDump, "</font>" );
        if( str != NULL ) fprintf( FileListDump, "\n\n" );

         fprintf( FileListDump, "<img src = \"%s\", style = \" max-width: 95vw \">", graphName );
         fprintf( FileListDump, "<hr>" );
    }

    va_end( arg );
    return 1;
}

//-----------------------------------------------------------------------------
//  Local function

int ListResize( List* list )
{
    if( list == NULL ) return 0;

    int numResize = list->capacity;

    if( size_t( list->size ) <= size_t( (list->capacity - 1) / (2*list->coeffResize) ) )
    {
        numResize /= list->coeffResize;
    }

    if( list->free >= numResize )
    {
        numResize *= list->coeffResize;
    }

    if( numResize < 1 ) numResize = 1;
 
    list->nodes = ( ListNode* )realloc( list->nodes, numResize * sizeof( ListNode ) );

    FillListNodeArr( list->nodes, list->capacity, numResize - 1 );

    list->capacity = numResize;

    return 1;
}

//-----------------------------------------------------------------------------

int ListHead( List* list )
{
    if( list == NULL ) return 0;

    return list->nodes[0].next;
}

//-----------------------------------------------------------------------------

int ListTail( List* list )
{
    if( list == NULL ) return 0;

    return list->nodes[0].prev;
}

//-----------------------------------------------------------------------------

int ListPushBack( List* list, Elem_t val )
{
    if( list == NULL ) return 0;

    return ListInsert( list, ListTail( list ), val );
}

//-----------------------------------------------------------------------------

int ListPopBack( List* list, Elem_t val )
{
    if( list == NULL ) return 0;

    return ListInsert( list, list->nodes[ ListHead( list ) ].prev, val );
}

//-----------------------------------------------------------------------------

int ListInsert( List* list, int pos, Elem_t val )
{
    if( list == NULL ) return 0;

    ListResize( list );

    if( pos != ListTail( list ) ) list->isSorted = false;

    int tempFree = list->free;
    int newFree  = list->nodes[list->free].next;
    
    // Set value
    list->nodes[tempFree].elem = val;

    list->nodes[tempFree].prev = pos;
    list->nodes[tempFree].next = list->nodes[pos].next;

    list->nodes[list->nodes[pos].next].prev = tempFree;
    list->nodes[pos                  ].next = tempFree;

    list->size++;

    list->free = newFree;
    return tempFree;
}

//-----------------------------------------------------------------------------

Elem_t ListRemove( List* list, int pos )
{
    if( list == NULL ) return 0;

    size_t prevTemp = list->nodes[pos].prev;
    size_t nextTemp = list->nodes[pos].next;

    Elem_t elem = list->nodes[pos].elem;

    list->nodes[prevTemp].next = int( nextTemp );
    list->nodes[nextTemp].prev = int( prevTemp );

    list->nodes[pos].prev = -1;
    list->nodes[pos].elem = 0;
    list->nodes[pos].next = list->free;

    list->free = pos;
    list->size--;

    list->isSorted = false;

    return elem;
}

//-----------------------------------------------------------------------------

int ListLinearize( List* list )
{
    if( list == NULL ) return 0;

    ListNode* newNodes = ( ListNode* )calloc( list->capacity, sizeof( ListNode ) );

    int pos = ListHead( list );
    int i = 1;
    for( ; i <= list->size; i++ )
    {  
        newNodes[i].elem = list->nodes[pos].elem;
        newNodes[i].next = i + 1;
        newNodes[i].prev = i - 1;
                 
        pos = list->nodes[pos].next;
    }

    free( list->nodes );
    list->nodes    = newNodes; 
    
    TAIL = i - 1;
    HEAD = 1;

    list->nodes[TAIL].next = 0;
    list->nodes[HEAD].prev = 0;

    list->free = i;

    FillListNodeArr( list->nodes, list->free, list->capacity - 1 );

    list->isSorted = true;

    return 1;
}

//-----------------------------------------------------------------------------

int ListLogicalPosToPhysical( List* list, int desiredLogicalPos )
{
    if( list == NULL ) return 0;

    if( list->isSorted ) return desiredLogicalPos;
    
    int curPos = ListHead( list );

    for( int i = 1; ; i++ )
    {
        if( i      == desiredLogicalPos ) return curPos;
        if( curPos == ListTail( list )  ) return 0;
        
        curPos = list->nodes[curPos].next; 
    }

    return 0;
}

//-----------------------------------------------------------------------------

int ListFindElemByValue( List* list, Elem_t value, int ( *cmp_function )( Elem_t elem_1, Elem_t elem_2 ) )
{
    if( list == NULL ) return 0;

    int  curPos = ListHead( list );
    if( !curPos ) return 0;

    while( true )
    {        
        if( !StrCmpAVX( list->nodes[curPos].elem, value ) ) return curPos;
        
        if( curPos == ListTail( list ) ) return 0;

        curPos = list->nodes[curPos].next;  
    }

    return 0;
}

//  End List functions
//-----------------------------------------------------------------------------
