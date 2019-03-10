// otp_dec_d.c
//
// Phi Luu
//
// Oregon State University
// CS_344_001_W2019 Operating Systems 1
// Program 4: OTP
//
// This program performs exactly like  otp_enc_d , in syntax and usage. In this
// case, however,  otp_dec_d  will decrypt ciphertext it is given, using the
// passed-in ciphertext and key. Thus, it returns plaintext again to  otp_dec .
//
// Syntax:
//
//   ./otp_dec_d listening_port
//   ./otp_dec_d listening_port &
//
// where
//   listening_port  port this program listens on
//
// This program is recommended to run in background, using the second syntax.
//
// Outputs:
//   All output text is output to  stdout , if any.
//   All error text is output to  stderr , if any.

#include <stdio.h>

int main(void) {
    printf("Hello world!\n");

    return 0;
}
