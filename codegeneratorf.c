#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include"lexerf.h"
#include"parserf.h"
#include<unistd.h>
#include<string.h>
#include<limits.h>
#include"codegeneratorf.h"
#include<unistd.h>
#include<wait.h>


extern int var_num;

int operations_int(int a, int b, char op){
    if(op == '+')return a+b;
    if(op == '-')return a-b;
    if(op == '*')return a*b;
    if(op == '/')return a/b;
    if(op == '%')return a%b;

    exit(-1);
}
int operations_asm(int a, int b, char op, FILE* fp){
    if(op == '+'){
        fprintf(fp, "mov rax, %d\nadd rax, %d\n", a, b);
        return a+b;
    }
    else if(op == '-'){
        fprintf(fp, "mov rax, %d\nsub rax, %d\n", a, b);
        return a-b;
    }
    else if(op == '*'){
        fprintf(fp, "mov rax, %d\nimul rax, %d\n", a, b);
        return a*b;
    }
    else if(op == '/'){
        fprintf(fp, "mov rax, %d\nmov r10, %d\nidiv r10\n", a, b);
        return a/b;
    }
    else if(op == '%'){
        fprintf(fp, "mov rax, %d\nmov r10, %d\nidiv r10\nmov rax, rdx\n",a , b);
        return a%b;
    }
    
    exit(-1);
}



int sysgen(char* call){
    int result;
    FILE* ff = fopen("checkout", "r");
    if(!ff){
        perror("Impossible checkout\n");
    }
    long length;
    fseek(ff, 0, SEEK_END);
    length = ftell(ff);
    fseek(ff, 0, SEEK_SET);
    char* buf = malloc(length*sizeof(char));
    fread(buf, 1, length, ff);
    buf[length*sizeof(char)-1] = '\0';
    int i = 0;
    char* current = strdup(buf);
    char* rem_str = current;
    for(char* p = strtok(current, ";"); p; p = strtok(rem_str, ";")){
            char* p2 = strdup(p);
            char* s = strtok(p2, " ");
            if(!strcmp(s, call)){
                result = atoi(p2+strlen(s)+1);
                break;
            }
            i++;
            rem_str = rem_str+strlen(p)+1;
            free(p2);
    }
    fclose(ff);
    return result;
}


static int num_vars = 1;

int calculate_node(Node** op_node, FILE* fp){
    
    if(!(*op_node)){
        perror("error op tree\n");
        exit(-1);
    }
    if((*op_node)->type == INT){
        return atoi((*op_node)->value);
    }

    if((*op_node)->left->type == OPERATOR){
        calculate_node(&((*op_node)->left), fp);
    }

    if((*op_node)->right->type == OPERATOR){
        calculate_node(&((*op_node)->right), fp);
    }

    if((*op_node)->left->type == INT && (*op_node)->right->type == INT){
        int a = atoi((*op_node)->left->value);
        int b = atoi((*op_node)->right->value);
        int digit = operations_asm(a, b, *((*op_node)->value), fp);

        if(digit < 0){
            char* newbf = (char*)malloc(sizeof(char) * strlen((*op_node)->value)+2);
            newbf[sizeof(char)*strlen((*op_node)->value) + 1] = '\0';
            sprintf(newbf, "%d", digit);
            (*op_node)->value = newbf;
        }
        else
            sprintf((*op_node)->value ,"%d",digit);

        (*op_node)->type = INT;
    }
   return atoi((*op_node)->value);
}


void traverse(Node* root, FILE* fp, calls* c){
    if(!root){
        return;
    }
    if(!strcmp(root->value, "exit")){
        printf("exits\n");
        //fprintf(fp, "exit:\n");
        //fprintf(fp, "mov rax, %d\n", sysgen("exit"));
        printf("SOLAR\n");
        c->func_call = "exit";
        c->var_name = NULL;
        c->number = sysgen("exit");
    }

    if(!strcmp(root->value, "(")){
        printf("( appeared\n");
    }

    if(!strcmp(root->value, ")")){
        printf(") appeared\n");
    }
    int t = (root->type);
    if((t == INT || t == OPERATOR) && *root->value != '='){
        int num = calculate_node(&root, fp);
        printf("Magnetar:%s\n", root->value);

        printf("black hole: %d\n", num);
        free(root->right);
        free(root->left);
        root->right = NULL;
        root->left = NULL;
        if(c->var_name){
            c->number = atoi(root->value);
        }

        fprintf(fp, "mov r10, rax\n");
    }

    if(!strcmp(root->value, "int")){
        printf("variable appeared\n");
        c->var_name = root->left->value;
        c->func_call = NULL;
    }

    if((c->func_call || c->var_name) && !strcmp(root->value, ";")){
        printf("; appeared\n");
        if(c->func_call && !strcmp(c->func_call, "exit")){
        fprintf(fp, "%s:\nmov rax, %d\nmov rdi, r10\n", c->func_call, c->number);
        fprintf(fp, "syscall\n");
        }else if(c->var_name){
            fprintf(fp,"sub rsp, 4\nmov dword [rbp-%d], %d\n", 4*num_vars, c->number);
            num_vars++;
        }
    }
    if(root->left)
        traverse(root->left, fp, c);
    if(root->right)
        traverse(root->right, fp, c);
}

int generate_code(Node* root){
    FILE* f = fopen("./assembly/gencra.asm", "w");
    if(!f){
        perror("File not opened!!\n");
        exit(1);
    }
    
    
    calls* c = (calls*)malloc(sizeof(calls));
    c->func_call = NULL;
    c->var_name = NULL;
    c->number = INT_MIN;
    int syscode;
    if(root->right){
    syscode = sysgen(root->right->value);
    }
    fprintf(f, "global _start\n");
    fprintf(f, "section .text\n_start:\n");
    if(var_num > 0){
        fprintf(f, "push rbp\nmov rbp, rsp\n");
    }
    traverse(root, f, c);
    fclose(f);
    
    
    pid_t id = fork();
    if(id == 0){
        execlp("nasm", "nasm", "-f", "elf64", "./assembly/gencra.asm", "-o", "./assembly/gencra.o", NULL);
        perror("nasm error\n");
        exit(-1);
    }else{
        waitpid(id, NULL, 0);
    }

    pid_t id2 = fork();
    if(id2 == 0){
        execlp("ld", "ld", "./assembly/gencra.o", "-o", "./assembly/gencra", NULL);
        perror("ld error\n");
        exit(-1);
    }else{
        waitpid(id2, NULL, 0);
    }

    pid_t id3 = fork();
    if(id3 == 0){
        execlp("rm", "rm", "./assembly/gencra.o", NULL);
        perror("no rm\n");
        exit(-1);
    }else{
        waitpid(id3, NULL, 0);
    }
    //system("rm ./assembly/gencra.asm");
    return 0;
}


