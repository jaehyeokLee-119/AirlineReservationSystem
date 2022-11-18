#include <stdio.h>
#include <stdlib.h>
#include "rb_tree.h"

node* nil;

int main() {
    node* root = init_rbt();
    nil = root;

    node* n = create_node(5);
    insert(&root, n);
    print_inorder(root);
}