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

El juego fue implementado de forma que se actualiza el numero de turnos al final de cada ronda cuando se usan los poderes.

### Cliente - Servidor

## Supuestos

### Supuestos de habilidades de Monstruo Ruiz

* Con el fin de hacer el juego más dificil para los jugadores, se supuso que cuando el monstruo ejecuta Caso Copia más de una vez, las habilidades son acumulativas. Es decir, el monstruo podría tener activa la Inyección SQL (ataques se multiplican por 2) y realizar el Ataque DDOS pero con doble intencidad. También, a la operación anterior se le podría sumar el sangrado sobre un jugador si es que corresponde, pero este al no ser un ataque directamente, más bien, un efecto de turnos anteriores no se ve incrementado por Inyección SQL. Además, la Inyección SQL del monstruo sirve para dos turnos del monstruo, indp

* Cuando se vuelve a jugar otra guerra, se vuelven a preguntar los nombres de los jugadores por si quieren cambiar su username.

* Dado que la inyección SQL es para los ataques, las curaciones no caen en este beneficio.