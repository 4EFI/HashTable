
# Хеш-таблица

## Введение

В данной работе нам предстоит реализовать хеш-таблицу с 7-ью различными хеш-функциями (ВАЖНО: для одной таблицы - одна хеш-функция). Для каждой таблицы построить гистограмму заселенности с заранее выбранным масштабом для дальнейшего сравнения. 

Во второй части работы, нам предстоит заняться оптимизацией с использованием: векторных инструкций, ассемблерных вставок и функций, написанных на ассемблере. 

## Теоретическая справка

Хеш-таблица - это структура данных, представляющая собой массив, который позволяет хранить пары (ключ, значение) и выполнять три операции: операцию добавления новой пары, операцию удаления и операцию поиска пары по ключу.

Ключ - это уникальное слово, которое используется для получения значения. 
Хеш-функция - это функция, которая преобразует ключ в специальный индекс, по которому будет располагаться наша пара. 

В данной работе мы будем пользоваться методом цепочек - это метод, благодаря которому, два ключа с одним хешом не затрут друг друга. Для этого мы будем использовать массив из списков и при обнаружении коллизии, добавлять элемент в соответствующий список, а не затирать предыдущий. Ниже я попытался представить это схематично:

<p style="text-align: center"><img src=res/hash_table.png width="350px"/></p>

## Часть 1. Хеш-функции

___

### 1. Const hash 

В независимости от значения, возвращает константу (в нашем случае 1).

```C++
size_t GetConstHash( Elem_t elem )
{        
    return 1;
}
```

<p style="text-align: center"><img src=res/bar_const.png width="550px"/></p>

> Дисперсия = 26362

___

### 2. First ASCII hash

Возвращает ASCII код первого символа слова. 

```C++
size_t GetFirstAsciiHash( Elem_t elem )
{
    return elem[0];
}
```

<p style="text-align: center"><img src=res/bar_first_ascii.png width="550px"/></p>

> Дисперсия = 1102

___

### 3. LenWord hash

Возвращает длину слова.

```C++
size_t GetLenWordHash( Elem_t elem )
{
    return strlen( elem );
}
```

<p style="text-align: center"><img src=res/bar_len_word.png width="550px"/></p>

> Дисперсия = 3612

___

### 4. Sum ASCII hash

Возвращает сумму ASCII кодов всех символов слова. 

```C++
size_t GetSumAsciiHash( Elem_t elem )
{
    size_t sum = 0;
    
    for( size_t i = 0; ; i++ )
    {
        if( !elem[i] ) break;
        
        sum += elem[i];
    }
    
    return sum;
}
```

<p style="text-align: center"><img src=res/bar_sum_ascii.png width="550px"/></p>

> Дисперсия = 34

___

### 5. ROL hash

Хеш-функция, которая циклически сдвигает хеш влево (ROL) и выполняет "исключающее или" (XOR) к текущему элементу слова. Итератор увеличивается до тех пор, пока элемент не станет равным 0.

```C++
size_t GetRolHash( Elem_t elem )
{
    size_t hash  = 0;

    for( size_t i = 0; ; i++ )
    {
        if( !elem[i] ) break;

        hash = ROL( hash, 1 ) ^ elem[i];
    }

    return hash;
}
```

<p style="text-align: center"><img src=res/bar_rol.png width="550px"/></p>

> Дисперсия = 8

___

### 6. ROR hash

Хеш-функция аналогичная предыдущей за исключением того, что циклический сдвиг происходит вправо. 

```C++
size_t GetRorHash( Elem_t elem )
{
    size_t hash  = 0;

    for( size_t i = 0; ; i++ )
    {
        if( !elem[i] ) break;

        hash = ROR( hash, 1 ) ^ elem[i];
    }

    return hash;
}
```

<p style="text-align: center"><img src=res/bar_ror.png width="550px"/></p>

> Дисперсия = 7

___

### 7. BKDR hash

Хеш-функция, которая вычисляется итеративно и заканчивается когда достигает нулевого символа. Хеш умножается на число, заданное заранее по шаблону .31...31.. и прибавляется ASCII код текущего символа слова. 

```C++
size_t GetBKDRHash( Elem_t elem )
{
   size_t seed = 31; /* 31 131 1313 13131 131313 etc.. */
   size_t hash = 0;

   for( size_t i = 0; ; i++ )
   {
        if( !elem[i] ) break;
        
        hash = ( hash * seed ) + ( elem[i] );
   }

   return hash;
}
```

<p style="text-align: center"><img src=res/bar_bkdr.png width="550px"/></p>

> Дисперсия = 5

___

### 8. CRC32 hash

Хеш-функция, для дальнейшей оптимизации в виде векторных инструкций предоставленных AVX2 (_mm_crc32_u64).

```C++
size_t GetCrc32Hash( Elem_t elem ) 
{
    size_t crc  = 0xFFFFFFFF; 
    size_t mask = 0;

    for( size_t i = 0; ; i++ )
    {
        if( !elem[i] ) break;
        
        crc = crc ^ elem[i];

        for( size_t j = 0; j < 8; j++ )     // Do 8 times
        {    
            mask = -( crc &  1 );
            crc  =  ( crc >> 1 ) ^ ( 0xEDB88320 & mask );
        }
    }
    return ~crc;
}
```

<p style="text-align: center"><img src=res/bar_crc32.png width="550px"/></p>

> Дисперсия = 5

___