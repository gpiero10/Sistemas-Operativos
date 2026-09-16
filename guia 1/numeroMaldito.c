#define _POSIX_C_SOURCE 200809L
#define N 3

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/types.h>


enum {READ, WRITE};
int flagNietoJobDone;

/*funciones implementadas por mi para darle un sentido a este codigo y por que no a la existencia...*/
void handlerHijo() { flagNietoJobDone = 1;}
int dameNumero(pid_t p) { return (p % 7);}
void informarResultado(int numero, int resultado)
{
    printf("El numero pedido %d dio este resultado: %d \n", numero, resultado);
    return;
}
int calcular(int n) 
{ 
    int res = n;
    for (int j = 0; j < n ; j++) { res += n*2;} 
    sleep(30); // en algo hay que demorar no?
    return res;
}
// fin seccion creativa

void nietoProcess(int pipeHijoNieto[2][2])
{
    int numero;
    int r;
    read(pipeHijoNieto[0][READ], &numero, sizeof(int));

    r = calcular(numero);
    kill(getppid(), SIGBUS);
    write(pipeHijoNieto[1][WRITE], &r, sizeof(int));
    exit(EXIT_SUCCESS);
}

void ejecutarHijo(int i, int pipes[6][2]) 
{
    int pipeHijoNieto[2][2];
    pipe(pipeHijoNieto[0]);
    pipe(pipeHijoNieto[1]);
    flagNietoJobDone = 0;

    pid_t nieto = fork();
    
    if (nieto == 0)
    {
        nietoProcess(pipeHijoNieto);
    }
    else if (nieto > 0)
    {
        int numero;
        int r;
        read(pipes[i][READ], &numero, sizeof(int));
        write(pipeHijoNieto[0][WRITE], &numero, sizeof(int));

        sigset_t mask, original;
        sigemptyset(&mask);
        sigaddset(&mask, SIGBUS);
        sigprocmask(SIG_BLOCK, &mask, &original);

        struct sigaction sa;
        sa.sa_handler = handlerHijo;
        sigaction(SIGBUS, &sa, NULL);

        sigprocmask(SIG_SETMASK, &original, NULL);

        while(!flagNietoJobDone)
        {
            char a;
            read(pipes[i][READ], &a, sizeof(char));
            char b = '\0';
            write(pipes[N+i][WRITE], &b, sizeof(char));    
        }

        read(pipeHijoNieto[1][READ], &r, sizeof(int));
        write(pipes[N + i][WRITE], &numero, sizeof(int));
        write(pipes[N + i][WRITE], &r, sizeof(int));
    }
    exit(EXIT_SUCCESS);

    return;
}

int main(int argc, char *argv[])
{
    if (argc< 2) 
    {
        printf ("Debe ejecutar con la cantidad de hijos como parametro\n");
        return 0; 
    }
    
    // int N = atoi(argv[1]); 
    //int N = 3;  //voy a probar con 3 -> ahora es un define
    int pipes[N*2][2];
    
    for ( int i=0; i< N*2; i++) { pipe(pipes[i]); }
    
    for (int i=0; i< N; i++) 
    {
        int pid = fork ();
        if (pid==0) 
        {
            ejecutarHijo(i,pipes);
            return 0;
        } else {
            int numero = dameNumero(pid) ;
            write(pipes[i][1], &numero, sizeof(numero)); 
        } 
    }
    int cantidadTerminados = 0;
    // char hijoTermino[N] = {0};
    char hijoTermino[3] = {0};
    
    while (cantidadTerminados < N) 
    {
        for ( int i=0; i< N; i++) 
        {
            if (hijoTermino[i]) { continue; }
            char termino = 0;
            write(pipes[i][1], &termino, sizeof(termino));
            read(pipes[N+i][0], &termino, sizeof(termino));
            if (termino) 
            {
                int numero;
                int resultado ;
                read(pipes[N+i][0], &numero, sizeof(numero));
                read(pipes[N+i][0], &resultado, sizeof(resultado));
                informarResultado(numero, resultado);
                hijoTermino[i] = 1;
                cantidadTerminados++; 
            } 
        } 
    }
    wait(NULL) ;
    return 0; 
}