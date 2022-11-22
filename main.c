#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef enum { RED, BLACK } color_t;

typedef char city_name;

typedef struct rbt_node {
    color_t color;
    int key;
    struct rbt_node *parent;
    struct rbt_node *left;
    struct rbt_node *right;
} rbt_node;

rbt_node* nil;

typedef struct link_node {
	city_name name;
	struct link_node* nextPtr;
}link_node;

rbt_node* create_node(int key) {
    rbt_node* new = (rbt_node*) malloc(sizeof(rbt_node));
    
    new->color = RED;
    new->key = key;
    new->parent = NULL;
    new->left = NULL;
    new->right = NULL;

    return new;
}   
void* rotate_right(rbt_node** root, rbt_node* x) {
    rbt_node* y = x->left;
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
void* rotate_left(rbt_node** root, rbt_node* x) {
    rbt_node* y = x->right;
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
void insert_fixup(rbt_node** root, rbt_node* z) {
    rbt_node* y;
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
rbt_node* rbt_insert(rbt_node** root, rbt_node* z) {
    rbt_node* y = nil;
    rbt_node* x = (*root);
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
rbt_node* tree_successor(rbt_node** root, rbt_node* x) {
    if (x->right != NULL && x->right != nil) {
        rbt_node* c = x->right;
        while(c->left != NULL && c->left != nil) {
            c = c->left;
        }
        return c;
    } else {
        rbt_node* y = x->parent;
        while ((y!=NULL && y!=nil) && x == y->right) {
            x = y;
            y = y->parent;
        }
        return y;
    }
}
void delete_fixup(rbt_node** root, rbt_node* x) {
    rbt_node* w;
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
void rbt_delete(rbt_node** root, rbt_node* z){
    rbt_node* y = z;
    rbt_node* x;
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
rbt_node* rbt_search(rbt_node** root, int val) {
    rbt_node* current = (*root);
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
void recurrent_print(rbt_node* node, int tabs) {
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
void print_inorder(rbt_node* node) {
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
rbt_node* rbt_init() {
    rbt_node* nil = create_node(-1);
    nil->color = BLACK;
    return nil;
}


void insert(link_node** ptr_homePtr, city_name value) {
    link_node* new;
    new = (link_node*)malloc(sizeof(link_node));
    new->name = value;
    new->nextPtr = NULL;
    link_node* current;
    if (*ptr_homePtr == NULL) {
        *ptr_homePtr = new;
        return;
    } else {
        current = *ptr_homePtr;
        while(current->nextPtr) {
            current = current->nextPtr;
        }
        current->nextPtr = new;
    }
}

int alphabet_to_int(city_name alphabet) {
    return (int) (alphabet-'a');
}


int main() {
    rbt_node* root = rbt_init();
    nil = root;

    int reservation_num = 0;

    rbt_node* n = create_node(5);
    rbt_insert(&root, n);
    print_inorder(root);

	link_node* graph[26] = {NULL};
    
	srand(time(NULL));
    city_name adjacencies[26][10] = {0};
    // adjacencies에는 a-z를 0-25로 표현 
    for (int i = 0; i < 26; i++) {
        printf("%c -", i+'a');
        for (int j = 0; j < 10; j++) {
            adjacencies[i][j] = -1;
            printf("  %c", adjacencies[i][j]+'a');
        }
        puts("");
    }
    // -1로 초기화

    for (int i = 0; i < 26; i++) {
        for (int j = 0; j < 10; j++) {
            int tmp = rand()%26;

            // (i, tmp) 쌍이 이미 있는지 확인해야 함
            // 
            adjacencies[i][j] = tmp;
        }
    }

    for (int i = 0; i < 26; i++) {
        printf("%c -", i+'a');
        for (int j = 0; j < 10; j++) {
            printf("  %c", adjacencies[i][j]+'a');
        }
        puts("");
    }



}