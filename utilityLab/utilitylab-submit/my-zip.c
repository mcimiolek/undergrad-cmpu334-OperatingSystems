/*********************************************************
* my-zip.c
*
* Your Name: Matthew Imiolek
*********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LENGTH 10000

int main(int argc, char *argv[]) {
  int count = -1;
  char runChar;

  //Check for input files
  if (argc < 2) {
    printf("my-zip: file1 [file2 ...]\n");
    exit(1);
  }
  // Go through an compress the file
  for(int i = 1; i < argc; ++i) {
    FILE *fp = NULL;
    fp = fopen(argv[i], "r");

    // Checks if the file is valid, if not returns error statement and exits
    if (fp == NULL) {
      printf("my-zip: cannot open file\n");
      exit(1);
    }

    int readCount;
    char *buffer = malloc(LENGTH * sizeof(char));

    // Goes through the file and puts each character in a buffer. Also sets up
    // the buffer, and also sets the size of the buffer.
    while((readCount = fread(buffer, sizeof(char), LENGTH, fp))) {

      // Set up for initial character in buffer
      if (count == -1) {
        count = 0;
        runChar = buffer[0];
      }

      // Counts the number of characters in a row and compress them when a new
      // letter comes
      for(int i = 0; i < readCount; i++) {
        // If the same letter as the previous is present, increases the count of
        // that letter
        if(buffer[i] == runChar) {
          count++;
        }
        // Otherwise writes the count of the previous letter and that letter to
        // the output file and resets the count and sets the new letter to be
        // compared to
        else {
          fwrite(&count, 1, sizeof(int), stdout);
          fwrite(&runChar, 1, sizeof(char), stdout);
          count = 1;
          runChar = buffer[i];
        }
      }
    }

    fclose(fp);
  }

  // Account for the last case which will never enter the else statement as it
  // is the end of the file
  fwrite(&count, 1, sizeof(int), stdout);
  fwrite(&runChar, 1, sizeof(char), stdout);

  return 0;
}
