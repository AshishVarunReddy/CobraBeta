#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include<ctype.h>
#include"lexerf.h"


typedef enum{
    BEGINNING,
    INT_LIT,
    STATEMENTS,
    SEPERATORS,
    OTHERS,
}Nodetypes;


typedef struct Node{
    char* value;
    Nodetypes type;
    struct Node* left;
    struct Node* right;
}Node;

Node* create_node(char* value, Nodetypes type){
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->value = value;
    newNode->type = type;
    newNode->left = NULL;
    newNode->right = NULL;

    return newNode;
}

void print_tree(Node* root ){
    if(root == NULL){
        return;
    }
    printf("value: %s", root->value);

   print_tree(root->left);
   print_tree(root->right);
}


void parser(Token** tokenArray){
    Token* current_token = tokenArray[0];
    int i = 0;

    Node* root = create_node("PROGRAM",BEGINNING); 
    Node* left = create_node("LEFT", OTHERS);
    Node* right = create_node("RIGHT", OTHERS);

    root->left = left;
    root->right = right;
    Node* current = root;
    while(current_token->Type != EOFILE){
        if(current == NULL){
            return;
        }
        if(current == root){
            printf("root\n");
        }
        if(current->left == NULL && current->right != NULL){
            current = current->right;
            printf("Went right\n");
        }else if(current->left && current->right){
            current = current->left;
            printf("Went left\n");
        }else if(!current->left && !current->right){

        }

        printf("no: %d\n", i);
        current_token = tokenArray[i++];
    }
    free(root);
}




