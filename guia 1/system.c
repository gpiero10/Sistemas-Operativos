#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


void system(const char *arg)
{
    int status;
    pid_t shellProgram = fork();

    if (shellProgram == 0)
    {
        execl("/bin/sh", "sh", "-c", arg, (char *) NULL);
        exit(EXIT_SUCCESS);
    }
    else if (shellProgram > 0)
    {
        waitpid(shellProgram, &status, 0);
    }
    
    return;
}

int main()
{
    system("ls -l");
    
    return 0;
}