#include "pagedir.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

bool pagedir_init(const char *pageDirectory) {
    struct stat st;

    // Check if the directory already exists
    if (stat(pageDirectory, &st) == 0) {
        // Directory exists
        if (S_ISDIR(st.st_mode)) {
            return true; // Directory is valid
        } else {
            fprintf(stderr, "Error: %s is not a directory.\n", pageDirectory);
            return false;
        }
    } else {
        // Try to create the directory
        if (mkdir(pageDirectory, 0700) == 0) {
            return true; // Directory created successfully
        } else {
            fprintf(stderr, "Error: Unable to create directory %s.\n", pageDirectory);
            return false;
        }
    }
}

void pagedir_save(const webpage_t *page, const char *pageDirectory, const int documentID) {
    // Construct the filename
    char filename[100]; // Adjust the size as needed
    snprintf(filename, sizeof(filename), "%s/%d", pageDirectory, documentID);

    // Open the file for writing
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Error: Unable to open file %s for writing.\n", filename);
        return;
    }

    // Write the URL, depth, and HTML content to the file
    fprintf(file, "%s\n%d\n%s", page->url, page->depth, page->html);

    // Close the file
    fclose(file);
}
