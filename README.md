# Web Crawler Implementation
The Web Crawler is a standalone program designed to crawl the web, starting from a specified "seed" URL. It retrieves web pages, parses their content, and recursively explores embedded URLs up to a given depth.

# Crawling Logic
- Crawls internal pages up to the specified maxDepth.
- Ignores external pages (different domain names).
- Saves each explored page to the pageDirectory with a unique document ID.

#Error Handling
Exits with an error message to stderr and non-zero exit status if an unrecoverable error is encountered, including:

 - Out of memory
 - Invalid number of command-line arguments
 - Invalid seedURL
 - Invalid maxDepth or out of range
 - Unable to create a file in pageDirectory

## Usage
You can compile by running:
```
make
```

Then you can run the program by running:
```
./crawler seedURL pageDirectory maxDepth
```

