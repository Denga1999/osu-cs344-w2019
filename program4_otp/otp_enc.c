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

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdbool.h>
#include <assert.h>

#define HOSTNAME        (char*)"localhost"
#define MAX_BUFFER_SIZE (int)70000

int ValidateAndReadFile(const char* fname, char* buffer, const char* pname);
int ToPositiveInt(const char* str);

int main(int argc, char** argv) {
    // ensure correct usage of command line arguments
    if (argc != 4) {
        fprintf(stderr, "Usage: %s plaintext key port\n", argv[0]);
        return 1;
    }

    // ensure the  port  argument is a positive integer
    int port = ToPositiveInt(argv[3]);
    if (!port) {
        fprintf(stderr, "%s:  port  must be a positive integer\n", argv[0]);
        return 1;
    }

    // open, validate, and read from plaintext file
    char plaintext_buffer[MAX_BUFFER_SIZE];
    int plaintext_size = ValidateAndReadFile(argv[1], plaintext_buffer, argv[0]);

    // open, validate, and read from key file
    char key_buffer[MAX_BUFFER_SIZE];
    int key_size = ValidateAndReadFile(argv[2], key_buffer, argv[0]);

    // if key is shorter than plaintext, terminate with code 1
    if (key_size < plaintext_size) {
        fprintf(stderr, "%s: The key is shorter than the plaintext\n", argv[0]);
        return 1;
    }

    // set up the server address struct
    // convert machine name into a special form of address and ensure success
    struct hostent* server_host_info = gethostbyname(HOSTNAME);
    if (!server_host_info) {
        fprintf(stderr, "%s: gethostbyname() error: Could not resolve host %s\n",
                argv[0], HOSTNAME);
        return 1;
    }
    // initialize and clear out the address struct
    struct sockaddr_in server_address;
    memset((char*)&server_address, '\0', sizeof(server_address));
    server_address.sin_family = AF_INET;    // network-capable socket
    server_address.sin_port = htons(port);  // store port number
    // copy the special address to this address struct
    memcpy((char*)&server_address.sin_addr.s_addr,   // destination
           (char*)server_host_info->h_addr_list[0],  // source
           server_host_info->h_length);              // how much to copy

    // set up the socket and ensure successful setup
    // general-purpose socket, use TCP, normal behavior
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1) {
        fprintf(stderr, "%s: socket() error: Could not open socket\n", argv[0]);
        return 1;
    }

    // connect to server and ensure successful connection
    if (connect(socket_fd, (struct sockaddr*)&server_address,
                sizeof(server_address)) != 0) {
        fprintf(stderr, "%s: connect() error: Could not connect to %s:%d\n",
                argv[0], HOSTNAME, port);
        close(socket_fd);
        return 1;
    }

    // send the plaintext to server (i.e. otp_enc_d daemon)
    

    // close socket
    close(socket_fd);

    return 0;
}

// Validates and reads the file with the provided file name.
//
// Argument:
//   fname   name of the file to be opened and read from
//   buffer  buffer containing text read from  file
//   pname   name of the program
//
// On success, this function returns size of  buffer  back to  main()  and lets
// main()  continue the program.
//
// On either of these error cases:
// - If the file named  fname  could not be opened
// - If  buffer  read from file is empty
// - If  buffer  read from file contains a bad character (neither an alphabet
//       nor a space)
// The program will print according error messages and exit with code 1.
//
// Notes: On all cases, file will be closed after it has been opened and read.
int ValidateAndReadFile(const char* fname, char* buffer, const char* pname) {
    assert(fname && buffer && pname);

    FILE* file = fopen(fname, "r");

    // check non-opened files
    if (!file) {
        fprintf(stderr, "%s: Could not open file \"%s\"\n", pname, fname);
        exit(1);
    }

    // read until the end-of-line character or until running out of buffer
    memset(buffer, '\0', MAX_BUFFER_SIZE);     // clear out buffer
    fgets(buffer, MAX_BUFFER_SIZE - 1, file);  // size - 1 due to \0 termination
    buffer[strcspn(buffer, "\n")] = '\0';      // remove trailing newlines

    // check for empty buffer
    int size = strlen(buffer);
    if (size == 0) {
        fprintf(stderr, "%s: Empty buffer was read from \"%s\"\n", pname, fname);
        fclose(file);
        exit(1);
    }

    // check for bad characters
    for (size_t i = 0; i < size; i++) {
        if (!isupper(buffer[i]) && buffer[i] != ' ') {
            fprintf(stderr, "%s: File \"%s\" contains bad characters\n",
                    pname, fname);
            fclose(file);
            exit(1);
        }
    }

    // if passed all checks, reading from  file  was successful
    fclose(file);
    return size;
}

// Converts a string to a positive integer. If any character of the string is
// not a digit, return -1 as an error.
//
// Argument:
//   str  the string to be converted into a positive integer
//
// Returns:
//   0  if there exists a non-digit (not in 0-9) character in  str  string
//   The correspond positive integer on success
int ToPositiveInt(const char* str) {
    assert(str);

    for (size_t i = 0, size = strlen(str); i < size; i++)
        if (!isdigit(str[i])) return 0;

    return atoi(str);
}
