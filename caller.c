#include <stdio.h>
#include <errno.h>
#include <unistd.h>
int main(void) {
    extern char **environ;
    char *arg0 = NULL;
    const int result = execve("./callee", &arg0, environ);
    printf("caller: execve returned %d, errno = %d\n", result, errno);
}
