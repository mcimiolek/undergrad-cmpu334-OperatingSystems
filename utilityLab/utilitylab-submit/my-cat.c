/*********************************************************
* my-cat.c
*
* Your Name: Matthew Imiolek
*********************************************************/
#include <stdio.h>
#include <stdlib.h>
#define LENGTH 1000

int main(int argc, char *argv[]) {

  // Loop through all input files while skipping output file argument
  for(int i = 1; i != argc; ++i) {
    FILE *fp = NULL;
    fp = fopen(argv[i], "r");

    // Checks if the file is valid, if not returns error statement and exits
    if (fp == NULL) {
      printf("my-cat: cannot open file\n");
      exit(1);
    }
    // Otherwise finds the length of the file and then goes through the file
    // line by line adding it to an output file
    else {
      // Set up the buffer
      char *buffer = malloc(LENGTH * sizeof(char));
      while(fgets(buffer,LENGTH,fp) != NULL) {
        printf("%s", buffer);
      }
    }

    // Close the file
    fclose(fp);
  }

  return 0;
}
