#include <stdlib.h>
#include <stdio.h>

/**
 * This will print all the environmental variables to standard output.
 *
 */
int main(int argc, char* argv[], char* env[]) {
    // loops through and prints all the environmental variables.
    for (int i = 0; env[i] != NULL; i++) {
        printf("\n%s", env[i]);
    } // for
} // main
