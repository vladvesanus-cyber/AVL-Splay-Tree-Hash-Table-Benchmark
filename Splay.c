#include <stdlib.h>

typedef struct splay_node {
    int value;
    struct splay_node *left;
    struct splay_node *right;
} SPLAY_NODE;

SPLAY_NODE *splay_rotate_left(SPLAY_NODE *x) {
    SPLAY_NODE *y = x->right;
    x->right = y->left;
    y->left = x;
    return y;
}

SPLAY_NODE *splay_rotate_right(SPLAY_NODE *x) {
    SPLAY_NODE *y = x->left;
    x->left = y->right;
    y->right = x;
    return y;
}

SPLAY_NODE *splay(SPLAY_NODE *root, int key) {
    if (!root) return NULL;

    SPLAY_NODE N, *l, *r;
    N.left = N.right = NULL;
    l = r = &N;

    while (1) {
        if (key < root->value) {
            if (!root->left) break;
            if (key < root->left->value) {
                root = splay_rotate_right(root);
                if (!root->left) break;
            }
            r->left = root;
            r = root;
            root = root->left;
        }
        else if (key > root->value) {
            if (!root->right) break;
            if (key > root->right->value) {
                root = splay_rotate_left(root);
                if (!root->right) break;
            }
            l->right = root;
            l = root;
            root = root->right;
        }
        else break;
    }

    l->right = root->left;
    r->left = root->right;
    root->left = N.right;
    root->right = N.left;

    return root;
}

SPLAY_NODE *splay_insert(SPLAY_NODE *root, int value, int *count) {
    if (!root) {
        SPLAY_NODE *n = malloc(sizeof(SPLAY_NODE));
        n->value = value;
        n->left = n->right = NULL;
        (*count)++;
        return n;
    }

    root = splay(root, value);

    if (root->value == value)
        return root;

    SPLAY_NODE *n = malloc(sizeof(SPLAY_NODE));
    n->value = value;
    (*count)++;

    if (value < root->value) {
        n->left = root->left;
        n->right = root;
        root->left = NULL;
    } else {
        n->right = root->right;
        n->left = root;
        root->right = NULL;
    }

    return n;
}

SPLAY_NODE *splay_find(SPLAY_NODE *root, int value) {
    return splay(root, value);
}

SPLAY_NODE *splay_delete(SPLAY_NODE *root, int value, int *count) {
    if (!root) return NULL;

    root = splay(root, value);

    if (root->value != value)
        return root;

    SPLAY_NODE *tmp;
    if (!root->left) {
        tmp = root->right;
    } else {
        tmp = splay(root->left, value);
        tmp->right = root->right;
    }

    free(root);
    (*count)--;

    return tmp;
}

void splay_free(SPLAY_NODE *node) {
    if (!node) return;
    splay_free(node->left);
    splay_free(node->right);
    free(node);
}
