#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <application-name>\n", argv[0]);
        return 1;
    }

    pid_t pid = fork();
    
    switch (pid)
    {
    case -1:
        perror("fork");
        return 1;
    
    case 0:
        char *args[] = {argv[1], NULL};
        execvp(argv[1], args);
        printf("excvp returned\n");
        perror("execvp");
        exit(1);
    default:
        int status;
        printf("Child process started with PID: %d\n", pid);
        waitpid(pid, &status, 0);
        printf("Child process terminated\n");
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
            printf("Child process terminated with an error\n");
            return 1;
        }
        break;
    }

    return 0;
}