

# Doc

Este es un juego de red implementado como una aplicación cliente/servidor en lenguaje C. El juego se llama "Juego del Caballero", y el objetivo es llevar un Datagrama IP desde un punto a otro en un tablero esquivando obstáculos. Tanto el cliente como el servidor funcionan como programas de terminal y se comunican mediante sockets. Aquí proporcionaremos una descripción detallada de cómo funciona el juego y cómo se implementa.
![](https://image.lexica.art/full_jpg/f079a696-34a1-4ef8-9e26-bb81c4145100)

								Lexica Aperture v2

`

## Cliente

### Funcionamiento

El cliente es la parte del juego que interactúa con el usuario. El jugador utiliza el cliente para mover al caballero a través del tablero enviando comandos al servidor. Los comandos disponibles son:

- `U`: Mover hacia arriba.
- `D`: Mover hacia abajo.
- `L`: Mover hacia la izquierda.
- `R`: Mover hacia la derecha.

El cliente envía estos comandos al servidor y recibe respuestas según la casilla en la que aterriza el caballero.

### Comunicación con el Servidor

La comunicación entre el cliente y el servidor se realiza a través de sockets. El cliente se conecta al servidor utilizando la dirección IP y el puerto proporcionados en línea de comandos. Luego, el cliente envía comandos al servidor y recibe respuestas del mismo.

### Lógica del Juego

- El cliente recibe el estado actual del tablero del servidor, que se representa como una cuadrícula con diferentes tipos de casillas: ' ' (casillas libres), 'T' (casillas con trampas), 'P' (casillas de pared), 'V' (casillas que incrementan 1 vida), 'S' (puerto de llegada).
- El jugador ingresa comandos para mover al caballero en el tablero.
- El servidor envía la casilla correspondiente a la posición actual del caballero y mueve al caballero según corresponda.
- El juego continúa hasta que el caballero llega al puerto de llegada ('S') o pierde todas sus vidas.

## Servidor

### Funcionamiento

El servidor es responsable de gestionar el juego, mantener el estado del tablero, y comunicarse con el cliente. Cuando un cliente se conecta, el servidor inicia una sesión del juego para ese cliente.

### Comunicación con el Cliente

El servidor escucha en un puerto especificado y acepta conexiones de clientes. Una vez que se establece una conexión, el servidor envía el estado actual del tablero al cliente y recibe comandos de movimiento. Luego, el servidor envía la respuesta al cliente.

### Tablero Inicial

El servidor comienza con un tablero inicial que se representa como una matriz de caracteres. Puedes personalizar el tablero inicial ajustando el tamaño y el contenido según tus necesidades. El tablero es esencial para el funcionamiento del juego y determina la ubicación de las casillas ' ', 'T', 'P', 'V', y 'S'.

### Lógica del Juego

- El servidor envía el estado actual del tablero al cliente, incluyendo la posición actual del caballero.
- El servidor recibe comandos del cliente ('U', 'D', 'L', 'R') y actualiza la posición del caballero en el tablero.
- El servidor procesa las casillas a las que se mueve el caballero y envía respuestas al cliente según la casilla.
- El juego continúa hasta que el caballero llega al puerto de llegada ('S') o pierde todas sus vidas.

## Ejecución del Programa

### Cliente

Para ejecutar el cliente, utiliza el siguiente comando en la línea de comandos:

```shell
./cliente <IP_del_servidor> <Puerto_del_servidor>
```

Reemplaza `<IP_del_servidor>` con la dirección IP del servidor y `<Puerto_del_servidor>` con el número de puerto en el que el servidor está escuchando.

### Servidor

Para ejecutar el servidor, utiliza el siguiente comando en la línea de comandos:

```shell
./servidor <Puerto_del_servidor>
```

Reemplaza `<Puerto_del_servidor>` con el número de puerto en el que el servidor estará escuchando.

## Personalización del Juego

Puedes personalizar el juego ajustando el tamaño y el contenido del tablero inicial en el servidor. 
### Posibles Mejoras

 - Podemos agregar características adicionales tales como: Obstáculos o
   eventos especiales en el tablero. 
   
 - Una base de datos donde se registre
   el cliente y su tablero actualizado (con esto permitimos abrir mas
   puertos ó clientes)



## Conclusión

El "Juego del Caballero" es un ejemplo de una aplicación cliente/servidor en lenguaje C que utiliza sockets para la comunicación. Los jugadores pueden mover al caballero en un tablero y deben evitar obstáculos para llegar al puerto de llegada. Esta documentación proporciona una visión general de cómo funciona el juego y cómo se implementa la comunicación entre el cliente y el servidor.
