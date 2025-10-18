#include"hashtable.h"


item** create_hash(int n){
    item** itemp_array = (item**)malloc(sizeof(item*)*n);
    for(int i = 0; i<n; i++){
        item* itemp = (item*)malloc(sizeof(item));
        itemp->key = NULL;
        itemp->value = NULL;
        itemp->depth = -1;
        itemp->type = NULL;
        itemp->editIndex = -1;
        itemp->edits = NULL;
        itemp->totaledits = 0;
        itemp_array[i] = itemp;
    }

    return itemp_array;
}

item** resize_hash(int prev_n, int new_n, item** itemp_array){
    itemp_array = (item**)realloc(itemp_array, new_n);
    for(int i = prev_n; i<new_n; i++){
        item* itemp = (item*)malloc(sizeof(item));
        itemp->key = NULL;
        itemp->value = NULL;
        itemp->depth = -1;
        itemp->type = NULL;
        itemp_array[i] = itemp;

    }

    return itemp_array;
}


item* search_var(item** items, size_t size, const char* key){
    printf("hsh38::: %zu\n", size);
    for(int i = 0; i<size; i++){
        printf("hsh40:: %s\n", items[0]->key);
        printf("hsh43\n");
        if(!strcmp(key, items[i]->key)){
            return items[i];
        }
        printf("hsh44-> %d\n", i);
    }
    printf("hsh43\n");

    return NULL;
}
