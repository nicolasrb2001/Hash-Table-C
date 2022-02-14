/*
 ============================================================================
 
 Implement your own Hash Table in C for storing and searching names, i.e. char
 arrays. In the event of collisions, you should use linear probing with an
 interval of 1. The hash function should be the sum of the ASCII values of the
 string modulo the size of the underlying data structure. Your Hash Table
 implementation should have the following interface:
	int hash_function(const char *key)
	void resize_map(int new_size)
	void add_to_map(const char *name)
	int remove_from_map(const char *name)
	int search_map(const char *name)
	void print_map()

 ============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** hash_map;  // this is where you should store your names
int size;//stores size of hash map
char placeholder;//its address is used as a placeholder value
int number = 0;//current number of strings in the hash table
char** hash_map2;//used to expand or shrink the hash table

//returns the hash value of the string passed in
int hash_function(const char *key) {
    int index = 0,total = 0;
    //iterates until a null terminating character is encountered
    while(1){
        if(*(key+index) == '\0'){
            break;
        }
        total = total + (int) *(key+index);
        index++;

    }
    return total % (size);
}
int linear_probing(int hash_value, const char* name, int check, int rehash){
    //iterates until it finds the appropriate address
    for(int i = 0; i < size; i++) {
        int t = 0;
        if (hash_map + hash_value + i >= hash_map + size) {
            t = size;   //if the memory address being pointed to exceeds the range allocated to hash map, the sie of the hash map is subtracted from the address
        }
        //if check == 1 it checks to see if the string entered is already stored
        if (check == 1) {
            if (*(hash_map + hash_value + i - t) == 0) {
                return 1;

            }else {
                //if the address doesn't contain a placeholder and it contains the same string as the one entered it prints an error message
                if (*(hash_map + hash_value + i - t) != &placeholder && strcmp(name, *(hash_map + hash_value + i - t)) == 0) {
                    printf("string = %s already in hash table, it needs to be unique\n", name);
                    return 0;
                }
            }
        }else {
            //if the contents are being rehashed then the memory addresses of hash map2 are inspected
            if(rehash == 1){
                //if the current address is empty then its assigned the address of the string
                if (*(hash_map2 + hash_value + i - t) == 0) {
                    *(hash_map2 + hash_value + i - t) = (char *) name;
                    break;
                }
            }else{
                //else the memory addresses of hash map are inspected
                //if the memory address is empty or contains a placeholder then its assigned the address of the string
                if (*(hash_map + hash_value + i - t) == 0 || *(hash_map + hash_value + i - t) == &placeholder) {
                    *(hash_map + hash_value + i - t) = (char *) name;
                    break;
                }
            }

        }
    }
    return 1;
}
//shrinks the size and rehashes the hash table
void shrink(){
    hash_map2 = (char**) malloc((size/2)*sizeof(char*));
    memset(hash_map2, 0, (size/2)*sizeof(char*));
    int length = size;
    size = size/2;
    //this hashes all the values stored in hash map into hash map2, which is half the size of hash map
    //After hashing, the size of hash map halves and the contents of hash map2 re copied into hash map
    for(int i = 0; i<length; i++){
        if(*(hash_map+i) != &placeholder && *(hash_map+i) != 0){
            linear_probing(hash_function(*(hash_map+i)), *(hash_map+i), 0, 1);
        }
    }
    hash_map = (char **) realloc(hash_map, size*sizeof(char*));
    memcpy(hash_map,hash_map2,size* sizeof(char*));
    free(hash_map2);
}

//expands the size and rehashes the hash table
void expand(){
    size = size*2;
    hash_map = (char **) realloc(hash_map, size*sizeof(char*));
    for(int i = size/2; i<size; i++){
        *(hash_map + i) = 0;//sets to 0 the values of the new addresses added to hash map
    }
    //the following code takes the strings stored in hash map and hashes them into hash map2 which is then copied to hash map
    hash_map2 = (char**) malloc(size*sizeof(char*));
    memset(hash_map2, 0, size*sizeof(char*));
    for(int i = 0; i<size; i++){
        if(*(hash_map+i) != &placeholder && *(hash_map+i) != 0){
            linear_probing(hash_function(*(hash_map+i)), *(hash_map+i), 0, 1);
        }
    }
    memcpy(hash_map, hash_map2,size*sizeof(char*));
    free(hash_map2);
}


//initialises size of hash table
void resize_map(int new_size) {
    size = new_size;
    hash_map = (char **) malloc(size*sizeof(char*));
    memset(hash_map, 0, size*sizeof(hash_map));
}



void add_to_map(const char *name) {
    if(size == 0){
        resize_map(6);
    }
    int hash_value = hash_function(name);

    //checks to see if the key entered is already stored in the hash map, if it hasn't then it returns 1
    if(linear_probing(hash_value, name, 1, 0) == 1){
        number++;
        //the hash table doubles if it exceeds the threshold
        if(number > size*0.7){
            expand();
        }
        //calls linear_probing to add the string at the appropriate address
        linear_probing(hash_value, name, 0, 0);
    }
}

//searches for the string passed in and removes it if specified(and found)
int search_map(const char *name, int remove) {
    //hash map has not been initialised yet
    if(size == 0){
        return 0;
    }
    int hash_value = hash_function(name) % size;
    for(int i = 0; i<size; i++){
        char** address = hash_map + hash_value + i;//address stores the memory address of the next char* pointer(ie string) to be examined
        if(address> hash_map+size-1){
            address = address - size;//if the memory address isn't allocated to hash map then its starts from the beginning of the table
        }
        if(*address ==  &placeholder){
            continue;
        }
        if (*address == 0){
            break;//the string has not been found since the value of the address to which address points to is empty and doesnt contain a placeholder

        }else{
            if(strcmp(name, *address) == 0){
                //found
                if(remove == 1){
                    printf("String = '%s' has been removed\n", name);
                    *(hash_map + hash_value + i) = &placeholder; //places a placeholder at the memory address where the string has been removed
                    number--;
                    //if the hash map becomes empty it frees the memory allocated to it
                    if(number == 0){
                        free(hash_map);
                        size =0;
                    }else{
                        if(number< (0.7/4)*size){//if the load factor falls bellow this threshold the hash map halves
                            shrink();
                        }
                    }
                }
                return 1;
            }
        }
    }
    return 0;

}

//this function searches for the item to be removed and if it finds it then it removes it and if it doesn't it prints an error message
void remove_from_map(const char *name) {
    int result= search_map(name, 1);
    if(result == 0){
        printf("string = %s to be removed is not found\n",name);
    }

}
void print_map() {
    //iterates over all addresses allocated to hash map and prints their contents
    for(int i =0; i< size; i++){
        if(*(hash_map+i) == 0 || *(hash_map+i) == &placeholder){
            printf("String at bucket %p is empty\n", hash_map+i);
        }else{
            printf("String at bucket %p = %s\n", hash_map+i, *(hash_map+i));
        }
    }
    printf("\n");
}




int main(int argc, char *argv[]) {
    char *stringOne = "Hello world";
    char *stringTwo = "How are you?";
    char *stringThree = "Be the best you...!!";
    char *stringFour = "Be kind to yourself";
    char *stringFive = "This hash table is fully functional :)";
    add_to_map(stringOne);
    add_to_map(stringTwo);
    add_to_map(stringOne);
    add_to_map(stringThree);
    add_to_map(stringFive);
    add_to_map(stringFour);
    print_map();
    int ret = search_map(stringOne,0);
    if(ret == 1)
        printf("Found %s!\n", stringOne);
    print_map();
    remove_from_map(stringThree);

    ret = search_map(stringFive,0);
    if(ret == 1)
        printf("Found %s!\n", stringFive);
    print_map();
    return EXIT_SUCCESS;
}
