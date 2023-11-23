#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>

// Tamaño del tablero
#define ROWS 5
#define COLS 5
#define MAXPLAYERS 20
#define MAXLIVES 3

struct DataClient
{
    uint32_t id;
    char command;
    int fila;
    int columna;

};

struct Jugador
{
    uint32_t id;
    int lives;
    int row;
    int col;
};
struct Jugador jugadores[MAXPLAYERS];

// Definir el tablero inicial
char board[ROWS][COLS] = {
        {' ', ' ', 'T', ' ', 'S'},
        {' ', ' ', 'P', ' ', 'P'},
        {'T', 'V', ' ', 'T', ' '},
        {'P', 'T', 'T', ' ', 'V'},
        {' ', ' ', ' ', 'T', 'T'}
};

int ServerConnect(char *argv)
{

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
    server_addr.sin_port = htons(atoi(argv));
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
    return server_socket;
}

//Comprobar si el jugador esta registrado
bool isPlayerRegistered(struct DataClient packClient){
    for (int i = 0; i < MAXPLAYERS; ++i) {
        if (jugadores[i].id == packClient.id){
            return true;
        }
    }
    return false;
}

//Obtener la posicion del array de clientes
int getClientNumber(struct DataClient packClient){
    for (int i = 0; i < MAXPLAYERS; ++i) {
        if (jugadores[i].id == packClient.id){
            return i;
        }
    }
    return -1;
}

//Registrar a un jugador e inicializar sus vidas y posicion
int registerPlayer(int client_socket, uint32_t id){
    for (int i = 0; i < MAXPLAYERS; ++i) {
        if (jugadores[i].id == 0){
            jugadores[i].id = id;
            jugadores[i].lives = MAXLIVES;
            jugadores[i].row = 0;
            jugadores[i].col = 0;
            return i;
        }
    }
    return -1;
}

//Comprobar si el jugador esta vivo
bool isPlayerAlive(int clientNumber){
    if (jugadores[clientNumber].lives > 0){
        return true;
    }
    return false;
}

//Ver hacia donde se dirige
char whereIsGoingTo(int clientNumber, char command){
    int row = jugadores[clientNumber].row;
    int col = jugadores[clientNumber].col;
    switch (command) {
        case 'U':
            if (row > 0) row--;
            else return 'P';
            break;
        case 'D':
            if (row < ROWS - 1) row++;
            else return 'P';
            break;
        case 'L':
            if (col > 0) col--;
            else return 'P';
            break;
        case 'R':
            if (col < COLS - 1) col++;
            else return 'P';
            break;
    }
    return board[row][col];
}

//Actualizar la posicion del jugador
void setPosition(int clientNumber, char command){
    switch (command) {
        case 'U':
            if (jugadores[clientNumber].row > 0) jugadores[clientNumber].row--;
            else (printf("El cliente %u no puede ir hacia arriba\n", jugadores[clientNumber].id));
            break;
        case 'D':
            if (jugadores[clientNumber].row < ROWS - 1) jugadores[clientNumber].row++;
            else (printf("El cliente %u no puede ir hacia abajo\n", jugadores[clientNumber].id));
            break;
        case 'L':
            if (jugadores[clientNumber].col > 0) jugadores[clientNumber].col--;
            else (printf("El cliente %u no puede ir hacia izquierda\n", jugadores[clientNumber].id));
            break;
        case 'R':
            if (jugadores[clientNumber].col < COLS - 1) jugadores[clientNumber].col++;
            else (printf("El cliente %u no puede ir hacia derecha\n", jugadores[clientNumber].id));
            break;
    }
}

//Mostrar el tablero con el jugador
void printBoard(int clientNumber){
    for (int i = 0; i < ROWS; ++i) {
        printf("|");
        for (int j = 0; j < COLS; ++j) {
            if (i == jugadores[clientNumber].row && j == jugadores[clientNumber].col){
                printf("K");
            }else{
                printf("%c", board[i][j]);
            }
            printf("|");
        }
        printf("\n");
    }
}



int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <Puerto del servidor>\n", argv[1]);
        exit(1);
    }
    // Lógica del juego y comunicación con el cliente
    int currentClientSocket = 0;
    int currentRow = 0;
    int currentCol = 0;

    //Para guardar los paquetes recibidos
    struct DataClient packClient;

    // Encender el servidor y guardar el numero de socket
    int server_socket = ServerConnect(argv[1]);
    // *** Servidor Operativo ***
    fprintf(stderr, "Servidor operativo por el puerto %s\n", argv[1]);

    while (1) {

        // Aceptar conexiones de clientes
        printf("Esperando conexiones de clientes...\n");
        int client_socket = accept(server_socket, NULL, NULL);
        printf("Conexión aceptada\n");
        if (client_socket == -1) {
            perror("Error al aceptar la conexión del cliente");
            continue;
        }

        // Recibir el comando del cliente
        recv(client_socket, &packClient, sizeof(struct DataClient), 0);
        printf("El cliente %u envió el comando %c\n", packClient.id, packClient.command);

        //Registrar cliente si no lo esta
        if (!isPlayerRegistered(packClient)){
            currentClientSocket = registerPlayer(client_socket, packClient.id);
        }else{
            currentClientSocket = getClientNumber(packClient);
        }



        //Comprobar si el jugador esta vivo
        if(!isPlayerAlive(currentClientSocket)){
            //Enviar respuesta 'D' al cliente y posicion actual
            packClient.command = 'D';
            packClient.fila = jugadores[currentClientSocket].row;
            packClient.columna = jugadores[currentClientSocket].col;
            send(client_socket, &packClient, sizeof(struct DataClient), 0);
        }else{
            switch(whereIsGoingTo(currentClientSocket, packClient.command)){
                case ' ':

                    //Enviar respuesta 'D' al cliente y posicion actual
                    setPosition(currentClientSocket, packClient.command);
                    packClient.command = ' ';
                    packClient.fila = jugadores[currentClientSocket].row;
                    packClient.columna = jugadores[currentClientSocket].col;
                    send(client_socket, &packClient, sizeof(struct DataClient), 0);
                    break;
                case 'T':
                    //Enviar respuesta 'T' al cliente y posicion actual
                    setPosition(currentClientSocket, packClient.command);
                    packClient.command = 'T';
                    packClient.fila = jugadores[currentClientSocket].row;
                    packClient.columna = jugadores[currentClientSocket].col;
                    jugadores[currentClientSocket].lives--;
                    send(client_socket, &packClient, sizeof(struct DataClient), 0);
                    break;
                case 'P':
                    //Enviar respuesta 'P' al cliente y posicion actual
                    packClient.command = 'P';
                    packClient.fila = jugadores[currentClientSocket].row;
                    packClient.columna = jugadores[currentClientSocket].col;
                    send(client_socket, &packClient, sizeof(struct DataClient), 0);
                    break;
                case 'S':
                    //Enviar respuesta 'S' al cliente y posicion actual
                    setPosition(currentClientSocket, packClient.command);
                    packClient.command = 'S';
                    packClient.fila = jugadores[currentClientSocket].row;
                    packClient.columna = jugadores[currentClientSocket].col;
                    send(client_socket, &packClient, sizeof(struct DataClient), 0);
                    break;
                case 'V':
                    //Enviar respuesta 'V' al cliente y posicion actual
                    setPosition(currentClientSocket, packClient.command);
                    packClient.command = 'V';
                    packClient.fila = jugadores[currentClientSocket].row;
                    packClient.columna = jugadores[currentClientSocket].col;
                    send(client_socket, &packClient, sizeof(struct DataClient), 0);
                    break;
            }

        }

        // Cerrar el socket del cliente
        printBoard(currentClientSocket);
        close(client_socket);

        printf("Cerrando socket del cliente %u\n", packClient.id);
    }

    // Cerrar el socket del servidor
    close(server_socket);

    return 0;
}