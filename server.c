#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

// Tamaño del tablero
#define ROWS 5
#define COLS 5

struct DataClient 
{
    uint32_t id;
    char command;

};

// Definir el tablero inicial
char board[ROWS][COLS] = {
    {' ', ' ', ' ', 'P', 'S'},
    {'T', 'V', ' ', ' ', 'P'},
    {' ', 'T', ' ', 'T', ' '},
    {'P', ' ', 'T', ' ', 'V'},
    {' ', ' ', ' ', 'T', 'T'}
};

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <Puerto del servidor>\n", argv[1]);
        exit(1);
    }
    

    // Crear socket del servidor
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Error al crear el socket del servidor");
        exit(2);
    }

    // Configurar la dirección del servidor
    struct sockaddr_in server_addr;
        server_addr.sin_family = AF_INET; 
        server_addr.sin_addr.s_addr = INADDR_ANY; 
        server_addr.sin_port = htons(atoi(argv[1])); 
    // Enlazar el socket a la dirección
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error al enlazar el puerto");
        exit(3);
    }

    // Escuchar en el puerto
    if (listen(server_socket, 5) == -1) {
        perror("Error al escuchar en el puerto");
        exit(4);
    }

    // *** Servidor Operativo ***
    fprintf(stderr, "Servidor operativo por el puerto %s\n", argv[1]);
    while (1) {
        // Aceptar conexiones de clientes
        int client_socket = accept(server_socket, NULL, NULL);
        if (client_socket == -1) {
            perror("Error al aceptar la conexión del cliente");
            continue;
        }

        // Lógica del juego y comunicación con el cliente
        int lives = 3;
        int row = 0;
        int col = 0;

        while (lives > 0) {
            // Recibir el comando del cliente
            struct DataClient packClient;
            recv(client_socket, &packClient, sizeof(struct DataClient), 0);
            printf("EL id del cliente %u y su respuesta: %c \n ",packClient.id,packClient.command);
            printf("prueba fila %d prueba col %d\n",row, col);
            // Procesar el comando y actualizar la posición del caballero
            switch (packClient.command) {
                case 'U':
                    if (row > 0) row--;
                    break;
                case 'D':
                    if (row < ROWS - 1) row++;
                    break;
                case 'L':
                    if (col > 0) col--;
                    break;
                case 'R':
                    if (col < COLS - 1) col++;
                    break;
            }
            // Actualizar las vidas según la casilla actual
            if (board[row][col] == 'T') {
                lives--;
            } else if (board[row][col] == 'V') {
                lives++;
            } else if (board[row][col] == 'S') {
                // El cliente ganó el juego
                char win_message = 'S';
                send(client_socket, &win_message, sizeof(char), 0);
                break;
            }
            // Enviar el estado actual del tablero al cliente
            char current_cell = board[row][col];
            send(client_socket, &current_cell, sizeof(char), 0);
        }

        // Cerrar el socket del cliente
        close(client_socket);
    }

    // Cerrar el socket del servidor
    close(server_socket);

    return 0;
}