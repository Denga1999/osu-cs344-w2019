/**
 * luuph.buildrooms.c
 *
 * Phi Luu
 *
 * Oregon State University
 * CS_344_001_W2019 Operating Systems 1
 * Program 2: Adventure
 */
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define PERM_755       (mode_t)(S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)
#define PERM_644       (mode_t)(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
#define ONID_USERNAME  (char*)"luuph"
#define PID            (int)getpid()
#define MAX_ROOM_FILE_NAME_LENGTH  (int)32
#define NUM_ROOM_FILES (int)7
#define NUM_ROOMS      (int)10

/* hard-coded names for room files */
const char* ROOM_FILES[NUM_ROOM_FILES] = {
    "Garage_Room",
    "Archive_Room",
    "Lobby_Room",
    "Press_Room",
    "Visa_Office",
    "Meeting_Room",
    "Administration_Office"
};

/* hard-coded names for rooms */
const char* ROOM_NAMES[NUM_ROOMS] = {
    /* basement */
    "Garage",
    "Archives",
    "CCTV",
    "Cafe",
    /* 1st floor */
    "Lobby",
    "Press",
    "Visa",
    "Tellers",
    /* 2nd floor */
    "Meeting",
    "Admin"
};

char* GenerateRoomDir(void);
void GenerateRoomFiles(char* dirname, int num_files);
void GenerateRoom(FILE* roomfile, int room_rand_begin);

int main(void) {
    char* room_path = GenerateRoomDir();

    GenerateRoomFiles(room_path, NUM_ROOM_FILES);

    /* clean up */
    free(room_path);

    return 0;
}

/**
 * Dynamically allocates a name and generates a directory using that name to
 * contain room files.
 * Directory name = ONID_USERNAME.rooms.PROCESS_ID
 *
 * Returns:
 *   The address of the dynamically allocated directory name.
 */
char* GenerateRoomDir(void) {
    /* allocate and init a name for the room directory */
    char* room_path = malloc(MAX_ROOM_FILE_NAME_LENGTH * sizeof(char));
    room_path[0] = '\0';

    /* ensure directory does not exist */
    struct stat st = {0};
    if (stat(room_path, &st) == -1) {
        /* room directory name = ONID.rooms.PID */
        sprintf(room_path, "%s.%s.%d", ONID_USERNAME, "rooms", PID);
        /* create a directory with rwxr-xr-x permission to contain room files */
        mkdir(room_path, PERM_755);
    }

    return room_path;
}

/**
 *
 */
void GenerateRoomFiles(char* dirname, int num_files) {
    int i;
    bool room_chosen[NUM_ROOMS] = {false};  /* whether the room has been chosen */
    int room_rand_begin = 0;  /* random start with first index */

    for (i = 0; i < num_files; i++) {
        /* construct a file name for the room file */
        char filename[MAX_ROOM_FILE_NAME_LENGTH * 2];
        filename[0] = '\0';
        strcat(filename, dirname);
        strcat(filename, "/");
        strcat(filename, ROOM_FILES[i]);

        /* try opening the file */
        FILE* roomfile = fopen(filename, "w");

        /* only proceed if file is opened successfully */
        if (roomfile) {
            GenerateRoom(roomfile, room_rand_begin);
            fclose(roomfile);
        }
    }
}

/**
 *
 */
void GenerateRoom(FILE* roomfile, int room_rand_begin) {
    /* choose a random room name from the remaining list of room name */
    int room_idx = room_rand_begin + rand() % (NUM_ROOMS - room_rand_begin + 1);
    /* swap this room name with the first room name */


    fprintf(roomfile, "ROOM NAME: %s\n", );
}
