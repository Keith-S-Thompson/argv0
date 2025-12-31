#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
int main(void) {
    extern char **environ;
    char *arg0 = NULL;
    const int result = execve("./callee", &arg0, environ);
    printf("caller: execve returned %d, errno = %d, %s\n",
           result, errno, strerror(errno));
}
