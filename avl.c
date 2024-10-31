#include "avl.h"
#include "chunksconfig.h"
#include "mymalloc.h"

// Utils
int max(int a, int b) {
    return a > b ? a : b;
}

int avl_retrieve_value(void* target) {
    return ((Chunk*)target)->size; 
}

void* allocate_memory(size_t size) {
    return mymalloc(size);
}

void* deallocate_memory(void* target) {
    myfree(target); 
}

// AVL methods
int avl_height(Node *node) {
    return node ? node->height : 0;
}

int avl_balance(Node *node) {
    return avl_height(node->left) - avl_height(node->right);
}

Node* avl_min_node(Node *node) {
    Node *current = node;
    while (current && current->left) {
        current = current->left;
    }
    return current;
}

Node* avl_create_node(void* target) {
    Node *node = (Node*)allocate_memory(sizeof(Node));
    if (!node) return NULL; 
    node->value = target; 
    node->right = NULL;
    node->left = NULL;
    node->height = 1; 
    return node;
}

Node* avl_right_rotate(Node *y) {
    Node *x = y->left;
    Node *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(avl_height(y->left), avl_height(y->right)) + 1;
    x->height = max(avl_height(x->left), avl_height(x->right)) + 1;

    return x;
}

Node* avl_left_rotate(Node *x) {
    Node *y = x->right;
    Node *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(avl_height(x->left), avl_height(x->right)) + 1;
    y->height = max(avl_height(y->left), avl_height(y->right)) + 1;

    return y;
}

Node* avl_insert_node(Node *node, void* value) {
    if (node == NULL) {
        return avl_create_node(value);
    }

    if (avl_retrieve_value(value) < avl_retrieve_value(node->value)) {
        node->left = avl_insert_node(node->left, value);
    } else if (avl_retrieve_value(value) >= avl_retrieve_value(node->value)) {
        node->right = avl_insert_node(node->right, value);
    }

    node->height = 1 + max(avl_height(node->left), avl_height(node->right));

    int balance = avl_balance(node);

    if (balance > 1 && avl_retrieve_value(value) < avl_retrieve_value(node->left->value)) {
        return avl_right_rotate(node);
    }

    if (balance < -1 && avl_retrieve_value(value) > avl_retrieve_value(node->right->value)) {
        return avl_left_rotate(node);
    }

    if (balance > 1 && avl_retrieve_value(value) > avl_retrieve_value(node->left->value)) {
        node->left = avl_left_rotate(node->left);
        return avl_right_rotate(node);
    }

    if (balance < -1 && avl_retrieve_value(value) < avl_retrieve_value(node->right->value)) {
        node->right = avl_right_rotate(node->right);
        return avl_left_rotate(node);
    }

    return node; 
}

Node* avl_remove_node(Node *node, void* value) {
    if (node == NULL) {
        return node;
    }

    if (avl_retrieve_value(value) < avl_retrieve_value(node->value)) {
        node->left = avl_remove_node(node->left, value);
    } else if (avl_retrieve_value(value) > avl_retrieve_value(node->value)) {
        node->right = avl_remove_node(node->right, value);
    } else {
        if ((node->left == NULL) || (node->right == NULL)) {
            Node *temp = node->left ? node->left : node->right;

            if (temp == NULL) {
                temp = node;
                node = NULL;
            } else {
                *node = *temp;
            }
            deallocate_memory(temp); 
        } else {
            Node *temp = avl_min_node(node->right);
            node->value = temp->value; 
            node->right = avl_remove_node(node->right, temp->value);
        }
    }

    if (node == NULL) {
        return node;
    }

    node->height = 1 + max(avl_height(node->left), avl_height(node->right));

    int balance = avl_balance(node);

    if (balance > 1 && avl_balance(node->left) >= 0) {
        return avl_right_rotate(node);
    }

    if (balance > 1 && avl_balance(node->left) < 0) {
        node->left = avl_left_rotate(node->left);
        return avl_right_rotate(node);
    }

    if (balance < -1 && avl_balance(node->right) <= 0) {
        return avl_left_rotate(node);
    }

    if (balance < -1 && avl_balance(node->right) > 0) {
        node->right = avl_right_rotate(node->right);
        return avl_left_rotate(node);
    }

    return node; 
}
