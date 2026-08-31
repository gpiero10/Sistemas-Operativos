#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
// gcc -g -Wall -Wextra olaya_sound_system.c -o programa

void systemOlaya(const char *arg)
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
    systemOlaya("ls -l");
    
    return 0;
}