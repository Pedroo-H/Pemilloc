#ifndef AVLHEADER_H
#define AVLHEADER_H

typedef struct Node {
    void* target; // Change to VALUES.
    int height;
    struct Node *left, *right; 
} Node;

// Utils
int max(int, int);

// Methods to facilitate using malloc or pemilloc
int avl_retrieve_value(void* target);
void* allocate_memory(size_t size);
void* deallocate_memory(void* target);

// Avl's methods
int avl_height(Node *node);
int avl_get_balance(Node *node);

Node* avl_create_node(void* target);
Node* avl_insert_node(Node *node, void* value);
Node* avl_remove_node(Node *node, void* value);

Node* avl_get_min(Node *node);

Node* avl_right_rotate(Node *x);
Node* avl_left_rotate(Node *y);
#endif