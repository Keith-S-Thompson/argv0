#include <unistd.h>
int main(void) {
    extern char **environ;
    char *arg0 = NULL;
    execve("./callee", &arg0, environ);
}
