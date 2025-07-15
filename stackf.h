#ifndef _STACK_H_
#define _STACK_H_

typedef struct stack{
    char* value;
    struct stack* next;
}stack;


stack* create_stack_element(char* value);

void push(stack** top, char* value);

int popStack(stack** top);

char* peep(stack* top);

int empty_stack(stack* top);


#endif
