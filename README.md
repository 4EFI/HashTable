
# Хеш-таблица

## Введение

В данной работе нам предстоит реализовать хеш-таблицу с 7-ью различными хеш-функциями (ВАЖНО: для **одной** таблицы - **одна** хеш-функция). Для каждой таблицы построить диаграмму заселенности с заранее выбранным масштабом для дальнейшего сравнения. 

Во второй части работы, нам предстоит заняться оптимизацией с использованием: векторных инструкций, ассемблерных вставок и функций, полностью реализованных на ассемблере. 

## Теоретическая справка

**Хеш-таблица** - это структура данных, представляющая собой массив, который позволяет хранить пары (ключ, значение) и выполнять три операции: операцию добавления новой пары, операцию удаления и операцию поиска пары по ключу.

**Ключ** - это уникальное слово, которое используется для получения значения. 
**Хеш-функция** - это функция, которая преобразует ключ в специальный индекс, по которому будет располагаться наша пара. 

В данной работе мы будем пользоваться **методом цепочек** - это метод, благодаря которому, два ключа с одним хешом не затрут друг друга. Для этого мы будем использовать массив из списков и при обнаружении коллизии, добавлять элемент в соответствующий список, а не затирать предыдущий. Ниже я попытался представить это схематично:

<p style="text-align: center"><img src=res/hash_table.png width="350px"/></p>

Более подробнее можете ознакомиться с материалом на [википедии](https://ru.wikipedia.org/wiki/%D0%A5%D0%B5%D1%88-%D1%82%D0%B0%D0%B1%D0%BB%D0%B8%D1%86%D0%B0).

## Часть 1. Хеш-функции

В данной части работы нам нужно реализовать различные хеш-функции и выбрать одну из них для дальнейшей оптимизации. А для того, чтобы определиться какую именно хеш-функцию стоит использовать, мы для каждой хеш-таблицы построим диаграмму заселенности и рассчитаем дисперсию. 

Хеш-таблицу заполним, опираясь на произведение "Hamlet", в котором более 31000 слов. Размер таблицы возьмем равным 1000 элементов, чтобы средняя заселенность была 10-20 элементов.
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

<details>
<summary>ROL функция</summary>

```C++
size_t ROL( size_t num, size_t shift )
{
    return ( num << shift ) | ( num >> ( 8 * sizeof( size_t ) - shift ) );
}
```

Также, стоит отметить, что данная функция циклического сдвига оптимизируется компилятором (-O3) в одну ассемблерную команду (rol). 

<p style="text-align: center"><img src=res/ROL_cpp.png width="550px"/></p>
<p style="text-align: center"><img src=res/ROL_asm.png width="550px"/></p>

</details>

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

<details>
<summary>ROR функция</summary>

```C++
size_t ROR( size_t num, size_t shift )
{
    return ( num >> shift ) | ( num << ( 8 * sizeof( size_t ) - shift ) );
}
```

Как и ROL, ROR функция оптимизируется компилятором до одной ассемблерной команды (ror). Убедиться в этом остается читателю в виде упражнения.   

</details>

<p style="text-align: center"><img src=res/bar_ror.png width="550px"/></p>

> Дисперсия = 19

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

### Промежуточные итоги 

BKDR хеш-функция показала наилучший результат, имея наименьшую дисперсию (= 5) распределения по хеш-таблице. Для дальнейшей оптимизации возьмем именно ее. 

## Часть 2. Оптимизация   

В данной части работы мы займемся оптимизацией функции поиска по хеш-таблице. Мы не ограничимся поиском одного слова. Так как заселенность хеш таблицы не равна константе, и некоторые слова лежат в начале списка, другие в конце - мы получаем различное время поиска и ,чтобы получить более точные результаты, мы пробегаемся по всей хеш-таблице и выполняем функцию поиска для текущего элемента, так делаем итеративно.

```C++
void HashTableFindAllWords( HashTable* hash_table )
{
    for( size_t curr_list = 0; curr_list < hash_table->size; curr_list++ )
    {
        for( size_t i = 1; i <= hash_table->arr[curr_list].size; i++ )
        {
            Elem_t curr_elem  = hash_table->arr[curr_list].nodes[i].elem;

            HashTableFindWord( hash_table, curr_elem );
        }
    }
}
```

Оптимизировать циклы у нас не получится. Давайте перейдем к функции поиска - функции, принимающей два аргумента: хеш-таблицу и элемент, который необходимо найти. 

```C++
size_t HashTableFindWord( HashTable* hash_table, Elem_t elem )
{
    size_t hash      = hash_table->hash_function( elem );
    size_t curr_list = hash % hash_table->size;

    size_t list_size = hash_table->arr[curr_list].size;

    for( size_t i = 1; i <= list_size; i++ )
    {
        Elem_t curr_elem = hash_table->arr[curr_list].nodes[i].elem;
        
        if( !strcmp( curr_elem, elem) ) return 1;
    }
    
    return 0;
}
```

___

Перейдем непосредственно к анализу. Для этого воспользуемся [valgrind](https://ru.wikipedia.org/wiki/Valgrind), а именно одним из его модулей - callgrind. Данный модуль предназначен для профилирования кода, предоставляющий полноценный анализ всех функций, задействованных в программе. После его выполнения мы получаем файл callgrind.out.XXXXX. Данный файл сложно прочитать в текстовом режиме, поэтому воспользуемся [KCacheGrind](https://kcachegrind.sourceforge.net/html/Home.html) - средством, которое помогает визуализировать данный файл. 

> Для кого, это что-то новое, советую посетить [сайтик](https://baptiste-wicht.com/posts/2011/09/profile-c-application-with-callgrind-kcachegrind.html) c документацией по установке и использованию. Краткое введение без замысловатых слов.  

### 0. Предварительный анализ 

Запуская профилирование без какой-либо оптимизации, получаем следующее:

| Оптимизация | Число машинных команд | Коэффициент ускорения | 
|-------------|-----------------------|-----------------------|
|    Нет      |      1 833 595        |          1.0          |

<p style="text-align: center"><img src=res/opt_graph_0.png width="700px"/></p>

Давайте скомпилируем с флагом оптимизации "-O2":

| Оптимизация | Число машинных команд | Коэффициент ускорения | 
|-------------|-----------------------|-----------------------|
|    Нет      |      1 833 595        |          1.00         |
|    -O2      |      1 090 918        |          1.68         |

<details>
<summary>Граф вызовов функций</summary>
<p style="text-align: center"><img src=res/opt_graph_O2.png width="700px"/></p> 
</details>

### 1. Хеш-функция на ассемблере

При данной оптимизации мы полностью перепишем функцию хеширования на ассемблере. 

```Asm
GetBkdrHashAsm:     proc
                    
                    mov rax, 0                  ; hash = 0    
                    mov r8,  31                 ; seed = 31
                    xor rcx, rcx

                    .loop:
                        mul r8
                        
                        mov cl, [rdi]           ; hash += *str_addr
                        add rax, rcx          
                        
                        inc rdi                 ; str_addr++ 

                        cmp byte [rdi], 0       ; if( *str_addr == '\0' ) break;   
                        jne .loop                

                    ret 
                    endp
```

После профилирования получаем следующий результат:

| Оптимизация  | Число машинных команд | Коэффициент ускорения | 
|--------------|-----------------------|-----------------------|
|    Нет       |      1 833 595        |          1.00         |
|    -O2       |      1 090 918        |          1.68         |
| asm function |      1 371 751        |          1.34         |


<details>
<summary>Граф вызовов функций</summary>
<p style="text-align: center"><img src=res/opt_graph_asm_func.png width="700px"/></p> 
</details>

### 2. Ассемблерная вставка

Можем заметить, что вторая существенная зависимость - это функция сравнения строк "strcmp". Давайте попробуем заменить ее на ассемблерную версию, воспользовавшись ассемблерной вставкой. 

```C++
inline int StrCmp( const char* str_1, const char* str_2 )
{
    int res = 0;
    asm
    (
        ".intel_syntax noprefix;"
        ".loop:"
            "mov r11b, [rsi];"
            "mov r10b, [rdi];"
    	    "cmp r10b, 0;"
    	    "je .done;"
    	    "cmp r11b, 0;"
    	    "je .done;"
    	    "cmp r11b, r10b;"
    	    "jne .done;"
    	    "inc rdi;"  
    	    "inc rsi;"
    	    "jmp .loop;"
        ".done:"
            "movzx rax, r10b;"
            "movzx rbx, r11b;"
    	    "sub   rax, rbx;"
        ".att_syntax prefix;"
        : "=a" ( res )
    );
    
    return res;
}
```

| Оптимизация  | Число машинных команд | Коэффициент ускорения | 
|--------------|-----------------------|-----------------------|
|    Нет       |      1 833 595        |          1.00         |
|    -O2       |      1 090 918        |          1.68         |
| asm function |      1 371 751        |          1.34         |
| asm insert   |      2 186 136        |          0.83         |

<details>
<summary>Граф вызовов функций</summary>
<p style="text-align: center"><img src=res/opt_graph_asm_insert.png width="700px"/></p> 
</details>

По итогу, вместо ускорения программы, мы получили ее замедление. Как оказалось, это связано с тем, что strcmp пользуется векторными инструкциями, как следствие, простой вставкой нам ускорить ничего не удалось.



