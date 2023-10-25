/* CLIENT */
/* Es tracta d'un exemple per als professors */
/* Als estudiants els posarem un esquelet */

/* Inclusio de fitxers .h habituals */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Inclusio de fitxers .h per als sockets */
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

/* Inclusio de time.h per calcular el numero aleatori */
#include <time.h>

#define MIDA_BUFFER 1024

struct DataClient 
{
    uint32_t id;
    char command;

};


uint32_t generarIdAleatoria() {
    // Inicializa la semilla del generador de números aleatorios con la hora actual
    srand(time(NULL));

    // Genera 4 números de 8 bits y los combina en un número de 32 bits
    uint32_t randomNum = 0;
    for (int i = 0; i < 4; i++) {
        uint32_t aleatorio8Bits = rand() & 0xFF; // Genera un número aleatorio de 8 bits
        randomNum = (randomNum << 8) | aleatorio8Bits; // Combina los bits
    }

    return randomNum;
}

int main(int argc, char *argv[]) {
    char response;
    struct sockaddr_in server_addr;
    struct DataClient package;

    package.id = generarIdAleatoria();
    printf("Número aleatorio de 32 bits: %u\n", package.id);

    // Crear socket del cliente
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Error al crear el socket del cliente");
        exit(2);
    }

    // Definir la dirección del servidor
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    inet_pton(AF_INET, argv[1], &server_addr.sin_addr);

    // Conectar al servidor
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error al conectar al servidor");
        exit(3);
    }

        // Implementa la lógica del juego y la comunicación con el servidor aquí
        // Envía comandos al servidor y recibe respuestas

        // Leer el comando del usuario (U, D, L, R)
        printf("Ingrese un comando (U, D, L, R): ");
        scanf(" %c", &package.command);

        // Enviar el comando al servidor
        send(client_socket, &package, sizeof(struct DataClient), 0);

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


        // Cierra el socket del cliente
        close(client_socket);
        return 0;
    }
