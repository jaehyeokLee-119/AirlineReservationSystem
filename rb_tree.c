#include "rb_tree.h"

node* nil;

node* create_node(int key) {
    node* new = (node*) malloc(sizeof(node));
    
    new->color = RED;
    new->key = key;
    new->parent = NULL;
    new->left = NULL;
    new->right = NULL;

    return new;
}   

void* rotate_right(node** root, node* x) {
    node* y = x->left;
    x->left = y->right;
    if (y->right != nil) {
        y->right->parent = x;
    }
    y->parent = x->parent;

    if (x->parent == nil || x->parent == NULL) {
        (*root) = y;
    } else {
        if (x == x->parent->left) {
            x->parent->left = y;
        }
        else {
            x->parent->right = y;
        }

    }
    y->right = x;
    x->parent = y;
}

void* rotate_left(node** root, node* x) {
    node* y = x->right;
    x->right = y->left;
    if (y->left != nil) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == nil || x->parent == NULL) {
        (*root) = y;
    } else {
        if (x == x->parent->right) {
            x->parent->right = y;
        }
        else {
            x->parent->left = y;
        }
    }

    y->left = x;
    x->parent = y;
}

void insert_fixup(node** root, node* z) {
    node* y;
    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            y = z->parent->parent->right;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    rotate_left(root, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rotate_right(root, z->parent->parent);
            }
        } else {
            y = z->parent->parent->left;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rotate_right(root, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rotate_left(root, z->parent->parent);
            }
        }       
    }
    (*root)->color = BLACK;
}

node* rbt_insert(node** root, node* z) {
    node* y = nil;
    node* x = (*root);
    while (x != nil) {
        y = x;
        if (z->key < x->key) 
            x = x->left;
        else
            x = x->right;
    }
    z->parent = y;
    if (y == nil) 
        (*root) = z;
    else if (z->key < y->key)
        y->left = z;
    else 
        y->right = z;

    z->left = nil;
    z->right = nil;
    z->color = RED;

    insert_fixup(&(*root), z);
}

node* tree_successor(node** root, node* x) {
    if (x->right != NULL && x->right != nil) {
        node* c = x->right;
        while(c->left != NULL && c->left != nil) {
            c = c->left;
        }
        return c;
    } else {
        node* y = x->parent;
        while ((y!=NULL && y!=nil) && x == y->right) {
            x = y;
            y = y->parent;
        }
        return y;
    }
}

void delete_fixup(node** root, node* x) {
    node* w;
    while (x != (*root) && x->color == BLACK) {
        if (x == x->parent->left) {
            w = x->parent->right;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rotate_left(root, x->parent);
                w = x->parent->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    rotate_right(root, w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                rotate_left(root, x->parent);
                x = (*root);
            }
        } else {
            w = x->parent->left;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rotate_right(root, x->parent);
                w = x->parent->left;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    rotate_left(root, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                rotate_right(root, x->parent);
                x = (*root);
            }
        }
    }
    x->color = BLACK;
}

void rbt_delete(node** root, node* z){
    node* y = z;
    node* x;
    color_t ycolor = y->color;
    if (z->left == nil || z->right == nil) {
        y = z;
    } else {
        y = tree_successor(root, z);
    }
    if (y->left != nil) {
        x = y->left;
    } else {
        x = y->right;
    }
    x->parent = y->parent;
    if (y->parent == nil) {
        (*root) = x;
    } else {
        if (y == y->parent->left) {
            y->parent->left = x;
        } else {
            y->parent->right = x;
        }
    }
    if (y != z) {
        z->key = y->key;
    }
    if (y->color == BLACK) {
        delete_fixup(root, x);
    }
}

int nearest_neighbor(node** root, int val) {
    node* current = (*root);
    int found_value = current->key;
    int min_diff = val>min_diff ? val-min_diff : min_diff-val;
    int tmp_diff;
    while(1) {
        if (val == current->key) {
            return current->key;
        } else {
            tmp_diff = (val > current->key) ? val - current->key : current->key - val;
            if (tmp_diff < min_diff) {
                found_value = current->key;
                min_diff = tmp_diff;
            }
            if (val > current->key) {
                current = current->right;
            } else {
                current = current->left;
            }
            if (current == nil) {
                return found_value;
            }
        }
    }
}

node* rbt_search(node** root, int val) {
    node* current = (*root);
    while(1) {
        if (val == current->key) {
            return current;
        } else {
            if (val > current->key) {
                current = current->right;
            } else {
                current = current->left;
            }
            if (current == nil) {
                return NULL;
            }
        }
    }
}

void recurrent_print(node* node, int tabs) {
    if (node != nil) {
        recurrent_print(node->right, tabs+1);
        for(int i = 0; i < tabs-1; i++) printf("%13s"," ");
        printf("    \033[0;30m%2d--\033[0m", node->parent->key);
        if (node->color == RED) 
            //printf("(R %2d) ", node->key);
            printf("\033[0;31m[%2d]\033[0m", node->key);
        else 
            //printf("(B %2d) ", node->key);
            printf("\033[0;37m[%2d]\033[0m", node->key);
        puts("");
        recurrent_print(node->left, tabs+1);
    }
}
void print_inorder(node* node) {
    printf("---------------------------------------------------------\n");
    if (node != nil) {
        recurrent_print(node->right, 1);
        if (node->color == RED) 
            printf("\033[0;31m[%2d]\033[0m", node->key);
        else 
            printf("[%2d]", node->key);
        puts("");
        recurrent_print(node->left, 1);
    }
    printf("---------------------------------------------------------\n");
}

node* rbt_init() {
    node* nil = create_node(-1);
    nil->color = BLACK;
    return nil;
}
/*
int main() {
    node* root = init_rbt();
    nil = root;

	srand(time(NULL));
	int A[15];
	int it = 0;
	int tmp;

	while(it < 15) {
        tmp = rand()%50;
		int sign = 0;
        for(int i = 0; i < it; i++) {
            if (A[i] == tmp) {
				sign = 1;
                break;
            }
        }
		if (sign == 0) {
			A[it] = tmp;
			it++;
		} else continue;
    }

    printf("2) inserting A[0:15]\n[");
    for(int i = 0; i < 15; i++) {
        node* n = create_node(A[i]);
        if (i != 14) 
            printf("%d, ", A[i]);
        else 
            printf("%d]\n", A[i]);
        insert(&root, n);
    }
    
    print_inorder(root);


    printf("\n\n3) inserting 13, 22, 21, 11, 45\n");
    int B[5] = {13, 22, 21, 11, 45};
    for(int j = 0; j < 5; j++) {
        int sign = 0;
        for(int i = 0; i < 15; i++) { // 중복을 체크하며 inserting
            if (A[i] == B[j]) {
                sign = 1;
                break;
            }
        }
        if (sign == 0) { // no duplicated key
            // B[j] insert
            node* n = create_node(B[j]);
            insert(&root, n);
            printf("\n3-%d>: Insert %d: \n", j+1, B[j]);
            print_inorder(root);
        } else { // duplicated key
            printf("\n3-%d>: %d is already in the Tree\n", j+1, B[j]);
        }
    }
    
    // 4: Nearest Neighbor 5, 23, 38
    printf("\n4) NEARST-NEIGHBOT of  5, 23, 38\n");
    print_inorder(root);
    printf("NEAREST-NEIGHBOR(T, %d): %d\n", 5, nearest_neighbor(&root, 5));
    printf("NEAREST-NEIGHBOR(T, %d): %d\n", 23, nearest_neighbor(&root, 23));
    printf("NEAREST-NEIGHBOR(T, %d): %d\n", 38, nearest_neighbor(&root, 38));

    // 5: Deletion
    
    printf("\n5) Deletion of  13, 22, 45, 11, root from the Tree\n");
    printf("Delete 13: \n");
    delete(&root, search(&root, 13));
    print_inorder(root);
    
    printf("\nDelete 22: \n");
    delete(&root, search(&root, 22));
    print_inorder(root);
    
    printf("\nDelete 45: \n");
    delete(&root, search(&root, 45));
    print_inorder(root);
    
    printf("\nDelete 11: \n");
    delete(&root, search(&root, 11));
    print_inorder(root);
    
    printf("\nDelete root(%d): \n", root->key);
    delete(&root, search(&root, root->key));
    print_inorder(root);


}
*/