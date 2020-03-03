//
// Created by esteban on 29/2/20.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main() {
    struct sockaddr_in direccionServidor;
    direccionServidor.sin_family = AF_INET;
    direccionServidor.sin_addr.s_addr = inet_addr("127.0.0.1");
    direccionServidor.sin_port = htons(8080);

    int cliente = socket(AF_INET, SOCK_STREAM, 0);
    if (connect(cliente, reinterpret_cast<const sockaddr *>(&direccionServidor), sizeof(direccionServidor)) != 0) {
        perror("No se pudo conectar");
        return 1;
    }

    while (1) {
        char mensaje[1000];
        scanf("%s", mensaje);

        send(cliente, mensaje, strlen(mensaje), 0);
    }

    return 0;
}
