#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
// gcc -g -Wall -Wextra archivo.c -o programa
int main()
{
    int status;
    printf("Hola soy Abraham!\n");

    pid_t homero = fork();
    if (homero == 0)
    {
        printf("Hola soy home...AY AY AY AY AY\n");

        pid_t lisa = fork();
        if (lisa == 0)
        {
            printf("Hola soy Lisa la frigida\n");
            exit(EXIT_SUCCESS);
        }
        else if(lisa > 0) {waitpid(lisa,&status,0);}

        pid_t bart = fork();
        if (bart == 0)
        {
            printf("Hola soy el Barto\n");
            exit(EXIT_SUCCESS);
        }
        else if(bart > 0){waitpid(bart,&status,0);}

        pid_t maggie = fork();
        if (maggie == 0)
        {
            printf("... agu ... a Nisman lo mataron\n");
            exit(EXIT_SUCCESS);
        }
        else if(maggie > 0){ waitpid(maggie,&status,0);}

        exit(EXIT_SUCCESS);
    }
    waitpid(homero,&status,0);
    exit(EXIT_SUCCESS);
}