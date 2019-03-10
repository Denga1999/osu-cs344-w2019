// otp_enc.c
//
// Phi Luu
//
// Oregon State University
// CS_344_001_W2019 Operating Systems 1
// Program 4: OTP
//
// This program connects to  otp_enc_d  and asks it to perform a one-time pad
// style encryption. By itself,  otp_enc  doesn't do the encyption-- otp_enc_d
// does.
//
// Syntax:
//
//   ./otp_enc plaintext key port
//
// where
//   plaintext  file containing the plaintext to encrypt
//   key        file containing the encryption key to use to encrypt the text
//   port       port this program attempts to connect to  otp_enc_d  on
//
// Outputs:
//   All output text is output to  stdout , if any.
//   All error text is output to  stderr , if any.

#include <stdio.h>

int main(void) {
    printf("Hello world!\n");

    return 0;
}
