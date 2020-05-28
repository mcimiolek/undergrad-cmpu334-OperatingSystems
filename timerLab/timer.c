#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>


int main(int argc, char *argv[]) {

    int nb;  // Number of bytes read
    long oneTime = 0; // Holds time for one system call
    long totTime = 0; // Holds the total time of all runs
    int iterations = 100000000; // Number of times to run loop
    long avgTime = 0; // Average time for system call
    struct timespec start, end; // Clock times

    //start loop
    for (int i = 0; i < iterations; i++) {

      // use clock_gettime with MONOTONIC
      clock_gettime(CLOCK_MONOTONIC, &start);

      // What a useful system call!
      nb = write(fileno(stdout), NULL, 0);

      clock_gettime(CLOCK_MONOTONIC, &end);

      // Calculate system call time and print it
      oneTime = ((end.tv_sec - start.tv_sec) * 1000000000) + (end.tv_nsec - start.tv_nsec);
      totTime = totTime + oneTime;
    }

    avgTime = totTime / iterations;
    printf("Average elapsed timeL: %ld\n", avgTime);

    if (nb != 0) {
        printf("Error with read\n");
    }
    return 0;
}
