
#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "list_ptr.h" 
#include "LOG.h" 

//-----------------------------------------------------------------------------

const char* FileListDumpName = "list_dump.html";
FILE* FileListDump = fopen( FileListDumpName, "w" );

#define HEAD list->poisonNode.next
#define TAIL list->poisonNode.prev

//-----------------------------------------------------------------------------

//  List functions
//-----------------------------------------------------------------------------

int ListCtor( List* list )
{
    if( list == NULL ) return 0;

    list->size = 0;

    HEAD = &list->poisonNode;
    list->poisonNode.elem = ListPoison;
    TAIL = &list->poisonNode;

    return 1;
}

//-----------------------------------------------------------------------------

int ListDtor( List* list )
{
    if( list == NULL ) return 0;

    list->size = ListPoison;

    return 1;
}

//-----------------------------------------------------------------------------

int GraphVizListInfo( List* list, FILE* tempFile )
{
    if( list == NULL || tempFile == NULL ) return 0;
    
    fprintf( tempFile, "info[ shape = record, style = \"filled\", fillcolor = \"lightyellow\", "
                       "label = \" head = %p | tail = %p | size = %d \" ];\n",  
                        ListHead( list ), ListTail( list ), list->size );

    return 1;
}

//-----------------------------------------------------------------------------

int GraphVizList( List* list, FILE* tempFile )
{
    if( list == NULL || tempFile == NULL ) return 0;
    
    // Create null-poison node
    fprintf( tempFile, "node%p[ shape = record, style = \"filled\", fillcolor = \"#FF7373\", "
                       "label = \"<p> prev = %p | <d> %p[0]\\nPOISON | <n> next = %p\" ];\n", 
                        &list->poisonNode,
                        list->poisonNode.prev, &list->poisonNode, list->poisonNode.next );
    

    ListNode* curNode = ListHead( list );
    for( int i = 1; i <= list->size; i++ )
    {   
        fprintf( tempFile, "node%p[ shape = record, style = \"filled\", fillcolor = \"#B0F0F0\", " 
                           "label = \"<p> prev = %p | <d> %p[%d] \\n %d | <n> next = %p\" ];\n", 
                            curNode, curNode->prev, curNode, i, curNode->elem, curNode->next );                             

        curNode = curNode->next;
    }

    
    // Connect nodes with invis edges
    fprintf( tempFile, "edge[ style = invis ]" );
    curNode = ListHead( list );
    for(int i = 1; i <= list->size; i++ )
    {
        // Connect nodes empty edges 
        fprintf( tempFile, "node%p -> node%p [ style = invis ];", curNode->prev, curNode );

        curNode = curNode->next;
    }

    
    // Connect prev/next to data 
    fprintf( tempFile, "graph[ nodesep = 1, splines = ortho ];\n" );
    curNode = ListHead( list );
    for( int i = 1; i <= list->size; i++ )
    {        
        fprintf( tempFile, "node%p -> node%p [ style = dashed, constraint = false ];", curNode, curNode->prev );
        fprintf( tempFile, "node%p -> node%p [ style = solid,  constraint = false ];", curNode, curNode->next );

        curNode = curNode->next;
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

        GraphVizList( list, tempDotFile );
        GraphVizListInfo   ( list, tempDotFile );
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

int ListDump( List* list, const char* str, ... )
{
    if( list == NULL ) return 0;

    va_list   arg = {0};
    va_start( arg, str );

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

    va_end( arg );
    return 1;
}

//-----------------------------------------------------------------------------

ListNode* ListHead( List* list )
{
    return HEAD;
}

//-----------------------------------------------------------------------------

ListNode* ListTail( List* list )
{
    return TAIL;
}

//-----------------------------------------------------------------------------

ListNode* ListPushBack( List* list, Elem_t val )
{
    if( list == NULL ) return 0;

    return ListInsert( list, ListTail( list ), val );
}

//-----------------------------------------------------------------------------

ListNode* ListPopBack( List* list, Elem_t val )
{
    if( list == NULL ) return 0;

    return ListInsert( list, ListHead( list )->prev, val );
}

//-----------------------------------------------------------------------------

ListNode*
ListInsert( List*     list,  //!< This is a list 
            ListNode* pos,   //!< This is a pos
            Elem_t    val )  //!< This is a CRINGE
{
    if( list == NULL ) return 0;

    ListNode* tempNode = ( ListNode* )calloc( 1, sizeof( ListNode ) );

    // Set value
    tempNode->elem = val;

    tempNode->prev = pos;
    tempNode->next = pos->next;    

    pos->next->prev = tempNode;
    pos->next       = tempNode;

    list->size++;

    return tempNode;
}

//-----------------------------------------------------------------------------

int ListRemove( List* list, ListNode* node )
{
    if( list == NULL ) return 0;

    ListNode* prevNode = node->prev;
    ListNode* nextNode = node->next;

    prevNode->next = nextNode;
    nextNode->prev = prevNode;

    free( node );
    list->size--;

    return 1;
}

//-----------------------------------------------------------------------------

int ListGetLogicalPosByPtr( List* list, ListNode* pos )
{
    if( list == NULL ) return 0;

    ListNode* curPos = ListHead( list );
    
    for( int i = 1; i <= list->size; i++ )
    {
        if( curPos == pos ) return i;

        curPos = curPos->next;
    }

    return 0;
}

//-----------------------------------------------------------------------------

ListNode* ListGetPtrByLogicalPos( List* list, int pos )
{
    if( list == NULL ) return 0;

    ListNode* curPos = ListHead( list );

    for( int i = 1; i <= list->size; i++ )
    {
        if( i == pos ) return curPos;
        
        curPos = curPos->next;
    }

    return NULL;
}

//-----------------------------------------------------------------------------

ListNode* ListFindElemByValue( List* list, Elem_t value )
{
    if( list == NULL ) return 0;

    ListNode* curPos = ListHead( list );

    while( true )
    {
        if( curPos->elem == value ) return curPos;
        
        if( curPos == ListTail( list ) ) return 0;

        curPos = curPos->next;  
    }

    return 0;
}

//  End List functions
//-----------------------------------------------------------------------------
