#include"lexerf.h"
#include"hashtable.h"
#ifndef PARSER_H

#define PARSER_H

typedef struct Node{
    char* value;
    Tokentype type;
    struct Node* left;
    struct Node* right;
}Node;


Node* create_node(char* value, Tokentype type);

Node* parser(Token** tokenArray, item*** variable_s);

int op_prec(char s);

char** exp_convertor(char** expr, int iter, int* mov);

Node* generate_operation_tree(char** expression_array, int lt);

void print_tree(Node* root, char* prestatement, int spaces);

void print_error(char* errorType);

item** symbol_returner(item** itemizer);


#endif
