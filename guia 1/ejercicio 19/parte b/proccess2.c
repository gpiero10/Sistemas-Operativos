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
    // SOY PROCESO 2
    int n = 0;

    int servidorP2;
    int servidorP1;
    int clienteP3;
    
    struct sockaddr_un p2ServerAddr;
    struct sockaddr_un p1ServerAddr;

    // Seteo socket server P2
    strcpy(p2ServerAddr.sun_path, "p2 Socket");
    unlink(p2ServerAddr.sun_path);
    p2ServerAddr.sun_family = AF_UNIX;

    servidorP2 = socket(AF_UNIX, SOCK_STREAM, 0);
    if (bind(servidorP2, (struct sockaddr *) &p2ServerAddr, sizeof(p2ServerAddr)) == -1) {
        perror("Error");
        exit(EXIT_FAILURE);
    }
    if (listen(servidorP2, 1) == -1) {
        perror("Error");
        exit(EXIT_FAILURE);
    }

    // Conectarse a servidor P1
    strcpy(p1ServerAddr.sun_path, "p1 Socket");
    p1ServerAddr.sun_family = AF_UNIX;
    servidorP1 = socket(AF_UNIX, SOCK_STREAM, 0);
    while (connect(servidorP1, (struct sockaddr *) &p1ServerAddr, sizeof(p1ServerAddr)) == -1) {usleep(50000);}

    // Acepto conexion de P3
    clienteP3 = accept(servidorP2, NULL, NULL); // no me interesa la info del cliente

    if (clienteP3 == -1) {
        perror("Error");
        exit(EXIT_FAILURE);
    }

    while(read(servidorP1, &n, sizeof(int))>0)
    {
        printf("[Proceso 2] Recibi %d\n", n);
        n ++;
        write(clienteP3, &n, sizeof(int));
    }

    close(servidorP2);
    close(clienteP3);
    close(servidorP1);
    
    exit(EXIT_SUCCESS);
}
