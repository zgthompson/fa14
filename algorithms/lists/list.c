#include <stdio.h>
#include <stdlib.h>

typedef struct list {
    int item;
    struct list *next;
} list;


list *list_search(list *l, int x)
{
    if (l == NULL) return NULL;

    if (l->item == x) return(l);

    else return ( list_search(l->next, x) );
}


void list_insert(list **l, int x)
{
    list *p;

    p = malloc( sizeof(list) );
    p->item = x;
    p->next = *l;
    *l = p;
}


list *list_predecessor(list *l, int x)
{
    if ((l==NULL) || (l->next == NULL)) {
        return NULL;
    }

    if ((l->next)->item == x) return l;

    else return list_predecessor(l->next, x);
}


void list_delete(list **l, int x)
{
    list *p;
    list *pred;

    p = list_search(*l, x);
    if (p != NULL) {
        pred = list_predecessor(*l, x);
        if (pred == NULL) {
            *l = p->next;
        }
        else {
            pred->next = p->next;
        }

        free(p);
    }
}


void list_print(list *l) {
    list *curNode;
    curNode = l;

    while (curNode != NULL ) {
        printf("%d ", curNode->item);
        curNode = curNode->next;
    }

    printf("\n");
}


int main() {

    list *root;

    root = malloc( sizeof(list) );
    root->item = 5;
    root->next = NULL;

    list_print(root);

    list_insert(&root, 6);
    list_print(root);

    list_delete(&root, 5);
    list_print(root);

    return 0;
}
