#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

int main() {
    int n = 0;

    int server_socket;
    struct sockaddr_un server_addr;
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, "Eden Hazard - Rusia 2018");

    server_socket = socket(AF_UNIX, SOCK_STREAM, 0);

    if (connect(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error");
        exit(EXIT_FAILURE);
    } 

    while (n < 50)
    {
        if (read(server_socket, &n, sizeof(int)) == 0) 
        {
            perror("Error");
            exit(EXIT_FAILURE);
        }

        printf("[Proceso 2] Recibi %d\n", n);

        n++;

        if (write(server_socket, &n, sizeof(int)) == -1) 
        {
            perror("Error");
            exit(EXIT_FAILURE);
        }

    }


    close(server_socket);
    exit(0);
}
