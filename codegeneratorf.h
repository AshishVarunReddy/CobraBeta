#include"parserf.h"
#include"lexerf.h"


#ifndef _GENERATOR_H_
#define _GENERATOR_H_

typedef struct calls{
    int number;
    char* func_call;
    char* var_name;
}calls;


int operations_int(int a, int b, char op);

int operations_asm(int a, int b, char op, FILE* fp);

int sysgen(char* call);

int calculate_node(Node** op_node, FILE* fp);

void traverse(Node* root, FILE* fp, calls* c);

int generate_code(Node* root);

#endif
