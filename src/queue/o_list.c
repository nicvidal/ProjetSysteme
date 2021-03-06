#include "o_list.h"

struct node *createnode(void *data) {
    struct node *newNode = malloc(sizeof(struct node));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

struct list *emptylist() {
    struct list *list = malloc(sizeof(struct list));
    list->head = NULL;
    return list;
}

void add(void *data, struct list *list) {
    struct node *current = NULL;
    if (list->head == NULL) {
        list->head = createnode(data);
    } else {
        current = list->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = createnode(data);
    }
}

void delete(void *data, struct list *list) {
    struct node *current = list->head;
    struct node *previous = current;
    while (current != NULL) {
        if (current->data == data) {
            previous->next = current->next;
            if (current == list->head)
                list->head = current->next;
            free(current);
            return;
        }
        previous = current;
        current = current->next;
    }
}

void delete_node(struct node *node, struct list *list) {
    struct node *current = list->head;
    struct node *previous = current;
    while (current != NULL) {
        if (current == node) {
            previous->next = current->next;
            if (current == list->head)
                list->head = current->next;
            free(current);
            return;
        }
        previous = current;
        current = current->next;
    }
}

int has_next(struct node *node) {
    return node != NULL && node->next != NULL;
}

void destroy(struct list *list) {
    struct node *current = list->head;
    struct node *next;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
}

int find(struct list * list, void * elem){
    struct node* n = list->head;
    while(has_next(n)){
        if(n->data == elem)
            return 0;

        n = n->next;
    }

    return 1;
}