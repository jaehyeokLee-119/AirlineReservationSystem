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
node* init_rbt();
node* create_node(int key);
node* insert(node** root, node* z);
void delete(node** root, node* z);
