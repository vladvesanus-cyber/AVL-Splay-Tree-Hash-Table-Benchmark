#include <stdlib.h>

typedef struct avl_node {
    int value, height;
    struct avl_node *left;
    struct avl_node *right;
} AVL_NODE;

static inline int avl_max(int a, int b) {
    return (a > b) ? a : b;
}

static inline int avl_height(AVL_NODE *node) {
    return (node == NULL) ? 0 : node->height;
}

static AVL_NODE *avl_rotate_left(AVL_NODE *node) {
    AVL_NODE *new_root = node->right;
    AVL_NODE *temp = new_root->left;
    new_root->left = node;
    node->right = temp;
    node->height = 1 + avl_max(avl_height(node->left), avl_height(node->right));
    new_root->height = 1 + avl_max(avl_height(new_root->left), avl_height(new_root->right));
    return new_root;
}

static AVL_NODE *avl_rotate_right(AVL_NODE *node) {
    AVL_NODE *new_root = node->left;
    AVL_NODE *temp = new_root->right;
    new_root->right = node;
    node->left = temp;
    node->height = 1 + avl_max(avl_height(node->left), avl_height(node->right));
    new_root->height = 1 + avl_max(avl_height(new_root->left), avl_height(new_root->right));
    return new_root;
}

AVL_NODE *avl_find(AVL_NODE *root, int value) {
    while (root != NULL) {
        if (root->value == value) return root;
        else if (root->value > value) root = root->left;
        else root = root->right;
    }
    return NULL;
}

AVL_NODE *avl_insert(AVL_NODE *node, int value, int *count) {
    if (node == NULL) {
        node = (AVL_NODE*)malloc(sizeof(AVL_NODE));
        node->value = value;
        node->left = NULL;
        node->right = NULL;
        node->height = 1;
        (*count)++;
        return node;
    }
    if (value < node->value)
        node->left = avl_insert(node->left, value, count);
    else if (value > node->value)
        node->right = avl_insert(node->right, value, count);
    else
        return node;

    node->height = 1 + avl_max(avl_height(node->left), avl_height(node->right));
    int balance = avl_height(node->left) - avl_height(node->right);

    if (balance > 1 && value < node->left->value)
        return avl_rotate_right(node);                          // LL
    if (balance < -1 && value > node->right->value)
        return avl_rotate_left(node);                           // RR
    if (balance > 1 && value > node->left->value) {
        node->left = avl_rotate_left(node->left);
        return avl_rotate_right(node);                          // LR
    }
    if (balance < -1 && value < node->right->value) {
        node->right = avl_rotate_right(node->right);
        return avl_rotate_left(node);                           // RL
    }
    return node;
}

AVL_NODE *avl_delete(AVL_NODE *node, int value, int *count) {
    if (node == NULL) return NULL;
    if (value < node->value)
        node->left = avl_delete(node->left, value, count);
    else if (value > node->value)
        node->right = avl_delete(node->right, value, count);
    else {
        if (node->left != NULL && node->right != NULL) {
            AVL_NODE *successor = node->right;
            AVL_NODE *successor_parent = node;
            while (successor->left != NULL) {
                successor_parent = successor;
                successor = successor->left;
            }
            node->value = successor->value;
            if (successor_parent == node)
                successor_parent->right = successor->right;
            else
                successor_parent->left = successor->right;
            free(successor);
            (*count)--;
        } else {
            AVL_NODE *child = (node->left != NULL) ? node->left : node->right;
            free(node);
            (*count)--;
            return child;
        }
    }

    node->height = 1 + avl_max(avl_height(node->left), avl_height(node->right));
    int balance = avl_height(node->left) - avl_height(node->right);

    if (balance > 1) {
        int lb = avl_height(node->left->left) - avl_height(node->left->right);
        if (lb >= 0) return avl_rotate_right(node);             // LL
        node->left = avl_rotate_left(node->left);
        return avl_rotate_right(node);                          // LR
    }
    if (balance < -1) {
        int rb = avl_height(node->right->left) - avl_height(node->right->right);
        if (rb <= 0) return avl_rotate_left(node);              // RR
        node->right = avl_rotate_right(node->right);
        return avl_rotate_left(node);                           // RL
    }
    return node;
}

void avl_free(AVL_NODE *node) {
    if (node == NULL) return;
    avl_free(node->left);
    avl_free(node->right);
    free(node);
}