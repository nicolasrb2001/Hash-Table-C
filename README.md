# Hash-Table-C
This is my implementation of a Hash Table Data Structure with linear probing for strings in C.

## Motivation :mountain:
---
This was part of my coursework during 1st year and it helped me gain a better understanding of pointers.

### Features

- The hash table has an initial size = 6, and **doubles** if nº items stored > current size * 0.7. 
- The hash table **shrinks** if nº items stored < (0.7/4)*size
- Supports **adding** and **removing** string items.
- Rehashes items in hash table when it expands or shrinks
- The hash function is the sum of the ASCII values of the string modulo the current size of the hash table.

### Technical Information
This was developed using [CLion](https://www.jetbrains.com/clion/). Includes a main function for testing purposes.

### Screenshots
The program adds 2 strings to the table and prints its contents. It then tries to add a string already in the table but it is denied. It then adds more strings, expands and rehashes the current items stored and then keeps adding strings.

![](/screenshots/screenshot1.png)

This time, the program removes three strings from the table, the number of items falls below the threshold and so the table shrinks.

![](/screenshots/screenshot2.png)
