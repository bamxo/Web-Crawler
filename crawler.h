#ifndef CRAWLER_H
#define CRAWLER_H

#include "bag.h"
#include "hashtable.h"
#include "set.h"
#include "webpage.h"

typedef struct bag_t {
    webpage_t *page;
    struct bag *next;
} bag_t;

typedef struct {
    char *url;
    struct set *next;
} set;

typedef struct hashtable {
    set **list;
    int size;
} hashtable_t;

bool webpage_fetch(webpage_t *page);

int webpage_getNextURL(const webpage_t *page, int pos, char **url);


#endif
