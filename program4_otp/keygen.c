// keygen.c
//
// Phi Luu
//
// Oregon State University
// CS_344_001_W2019 Operating Systems 1
// Program 4: OTP
//
// This program creates a key file of specified length. The characters in the
// file generated are any of the 27 allowed characters, generated using the
// standard UNIX randomization methods. Note that the randomization is very
// simple and is not secure. The last character keygen outputs is a newline.
//
// Syntax:
//
//   ./keygen keylength
//
// where
//   keylength  length of the key file in characters
//
// Outputs:
//   All output text is output to  stdout , if any.
//   All error text is output to  stderr , if any.
//
// Note: 27 allowed characters are 26 capital letters and the space character.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define RAND_RANGE     (char)27  // 26 capital alphabet letters + 1 space
#define SPACE_RAND_IDX (char)(RAND_RANGE - 1)  // 0-25 for letters, 26 for space

int main(int argc, char** argv) {
    // ensure correct usage of command line arguments
    if (argc != 2) {
        fprintf(stderr, "Usage: ./keygen keylength\n");
        return 1;
    }

    // try convert 1st arg from string to int
    // atoi()  returns 0 on non-numeric string, but in this case  keylength
    // must be positive. So, if the return value is not positive, exit
    int keylength = atoi(argv[1]);
    if (keylength <= 0) {
        fprintf(stderr, "Usage: ./keygen keylength\n");
        fprintf(stderr, " keylength  must be a positive integer\n");
        return 1;
    }

    // seed the random number generator
    srand(time(0));

    // create a random string character by character
    for (int i = 0; i < keylength; i++) {
        // randomize an integer between [0, RAND_RANGE - 1]
        char rand_idx = rand() % RAND_RANGE;

        // if randomized to index used for space, output space
        if (rand_idx == SPACE_RAND_IDX) {
            printf(" ");
        } else {
            // otherwise (randomized to a capital letter), output that letter
            printf("%c", 'A' + rand_idx);
        }
    }

    // end the key with a newline
    printf("\n");

    return 0;
}
