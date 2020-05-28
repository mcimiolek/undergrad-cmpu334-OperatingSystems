/*********************************************************
* my-grep.c
*
* Your Name: Matthew Imiolek
*********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LENGTH 1000

int main(int argc, char *argv[]) {
  size_t len = 0;
  // Set up buffer
  char *buffer = NULL;

  // Checks if grep was given arguments
  if (argc < 2) {
    printf("my-grep: searchterm [file ...]\n");
    exit(1);
  }

  // In the case no input files are given, checks stdin for input
  if (argc == 2) {
    while(getline(&buffer, &len, stdin) != -1) {
      if(strstr(buffer, argv[1]) != NULL) {
        printf("%s", buffer);
      }
    }

    return 0;
  }

  // Go through each file searching for the search term
  for(int i = 2; i != argc; ++i) {
    FILE *fp = NULL;
    fp = fopen(argv[i], "r");

    // Checks if the file is valid, if not returns error statement and exits
    if (fp == NULL) {
      printf("my-grep: cannot open file\n");
      exit(1);
    }

    // Otherwise search all given files for the search term and return those
    // lines
    else {
      while(getline(&buffer, &len, fp) != -1) {
        if(strstr(buffer, argv[1]) != NULL) {
          printf("%s", buffer);
        }
      }
    }

    fclose(fp);
  }

  return 0;
}
