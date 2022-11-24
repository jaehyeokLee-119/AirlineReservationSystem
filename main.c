#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef enum { RED, BLACK } color_t;

typedef char city_name;

typedef struct link_node {
	city_name name;
	struct link_node* nextPtr;
}link_node;

typedef struct reservation_node {
	city_name name;
    int time;
    int date;
	struct reservation_node* nextPtr;
}reservation_node;


typedef struct stack_node {
	city_name name;
    int departure_time;
	struct stack_node* nextPtr;
}stack_node;

typedef struct rbt_node {
    color_t color;
    int rid;
    char name[6];
    reservation_node* reservation;
    struct rbt_node *parent;
    struct rbt_node *left;
    struct rbt_node *right;
} rbt_node;

typedef struct Stack {
    stack_node* top;
    int found;
} Stack;


rbt_node* nil;

rbt_node* create_node(int key, reservation_node* reservation_path, char* name) {
    rbt_node* new = (rbt_node*) malloc(sizeof(rbt_node));
    
    new->color = RED;
    new->rid = key;
    new->reservation = reservation_path;
    if (name != NULL) {
        for(int i = 0; i < 6; i++) 
            new->name[i] = name[i];
    }
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
        if (z->rid < x->rid) 
            x = x->left;
        else
            x = x->right;
    }
    z->parent = y;
    if (y == nil) 
        (*root) = z;
    else if (z->rid < y->rid)
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
        z->rid = y->rid;
    }
    if (y->color == BLACK) {
        delete_fixup(root, x);
    }
}
rbt_node* rbt_search(rbt_node** root, int val) {
    rbt_node* current = (*root);
    while(1) {
        if (val == current->rid) {
            return current;
        } else {
            if (val > current->rid) {
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
        printf("    \033[0;30m%2d--\033[0m", node->parent->rid);
        if (node->color == RED) 
            //printf("(R %2d) ", node->key);
            printf("\033[0;31m[%2d]\033[0m", node->rid);
        else 
            //printf("(B %2d) ", node->key);
            printf("\033[0;37m[%2d]\033[0m", node->rid);
        puts("");
        recurrent_print(node->left, tabs+1);
    }
}
void print_inorder(rbt_node* node) {
    printf("---------------------------------------------------------\n");
    if (node != nil) {
        recurrent_print(node->right, 1);
        if (node->color == RED) 
            printf("\033[0;31m[%2d]\033[0m", node->rid);
        else 
            printf("[%2d]", node->rid);
        puts("");
        recurrent_print(node->left, 1);
    }
    printf("---------------------------------------------------------\n");
}
rbt_node* rbt_init() {
    rbt_node* nil = create_node(-1, NULL, NULL);
    nil->color = BLACK;
    return nil;
}


int alphabet_to_int(city_name alphabet) {
    return (int) (alphabet-'a');
}
char* int_to_timeString(int time) {
    int hour = time/60;
    int min = time%60;
    int is_pm = (hour/12);
    hour = hour%12;
    char input[8];
    char* to_ret = NULL;

    if (hour == 0) {
        hour = 12;
    }
    input[0] = hour/10+'0';
    input[1] = hour%10+'0';
    input[2] = ':';
    input[3] = min/10+'0';
    input[4] = min%10+'0';

    if (is_pm >= 1) {
        input[5] = 'p';
    } else {
        input[5] = 'a';
    }
    input[6] = 'm';

    int start_point;
    if (hour<10) {
        to_ret = (char*)malloc(sizeof(char)*(7));
        start_point = 1;
    }
    else {
        to_ret = (char*)malloc(sizeof(char)*(8));
        start_point = 0;
    }

    for(int i = 0; start_point < 7; i++) {
        to_ret[i] = input[start_point];
        start_point++;
    }
    to_ret[start_point] = '\0';
    return to_ret;
}

void adjacency_insert(link_node** ptr_homePtr, city_name value) {
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

int graph_search(link_node* graph[26], city_name src, city_name dst) {
    link_node *tmp = graph[src];
    while(tmp) {
        if (tmp->name == dst) {
            return 1;
            break;
        }
        tmp = tmp->nextPtr;
    }
    return 0;
}
void stack_push(Stack *s, city_name value, int time) {
    stack_node* node_to_push = (stack_node*) malloc (sizeof(stack_node));
    if (node_to_push == NULL) return;
    node_to_push->name = value;
    node_to_push->departure_time = time;
    node_to_push->nextPtr = s->top;
    s->top = node_to_push;
}
city_name stack_pop(Stack *s) {
    stack_node* node_to_pop = s->top;
    if (node_to_pop == NULL) return -1;

    city_name data = node_to_pop->name;
    s->top = node_to_pop->nextPtr;
    free(node_to_pop);
    return data;
}
int stack_traverse(Stack *s) {
    stack_node *curPos = s->top;
    while (curPos) {
        printf("%c(%s) ", 'a'+curPos->name, int_to_timeString(curPos->departure_time));
        curPos = curPos->nextPtr;
    }
    printf("\n");
    return 0;
}
Stack *InitStack(void){
    Stack *stack = (Stack *)malloc(sizeof(Stack)); 
    stack->top = NULL;
    stack->found = 0;
    return stack;
}
city_name stack_top(Stack *s) {
    return s->top->name;
}
int stack_search(Stack *s, city_name name) {
    stack_node *curPos = s->top;

    while (curPos) {
        if (curPos->name == name) {
            return 1;
        }
        curPos = curPos->nextPtr;
    }
    return 0;
}

void print_adjacency(link_node* graph[26]) {
    for(int i = 0; i < 26; i++) {
        link_node *a = graph[i];
        printf("%c: ", i+'a');
        while(a) {
            printf("- %c", a->name+'a');
            a = a->nextPtr;
        }
        puts("");
    }
}
void print_adjacency_and_schedule(link_node* graph[26], int schedule[26][26]) {
    for(int i = 0; i < 26; i++) {
        link_node *a = graph[i];
        printf("%c: ", i+'a');
        while(a) {
            printf("- %c(%d)", a->name+'a',schedule[i][a->name]);
            a = a->nextPtr;
        }
        puts("");
    }
}



void initialize_adjacencies(link_node* graph[26]) {
    city_name adjacencies[26][10] = {0};
    static int loop_num = 0;
    // initialize adjacency list to all values -1
    // i: a~z - 0~25 
    for (int i = 0; i < 26; i++) {
        for (int j = 0; j < 10; j++) {
            adjacencies[i][j] = -1;
        }
    }

    int selected[26] = { 0 };
    int min_selected = 0;

    for (int i = 0; i < 26; i++) {
        for (int j = 0; j < 10; j++) {
            if (adjacencies[i][j] != -1) 
                    continue;
                    
            int tmp;
            while(1) {
                loop_num++;

                if (min_selected == 10) {
                    break;
                }
                tmp = rand()%26;
                
                int sign = 0;

                if (i == tmp) {  // avoid self-edge
                    //printf("self, i,tmp = %d\n", i);
                    continue;
                }

                if (selected[tmp] != min_selected) { // min_selected probing
                    int tmp_min = 99;
                    for(int ii = 0; ii < 26; ii++) {
                        if (tmp_min > selected[ii] && ii != i) {
                            tmp_min = selected[ii];
                        }
                    }
                    if (tmp_min > selected[i]) {
                        min_selected += 1;
                        continue;
                    }       // dealing with the case of 'i' is min_selected city 
                    continue;
                    // only select 'least selected cities'
                } 

                for (int jj = 0; jj < 10; jj++) { // avoid already set path to same city
                    if (adjacencies[i][jj] == tmp) {
                        sign = 1;
                        break;
                    }
                }

                if (sign == 0) {
                    int complete = 0;
                    // search empty path slot of target city
                    for (int k = 0; k < 10; k++) {
                        if (adjacencies[tmp][k] == -1) { // if there is an empty path slot in target city
                            complete = 1;
                            adjacencies[i][j] = tmp;
                            adjacencies[tmp][k] = i;
                            // fill both cities entry
                            selected[i]++;
                            selected[tmp]++;
                            // raise select count by 1
                            break;
                        }
                    }
                    if (complete == 0)
                        continue;
                    else // if there isn't an empty slot in target city
                        break;
                }
            }
            min_selected = 99; // re-calculate min_selected
            for(int a = 0; a < 26; a++) {
                if (min_selected > selected[a])
                    min_selected = selected[a]; 
            }
        }
    }


    for (int i = 0; i < 26; i++) {
        for (int j = 0; j < 10; j++) {
            adjacency_insert(&graph[i], adjacencies[i][j]);
        }
    }
}

Stack* stack_copy(Stack* src, Stack* dst) {
    stack_node* cur_tmp = src->top;
    while(cur_tmp) {
        stack_push(dst, cur_tmp->name, cur_tmp->departure_time);
        cur_tmp = cur_tmp->nextPtr;
    }
}

void reservation_insert(reservation_node** ptr_homePtr, city_name value, int time, int date) {
    reservation_node* new;
    new = (reservation_node*)malloc(sizeof(reservation_node));
    new->name = value;
    new->time = time;
    new->nextPtr = NULL;
    new->date = date;
    if (time >= 1440) { // when arrival time exceed 11:59pm
        new->time -= 1440;
        new->date = (new->date+1)%31;        // store next day
    }
    reservation_node* current;
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


void print_reservation(reservation_node* reserv) {
    reservation_node* cur = reserv;
    printf("%c(%s, %d)", cur->name+'a', int_to_timeString(cur->time), cur->date);
    cur = cur->nextPtr;
    while(cur) {
        printf("-%c(%s, %d)", cur->name+'a', int_to_timeString(cur->time), cur->date);
        cur = cur->nextPtr;
    }
}

void stack_to_reservation(Stack* stack, reservation_node** reserv, int date) {
    stack_node* cur = stack->top;
    while(cur) {
        if (cur->nextPtr != NULL) {
            reservation_insert(reserv, cur->name, cur->nextPtr->departure_time-60, date);
            cur = cur->nextPtr;
        } else {
            reservation_insert(reserv, cur->name, cur->departure_time, date);
            cur = cur->nextPtr;
            break;
        }
    }
}

Stack* recursive_pathfinding(city_name src, city_name dst, int time, Stack* stack, link_node* graph[26], int schedule[26][26], Stack* res_stack) {
    if (stack->found == 1) {
        return NULL;
    }
    stack_push(stack, src, time);
    if (stack_top(stack) == dst) {
        //printf("path found\n");
        //stack_traverse(stack);
        stack_copy(stack, res_stack);
        stack->found = 1;
        res_stack->found = 1;
        return NULL;
    } else {
        if (time < 1440) {
            city_name arrivals[10];       // arrivals[10]: all 10 arrivals that directly reachable from src(current location)
            link_node* current = graph[src];
            for(int i = 0; i < 10; i++) { // schedule[src][arrivals[i]]: departure time of src->arrivals[i]
                arrivals[i] = current->name;
                current = current->nextPtr;
                // printf("-%c(%s) ", arrivals[i]+'a', int_to_timeString(schedule[src][arrivals[i]]));
                if (current == NULL) break;
            }

            int is_there_anywhere_to_go = 0;
            for(int i = 0; i < 10; i++) {
                if (schedule[src][arrivals[i]] >= time) {
                    if (!stack_search(stack, arrivals[i])){ // select cities haven't visited
                        recursive_pathfinding(arrivals[i], dst, schedule[src][arrivals[i]]+60, stack, graph, schedule, res_stack);
                        is_there_anywhere_to_go = 1;
                    }
                }
            }
            if (is_there_anywhere_to_go == 0) {
                stack_pop(stack);
                return NULL;
            }
        } else {
            stack_pop(stack);
            return NULL;
        }
    }
    stack_pop(stack);
    return NULL;
}


reservation_node* init_pathfinding(city_name src, city_name dst, int date, link_node* graph[26], int schedule[26][26]) {
    int time = 0;
    Stack* stack = InitStack();
    Stack* res_stack = InitStack();
    recursive_pathfinding(src, dst, time, stack, graph, schedule, res_stack);
    if (res_stack->found == 1) {
        //stack_traverse(res_stack);
        reservation_node* reserv = NULL;
        stack_to_reservation(res_stack, &reserv, date);
        // printf("reserv(city): %c\n", reserv->name+'a');
        return reserv;
    } else {
        return NULL;
    }


    
    /*
    rbt에 저장할 행렬 구현
    현재 도착 시간으로 구현되어 있는데, 딱 출력만 하면 되도록 재구성하기

    <from (current)>
    각 도시에 도착한 시각 (출발지는 12:00am)

    <to>
    마지막을 제외한 경로: 도시에서 출발한 시각
        > 현재 도시에서 다음 도시를 보고 출발시각 schedule을 보고 넣음
    마지막: 마지막 도시에 도착한 시각



    */

    // stack_traverse(stack);

}


int main() {
    rbt_node* root = rbt_init();
    nil = root;

    int reservation_num = 0;
    /*
    rbt_node* n = create_node(5);
    rbt_insert(&root, n);
    print_inorder(root);
    */

	link_node* graph[26] = {NULL};
    
	srand(time(NULL));
    initialize_adjacencies(graph);
    /* 
    flight paths are in 'graph[26][10]'
        every pathes from(to) 'i' to(from) 'j'
            graph['i'][0~9] = 'j'
    
    the depart time of each one-way path is stored in 
        departure_schedule[i][j]
        it means
            the departure time from a city 'i' to a city 'j' is
                departure_schedule[i][j]
        it represents the departure time as an int number [0,1435], with the unit of minute (steps 5) 

        if there is no flight schedule 'i' to 'j', departure_schedule[i][j] to be -1
    */
    
    // Creating Departure schedule 
    int departure_schedule[26][26] = {0};
    for (int i = 0; i < 26; i++) {
        for(int j = 0; j < 26; j++) {
            if (graph_search(graph, i, j) == 1)
                departure_schedule[i][j] = rand()%288 * 5;
            else 
                departure_schedule[i][j] = -1;
        }
    }

    // -------------- //
    // Execution Part //
    // -------------- //
    char r_name[6];
    char r_src;
    char r_dst;
    int r_date;

    // input
    
    /*
    scanf("%c%c%c%c%c, %c, %c, %d", &r_name[0], &r_name[1], &r_name[2], &r_name[3], &r_name[4], &r_src, &r_dst, &r_date);
    r_name[5] = '\0';
    printf("name: %s\nsrc: %c\ndst: %c\ndate: %d\n", r_name, r_src, r_dst, r_date);
    */
    
    // print_adjacency_and_schedule(graph,departure_schedule);
    reservation_node* reservation_path = NULL;

    
    rbt_node* n;
    // insertion 10;
    for(int i = 0; i < 10; i++) {
        scanf("%c%c%c%c%c, %c, %c, %d", &r_name[0], &r_name[1], &r_name[2], &r_name[3], &r_name[4], &r_src, &r_dst, &r_date);
        r_name[5] = '\0';
        // printf("name: %s\nsrc: %c\ndst: %c\ndate: %d\n", r_name, r_src, r_dst, r_date);

        reservation_path = init_pathfinding(r_src-'a', r_dst-'a', r_date, graph, departure_schedule);
            // find path and store it in 'reservation_path'

        if (reservation_path == NULL) {
            printf("No path exists for that source, destination pair. Try again please\n");
            i--;
            continue;
        }
        n = create_node(reservation_num++, reservation_path, r_name);   
            // make rbt_node 'n' with the path found

        rbt_insert(&root, n);   // insert rbt_node to rbt tree 
        printf("%s, %d, ", n->name, n->rid);
        print_reservation(n->reservation);  // 
        puts(""); 
        print_inorder(root);
        getchar();  // clearing buffer with popping out '\n' from the buffer
    }
    // deletion 10;
    for(int i = 0; i < 5; i++) {

    }


}