#include <stdio.h>
int main(int argc, char **argv) {
    printf("argc = %d\n", argc);
    for (int i = 0; i <= argc; i ++) {
        printf("argv[%d] = ", i);
        if (argv[i] == NULL) {
            puts("NULL");
        }
        else {
            printf("\"%s\"\n", argv[i]);
        }
    }
}
