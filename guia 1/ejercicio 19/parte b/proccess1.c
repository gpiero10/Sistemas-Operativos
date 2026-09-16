#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define uint u_int32_t

int main()
{
    // SOY PROCESO 1
    int n = 0;

    int servidorP1;
    int servidorP3;
    int clienteP2;
    
    struct sockaddr_un p1ServerAddr;
    struct sockaddr_un p3ServerAddr;

    // Seteo socket server p1
    strcpy(p1ServerAddr.sun_path, "p1 Socket");
    unlink(p1ServerAddr.sun_path);
    p1ServerAddr.sun_family = AF_UNIX;

    servidorP1 = socket(AF_UNIX, SOCK_STREAM, 0);
    if (bind(servidorP1, (struct sockaddr *) &p1ServerAddr, sizeof(p1ServerAddr)) == -1) {
        perror("Error");
        exit(EXIT_FAILURE);
    }

    if (listen(servidorP1, 1) == -1) {
        perror("Error");
        exit(EXIT_FAILURE);
    }

    // Seteo socket server P3
    strcpy(p3ServerAddr.sun_path, "p3 Socket");
    p3ServerAddr.sun_family = AF_UNIX;
    
    // aceptar conexion de P2
    clienteP2 = accept(servidorP1, NULL, NULL); // no me interesa la info del cliente

    if (clienteP2 == -1) {
        perror("Error");
        exit(EXIT_FAILURE);
    }
    
    // conectarse a servidor P3
    servidorP3 = socket(AF_UNIX, SOCK_STREAM, 0);
    while (connect(servidorP3, (struct sockaddr *) &p3ServerAddr, sizeof(p3ServerAddr)) == -1) {usleep(50000);}

    while(n < 50)
    {
        write(clienteP2, &n, sizeof(int));
        read(servidorP3, &n, sizeof(int));
        printf("[Proceso 1] Recibi %d\n", n);
        n ++;
    }

    close(servidorP1);
    close(clienteP2);
    close(servidorP3);
    
    exit(EXIT_SUCCESS);
}