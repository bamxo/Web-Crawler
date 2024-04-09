#ifndef WEBPAGE_H
#define WEBPAGE_H

#include <stdlib.h>
#include <string.h>

typedef struct {
    char *url;
    int depth;
    char *html;
} webpage_t;

webpage_t *webpage_new(const char *url, int depth, const char *html);

const char *webpage_getURL(const webpage_t *page);

int webpage_getDepth(const webpage_t *page);

const char *webpage_getHTML(const webpage_t *page);

void webpage_delete(webpage_t *page);

#endif
