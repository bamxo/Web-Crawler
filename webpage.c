 #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "webpage.h"
#include "curl.h"

webpage_t *webpage_new(const char *url, int depth, const char *html) {
    webpage_t *newPage = malloc(sizeof(webpage_t));
    if (newPage == NULL) {
        fprintf(stderr, "Error: Unable to allocate memory for webpage.\n");
        exit(EXIT_FAILURE);
    }

    newPage->url = strdup(url);
    newPage->depth = depth;
    newPage->html = (html != NULL) ? strdup(html) : NULL;

    return newPage;
}

void webpage_delete(webpage_t *page) {
    if (page == NULL) {
        return;
    }

    free(page->url);
    free(page->html);
    free(page);
}

bool webpage_fetch(webpage_t *page) {
    if (page == NULL) {
        fprintf(stderr, "Error: Invalid webpage.\n");
        return false;
    }

    size_t html_size;
    char *downloaded_html = download(page->url, &html_size);

    if (downloaded_html == NULL) {
        fprintf(stderr, "Error: Failed to download content from URL: %s\n", page->url);
        return false;
    }

    // Free existing html if any
    free(page->html);

    // Allocate memory for new html
    page->html = malloc(html_size + 1);  // Add space for null terminator
    if (page->html == NULL) {
        fprintf(stderr, "Error: Unable to allocate memory for webpage content.\n");
        free(downloaded_html);
        return false;
    }

    // Copy downloaded HTML to the webpage structure
    memcpy(page->html, downloaded_html, html_size);
    page->html[html_size] = '\0';  // Null-terminate the string

    // Free the temporary buffer used for downloading
    free(downloaded_html);

    return true;
}

int webpage_getNextURL(webpage_t *page, int pos, char **url) {
    if (page == NULL || url == NULL) {
        fprintf(stderr, "Error: Invalid parameters.\n");
        return -1;
    }

    // Extract URLs from the HTML content (a simplified example)
    const char *html = webpage_getHTML(page);
    const char *start = html;
    const char *tag_start = "<a href=\"";
    const char *tag_end = "\">";

    // Handle both single and double quotes
    if (html[pos] == '\'' || html[pos] == '"') {
        tag_start = "<a href='";
        tag_end = "'>";
    }

    for (int i = 0; i <= pos; ++i) {
        const char *href = strstr(start, tag_start);
        if (href == NULL) {
            *url = NULL;
            return -1; // No more URLs
        }
        start = href + strlen(tag_start);
    }

    const char *end = strchr(start, *tag_end);
    if (end == NULL) {
        *url = NULL;
        return -1; // Malformed URL
    }

    // Allocate memory for the extracted URL
    size_t url_length = end - start;
    *url = malloc(url_length + 1);
    strncpy(*url, start, url_length);
    (*url)[url_length] = '\0';

    return pos + 1; // Move to the next URL
}
