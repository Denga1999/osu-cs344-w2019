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

#include <stdio.h>

int main(void) {
    printf("Hello world!\n");

    return 0;
}
