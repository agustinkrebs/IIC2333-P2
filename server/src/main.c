#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "comunication.h"
#include "conection.h"
#include "game.h"

char * revert(char * message){
  //Se invierte el mensaje

  int len = strlen(message) + 1;
  char * response = malloc(len);

  for (int i = 0; i < len-1; i++)
  {
    response[i] = message[len-2-i];
  }
  response[len-1] = '\0';
  return response;
}

int main(int argc, char *argv[]){
  // // Se define una IP y un puerto
  // char * IP = "0.0.0.0";
  // int PORT = 8080;

  // // Se crea el servidor y se obtienen los sockets de ambos clientes.
  // PlayersInfo * players_info = prepare_sockets_and_get_clients(IP, PORT);

  // // Le enviamos al primer cliente un mensaje de bienvenida
  // char * welcome = "Bienvenido Cliente 1!!";
  // server_send_message(players_info->socket_c1, 1, welcome);

  // // Guardaremos los sockets en un arreglo e iremos alternando a quién escuchar.
  // int sockets_array[2] = {players_info->socket_c1, players_info->socket_c2};
  // int my_attention = 0;
  // while (1)
  // {
  //   // Se obtiene el paquete del cliente 1
  //   int msg_code = server_receive_id(sockets_array[my_attention]);

  //   if (msg_code == 1) //El cliente me envió un mensaje a mi (servidor)
  //   {
  //     char * client_message = server_receive_payload(sockets_array[my_attention]);
  //     printf("El cliente %d dice: %s\n", my_attention+1, client_message);

  //     // Le enviaremos el mismo mensaje invertido jeje
  //     char * response = revert(client_message);

  //     // Le enviamos la respuesta
  //     server_send_message(sockets_array[my_attention], 1, response);
  //   }
  //   else if (msg_code == 2){ //El cliente le envía un mensaje al otro cliente
  //     char * client_message = server_receive_payload(sockets_array[my_attention]);
  //     printf("Servidor traspasando desde %d a %d el mensaje: %s\n", my_attention+1, ((my_attention+1)%2)+1, client_message);

  //     // Mi atención cambia al otro socket
  //     my_attention = (my_attention + 1) % 2;

  //     server_send_message(sockets_array[my_attention], 2, client_message);
  //   }
  //   printf("------------------\n");
  // }e

  /*
    la variable turn siempre aumenta, para acceder al jugador especifico se hace turn% # jugadores
  */
  int N_PLAYERS = 4;
  int turn = 0;
  int turn_value;
  Game* game = malloc(sizeof(Game));
  game->players = malloc(N_PLAYERS * sizeof(Player));
  for (int i = 0; i < N_PLAYERS; i++){
    game->players[i] = malloc(sizeof(Player));
    /*Falta hacer funcion  que asigna al player su tipo*/
  }
  while(1){
    turn_value = turn_choices(game, turn, N_PLAYERS);
    if (turn_value){
      if (turn_value == -1){
        /*El jugador del turno eligio rendirse */
      } else {
        /* turn_value es el valor del monstruo seleccionado en el primer turno*/
      }
    }
    /* Hacer algo con current_skill y current_target seteado en el jugador del turno*/
    turn ++;
  }
  /* Liberar memoria */
  return 0;
}
