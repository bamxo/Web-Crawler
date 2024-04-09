#ifndef BAG_H
#define BAG_H

#include <stdbool.h>

typedef struct bag_t bag_t;

// Function to create a new bag
bag_t *bag_new();

// Function to insert an item into the bag
void bag_insert(bag_t *bag, void *item);

// Function to extract an item from the bag
void *bag_extract(bag_t *bag);

// Function to check if the bag is empty
bool bag_is_empty(const bag_t *bag);

// Function to delete the bag
void bag_delete(bag_t *bag);

#endif
