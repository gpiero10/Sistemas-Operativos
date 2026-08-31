#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

enum {READ, WRITE};

int main()
{
    // Escribir el código de un programa que se comporte de la misma manera que la ejecución del comando “ls -al | wc -l” en una shell. No está permitido utilizar la función system, y cada uno de los programas involucrados en la ejecución del comando deberá ejecutarse como un subproceso.
    int pipefds[2];
    pipe(pipefds);
    pid_t ls = fork();
    if (ls == 0)
    {
        close(pipefds[READ]);
        dup2(pipefds[WRITE], STDOUT_FILENO); // aca recableo mi output al puerto de escritura del pipe
        execl("/bin/sh", "sh", "-c", "ls -al", (char *) NULL);
        exit(EXIT_SUCCESS);
    }
    else if (ls > 0)
    {
        pid_t wc = fork();
        if (wc == 0)
        {   
            close(pipefds[WRITE]);
            dup2(pipefds[READ], STDIN_FILENO); //recableo mi input al buffer/pipe que contendra el valor de ls -l
            execl("/bin/sh", "sh", "-c", "wc -l", (char *) NULL);
            exit(EXIT_SUCCESS);
        }
        else if (wc > 0)
        {
            close(pipefds[READ]);
            close(pipefds[WRITE]);  
        }
    }
    // Clave esperar a que los hijos se autoliquiden talque mi terminal no se llena de cosas raras...
    wait(NULL);
    wait(NULL);

    exit(EXIT_SUCCESS);

    return 0;
}