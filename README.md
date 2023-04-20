
# Хеш-таблица

## Введение

В данной работе нам предстоит реализовать хеш-таблицу, с 7-ью различными хеш-функциями (ВАЖНО: для одной таблицы - одна хеш функция). Для каждой таблицы построить гистограмму заселенности с заранее выбранным масштабом для дальнейшего сравнения. 

Во второй части работы, нам предстоит заняться оптимизацией с использованием: векторных инструкций, ассемблерных вставок и функций, написанных на ассемблере. 

## Теоретическая справка

Хеш-таблица - это структура данных, представляющая собой массив, который позволяет хранить пары (ключ, значение) и выполнять три операции: операцию добавления новой пары, операцию удаления и операцию поиска пары по ключу.

Ключ - это уникальное значение (слово), которое используется для получения значения. 
Хеш-функция - это функция, которая преобразует ключ в специальный индекс, по которому будет располагаться наша пара. 

В данной работе мы будем пользоваться методом цепочек - это метод, благодаря которому, два ключа с одним хешом не затрут друг друга. Для этого мы будем использовать массив из списков и при обнаружении коллизии, добавлять элемент в соответствующий список, а не затирать предыдущий. Ниже я попытался представить это схематично:

<p style="text-align: center"><img src=res/hash_table.png width="350px"/></p>