#ifndef _TREE_H
#define _TREE_H
#include"lexerf.h"


typedef struct Node{
  char* value;
  Tokentype type;
  struct Node* left;
  struct Node* right;
}Node;


#endif
