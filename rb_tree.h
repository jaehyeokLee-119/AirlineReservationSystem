#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef enum { RED, BLACK } color_t;

typedef struct node {
    color_t color;
    int key;
    struct node *parent;
    struct node *left;
    struct node *right;
} node;

void print_inorder(node* node);
node* rbt_init();
node* create_node(int key);
node* rbt_insert(node** root, node* z);
void rbt_delete(node** root, node* z);
