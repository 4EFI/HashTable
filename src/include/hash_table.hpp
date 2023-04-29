
#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "config.h"

struct List;

//-----------------------------------------------------------------------------

struct HashTable
{
    List*  arr;
    size_t size;

    size_t (*hash_function)( Elem_t );
};

//-----------------------------------------------------------------------------

size_t GetConstHash     ( Elem_t elem );
size_t GetFirstAsciiHash( Elem_t elem );
size_t GetLenWordHash   ( Elem_t elem );
size_t GetSumAsciiHash  ( Elem_t elem );
size_t GetRolHash       ( Elem_t elem );
size_t GetRorHash       ( Elem_t elem );
size_t GetBkdrHash      ( Elem_t elem );
size_t GetCrc32Hash     ( Elem_t elem );
size_t GetCrc32HashAVX  ( Elem_t elem );

struct HashFunction
{
    size_t (*function)( Elem_t );
    
    const char* name_output_file;
};

const HashFunction HashFunctions[] = 
{
    { GetConstHash,      "res/output_const.csv" },
    { GetFirstAsciiHash, "res/output_first_ascii.csv" },
    { GetLenWordHash,    "res/output_len_word.csv" },
    { GetSumAsciiHash,   "res/output_sum_ascii.csv" },
    { GetRolHash,        "res/output_rol.csv" },
    { GetRorHash,        "res/output_ror.csv" },
    { GetBkdrHash,       "res/output_bkdr.csv" },
    { GetCrc32Hash,      "res/output_crc32.csv" }
};

const int NumHashFunctions = sizeof( HashFunctions ) / sizeof( HashFunction ); 

//-----------------------------------------------------------------------------

int HashTableCtor( HashTable* hash_table, int size );
int HashTableDtor( HashTable* hash_table );

int HashTableLoadText   ( HashTable* hash_table, const char* text );
int HashTableLoadTextAVX( HashTable* hash_table, const char* text );

int HashTableMakeCSV( HashTable* hash_table, FILE* output_file );

int HashTableSetHashFunction( HashTable* hash_table, size_t (*hash_function)( Elem_t ) );

size_t HashTablePushWord   ( HashTable* hash_table, Elem_t elem );
size_t HashTableFindWord   ( HashTable* hash_table, Elem_t elem );
size_t HashTablePushWordAVX( HashTable* hash_table, Elem_t elem );
size_t HashTableFindWordAVX( HashTable* hash_table, Elem_t elem );

size_t HashTableFindAllWords( HashTable* hash_table );

//-----------------------------------------------------------------------------

size_t ROL( size_t num, size_t shift );
size_t ROR( size_t num, size_t shift );

//-----------------------------------------------------------------------------

long int GetFileSizeFromStat( FILE* file );
long int ReadAllFile        ( FILE* file, char** str );

//-----------------------------------------------------------------------------

#endif
