#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

enum {READ, WRITE};
enum {pipePadreHijo, pipePadreHije, pipeHijo_e_Hije};

int pipes[3][2];

pid_t dad;
pid_t hijo;
pid_t hije;
int cur;

// Padre -> hijo -> hije--|
//    ^                   |  
//    |                   |  
//    |-------------------|

// void bsend(pid dst, int msg) Envía el valor msg al proceso dst.
// int breceive(pid src) Recibe un mensaje del proceso src.


void padre_soi_io()
{
    cur = 0;

    close(pipes[pipePadreHijo][READ]);
    close(pipes[pipePadreHije][WRITE]);
    close(pipes[pipeHijo_e_Hije][READ]);
    close(pipes[pipeHijo_e_Hije][WRITE]);

    while (cur<50)
    {
        printf("Current valor is: %d\n", cur);
        write(pipes[pipePadreHijo][WRITE], &cur, sizeof(int));  
        int valorDeHije;
        read(pipes[pipePadreHije][READ], &valorDeHije, sizeof(int));
        cur = valorDeHije + 1;
    }

    kill(hijo, SIGKILL);
    kill(hije, SIGKILL);
    exit(EXIT_SUCCESS);
}

void hijoProcess()
{
    cur = 0;

    close(pipes[pipePadreHijo][WRITE]);
    
    close(pipes[pipePadreHije][WRITE]);
    close(pipes[pipePadreHije][READ]);

    close(pipes[pipeHijo_e_Hije][READ]);

    while(cur < 50)
    {
        read(pipes[pipePadreHijo][READ], &cur, sizeof(int));
        cur ++;
        
        write(pipes[pipeHijo_e_Hije][WRITE], &cur, sizeof(int));  
    }

}

void hije2Process()
{
    cur = 0;

    close(pipes[pipePadreHijo][WRITE]);
    close(pipes[pipePadreHijo][READ]);
    
    close(pipes[pipePadreHije][READ]);
    close(pipes[pipeHijo_e_Hije][WRITE]);

    while(cur<50)
    {
        read(pipes[pipeHijo_e_Hije][READ], &cur, sizeof(int));
        cur ++;
        write(pipes[pipePadreHije][WRITE], &cur, sizeof(int));  
    }
}

int main()
{
    dad = getpid();

    for (int i = 0; i < 3; i++) pipe(pipes[i]);

    hijo = fork();
    if (hijo==0)
    {
        hijoProcess();
    }
    else if (hijo > 0)
    {
        hije = fork();

        if(hije == 0)
        {
            hije2Process();
        }
        else if (hije > 0)
        {
            padre_soi_io();
        }
        
    }
    return 0;
}