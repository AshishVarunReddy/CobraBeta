#include"lexerf.h"
#ifndef PARSER_H

#define PARSER_H

typedef struct Node{
    char* value;
    Tokentype type;
    struct Node* left;
    struct Node* right;
}Node;


Token** parser(Token** tokenArray);
void print_tree(Node* root, char* prestatement, int spaces);
void print_error(char* errorType);


#endif
