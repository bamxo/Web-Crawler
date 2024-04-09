#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "crawler.h"
#include "curl.h"
#include "bag.h"
#include "pagedir.h"
#include "url.h"
#include "webpage.h"

const char *webpage_getURL(const webpage_t *page) {
    return page->url;
}

int webpage_getDepth(const webpage_t *page) {
    return page->depth;
}

const char *webpage_getHTML(const webpage_t *page) {
    return page->html;
}

static void pageScan(webpage_t *page, bag_t *pagesToCrawl, hashtable_t *pagesSeen, int maxDepth);

/**
 * Parses command-line arguments, placing the corresponding values into the pointer arguments seedURL,
 * pageDirectory and maxDepth. argc and argv should be passed in from the main function.
 */
static void parseArgs(const int argc, char *argv[], char **seedURL, char **pageDirectory, int *maxDepth) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s seedURL pageDirectory maxDepth\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    *seedURL = strdup(argv[1]);
    *pageDirectory = strdup(argv[2]);
    *maxDepth = atoi(argv[3]);

    // Validate maxDepth
    if (*maxDepth < 0 || *maxDepth > 10) {
        fprintf(stderr, "Error: maxDepth must be in the range [0..10]\n");
        exit(EXIT_FAILURE);
    }
}

void crawl(char *seedURL, char *pageDirectory, const int maxDepth) {
    if (!pagedir_init(pageDirectory)) {
        fprintf(stderr, "Failed to initialize page directory\n");
        exit(EXIT_FAILURE);
    }

    bag_t *pagesToCrawl = bag_new();
    hashtable_t *pagesSeen = hashtable_new(100);

    int documentID = 1;
    int fileCount = 0;  // Counter for the number of files printed

    webpage_t *seedPage = webpage_new(seedURL, 0, "");
    bag_insert(pagesToCrawl, seedPage);
    hashtable_insert(pagesSeen, seedURL, NULL);

    while (!bag_is_empty(pagesToCrawl) && fileCount < 133) {
        webpage_t *currentSeedPage = bag_extract(pagesToCrawl);

        printf("%d\tFetched: %s\n", currentSeedPage->depth, currentSeedPage->url);

        size_t htmlSize = 0;
        char *html = download(currentSeedPage->url, &htmlSize);
        currentSeedPage->html = html;

        if (currentSeedPage->html != NULL) {
            pagedir_save(currentSeedPage, pageDirectory, documentID++);

            if (currentSeedPage->depth < maxDepth) {
                printf("%d\tScanning: %s\n", currentSeedPage->depth, currentSeedPage->url);
                pageScan(currentSeedPage, pagesToCrawl, pagesSeen, maxDepth);
            }

            // Increment the file count after saving
            fileCount++;
        }

        webpage_delete(currentSeedPage);
    }

    bag_delete(pagesToCrawl);
    hashtable_delete(pagesSeen, NULL);
}

void pageScan(webpage_t *currentPage, bag_t *crawlPages, hashtable_t *visitedPages, int maxScanDepth) {
    if (currentPage == NULL || crawlPages == NULL || visitedPages == NULL) {
        fprintf(stderr, "Error: NULL pointer in pageScan.\n");
        return;
    }

    const char *startTag = "<a";
    const char *hrefAttr = "href=\"";
    const char *endQuote = "\"";

    const char *content = webpage_getHTML(currentPage);

    if (content == NULL) {
        printf("Invalid HTML content for URL: %s\n", webpage_getURL(currentPage));
        return;
    }

    size_t startTagLen = strlen(startTag);
    size_t hrefAttrLen = strlen(hrefAttr);
    size_t endQuoteLen = strlen(endQuote);

    while ((content = strstr(content, startTag)) != NULL) {
        const char *hrefPos = strstr(content, hrefAttr);

        if (hrefPos == NULL || hrefPos < content) {
            content += startTagLen;
            continue;
        }

        content = hrefPos + hrefAttrLen;

        const char *end = strstr(content, endQuote);

        if (end != NULL) {
            size_t urlLen = end - content;

            if (urlLen > 0) {
                char *urlExtracted = malloc(urlLen + 1);

                if (urlExtracted != NULL) {
                    strncpy(urlExtracted, content, urlLen);
                    urlExtracted[urlLen] = '\0';

                    if (strchr(urlExtracted, '#') != NULL) {
                        content = end + endQuoteLen;
                        free(urlExtracted);
                        continue;
                    }

                    char *normalizedURL = normalizeURL(webpage_getURL(currentPage), urlExtracted);

                    if (normalizedURL != NULL) {
                        if (isInternalURL(webpage_getURL(currentPage), normalizedURL)) {
                            if (!hashtable_find(visitedPages, normalizedURL)) {
                                printf("%d\tFound: %s\n", webpage_getDepth(currentPage), normalizedURL);
                                hashtable_insert(visitedPages, normalizedURL, NULL);

                                webpage_t *normalizedPage = webpage_new(normalizedURL, webpage_getDepth(currentPage) + 1, content);
                                bag_insert(crawlPages, normalizedPage);
                                printf("%d\tAdded: %s\n", webpage_getDepth(currentPage) + 1, normalizedURL);
                            } else {
                                printf("%d\tIgnDupl: %s\n", webpage_getDepth(currentPage), normalizedURL);
                            }
                        } else {
                            printf("%d\tIgnExtrn: %s\n", webpage_getDepth(currentPage), normalizedURL);
                        }
                        free(normalizedURL);
                    } else {
                        printf("Failed to normalize URL: %s\n", urlExtracted);
                    }

                    free(urlExtracted);
                } else {
                    printf("Memory allocation failed for extracted URL\n");
                }
            } else {
                printf("Invalid URL length detected\n");
            }
        } else {
            printf("Invalid URL length detected\n");
        }

        // Update content to continue searching for links
        content = end + endQuoteLen;
    }
}

int main(const int argc, char *argv[]) {
    char* seedURL;
    char* pageDirectory;
    int maxDepth;

    parseArgs(argc, argv, &seedURL, &pageDirectory, &maxDepth);

    // Create the page directory if it doesn't exist
    if (!pagedir_init(pageDirectory)) {
        fprintf(stderr, "Error: Unable to create or access the page directory\n");
        exit(EXIT_FAILURE);
    }

    crawl(seedURL, pageDirectory, maxDepth);

    free(seedURL);
    free(pageDirectory);

    return EXIT_SUCCESS;
}
