#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <IP del servidor> <Puerto del servidor>\n", argv[0]);
        exit(1);
    }

    // Crear socket del cliente
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Error al crear el socket del cliente");
        exit(2);
    }

    // Definir la dirección del servidor
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    inet_pton(AF_INET, argv[1], &server_addr.sin_addr);

    // Conectar al servidor
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr) == -1) {
        perror("Error al conectar al servidor");
        exit(3);
    }

    char command;
    char response;

    while (1) {
        // Implementa la lógica del juego y la comunicación con el servidor aquí
        // Envía comandos al servidor y recibe respuestas

        // Leer el comando del usuario (U, D, L, R)
        printf("Ingrese un comando (U, D, L, R): ");
        scanf(" %c", &command);

        // Enviar el comando al servidor
        send(client_socket, &command, sizeof(char), 0);

        // Recibir la respuesta del servidor
        recv(client_socket, &response, sizeof(char), 0);

        // Procesar la respuesta
        switch (response) {
            case ' ': // Casilla libre
                printf("Casilla libre. Continúa.\n");
                break;
            case 'T': // Casilla con trampa
                printf("¡Has caído en una trampa! Pierdes una vida.\n");
                break;
            case 'P': // Casilla de pared
                printf("No puedes pasar. Elige otro camino.\n");
                break;
            case 'V': // Casilla que incrementa una vida
                printf("¡Has encontrado una vida extra!\n");
                break;
            case 'S': // Puerto de llegada
                printf("DATAGRAMA ENTREGADO!! Has ganado el juego.\n");
                close(client_socket); // Cerrar el socket
                exit(0);
        }
    }

    // Cierra el socket del cliente
    close(client_socket);

    return 0;
}
