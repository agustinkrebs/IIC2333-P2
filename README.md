# IIC2333-P2

## Autores
* Agustín Krebs - 16637879
* Felipe Barrientos - 16637771
* Santiago Kinast - 16622529
* Sebastián Ricke - 16635825
* Sebastián Morales - 16635582

## Instrucciones para ejecutar

¿Cómo correr los Makefile?

¿Cómo correr para jugar?

## Diseño

### Juego

* Se crearon tres structs. Uno correspondiente a la clase Jugador, la cual contenía el tipo como un integer para diferenciar su comportamiento. Otro struct era para modelar a la clase Monstruo, con la misma metodología que el Jugador para diferenciar los tipos. Y por último el struct Game que contiene a los jugadores y al monstruo correspondiente.

* Una vez definido la instancia de Game, los diferentes jugadores comienzan la interacción con la consola y a usar sus diferentes skills, los cuales son manejados por una función específica para manejar todas las funciones de habilidades.

* El juego fue implementado de forma que se actualiza el numero de turnos al final de cada ronda cuando se usan los poderes.

* No implementamos ningún bonus

* Dada nuestra modelación, se simula que todas las acciones de una ronda suceden en paralelo (aunque realmente en la práctica se va a atacando de manera secuencial). Esto produce el que el monstruo pueda atacar aunque tenga vida cero (esto solo dentro de la misma ronda). Nuevamente, esto ocurre porque nuestra modelación contempla el que todo lo que sucede en una misma ronda, en la virtualidad del juego, pasa en paralelo. Un caso de esto sería que, por ejemplo, aunque un primer jugador ataque y mate al monstruo, aún asi un siguiente jugador (en la misma ronda) podría atacar al monstruo (en la misma ronda).

### Cliente - Servidor

## Paquetes utilizados para la comunicación

* Para el protocolo de comunicación se utilizó el formato explicitado en el enunciado. En particular, al enviar un mensaje, la función que se hace cargo de esta tarea gestiona el paquete incluyéndo en el primer byte del paquete un ID del tipo de mensaje. Luego, en el byte siguiente se muestra el largo del mensaje y luego se incluye el mensaje. Cuando se envía un mensaje vacío el payload size es 0.

* En términos de los paquetes utilizados, hay dos tipos de mensajes, los que envían información y los que les dicen al cliente que necesitan un input de usuario. En caso de que haya que mandar información, se utilizó el identificador 1. Con este mensaje se buscaba que se mostrara algo en la consola del cliente. Para el caso en que se necesitaba pedirle un input al usuario se utilizó el identificador {30, 40, 2, 3}. Ellos se diferenciaban por el formato de menú que desplegaban, pero la lógica (y los datos) venía desde el servidor. Finalmente, existe el último mensaje es el de identificador {65, 99}, el cúal apaga el socket del cliente. Si bien implementamos la lógica de que solo se envíen datos desde el servidor al cliente, por temas de tiempo no pudimos llegar a la granularidad deseada, ya que tuvimos que ampliar el espectro de identificadores al pedir información. Esto, como se dijo antes, produjo que tuvieramos que mostrar algún tipo de menú específico al mandar un identidicador. Si bien no era lo que planificamos, esto era aceptado dado las issues.

* El lobby: Para realizar el lobby, se presentó la dificultad de que el servidor debía estar gestionando varios sockets al mismo tiempo. Es por ello que se utilizó el comando select. Este comando intenta simular paralelismo, revisando todos los sockets activos el PC y viendo si hay mensajes nuevos.

### Supuestos de habilidades de Monstruo Ruiz

* Con el fin de hacer el juego más dificil para los jugadores, se supuso que cuando el monstruo ejecuta Caso Copia más de una vez, las habilidades son acumulativas. Es decir, el monstruo podría tener activa la Inyección SQL (ataques se multiplican por 2) y realizar el Ataque DDOS pero con doble intencidad. También, a la operación anterior se le podría sumar el sangrado sobre un jugador si es que corresponde, pero este al no ser un ataque directamente, más bien, un efecto de turnos anteriores no se ve incrementado por Inyección SQL. Además, la Inyección SQL del monstruo sirve para dos turnos del monstruo, indp

* Cuando se vuelve a jugar otra guerra, se vuelven a preguntar los nombres de los jugadores por si quieren cambiar su username.

* Dado que la inyección SQL es para los ataques, las curaciones no caen en este beneficio.