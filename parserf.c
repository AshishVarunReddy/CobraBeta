#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include<ctype.h>
#include"lexerf.h"
#include"codegeneratorf.h"
#include"stackf.h"

extern int token_index;

/*typedef struct Node{
    char* value;
    Tokentype type;
    struct Node* left;
    struct Node* right;
}Node;*/

Node* create_node(char* value, Tokentype type){
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->value = value;
    newNode->type = type;
    newNode->left = NULL;
    newNode->right = NULL;

    return newNode;
}

void print_error(char* errorType){
    fprintf(stderr, "Error: %s\n", errorType);
    exit(-1);
}

void print_tree(Node* root, char* prestatement, int spaces){
    if(root == NULL){
        return;
    }
    for(int i = 0; i<spaces; i++){
        printf("   ");
    }
    printf("%s: %s\n",prestatement, root->value);

   print_tree(root->left, "left", spaces+1);
   print_tree(root->right, "right", spaces+1);
}

int op_prec(char s){
    if(s == '^')return 3;
    if(s == '*' || s == '/' || s == '%')return 2;
    if(s == '+' || s == '-')return 1;

    return -1;
}

char** exp_convertor(char** expr, int iter, int* mov){
    int count = 0;
    for(int x = 0; x<iter; x++){
        if(!strcmp(expr[x], ")") || !strcmp(expr[x] , "(")){
            count++;
        }
    }
    *mov = count;
    int z = iter-1;
    char** prefix = (char**)malloc(iter* sizeof(char*)+1);
    prefix[iter] = NULL;
    stack* st = create_stack_element("STACK_END");
    for(int i = iter-1; i>=0; i--){
       
       if(isalnum(*expr[i]) || atoi(expr[i]) < 0){
            prefix[z] = expr[i];
            z--;
       }else if(*expr[i] == ')'){
            push(&st, expr[i]);
       }else if(*expr[i] == '('){
           while(empty_stack(st) == 0 && strcmp(peep(st), ")")){
               prefix[z] = peep(st);
               z--;
               popStack(&st);
           }
           popStack(&st);
       }else{
           while(empty_stack(st) == 0 && op_prec(*expr[i]) < op_prec(*peep(st))){//CHECK FOR EQUALITY!!
               prefix[z] = peep(st);
               z--;
               popStack(&st);
           }
           push(&st, expr[i]);

       }
       
    }
    while(strcmp(peep(st), "STACK_END")){
        prefix[z] = peep(st);
        z--;
        popStack(&st);
    }
    printf("start ");
    for(int i = count; i < iter; i++){
        printf("%s ", prefix[i]);
    }
    printf("\n");
    return prefix;
}

static int x = 0;

Node* generate_operation_tree(char** expression_array, int lt){
       if(lt <= x)return NULL;
       char* sl = expression_array[x];
       Node* node = create_node(expression_array[x], (isalnum(*sl) || atoi(sl)<0) ? INT : OPERATOR);
       x++;
       
       if(node->type == INT){
           return node;
       }

       node->left = generate_operation_tree(expression_array,  lt);

       node->right = generate_operation_tree(expression_array,  lt);
       return node;
}

char** expression_string_generator(Token** tokenArray, Node* current_node, int* ip, int inParen){
    char** expr = (char**)malloc((token_index+1)*sizeof(char*));
    expr[token_index] = NULL;
    Token* current_token = tokenArray[*ip];
    printf("PORCUPINE: %s\n", current_token->value);
     stack* s = create_stack_element("STACK_END");
     if(inParen == 1){
        push(&s, "(");
     }else{
         push(&s, "BUFFER");
     }
        if(!strcmp(current_token->value, "(") || current_token->Type == INT || *current_token->value == '-'){
            if(current_token->Type == INT && *tokenArray[*ip + 1]->value == ';'){
                Node* expr_node = create_node(current_token->value, current_token->Type);
                current_node->left = expr_node;
                expr[0] = current_token->value;
                return expr;
            }
            printf("TOOS\n");
            int j = *ip;
            int iter = 0;
            while(empty_stack(s) == 0 && tokenArray[j]->Type != EOFILE){
                printf("Soltice\n");
                if(*tokenArray[j]->value == ';'){
                    popStack(&s);
                }
              if(inParen == 1){
                if(strcmp(tokenArray[j] -> value, ")")){
                     push(&s, tokenArray[j]->value);
                }else{
                    while(strcmp(peep(s), "(")){
                         popStack(&s);
                    }
                     popStack(&s);
                }
              }
              if(empty_stack(s) == 0){
                if(iter == 0 && (*tokenArray[j]->value == '-')){
                    expr[iter++] = "0";
                    }else if((tokenArray[j-1]->Type == OPERATOR) && *tokenArray[j]->value == '-'){
                        if(tokenArray[j+1]->Type == INT){
                            int ln = strlen(tokenArray[j+1]->value);
                            char* bf = (char*)malloc(sizeof(char*)*(ln+2));
                            bf[ln+1] = '\0';
                            sprintf(bf, "-%s", tokenArray[j+1]->value);
                            tokenArray[j]->value = bf;
                            tokenArray[j]->Type = INT;
                            j+=2;
                            expr[iter] = bf;
                            iter++;
                            continue;
                        }else if(*tokenArray[j+1]->value == '('){
                            stack* substack = create_stack_element("STACK_END");
                            push(&substack, "BUFFER");
                            int k = j;
                            k++;
                            push(&s, tokenArray[k]->value);
                            expr[iter++] = tokenArray[k++]->value;
                            push(&s, "0");
                            expr[iter++] = "0";
                            push(&s, "-");
                            expr[iter++] = "-";
                            j+=2;
                            k = j;
                            while(empty_stack(substack) == 0){
                              char* substr = tokenArray[k]->value;
                              if(*substr == '('){
                                push(&substack, "(");
                              }else if(*substr == ')'){
                                  while(*peep(substack) != '(' && strcmp(peep(substack), "BUFFER")){
                                        popStack(&substack);
                                    }
                                  popStack(&substack);
                              }else if((*substr == '+' || *substr == '-') && !strcmp(peep(substack), "BUFFER")){
                                  if(*substr == '+'){
                                        *substr = '-';
                                    }else{
                                        if(*tokenArray[k-1]->value == '/' || *tokenArray[k-1]->value == '*')
                                            *substr = '-';
                                        else
                                            *substr = '+';
                                    }
                                }
                                k++;
                            }
                        }
                    }
            if(*tokenArray[j]->value != ' '){
                expr[iter++] = tokenArray[j]->value;
                printf("Markov\n");
            }
            j++;
            }
        }
            printf("CONCUSSION: \n");
            for(int p = 0; p<iter; p++){
                printf("%s ",expr[p]);
            }
            printf("\n");
            //printf("black hole:%s\n", exp_convertor(expr, iter));
            int mov = 0;
            char** expression_array = exp_convertor(expr, iter, &mov);
            printf("bull \n");
            //memmove(expression_array, expression_array+mov, ;
            for(int y = 0; y<iter-mov; y++){
                expression_array[y] = expression_array[y+mov];
                printf("%s ", expression_array[y]);
            }
            expression_array[iter-mov] = NULL;
            printf("\n");
            Node* synode = generate_operation_tree(expression_array, iter-mov);
            x = 0;
            print_tree(synode, "center", 0);
            current_node->left = synode;
            current_token = tokenArray[j];
            *ip = j;
            printf("black Hole: %s\n", current_token->value);
        
        }else{
            perror("Syntax Error at int or anthing else\n");
            exit(2);
        }
    return expr;

}

int handle_exit_syscall(Token** tokenArray, Node* current_node, int i){
        Token* current_token = tokenArray[i];
        Node* exitNode = create_node(current_token->value, KEYWORD);
        current_node->right = exitNode;
        current_node = current_node->right;
        current_token = tokenArray[++i];
                
        if(!strcmp(current_token->value, "(") && current_token->Type == SEPARATOR){
        stack* s = create_stack_element("STACK_END");
        push(&s, "(");
        Node* oParen = create_node(current_token->value, SEPARATOR);
        current_node->left = oParen;
        current_token = tokenArray[++i];
        if(!strcmp(current_token->value, "(") || current_token->Type == INT || *current_token->value == '-'){
            int j = i;
            char** expr = (char**)malloc((token_index+1)*sizeof(char*));
            expr[token_index] = NULL;
            int iter = 0;
            while(empty_stack(s) == 0 && tokenArray[j]->Type != EOFILE){
                                    
                if(strcmp(tokenArray[j] -> value, ")")){
                     push(&s, tokenArray[j]->value);
                }else{
                    while(strcmp(peep(s), "(")){
                         popStack(&s);
                    }
                     popStack(&s);
                }
                    if(empty_stack(s) == 0){
                if(iter == 0 && (*tokenArray[j]->value == '-')){
                    expr[iter++] = "0";
                    }else if((tokenArray[j-1]->Type == OPERATOR) && *tokenArray[j]->value == '-'){
                        if(tokenArray[j+1]->Type == INT){
                            int ln = strlen(tokenArray[j+1]->value);
                            char* bf = (char*)malloc(sizeof(char*)*(ln+2));
                            bf[ln+1] = '\0';
                            sprintf(bf, "-%s", tokenArray[j+1]->value);
                            tokenArray[j]->value = bf;
                            tokenArray[j]->Type = INT;
                            j+=2;
                            expr[iter] = bf;
                            iter++;
                            continue;
                        }else if(*tokenArray[j+1]->value == '('){
                            stack* substack = create_stack_element("STACK_END");
                            push(&substack, "BUFFER");
                            int k = j;
                            k++;
                            push(&s, tokenArray[k]->value);
                            expr[iter++] = tokenArray[k++]->value;
                            push(&s, "0");
                            expr[iter++] = "0";
                            push(&s, "-");
                            expr[iter++] = "-";
                            j+=2;
                            k = j;
                            while(empty_stack(substack) == 0){
                              char* substr = tokenArray[k]->value;
                              if(*substr == '('){
                                push(&substack, "(");
                              }else if(*substr == ')'){
                                  while(*peep(substack) != '(' && strcmp(peep(substack), "BUFFER")){
                                        popStack(&substack);
                                    }
                                  popStack(&substack);
                              }else if((*substr == '+' || *substr == '-') && !strcmp(peep(substack), "BUFFER")){
                                  if(*substr == '+'){
                                        *substr = '-';
                                    }else{
                                        if(*tokenArray[k-1]->value == '/' || *tokenArray[k-1]->value == '*')
                                            *substr = '-';
                                        else
                                            *substr = '+';
                                    }
                                }
                                k++;
                            }
                        }
                    }
            if(*tokenArray[j]->value != ' ')
                expr[iter++] = tokenArray[j]->value;
            j++;
            }
        }
            printf("bls: \n");
            for(int p = 0; p<iter; p++){
                printf("%s ",expr[p]);
            }
            printf("\n");
            //printf("black hole:%s\n", exp_convertor(expr, iter));
            int mov = 0;
            char** expression_array = exp_convertor(expr, iter, &mov);
            printf("bull \n");
            //memmove(expression_array, expression_array+mov, ;
            for(int y = 0; y<iter-mov; y++){
                expression_array[y] = expression_array[y+mov];
                printf("%s ", expression_array[y]);
            }
            expression_array[iter-mov] = NULL;
            printf("\nfinished \n");
            Node* synode = generate_operation_tree(expression_array, iter-mov);
            printf("SY\n");
            print_tree(synode, "center", 0);
            printf("NODE\n");
            current_node->left->left = synode;
            current_token = tokenArray[j];
            i = j;
            printf("black Hole: %s\n", current_token->value);
             if(!strcmp(current_token->value, ")") && current_token->Type == SEPARATOR){
                Node* cParen = create_node(current_token->value, SEPARATOR);
                current_node->left->right = cParen;
                current_token = tokenArray[++i];
                if(current_token->value != NULL && !strcmp(current_token->value, ";") && current_token->Type == SEPARATOR){
                    Node* semi = create_node(current_token->value, SEPARATOR);
                    current_node->right = semi;
                }else{
                    print_error("Semi Colon Not found");
                }
             }else{
                 print_error("No Closed Paren");
             }
        }else{
            print_error("Not an Integer");
        }
    }else{
        print_error("No Open Paren");
    }

    return i;
}

int create_variable(Token** tokenArray, Node* current_node, int i){
    Node* initial = current_node;
    Node* stopNode;
    printf("HIIII\n");
    Token* current_token = tokenArray[i];
    Node* var_node = create_node(current_token->value, KEYWORD);
    current_node->left = var_node;
    current_node = var_node;
    stopNode = current_node;
    i++;

    if(tokenArray[i]->Type == EOFILE || tokenArray[i]->Type != IDENTIFIER){
        perror("ERROR: Syntax error\n");
        exit(1);
    }
    if(tokenArray[i]->Type == IDENTIFIER){
        Node* ident_node = create_node(tokenArray[i]->value, tokenArray[i]->Type);
        current_node->left = ident_node;
        current_node = ident_node;
        i++;
    }
    if(tokenArray[i]->Type == OPERATOR){
        if(!strcmp(tokenArray[i]->value, "=")){
           Node* eqNode = create_node(tokenArray[i]->value, tokenArray[i]->Type);
           current_node->left = eqNode;
           current_node = eqNode;
           i++;
        }else{
            perror("Expected '=' but got something else\n");
            exit(1);
        }
    }else{
        perror("Syntax Error, No operator\n");
        exit(1);
    }
    expression_string_generator(tokenArray, current_node, &i, 0); 
    /*if(tokenArray[i]->Type == INT){
        Node* varInt_node = create_node(tokenArray[i]->value, tokenArray[i]->Type);
        current_node->left = varInt_node;
        current_node = stopNode;
        i++;
    }else{
        perror("Expected int, got something else\n");
        exit(1);
    }*/

    if(tokenArray[i]->Type == SEPARATOR){
        if(*tokenArray[i]->value == ';'){
            Node* semi_node = create_node(tokenArray[i]->value, tokenArray[i]->Type);
            current_node = stopNode;
            printf("SOLS: %s\n", current_node->value);
            current_node->right = semi_node;
        }
        current_node = initial;
    }
    return i;
}

Node* parser(Token** tokenArray){
    Token* current_token = tokenArray[0];
    int i = 0;

    Node* root = create_node("PROGRAM",BEGINNING); 

    Node* current_node = root;
    while(current_token->Type != EOFILE){
        if(current_node == NULL){
            exit(-1);
        }
        if(current_node == root){
            //printf("root\n");
        }

        current_token = tokenArray[i];
        switch(current_token->Type){
            case KEYWORD:
                if(!strcmp(current_token->value, "exit")){
                    printf("FERMIONS\n");
                    i =  handle_exit_syscall(tokenArray,  current_node, i);
                }else if(!strcmp(current_token->value, "int")){
                    i = create_variable(tokenArray, current_node, i);
                }
                print_tree(root, "root", 0);
                break;
            case SEPARATOR:
                //
                break;
            case IDENTIFIER:
                printf("IDENTIFIER identified\n");
                break;
            case INT:
                printf("INTEGER %s\n", tokenArray[i]->value);
                break;
            case OPERATOR:
                printf("operator incoming\n");
                break;
        }
        i++;

    }
    return root;
}




