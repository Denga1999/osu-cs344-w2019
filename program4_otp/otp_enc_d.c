// otp_enc_d.c
//
// Phi Luu
//
// Oregon State University
// CS_344_001_W2019 Operating Systems 1
// Program 4: OTP
//
// This program runs in the background as a daemon. Upon execution,  otp_enc_d
// outputs an error if it cannot be run due to a network error, such as the
// ports being unavailable. Its function is to perform the actual encoding, as
// described in  https://en.wikipedia.org/wiki/One-time_pad . This program
// listens on a particular port/socket, assigned when it is first run.
//
// When a connection is made,  otp_enc_d  calls  accept()  to generate the
// socket used for actual communication, and then uses a separate process to
// handle the rest of the transaction, which will occur on the newly accepted
// socket.
//
// Syntax:
//
//   ./otp_enc_d listening_port
//   ./otp_enc_d listening_port &
//
// where
//   listening_port  port program should listen on
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
