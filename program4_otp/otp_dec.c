// otp_dec.c
//
// Phi Luu
//
// Oregon State University
// CS_344_001_W2019 Operating Systems 1
// Program 4: OTP
//
// This program connects to  otp_dec_d  and asks it to decrypt ciphertext
// using a passed-in ciphertext and key, and otherwise performs exactly like
// otp_enc , and is runnable in the same three ways. This program is not able to
// connect to  otp_enc_d , even if it tries to connect on the correct port.
//
// Syntax--three ways to run:
//
//   ./otp_dec ciphertext key port
//   ./otp_dec ciphertext key port > plaintext
//   ./otp_dec ciphertext key port > plaintext &
//
// where
//   ciphertext  file containing the ciphertext to decrypt
//   key         file containing the encryption key to use to decrypt the text
//   port        port this program attempts to connect to  otp_dec_d  on
//   plaintext   file to output the decrypted text to
//
// Outputs:
//   All output text is output to  stdout , if any.
//   All error text is output to  stderr , if any.

#include <stdio.h>

int main(void) {
    printf("Hello world!\n");

    return 0;
}
