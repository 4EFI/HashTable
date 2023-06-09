
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

    int hash_table_size;
};

const int HashTableSize = 1001;

const HashFunction HashFunctions[] = 
{
    { GetConstHash,      "res/output_const.csv",            HashTableSize },
    { GetFirstAsciiHash, "res/output_first_ascii.csv",      HashTableSize },
    { GetLenWordHash,    "res/output_len_word.csv",         HashTableSize },
    { GetSumAsciiHash,   "res/output_sum_ascii.csv",        HashTableSize },
    { GetSumAsciiHash,   "res/output_sum_ascii_5000.csv",   5000 },
    { GetRolHash,        "res/output_rol.csv",              HashTableSize },
    { GetRorHash,        "res/output_ror.csv",              HashTableSize },
    { GetBkdrHash,       "res/output_bkdr.csv",             HashTableSize },
    { GetCrc32Hash,      "res/output_crc32.csv",            HashTableSize }
};

const int NumHashFunctions = sizeof( HashFunctions ) / sizeof( HashFunction ); 

//-----------------------------------------------------------------------------

int HashTableCtor( HashTable* hash_table, int size, size_t (*hash_function)( Elem_t ) );
int HashTableDtor( HashTable* hash_table );

int HashTableLoadText   ( HashTable* hash_table, const char* text );
int HashTableLoadTextAVX( HashTable* hash_table, const char* text );

int HashTableMakeCSV( HashTable* hash_table, FILE* output_file );

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
