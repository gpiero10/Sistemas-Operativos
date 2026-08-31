#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

/*
Funciones para Modificar el Tipo sigset t
Antes de cambiar la máscara del proceso, debemos construir el conjunto
de señales en memoria:
sigemptyset(&set): Vacı́a el conjunto (pone todos los bits en 0).
¡Siempre inicializar con esto!
sigfillset(&set): Incluye todas las señales en el conjunto (bits en
1).
sigaddset(&set, SIGINT): Añade una señal especı́fica al conjunto.
sigdelset(&set, SIGINT): Remueve una señal especı́fica del
conjunto.
sigismember(&set, SIGINT): Verifica si la señal está en el
conjunto (retorna 1 o 0).
*/

int sets;
int ejecutar;
pid_t child;
pid_t daddyPid;
char peticion;

void pong(int sig)
{
    sets ++;

    if (sets < 3)
    {
    dprintf(1, "Pong ... y mi PID es: %d\n", daddyPid);
    
    kill(child, SIGBUS);
    }

}

void ping(int sig)
{
    dprintf(1, "Ping ... y mi PID es: %d\n", getpid());
    kill(daddyPid, SIGBUS);
}

int main()
{
    
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGBUS);
    sigprocmask(SIG_BLOCK, &mask, NULL);
    daddyPid = getpid();

    child = fork();

    if (child == 0)
    {
        struct sigaction sa;
        sa.sa_handler = ping;
        sigaction(SIGBUS, &sa, NULL);

        // signal(SIGBUS, ping);
        sigdelset(&mask, SIGBUS);

        while (1)
        {
            sigsuspend(&mask);   
        }
    }
    else if (child > 0)
    {
        struct sigaction sa;
        sa.sa_handler = pong;
        sigaction(SIGBUS, &sa, NULL);
        //signal(SIGBUS, pong);

        sigdelset(&mask, SIGBUS);

        sigprocmask(SIG_SETMASK, &mask,NULL);
        
        ejecutar = 1;
        sets = 0;

        kill(child, SIGBUS);

        while (ejecutar) 
        {
            sigsuspend(&mask);
            if (sets >= 3) 
            {
                printf("Continuar con el juego? s/n: \n");
                scanf(" %c", &peticion);
                
                ejecutar = (peticion == 's') ? 1:0; 
                sets = (ejecutar == 1) ? 0:-1;
                
                if (sets == 0) kill(child, SIGBUS);
            }
        }
        kill(child, 9); // matar hijo
        exit(EXIT_SUCCESS);
    }

    return 0;
}