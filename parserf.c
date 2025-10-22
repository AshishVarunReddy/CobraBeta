#include"syscalls/_print.h"
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include<ctype.h>
#include"lexerf.h"
#include"codegeneratorf.h"
#include"stackf.h"
#include"hashtable.h"

extern int token_index;
extern int var_num;

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
   if(root->left)
      print_tree(root->left, "left", spaces+1);
   if(root->right)
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
       Node* node = create_node(expression_array[x], (isalnum(*(sl)) || atoi(sl)<0) ? (isalpha(*(sl))) ? IDENTIFIER: INT : OPERATOR);
        
       x++;
       
       if(node->type == INT || node->type == IDENTIFIER){
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
     stack* st = create_stack_element("STACK_END");
     if(inParen == 1){
        push(&st, "(");
     }else{
         push(&st, "BUFFERS");
     }
    if(!strcmp(current_token->value, "(") || current_token->Type == INT || *current_token->value == '-' || current_token->Type == IDENTIFIER){
        if(current_token->Type == INT && *tokenArray[*ip + 1]->value == ';'){
        Node* expr_node = create_node(current_token->value, current_token->Type);
        current_node->left = expr_node;
        expr[0] = current_token->value;
        return expr;
        }
          int j = *ip;
          int iter = 0;
          while(empty_stack(st) == 0 && tokenArray[j]->Type != EOFILE){
              if(tokenArray[j]->Type == KEYWORD){
        int* s = NULL;
        perror("KEYWORD occured before semicolon\n");
        exit(1);
      }
              if(*tokenArray[j]->value == ';' && inParen == 0){
                  popStack(&st);
              }
            if(inParen == 1){
              if(strcmp(tokenArray[j] -> value, ")")){
                   push(&st, tokenArray[j]->value);
              }else{
                  while(strcmp(peep(st), "(")){
                       popStack(&st);
                  }
                   popStack(&st);
              }
            }
            if(empty_stack(st) == 0){
              if(iter == 0 && (*tokenArray[j]->value == '-')){
                  expr[iter++] = "0";
                  }else if((tokenArray[j-1]->Type == OPERATOR && *tokenArray[j-1]->value != '=') && *tokenArray[j]->value == '-'){
                      if(tokenArray[j+1]->Type == INT){
                          int ln = strlen(tokenArray[j+1]->value);
                          char* bf = (char*)malloc(sizeof(char*)*(ln+2));
                          bf[ln+1] = '\0';
                          sprintf(bf, "-%s", tokenArray[j+1]->value);
                          tokenArray[j]->value = bf;
                          tokenArray[j]->Type = INT;
                          printf("BOSON\n");
                          j+=2;
                          expr[iter] = bf;
                          iter++;
                          continue;
                      }else if(*tokenArray[j+1]->value == '('){
                          stack* substack = create_stack_element("STACK_END");
                          push(&substack, "BUFFER");
                          int k = j;
                          k++;
                          push(&st, tokenArray[k]->value);
                          expr[iter++] = tokenArray[k++]->value;
                          push(&st, "0");
                          expr[iter++] = "0";
                          push(&st, "-");
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
            if(*tokenArray[j]->value != ' ' && tokenArray[j]->Type != KEYWORD){
                expr[iter++] = tokenArray[j]->value;
            }
            j++;
            }
        }
            printf("\npars209: ");
            for(int p = 0; p<iter; p++){
                printf("%s /",expr[p]);
            }
            printf("\n");
            //printf("black hole:%s\n", exp_convertor(expr, iter));
            int mov = 0;
            char** expression_array = exp_convertor(expr, iter, &mov);
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
            print_token(current_token);
            perror("Syntax Error at int or anthing else\n");
            exit(2);
        }
    return expr;

}
void printtok(const char* s, Token* token){
  printf("%s", s);
  print_token(token);
}
int handle_exit_syscall(Token** tokenArray, Node* current_node, int i, item** variable_table){
        if(i > 0 && *tokenArray[i-1]->value != ';'){
    perror("Function call must be first token or preceded by semicolon\n");
    exit(1);
  }
        Token* current_token = tokenArray[i];
        Node* exitNode = create_node(current_token->value, KEYWORD);
        current_node->right = exitNode;
        current_node = current_node->right;
        current_token = tokenArray[++i];
       // expression_string_generator(tokenArray, current_node, &i, 1);       
        if(!strcmp(current_token->value, "(") && current_token->Type == SEPARATOR){
        Node* oParen = create_node(current_token->value, SEPARATOR);
        current_node->left = oParen;
        current_node = oParen;
        current_token = tokenArray[++i];
        if(tokenArray[i]->Type == INT && *tokenArray[i+1]->value == ')'){
            Node* numNode = create_node(current_token->value, current_token->Type);
            current_node->left = numNode;
            current_node = exitNode->left;
            current_token = tokenArray[++i];
            printf("The INTEGER\n");
        }
        else if(tokenArray[i]->Type == INT){
            expression_string_generator(tokenArray, current_node, &i, 1);
            current_token = tokenArray[i];
            
        }else if(tokenArray[i]->Type == IDENTIFIER){
            print_token(current_token);
            if(search_var(variable_table, var_num, current_token->value)){
                Node* var_node = create_node(current_token->value, current_token->Type);
                current_node ->left = var_node;
                current_token = tokenArray[++i];
                printf("variable found\n");
            }else{
                perror("Variable not found\n");
                exit(1);
            }
        }
        if(*current_token->value == ')'){
            Node* cParen = create_node(current_token->value, SEPARATOR);
            current_node->right = cParen;
            current_token = tokenArray[++i];
            current_node = exitNode;
        }else{
            perror("Error at closing brackets\n");
            exit(1);
        }

        if(*current_token->value == ';'){
            Node* semi = create_node(current_token->value, SEPARATOR);
            current_node->right = semi;
            current_token = tokenArray[++i];
        }else{
            perror("No semi colons\n");
            exit(1);
        }

 
    }
    return i;
}
int create_variable(Token** tokenArray, Node**current_node, int i, item** variable_item){
    int sks;
    Node* initial = *current_node;
    Node* stopNode;
    Token* current_token = tokenArray[i];
    Node* var_node = create_node(current_token->value, KEYWORD);
    (*current_node)->left = var_node;
    *current_node = var_node;
    stopNode = *current_node;
    i++;

    if(tokenArray[i]->Type == EOFILE || tokenArray[i]->Type != IDENTIFIER){
        perror("ERROR: Syntax error\n");
        exit(1);
    }
    if(tokenArray[i]->Type == IDENTIFIER){
        print_token(tokenArray[i]);
        sks = i;
        Node* ident_node = create_node(tokenArray[i]->value, tokenArray[i]->Type);
        (*variable_item)->key = tokenArray[i]->value;
        printf("par315: %s\n", (*variable_item)->key);
        (*current_node)->left = ident_node;
        *current_node = ident_node;
        i++;
    }
    if(tokenArray[i]->Type == OPERATOR){
        if(!strcmp(tokenArray[i]->value, "=")){
           Node* eqNode = create_node(tokenArray[i]->value, tokenArray[i]->Type);
           (*current_node)->left = eqNode;
           *current_node = eqNode;
           i++;
        }else{
            perror("Expected '=' but got something else\n");
            exit(1);
        }
    }else{
        printf("That is %s\n", tokenArray[i]->value);
        perror("Syntax Error, No operator\n");
        exit(1);
    }
    if(tokenArray[i]->Type == INT && *tokenArray[i+1]->value == ';' ){
        Node* numNode = create_node(tokenArray[i]->value, tokenArray[i]->Type);
        (*current_node)->left = numNode;
        *current_node = stopNode;
        i++;
    }else
        expression_string_generator(tokenArray, *current_node, &i, 0); 
    
    
    printf("p355\n");
    print_token(tokenArray[sks]);
    if(tokenArray[i]->Type == SEPARATOR){
        if(*tokenArray[i]->value == ';'){
            Node* semi_node = create_node(tokenArray[i]->value, tokenArray[i]->Type);
            *current_node = stopNode;
            (*current_node)->right = semi_node;

            if(tokenArray[i+1] && tokenArray[i+1]->Type == IDENTIFIER ||tokenArray[i+2] && tokenArray[i+2]->Type == IDENTIFIER){
                *current_node = semi_node;
            }else{
                *current_node = initial;
            }
        }else{
      perror("No semicolon at the end\n");
      exit(1);
    }
    }
    return i;
}

void edit_variable(item* variable, Token** tokenArray, int* ip, Node** current_node){
      printf("382pars\n");
      Node* init_node = *current_node;
      int i = *ip;
      Node* editVar = create_node(tokenArray[i]->value, tokenArray[i]->Type);
      (*current_node)->left = editVar;
      (*current_node) = (*current_node)->left;
      Node* stopNode = (*current_node);
      
      i++;
      if(*tokenArray[i]->value != '='){
          printf("Found no assignment sign. Aborting the compilation...\n");
          exit(1);
      }
      Node* eqNode = create_node(tokenArray[i]->value, tokenArray[i]->Type);
      (*current_node)->left = eqNode;
      (*current_node) = (*current_node)->left; 
      *ip = ++i;
      if(tokenArray[i]->Type == INT && *tokenArray[i+1]->value == ';'){
          Node* numNode = create_node(tokenArray[i]->value, tokenArray[i]->Type);
          (*current_node)->left = numNode;
          (*ip)++;
      }else{
         expression_string_generator(tokenArray, *current_node, ip, 0);
      }
      (*current_node) = stopNode; //slightly different from creating variable. We move back to the identifier instead of data_type keyword.
      i = *ip;
      if(*tokenArray[i]->value != ';'){
          printtok("408pars: ", tokenArray[i]);
          printf("No semicolon at the end of the statment. Aborting...\n");
          exit(1);
      }
      Node* semiNode = create_node(tokenArray[i]->value, tokenArray[i]->Type);
      int iss = 4;
      printf("414pars: %s\n", semiNode->value);
      (*current_node)->right = semiNode;
      if((tokenArray[i+1] && tokenArray[i+1]->Type == IDENTIFIER) ||(tokenArray[i+2] && tokenArray[i+2]->Type == IDENTIFIER)){
          *current_node = semiNode;
          iss = 20;
      }else{
          printf("421pars Duckfudks: %s : %s\n", tokenArray[i+1]->value, tokenArray[i+2]->value);
          *current_node = init_node;
          iss = 30;
      }
      printf("425pars: %d\n", iss);
      print_tree(init_node, "edit", 0);
}

item** symbol_returner(item** itemizer){
    return itemizer;
}

int var_tracer = 0;


Node* parser(Token** tokenArray, item*** variable_s){
    Token* current_token = tokenArray[0];
    int i = 0;
    Node* root = create_node("PROGRAM",BEGINNING); 

    Node* current_node = root;
    item** variable_table = create_hash(var_num);
    item* current_item = variable_table[0];
    int var_it = 0;
    while(current_token->value && current_token->Type != EOFILE){
        if(current_node == NULL){
            exit(-1);
        }
     
        current_token = tokenArray[i];
        switch(current_token->Type){
            case KEYWORD:
                if(!strcmp(current_token->value, "exit")){
                    printtok("p399", tokenArray[i+2]);
                    i =  handle_exit_syscall(tokenArray,  current_node, i, variable_table);
                    i--;
                }else if(!strcmp(current_token->value, "int")){
                    int sks = i;
                    if(sks - 10 >= 0){
                          printtok("p411", tokenArray[sks-10]);
                      }
                    i = create_variable(tokenArray, &current_node, i, &current_item);
                    var_tracer++;
                    current_item = variable_table[++var_it];
                    if(sks - 10 >= 0){
                        printtok("p414", tokenArray[sks-10]);
                    }

                }
                printf("444pars\n");
                print_tree(root, "root", 0);
                break;
            case SEPARATOR:
                printf("SEPERATOR_IDENTIFIED\n");
                break;
            case IDENTIFIER:
                if(i > 0 && *tokenArray[i-1]->value == ';'){
                  item* found_item;
                  printf("454pars->%d\n", var_num);
                  if((found_item = search_var(variable_table, var_tracer, current_token->value))){
                        printf("478pars: UndeclaredKing\n");
                        edit_variable(found_item, tokenArray, &i, &current_node);
                        printf("490pars: %s\n", current_node->value);
                        print_tree(current_node, "totalTreeAfterediting", 0);
                  }else{
                        printf("Cannot edit undeclared variables. Aborting...\n");
                        exit(1);
                  }
                }else{
                    printf("A program starting with editing variables, or missing a semicolon. Rookie Mistakes. Aborting...\n");
                    exit(1);
                }
                printf("IDENTIFIER identified\n");
                break;
            case INT:
                printf("INTEGER %s\n", tokenArray[i]->value);
                break;
            case OPERATOR:
                printf("operator incoming\n");
                break;
            default:
                break;
        }
        i++;

    }
    if(!root){
        perror("Not possible\n");
        exit(1);
    }

    for(int i = 0; i<var_num; i++){
        printf("Nihilus: %s\n", variable_table[i]->key);
    }
    printf("soda\n");
    *variable_s = variable_table;
    return root;
}




