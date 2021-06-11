#include <unistd.h>
#include <stdio.h>
#include "comunication.h"
#include "conection.h"
#include "game.h"

int N_PLAYERS = 4;

int main(int argc, char *argv[]){
  int turn = 0;
  int turn_value;
  Game* game = malloc(sizeof(Game));
  game->players = malloc(N_PLAYERS*sizeof(Player));
  game->n_players = N_PLAYERS;
  game->players_connected = 0;
  game->monster = malloc(sizeof(Monster));
  for (int i = 0; i < N_PLAYERS; i++){
    game->players[i] = create_new_player();
  }
  while(1){
    turn_value = turn_choices(game, turn, N_PLAYERS);
    if (!turn){
      // First turn
      choose_monster(game, turn_value);
    } 
    else {
      printf("FALTA MOUNSTRO\n");
      //ya esta haciendo el llamado a las funciones de los jugadores, falta el mounstro
    }
      /* Hacer algo con current_skill y current_target seteado en el jugador del turno*/
    turn ++;
  }
  /* Liberar memoria */
  for (int i = 0; i < N_PLAYERS; i++){
    free(game->players[i]);
  }
  free(game->monster);
  free(game);
  return 0;


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
}
