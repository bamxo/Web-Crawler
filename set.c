#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "set.h"

// Node structure for each (key, item) pair
typedef struct node {
    char *key;
    void *item;
    struct node *next;
} node_t;

// Set structure
struct set {
    node_t *head;  // Head of the linked list
};

// Create a new (empty) set; return NULL if error.
set_t *set_new(void) {
    set_t *new_set = malloc(sizeof(set_t));
    if (new_set == NULL) {
        return NULL;  // Allocation error
    }

    new_set->head = NULL;
    return new_set;
}

// Helper function to find a node with the given key
static node_t *find_node(set_t *set, const char *key) {

    if (set == NULL || key == NULL) {
        return NULL;  // Invalid set or key
    }

    for (node_t *current = set->head; current != NULL; current = current->next) {
        if (strcmp(current->key, key) == 0) { // error here
            return current;  // Found the node with the key
        }
    }

    return NULL;  // Key not found
}

// Insert item, identified by a key (string), into the given set.
bool set_insert(set_t *set, const char *key, void *item) {
    if (set == NULL || key == NULL || item == NULL) {
        return false;  // Invalid set, key, or item
    }

    if (find_node(set, key) != NULL) {
        return false;  // Key already exists in the set
    }

    // Create a new node and copy the key string
    node_t *new_node = malloc(sizeof(node_t));
    if (new_node == NULL) {
        return false;  // Allocation error
    }

    new_node->key = strdup(key);
    if (new_node->key == NULL) {
        free(new_node);
        return false;  // Allocation error
    }

    new_node->item = item;
    new_node->next = set->head;
    set->head = new_node;

    return true;
}

// Return the item associated with the given key;
void *set_find(set_t *set, const char *key) {
    node_t *node = find_node(set, key);
    return (node != NULL) ? node->item : NULL;
}

// Print the whole set; provide the output file and func to print each item.
void set_print(set_t *set, FILE *fp, void (*itemprint)(FILE *fp, const char *key, void *item)) {
    if (set == NULL) {
        fprintf(fp, "(null)\n");
        return;
    }

    for (node_t *current = set->head; current != NULL; current = current->next) {
        itemprint(fp, current->key, current->item);
    }
}

// Iterate over all items in the set, in undefined order.
void set_iterate(set_t *set, void *arg, void (*itemfunc)(void *arg, const char *key, void *item)) {
    if (set == NULL || itemfunc == NULL) {
        return;
    }

    for (node_t *current = set->head; current != NULL; current = current->next) {
        itemfunc(arg, current->key, current->item);
    }
}

// Helper function to delete a node and its key
static void delete_node(node_t *node, void (*itemdelete)(void *item)) {
    if (itemdelete != NULL) {
        itemdelete(node->item);
    }

    free(node->key);
    free(node);
}

// Delete the whole set; ignore NULL set.
void set_delete(set_t *set, void (*itemdelete)(void *item)) {
    if (set == NULL) {
        return;
    }

    while (set->head != NULL) {
        node_t *temp = set->head;
        set->head = temp->next;
        delete_node(temp, itemdelete);
    }

    // Do not free(set);  // Do not free the set structure here
}



