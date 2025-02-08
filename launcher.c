
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

void fork_and_exit_parent()
{
    pid_t pid;

    pid = fork();
    switch (pid)
    {
    case -1:
        perror("fork");
        exit(EXIT_FAILURE);

    case 0:
        break;

    default:
        exit(EXIT_SUCCESS);
    }
}

void redirect_standard_io_to_null()
{
    int fd = open("/dev/null", O_RDWR);
    if (fd < 0)
    {
        perror("open /dev/null");
        exit(EXIT_FAILURE);
    }
    dup2(fd, STDIN_FILENO);
    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDERR_FILENO);
    if (fd > 2)
    {
        close(fd);
    }
}

void daemonize(const char *cmd)
{
    fork_and_exit_parent();
    if (setsid() < 0)
    {
        perror("setsid");
        exit(EXIT_FAILURE);
    }
    fork_and_exit_parent();

    umask(0);

    if (chdir("/") < 0)
    {
        perror("chdir");
        exit(EXIT_FAILURE);
    }

    redirect_standard_io_to_null();

    printf("Daemonization complete for %s\n", cmd);
}

int launch(char exec[])
{
    daemonize(exec);

    char *args[] = {exec, NULL};
    if (execvp(exec, args) < 0)
    {
        perror("execvp");
        return 1;
    }

    printf("Launched %s\n", exec);
    return 0;
}