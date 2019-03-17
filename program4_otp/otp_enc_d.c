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

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <assert.h>
#include <pthread.h>

#define MAX_BUFFER_SIZE (size_t)100000
#define MAX_LISTEN_CONNECTIONS (int)5
#define MAX_CONCURRENCY (int)5
#define CHAR_RANGE (int)27  // 26 capital alphabet letters + 1 space

static const char* CHAR_POOL = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";

static char* prog;  // executable's name, for convenience

void EncryptOtp(char* ciphertext, const char* plaintext, const char* key);
void ReadFromClient(int socket_fd, void* buffer, size_t len, int flags);
void WriteToClient(int socket_fd, void* buffer, size_t len, int flags);
int ToPositiveInt(const char* str);

int main(int argc, char** argv) {
    prog = argv[0];

    // ensure correct usage of command line arguments
    if (argc != 2) {
        fprintf(stderr, "Usage: %s listening_port\n", argv[0]);
        return 1;
    }

    // ensure  listening_port  argument is a positive integer
    int port = ToPositiveInt(argv[1]);
    if (!port) {
        fprintf(stderr, "%s:  listening_port  must be a positive integer\n", prog);
        return 1;
    }

    // set up the address struct for this process (i.e. the server)
    struct sockaddr_in server_address;
    memset((char*)&server_address, '\0', sizeof(server_address));
    server_address.sin_family = AF_INET;          // network-capable socket
    server_address.sin_port = htons(port);        // store port number
    server_address.sin_addr.s_addr = INADDR_ANY;  // accept any address

    // set up the socket and ensure successful setup
    // general-purpose socket, use TCP, normal behavior
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        fprintf(stderr, "%s: socket() error: Could not open socket\n", prog);
        return 1;
    }

    // enable socket to begin listening
    if (bind(socket_fd,
             (struct sockaddr*)&server_address,
             sizeof(server_address)) < 0) {
        fprintf(stderr, "%s: bind() error: Could not bind to port %d\n",
                prog, port);
        close(socket_fd);
        return 1;
    }

    // flip the socket on and start listening
    listen(socket_fd, MAX_LISTEN_CONNECTIONS);

    // listen for client's connection forever
    while (1) {
        // accept a connection, blocking if one is not available until one connects
        struct sockaddr_in client_address;
        socklen_t client_info_size = sizeof(client_address);
        int connection_fd = accept(socket_fd, (struct sockaddr*)&client_address,
                                   &client_info_size);
        if (connection_fd < 0)
            fprintf(stderr, "%s: accept() error\n", prog);

        /* printf("%s: Server connected to client at port %d\n", prog, */
        /*        ntohs(client_address.sin_port)); */

        // first read from client (i.e. otp_enc) how much data is going to be sent
        size_t client_data_len = 0;
        ReadFromClient(connection_fd, &client_data_len, sizeof(client_data_len), 0);
        // then read "KEY\nPLAINTEXT" combination from client
        char client_data[client_data_len + 1];  // +1 for \0
        memset(client_data, '\0', sizeof(client_data));
        ReadFromClient(connection_fd, client_data, client_data_len, 0);

        /* printf("%s: client_data = \"%s\", size = %zu\n", */
        /*        prog, client_data, strlen(client_data)); */

        // get plaintext
        char plaintext[MAX_BUFFER_SIZE];
        memset(plaintext, '\0', sizeof(plaintext));
        char* tmp = strtok(client_data, "\n");
        if (tmp) strcpy(plaintext, tmp);

        // get key
        char key[MAX_BUFFER_SIZE];
        memset(key, '\0', sizeof(key));
        tmp = strtok(NULL, "\n");
        if (tmp) strcpy(key, tmp);

        /* FILE* tmpf = fopen("test_plaintext_otp_enc_d", "w"); */
        /* fprintf(tmpf, "%s\n", plaintext); */
        /* fclose(tmpf); */
        /* tmpf = fopen("test_key_otp_enc_d", "w"); */
        /* fprintf(tmpf, "%s\n", key); */
        /* fclose(tmpf); */

        // TODO: Multithread the encryption process
        char ciphertext[MAX_BUFFER_SIZE];
        memset(ciphertext, '\0', sizeof(ciphertext));
        EncryptOtp(ciphertext, plaintext, key);

        // first tell client how much data is going to be sent
        size_t ciphertext_len = strlen(ciphertext);
        WriteToClient(connection_fd, &ciphertext_len, sizeof(ciphertext_len), 0);
        // then send the ciphertext to the client
        WriteToClient(connection_fd, ciphertext, strlen(ciphertext), 0);

        // close the existing socket which is connected to client
        close(connection_fd);
    }

    // close the listening socket
    close(socket_fd);

    return 0;
}

// Creates a one-time pad by encrypting a given plaintext using a provided key
// using modular addition.
//
// Arguments:
//   ciphertext  output of this function, i.e. the encrypted text
//   plaintext   plaintext to be encrypted
//   key         key used in the encryption process
//
// ciphertext  will be modified and will contain the encrypted text.
void EncryptOtp(char* ciphertext, const char* plaintext, const char* key) {
    assert(ciphertext && plaintext && key);

    // get lengths of plaintext and key and ensure
    // key is at least as long as plaintext
    size_t plaintext_len = strlen(plaintext);
    size_t key_len = strlen(key);
    assert(key_len >= plaintext_len);

    // for each character in plaintext, convert it to ciphertext
    for (size_t i = 0; i < plaintext_len; i++) {
        // use modular addition to get the index of ciphertext
        // strchr() returns a  char*  to the first occurrence of the character,
        // subtract  CHAR_POOL  from that will give the index
        // since  CHAR_POOL  is an array of character
        int cipher_idx = strchr(CHAR_POOL, plaintext[i]) - CHAR_POOL;
        cipher_idx += strchr(CHAR_POOL, key[i]) - CHAR_POOL;
        cipher_idx %= CHAR_RANGE;

        ciphertext[i] = CHAR_POOL[cipher_idx];
    }
}

// Reads data from client to a buffer with the provided socket and flags.
//
// Arguments:
//   socket_fd  file descriptor of the socket
//   buffer     buffer to write the read data from the client into
//   len        length of the buffer
//   flags      flags used in the  recv()  function, use 0 for normal behavior
//
// If the data cannot be read from the client, the function will print error
// messages accordingly and then terminate the program with code 1.
void ReadFromClient(int socket_fd, void* buffer, size_t len, int flags) {
    assert(socket_fd >= 0 && buffer && len > 0 && flags >= 0);

    void* tmp_buffer = buffer;
    ssize_t total_chars_read = 0;

    // keep receiving until all of the data has been read
    while (total_chars_read < len) {
        ssize_t chars_read = recv(socket_fd, tmp_buffer,
                                  len - total_chars_read, flags);
        if (chars_read < 0) {
            fprintf(stderr, "%s: recv() error: Could not read from socket\n", prog);
            break;
        }

        total_chars_read += chars_read;

        /* printf("%s: recv(): Read %zu. Total read %zu. Remaining %zu\n", prog, */
        /*        chars_read, total_chars_read, len - total_chars_read); */

        if (total_chars_read < len) {
            // move pointer to after the last read character
            tmp_buffer += chars_read;
            /* fprintf(stderr, "%s: recv() warning: Not all data was read from socket\n", prog); */
        }
    }
}

// Writes data from a buffer to client with the provided socket and flags.
//
// Arguments:
//   socket_fd  file descriptor of the socket
//   buffer     buffer containing the data to be written to client
//   len        length of the buffer
//   flags      flags used in the  send()  function, use 0 for normal behavior
//
// If the data cannot be written to the client, the function will print error
// messages accordingly and then terminate the program with code 1.
void WriteToClient(int socket_fd, void* buffer, size_t len, int flags) {
    assert(socket_fd >= 0 && buffer && len > 0 && flags >= 0);

    void* tmp_buffer = buffer;
    ssize_t total_chars_written = 0;

    // keep sending until all of the data has been sent
    while (total_chars_written < len) {
        ssize_t chars_written = send(socket_fd, tmp_buffer,
                                     len - total_chars_written, flags);
        if (chars_written < 0) {
            fprintf(stderr, "%s: send() error: Could not write to socket\n", prog);
            break;
        }

        total_chars_written += chars_written;

        /* printf("%s: recv(): Written %zu. Total written %zu. Remaining %zu\n", prog, */
        /*        chars_written, total_chars_written, len - total_chars_written); */

        if (total_chars_written < len) {
            // move pointer to after the last written character
            tmp_buffer += chars_written;
            /* fprintf(stderr, "%s: send() warning: Not all data was written to socket\n", prog); */
        }
    }
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

    for (size_t i = 0, len = strlen(str); i < len; i++)
        if (!isdigit(str[i])) return 0;

    return atoi(str);
}
