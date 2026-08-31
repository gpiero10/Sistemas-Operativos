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
    int n = 0;

    int server_socket;
    int client_socket;
    struct sockaddr_un server_addr;
    struct sockaddr_un client_addr;
    uint slen = sizeof(server_addr);
    uint clen = sizeof(client_addr);

    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, "Eden Hazard - Rusia 2018");
    unlink(server_addr.sun_path);

    server_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (bind(server_socket, (struct sockaddr *) &server_addr, slen) == -1) {
        perror("Error");
        exit(EXIT_FAILURE);
    }
    if (listen(server_socket, 1) == -1) {
        perror("Error");
        exit(EXIT_FAILURE);
    }


    client_socket = accept(server_socket, (struct sockaddr *) &client_addr, &clen);
    if (client_socket == -1) {
        perror("Error");
        exit(EXIT_FAILURE);
    }

    while(n < 50)
    {
        if (write(client_socket, &n, sizeof(int)) == -1) 
        {
            perror("Error");
            exit(EXIT_FAILURE);
        }
        read(client_socket, &n, sizeof(int));
        printf("[Proceso 1] Recibi %d\n", n);
        n++;
    }

    close(client_socket);
    exit(EXIT_SUCCESS);
}