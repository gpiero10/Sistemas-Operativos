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
    // SOY PROCESO 3
    int n = 0;

    int servidorP3;
    int servidorP2;
    int clienteP1;
    
    struct sockaddr_un p3ServerAddr;
    struct sockaddr_un p2ServerAddr;

    // Seteo server P3
    strcpy(p3ServerAddr.sun_path, "p3 Socket");
    unlink(p3ServerAddr.sun_path);
    p3ServerAddr.sun_family = AF_UNIX;

    servidorP3 = socket(AF_UNIX, SOCK_STREAM, 0);
    if (bind(servidorP3, (struct sockaddr *) &p3ServerAddr, sizeof(p3ServerAddr)) == -1) {
        perror("Error");
        exit(EXIT_FAILURE);
    }
    if (listen(servidorP3, 1) == -1) {
            perror("Error");
            exit(EXIT_FAILURE);
    }

    // Seteo para conectarse a P2 socket
    strcpy(p2ServerAddr.sun_path, "p2 Socket");
    p2ServerAddr.sun_family = AF_UNIX;
    
    // Conectarse a Servidor proccess 2
    servidorP2 = socket(AF_UNIX, SOCK_STREAM, 0);
    while (connect(servidorP2, (struct sockaddr *) &p2ServerAddr, sizeof(p2ServerAddr)) == -1) {usleep(50000);}

    // aceptar conexion de cliente P1
    clienteP1 = accept(servidorP3, NULL, NULL); // no me interesa la info del cliente
    if (clienteP1 == -1) {
        perror("Error");
        exit(EXIT_FAILURE);
    }

    while(read(servidorP2, &n, sizeof(int)) > 0)
    {
        printf("[Proceso 3] Recibi %d\n", n);
        n ++;
        write(clienteP1, &n, sizeof(int));
    }

    close(servidorP3);
    close(clienteP1);
    close(servidorP2);
    
    exit(EXIT_SUCCESS);
}