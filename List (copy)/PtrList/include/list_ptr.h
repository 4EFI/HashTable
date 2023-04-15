#ifndef LIST_H
#define LIST_H

#include "config.h"

//-----------------------------------------------------------------------------

static const int MaxStrLen = 255;

static const Elem_t ListPoison = 0x5E32DEAD;

extern const char* FileListDumpName;
extern FILE* FileListDump;

//-----------------------------------------------------------------------------

struct ListNode
{
    Elem_t    elem;
    ListNode* prev;
    ListNode* next;
};

struct List
{  
    ListNode poisonNode;
    
    int size;   
};

int ListCtor( List* list );
int ListDtor( List* list );

int GraphVizList    ( List* list, FILE* tempFile );
int GraphVizListInfo( List* list, FILE* tempFile );

FILE* CreateListDumpDotFile( List* list, const char* fileName );

int ListDump( List* list, const char* str = NULL, ... );

ListNode* ListHead( List* list );
ListNode* ListTail( List* list );

ListNode* ListPushBack( List* list, Elem_t val );
ListNode* ListPopBack ( List* list, Elem_t val );

ListNode* ListInsert( List* list, ListNode* pos, Elem_t val );

int ListRemove( List* list, ListNode* pos );

int       ListGetLogicalPosByPtr( List* list, ListNode* pos );
ListNode* ListGetPtrByLogicalPos( List* list, int       pos );

ListNode* ListFindElemByValue   ( List* list, Elem_t value );

//-----------------------------------------------------------------------------

int CreateGraphVizImg( const char* dotFileName, const char* fileName, const char* fileType );

//-----------------------------------------------------------------------------

#endif