/*********************************************************
* my-unzip.c
*
* Your Name: Matthew Imiolek
*********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LENGTH 10000

int main(int argc, char *argv[]) {
  //Check for input files
  if (argc < 2) {
    printf("my-unzip: file1 [file2 ...]\n");
    exit(1);
  }

  // Go through and de-compress the file
  for(int i = 1; i < argc; ++i) {
    FILE *fp = NULL;
    fp = fopen(argv[i], "r");

    // Checks if the file is valid, if not returns error statement and exits
    if (fp == NULL) {
      printf("my-unzip: cannot open file\n");
      exit(1);
    }

    // Variable to hold the number of a given letter in a row
    int number;
    // Variable that holds the letter
    char letter;
    
    // Goes through the file and reads first an integer and then a character.
    // It then prints the character the integer number of times.
    while(fread(&number, sizeof(int), 1, fp) &&
    fread(&letter, sizeof(char), 1, fp)) {
      for(int i = 0; i < number; i++) {
        printf("%c", letter);
      }
    }

    fclose(fp);
  }

  return 0;
}
