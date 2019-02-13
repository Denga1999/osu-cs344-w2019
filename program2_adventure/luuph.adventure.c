/**
 * luuph.adventure.c
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
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#include <assert.h>

/**
 * Preprocessor directives to avoid long expressions and "magic" values
 */
#define ONID_USERNAME (char*)"luuph"
#define MAX_DIRNAME_LEN (int)32
#define NUM_ROOMS (int)7
#define MAX_NAME_LEN (int)9
#define MAX_OUTBOUNDS (int)6

/**
 * Define new types
 */
typedef enum RoomType {
    START_ROOM,
    MID_ROOM,
    END_ROOM
} RoomType;

typedef struct Room {
    char name[MAX_NAME_LEN];
    struct Room* outbounds[MAX_OUTBOUNDS];
    int num_outbounds;
    RoomType type;
} Room;

typedef struct DynamicIntArray {
    int* values;
    int size;
    int capacity;
} DynIntArr;

void GetNewestRoomDir(char* dirname);
bool IsNewestRoomDir(char* dirname, time_t* current_newest);
void ReadRoomsFromDir(char* dirname, Room* rooms);
void PlayGame(Room* rooms);
int GetUserInput(Room* rooms, int index);
int FindRoomByName(Room* rooms, char* name);
int FindStartRoomIndex(Room* rooms);
void InitDynIntArr(DynIntArr* arr, int capacity);
void PushBackDynIntArr(DynIntArr* arr, int value);
void DeleteDynIntArr(DynIntArr* arr);

int main(void) {
    /* find the newest room directory */
    char room_dirname[MAX_DIRNAME_LEN];
    GetNewestRoomDir(room_dirname);

    printf("%s\n", room_dirname);

    return 0;

    /* read the rooms in the directory into the  rooms  array */
    Room rooms[NUM_ROOMS];
    ReadRoomsFromDir(room_dirname, rooms);

    PlayGame(rooms);

    return 0;
}

/**
 * Finds the newest room directory created in the current directory and copies
 * its name to  dirname .
 *
 * Argument:
 *   dirname  a string to hold the name of the newest room directory
 *
 * dirname  will be modified to hold the name of the newest room directory.
 */
void GetNewestRoomDir(char* dirname) {
    assert(dirname);

    /* open the current directory and see what's in it */
    struct dirent* entry;
    DIR* root_dir = opendir(".");
    if (root_dir) {
        /* run through all entries of the directory */
        time_t newest_mod_time = 0;
        while ((entry = readdir(root_dir))) {
            /* printf("Entry name: %s\n", entry->d_name); */
            /* if there is a newer directory found, mark that directory */
            if (IsNewestRoomDir(entry->d_name, &newest_mod_time)) {
                strcpy(dirname, entry->d_name);
            }
        }

        closedir(root_dir);
    }
}

/**
 * Returns  true  if the directory named  dirname  satisfies both conditions:
 * - has a valid room directory name format, that is
 *   ONID_USERNAME.rooms.PROCESS_ID
 * - is the most recently modified directory in the scope, that is the time of
 *   last modified is greater than  current_newest
 * Returns  false  otherwise.
 *
 * Arguments:
 *   dirname         a string to hold the name of the current room directory
 *   current_newest  the current newest (largest) last-modified time
 *
 * Whenever a there is a new latest-modifed time, the value of  current_newest
 * will be updated.
 */
bool IsNewestRoomDir(char* dirname, time_t* current_newest) {
    /* printf("  Current newest = %f\n", (double)*current_newest); */

    /* get statistics about this directory */
    struct stat st;
    int stat_status = stat(dirname, &st);

    /* get a temporary string to hold dirname since it will be split later */
    char dirname_tmp[MAX_DIRNAME_LEN];
    strcpy(dirname_tmp, dirname);

    /* printf("  Original dirname = %s\n", dirname); */

    /* if  stat()  does not succeed or if not a directory, return false */
    if (stat_status != 0 || !S_ISDIR(st.st_mode)) return false;

    /* printf("  Passed stat() and is a dir\n"); */

    /* divide the name into tokens with '.' being the delimiter */
    char* name_tok = strtok(dirname_tmp, ".");  /* get 1st token */

    /* if 1st token does not exist or not match name format, return false */
    if (!name_tok || strcmp(name_tok, ONID_USERNAME) != 0) return false;

    /* printf("  Passed 1st token\n"); */
    /* printf("  dirname = %s ; name_tok = %s\n", dirname, name_tok); */

    /* continue getting the 2nd token */
    name_tok = strtok(NULL, ".");

    /* if 2nd token does not exist or not match name format, return false */
    if (!name_tok || strcmp(name_tok, "rooms") != 0) return false;

    /* printf("  Passed 2nd token\n"); */
    /* printf("  dirname = %s ; name_tok = %s\n", dirname, name_tok); */

    /* continue getting the 3rd token */
    name_tok = strtok(NULL, ".");

    /* if 3rd token does not exist, return false */
    if (!name_tok) return false;

    /* printf("  dirname = %s ; name_tok = %s\n", dirname, name_tok); */

    int i, tok_len = strlen(name_tok);

    /* if 3rd token contains a non-digit character, return false */
    for (i = 0; i < tok_len; i++)
        if (!isdigit(name_tok[i])) return false;

    /* printf("  Passed 3rd token\n"); */

    /* if there are more than 3 tokens, return false */
    if (strtok(NULL, ".")) return false;

    /* printf("  Passed name conditions\n"); */

    /* if pass all tests above,  dirname  is the name of a room directory
     * now check if this directory's time of last modified is greater than the
     * current newest time. If so, this directory is the newest.
     */
    if ((double)difftime(st.st_mtime, *current_newest) > 0) {
        *current_newest = st.st_mtime;

        /* printf("  Passed all tests. Current newest = %f\n", */
        /*        (double)*current_newest); */
        return true;
    } else {
        return false;
    }
}

/**
 * Reads the room files in the provided  dirname  directory and parses the rooms
 * into the  rooms  array.
 *
 * Arguments:
 *   dirname  the name of the newest room directory
 *   rooms    an array of Room structures to hold  NUM_ROOMS  parsed rooms
 *
 * rooms  will be modified to hold the parsed rooms from the room files.
 */
void ReadRoomsFromDir(char* dirname, Room* rooms) {
    assert(dirname && rooms);

    /* TODO */
}

/**
 * Plays the game with the provided  rooms  array consisting of rooms.
 * The game starts at a  START_ROOM  in the  rooms  array and won't stop until
 * the user reaches the  END_ROOM  in the  rooms  array.
 *
 * The  rooms  array is guaranteed to have exactly one  START_ROOM  and one
 * END_ROOM .
 */
void PlayGame(Room* rooms) {
    assert(rooms);

    /* start at  START_ROOM */
    int room_idx = FindStartRoomIndex(rooms);  /* hold current room's index */
    int next_room_idx;  /* hold user-input next room name */

    /* keep track of the room index path
     * use dynamic array as the user can go into an uncountable loop */
    DynIntArr room_idx_path;
    InitDynIntArr(&room_idx_path, NUM_ROOMS);

    while (true) {
        /* save this room's index */
        PushBackDynIntArr(&room_idx_path, room_idx);

        /* if this room is the  END_ROOM , game is over*/
        if (rooms[room_idx].type == END_ROOM) break;

        /* keep prompting the user for outbound room name until valid input */
        do {
            next_room_idx = GetUserInput(rooms, room_idx);
            if (next_room_idx == -1) {
                printf("\nHUH? I DON'T UNDERSTAND THAT ROOM. TRY AGAIN\n\n");
            }
        } while (next_room_idx == -1);

        /* being here means the user has entered valid outbound room name,
         * so move to the next room */
        room_idx = next_room_idx;
    }

    /* game is over, print path taken, number of steps, and victory message */
    printf("\nYOU HAVE FOUND THE END ROOM. CONGRATULATIONS!\n");
    printf("YOU TOOK %d STEPS. YOUR PATH TO VICTORY WAS:\n",
           room_idx_path.size - 1);
    int i;
    for (i = 0; i < room_idx_path.size; i++)
        printf("%s\n", rooms[room_idx_path.values[i]].name);

    DeleteDynIntArr(&room_idx_path);
}

/**
 * Gets the user's raw choice of outbound for the current rooms[index].
 *
 * Arguments:
 *   rooms  the array consisting of room structures
 *   index  the index of the current room the user is in
 *
 * Returns:
 *   The index of the outbound room if the user enters correctly, OR
 *   -1 if the user enters invalid room outbound name.
 */
int GetUserInput(Room* rooms, int index) {
    assert(rooms && index >= 0);

    /* print menu */
    printf("CURRENT LOCATION: %s\n", rooms[index].name);

    printf("POSSIBLE CONNECTIONS:");
    int i;
    int num_outbounds = rooms[index].num_outbounds;
    for (i = 0; i < num_outbounds; i++) {
        printf(" %s", rooms[index].outbounds[i]->name);
        /* print separator ',' if not finished, otherwise print '.' and EOL */
        if (i < num_outbounds - 1) {
            printf(",");
        } else {
            printf(".\n");
        }
    }

    printf("WHERE TO? >");

    /* get user's raw input */
    char name_input[MAX_NAME_LEN];
    fgets(name_input, MAX_NAME_LEN, stdin);

    return FindRoomByName(rooms, name_input);
}

/**
 * Returns the index of the room with matched  name  in the  rooms  array.
 * Returns -1 if not found the room whose name is  name .
 */
int FindRoomByName(Room* rooms, char* name) {
    assert(rooms && name);

    /* search through all the rooms */
    int i;
    for (i = 0; i < NUM_ROOMS; i++)
        if (strcmp(rooms[i].name, name) == 0) return i;

    /* return -1 if no name matches */
    return -1;
}

/**
 * Returns the index of the room with  START_ROOM  type in the  rooms  array.
 * Returns -1 if not found the  START_ROOM  (fail-safe).
 */
int FindStartRoomIndex(Room* rooms) {
    assert(rooms);

    /* search through all the rooms */
    int i;
    for (i = 0; i < NUM_ROOMS; i++)
        if (rooms[i].type == START_ROOM) return i;

    /* return -1 if no starting room (fail-safe -- not supposed to happen) */
    return -1;
}

/**
 * Dynamically allocates an array of integers with the given capacity.
 *
 * Arguments:
 *   arr       the dynamic-array-of-integers structure that holds the array
 *   capacity  the initial capacity of the array
 *
 * The array inside  arr  will be given  capacity  slots of heap memory.
 */
void InitDynIntArr(DynIntArr* arr, int capacity) {
    assert(arr && capacity > 0);

    /* allocate  capacity  slots of memory for the array */
    arr->values = (int*)malloc(sizeof(int) * capacity);
    assert(arr->values);  /* make sure allocation was succesful */

    /* init the array's size and capacity */
    arr->size = 0;
    arr->capacity = capacity;
}

/**
 * Pushes a value to the back of the dynamic array (doubles the capacity if
 * needed).
 *
 * Arguments:
 *   arr    the dynamic-array-of-integers structure that holds the array
 *   value  the value to be pushed to the back of the array
 *
 * The array inside  arr  will have the additional value  value  at its back.
 * The capacity of the array will be doubled if necessary.
 */
void PushBackDynIntArr(DynIntArr* arr, int value) {
    assert(arr && arr->values);

    int i;

    /* before adding, must double the capacity if array is full */
    if (arr->size == arr->capacity) {
        /* init a new array with double capacity */
        int* new_values = (int*)malloc(sizeof(int) * arr->capacity * 2);
        assert(new_values);  /* make sure allocation was successful */

        /* move all values to the new array */
        for (i = 0; i < arr->size; i++) new_values[i] = arr->values[i];

        /* destroy the old array and set pointer to the new one */
        free(arr->values);
        arr->values = new_values;

        /* set the new capacity */
        arr->capacity *= 2;
    }

    /* add the new value to the back of the array */
    arr->values[arr->size++] = value;
}

/**
 * Deallocates the dynamic array  arr->values  from the heap memory,
 * only the array but not the container (the structure  arr ).
 */
void DeleteDynIntArr(DynIntArr* arr) {
    /* do nothing if the structure  arr  or the array  arr->values  is NULL */
    if (!arr || !arr->values) return;

    /* free up memory and reset parameters */
    free(arr->values);
    arr->size = 0;
    arr->capacity = 0;
}
