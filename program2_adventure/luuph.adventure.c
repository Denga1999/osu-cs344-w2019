#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdbool.h>

int main(void) {
    bool correct = true;
    bool incorrect = false;

    if (correct) {
        printf("true\n");
    }

    int i = 123;
    printf("i = %d\n", i);

    char str_no_init[20];
    printf("str_no_init[20] = \"");
    for (i = 0; i < 20; i++) printf("%c", str_no_init[i]);
    printf("\"\n");
    printf("str_no_init = %s\n", str_no_init);

    char str_init_first_idx[20];
    str_init_first_idx[0] = '\0';
    printf("str_init_first_idx[20] = \"");
    for (i = 0; i < 20; i++) printf("%c", str_init_first_idx[i]);
    printf("\"\n");
    printf("str_init_first_idx = %s\n", str_init_first_idx);

    char str_init_with_strcpy[20];
    strcpy(str_init_with_strcpy, "");
    printf("str_init_with_strcpy[20] = \"");
    for (i = 0; i < 20; i++) printf("%c", str_init_with_strcpy[i]);
    printf("\"\n");
    printf("str_init_with_strcpy = %s\n", str_init_with_strcpy);

    if (incorrect) {
        printf("false\n");
    }

    return 0;
}
