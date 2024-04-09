#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"
#include "set.h"

struct hashtable
{
    int num_slots;
    set_t **slots;
};

// Hash function
static unsigned long hash(const char *str, int num_slots)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
    {
        hash = ((hash << 5) + hash) + c;
    }

    return hash % num_slots;
}

hashtable_t *hashtable_new(const int num_slots)
{
    if (num_slots <= 0)
    {
        return NULL;
    }

    hashtable_t *ht = (hashtable_t *)malloc(sizeof(hashtable_t));
    if (ht == NULL)
    {
        return NULL;
    }

    ht->num_slots = num_slots;
    ht->slots = (set_t **)malloc(num_slots * sizeof(set_t *));
    if (ht->slots == NULL)
    {
        free(ht);
        return NULL;
    }
    
    for (int i = 0; i < num_slots; i++)
    {
        ht->slots[i] = set_new();
        if (ht->slots[i] == NULL)
        {
            for (int j = 0; j < i; j++)
            {
                set_delete(ht->slots[j], NULL);
            }
            free(ht->slots);
            free(ht);
            return NULL;
        }
    }

    return ht;
}

bool hashtable_insert(hashtable_t *ht, const char *key, void *item) {
    if (ht == NULL || key == NULL || item == NULL) {
        return false; // Invalid input
    }

    int slot_index = hash(key, ht->num_slots);
    return set_insert(ht->slots[slot_index], key, item);
}

void *hashtable_find(hashtable_t *ht, const char *key) {
    if (ht == NULL || key == NULL) {
        return NULL; // Invalid input
    }

    int slot_index = hash(key, ht->num_slots);
    return set_find(ht->slots[slot_index], key);
}

void hashtable_print(hashtable_t *ht, FILE *fp, void (*itemprint)(FILE *fp, const char *key, void *item)) {
    if (fp == NULL || ht == NULL || itemprint == NULL) {
        return; // Invalid input
    }

    for (int i = 0; i < ht->num_slots; i++) {
        set_print(ht->slots[i], fp, itemprint);
    }
}

void hashtable_iterate(hashtable_t *ht, void *arg, void (*itemfunc)(void *arg, const char *key, void *item)) {
    if (ht == NULL || arg == NULL || itemfunc == NULL) {
        return; // Invalid input
    }

    for (int i = 0; i < ht->num_slots; i++) {
        set_iterate(ht->slots[i], arg, itemfunc);
    }
}

void hashtable_delete(hashtable_t *ht, void (*itemdelete)(void *item)) {
    if (ht == NULL) {
        return; // Invalid input
    }

    for (int i = 0; i < ht->num_slots; i++) {
        set_delete(ht->slots[i], itemdelete);
    }

    free(ht->slots);
    free(ht);
}



