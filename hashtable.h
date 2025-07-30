#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>


#ifndef _HASHTABLE_H_

#define _HASHTABLE_H_


typedef struct item{
    char* key;
    char* value;
}item;

item** create_hash(int n);

item** resize_hash(int prev_n, int new_n, item** itemp_array);

item* search(item* items, size_t size, const char* key);



#endif
