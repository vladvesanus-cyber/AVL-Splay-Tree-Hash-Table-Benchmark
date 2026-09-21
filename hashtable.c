#include <stdio.h>
#include <stdlib.h>

typedef enum { CHAINING, DOUBLE_HASHING } HashType;
typedef enum { EMPTY, OCCUPIED, DELETED } State;

typedef struct HashNode {
    int key;
    struct HashNode *next;
} Node;

typedef struct {
    int key;
    State state;
} Entry;

typedef struct {
    void *table; 
    int buckets;
    int elements;
    HashType type;  
} HashTable;

void resize(HashTable *ht);

int is_prime(int n) {
    if (n <= 1) return 0;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) return 0;
    }
    return 1;
}

int next_prime(int n) {
    while (!is_prime(n)) n++;
    return n;
}

int Hash_function(int key, int buckets) {
    return ((key % buckets) + buckets) % buckets;
}

void init_hash_table(HashTable *ht, int buckets, HashType type) {
    ht->buckets = (type == DOUBLE_HASHING) ? next_prime(buckets) : buckets;
    ht->elements = 0;
    ht->type = type;
    if (type == CHAINING) {
        ht->table = malloc(ht->buckets * sizeof(Node*));
        for (int i = 0; i < ht->buckets; i++) {
            ((Node**)ht->table)[i] = NULL;
        }
    } else if (type == DOUBLE_HASHING) {
        ht->table = malloc(ht->buckets * sizeof(Entry));
        for (int i = 0; i < ht->buckets; i++) {
            ((Entry*)ht->table)[i].state = EMPTY;
        }
    }
}

void insert_chaining(HashTable *ht, int key) {
    int index = Hash_function(key, ht->buckets);
    Node *new_node = (Node*)malloc(sizeof(Node));
    if (!new_node) return;
    new_node->key = key;
    new_node->next = ((Node**)ht->table)[index];
    ((Node**)ht->table)[index] = new_node;
    ht->elements++;
}

void insert_double(HashTable *ht, int key) {
    int index = Hash_function(key, ht->buckets);
    int step = 1 + Hash_function(key, ht->buckets - 1);
    int i = 0;
    
    
    while (((Entry*)ht->table)[index].state == OCCUPIED) {
        if (((Entry*)ht->table)[index].key == key) return;
        i++;
        if (i >= ht->buckets) return; 
        index = (Hash_function(key, ht->buckets) + i * step) % ht->buckets;
    }
    ((Entry*)ht->table)[index].key = key;
    ((Entry*)ht->table)[index].state = OCCUPIED;
    ht->elements++;
}

void delete_chaining(HashTable *ht, int key) {
    int index = Hash_function(key, ht->buckets);
    Node *current = ((Node**)ht->table)[index];
    Node *prev = NULL;
    while (current != NULL) {
        if (current->key == key) {
            if (prev == NULL) {
                ((Node**)ht->table)[index] = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            ht->elements--;
            return;
        }
        prev = current;
        current = current->next;
    }
}

void delete_double(HashTable *ht, int key) {
    int index = Hash_function(key, ht->buckets);
    int step = 1 + Hash_function(key, ht->buckets - 1);
    int i = 0;
    while (((Entry*)ht->table)[index].state != EMPTY) {
        if (((Entry*)ht->table)[index].state == OCCUPIED &&
            ((Entry*)ht->table)[index].key == key) {
            ((Entry*)ht->table)[index].state = DELETED;
            ht->elements--;
            return;
        }
        i++;
        if (i >= ht->buckets) return;
        index = (Hash_function(key, ht->buckets) + i * step) % ht->buckets;
    }
}

void resize(HashTable *ht) {
    int old_buckets = ht->buckets;
    void *old_table = ht->table;

    
    int new_size = (ht->type == DOUBLE_HASHING) ? next_prime(old_buckets * 2) : old_buckets * 2;
    
    
    init_hash_table(ht, new_size, ht->type);
    
    ht->elements = 0; 

    if (ht->type == CHAINING) {
        for (int i = 0; i < old_buckets; i++) {
            Node *current = ((Node**)old_table)[i];
            while (current != NULL) {
                insert_chaining(ht, current->key);
                Node *temp = current;
                current = current->next;
                free(temp); 
            }
        }
    } else {
        for (int i = 0; i < old_buckets; i++) {
            Entry *e = &((Entry*)old_table)[i];
            if (e->state == OCCUPIED) {
                insert_double(ht, e->key);
            }
        }
    }
    free(old_table);
}

void search_chaining(HashTable *ht, int key) {
    int index = Hash_function(key, ht->buckets);
    Node *current = ((Node**)ht->table)[index];
    while (current != NULL) {
        if (current->key == key) return;
        current = current->next;
    }
}

void search_double(HashTable *ht, int key) {
    int index = Hash_function(key, ht->buckets);
    int step = 1 + Hash_function(key, ht->buckets - 1);
    int i = 0;
    while (((Entry*)ht->table)[index].state != EMPTY) {
        if (((Entry*)ht->table)[index].state == OCCUPIED &&
            ((Entry*)ht->table)[index].key == key) {
            return;
        }
        i++;
        if (i >= ht->buckets) return;
        index = (Hash_function(key, ht->buckets) + i * step) % ht->buckets;
    }
}

void free_hash(HashTable *ht) {
    if (ht->type == CHAINING) {
        for (int i = 0; i < ht->buckets; i++) {
            Node *current = ((Node**)ht->table)[i];
            while (current != NULL) {
                Node *temp = current;
                current = current->next;
                free(temp);
            }
        }
    }
    free(ht->table);
}