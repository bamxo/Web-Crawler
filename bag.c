#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "bag.h"

// Structure to represent a node in the bag
struct bag_node {
    void *item;
    struct bag_node *next;
};

// Structure to represent a bag
struct bag_t {
    struct bag_node *head;
};

// Function to create a new bag
bag_t *bag_new() {
    bag_t *bag = (bag_t *)malloc(sizeof(bag_t));
    if (bag == NULL) {
        fprintf(stderr, "Error: Unable to allocate memory for bag.\n");
        exit(EXIT_FAILURE);
    }

    bag->head = NULL;
    return bag;
}

// Function to insert an item into the bag
void bag_insert(bag_t *bag, void *item) {
    struct bag_node *new_node = (struct bag_node *)malloc(sizeof(struct bag_node));
    if (new_node == NULL) {
        fprintf(stderr, "Error: Unable to allocate memory for bag node.\n");
        exit(EXIT_FAILURE);
    }

    new_node->item = item;
    new_node->next = bag->head;
    bag->head = new_node;
}

// Function to check if the bag is empty
bool bag_is_empty(const bag_t *bag) {
    return bag->head == NULL;
}

// Function to extract an item from the bag
void *bag_extract(bag_t *bag) {
    if (bag_is_empty(bag)) {
        fprintf(stderr, "Error: Attempt to extract from an empty bag.\n");
        exit(EXIT_FAILURE);
    }

    struct bag_node *node = bag->head;
    void *item = node->item;
    bag->head = node->next;
    free(node);

    return item;
}

// Function to delete the bag
void bag_delete(bag_t *bag) {
    while (!bag_is_empty(bag)) {
        bag_extract(bag);
    }

    free(bag);
}
