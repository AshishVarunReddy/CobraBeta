#include<stdlib.h>
#include<stdio.h>
#include<string.h>

typedef struct stack{
    char* value;
    struct stack* next;
}stack;

stack* create_stack_element(char* value){
    stack* newSt = (stack*)malloc(sizeof(stack));
    newSt->value = value;
    //printf("cluser: %s", newSt->value);
newSt->next = NULL;
    return newSt;
}

void push(stack** top, char* value){
       stack* newStack = (stack*)malloc(sizeof(stack));
       newStack->value = value;
       newStack->next = (*top);
       (*top) = newStack;
}

int popStack(stack** top){
    if(*top == NULL)return -1;
    if(!strcmp((*top) -> value, "STACK_END")){
        printf("stack empty");
        return -1;
    }
    stack* temp = *top;
    *top = temp->next;
    free(temp);
    temp = NULL;
    return 0;
}

char* peep(stack* top){
    return top->value;
}



int empty_stack(stack* top){
    if(!strcmp(top->value, "STACK_END")){
        return -1;
    }

    return 0;
}



