#include <unistd.h>
#include <stdio.h>
#include "comunication.h"
#include "conection.h"
#include "game.h"

int N_PLAYERS = 2;

//////////////////////////////////// CODIGOS DE ENVIO DEL SERVIDOR

/*
(1) Mensaje de texto (no espera respuesta)
(2) Ingresar datos personales de juego

*/

//////////////////////////////////// CODIGOS DE ENVIO DEL CLIENTE

/*


*/

int prepare_socket(char * IP, int port);
PlayersInfo * get_team_leader(char server_socket);
int add_client(int server_socket);

int main(int argc, char *argv[]){
  // Se define una IP y un puerto
  char * IP = "0.0.0.0";
  int PORT = 8080;

  // Se crea un mensaje generico
  char *message[1000000];

  // Estructuras para los clientes
  int number_clients = 0;
  int clients[4], i; 

  // Se crea el servidor y se obtienen los sockets de ambos clientes.
  int server_socket = prepare_socket(IP, PORT);
  
  printf("Se comienza a escuchar clientes\n");

  while (number_clients < 4) {
    
    clients[number_clients] = add_client(server_socket);
    *message = "Bienvenido al Juego!\n";
    server_send_message(clients[number_clients], 1, *message);

    if (number_clients == 0){
      *message = "Eres el primer jugador en conectarse, por lo que serás el jefe!\n";
      server_send_message(clients[0], 1, *message);
    }

    *message = "Espera mientras los demás jugadores se conectan al juego ... \n";
    server_send_message(clients[number_clients], 1, *message);

    number_clients ++;
  }

  // Se envía una señal para que todos los jugadores envíen sus datos
  for (i = 0; i < 4; i ++){
    *message = "Todos los jugadores se lograron conectar! \n";
    server_send_message(clients[number_clients], 2, *message);
  }

  while (1) {
    ;
  }

  int turn = 0;
  int turn_value;
  Game* game = malloc(sizeof(Game));
  game->players = malloc(N_PLAYERS*sizeof(Player));
  game->n_players = N_PLAYERS;
  game->remaining_players = N_PLAYERS;
  game->players_connected = 0;
  game->rounds = 0;
  game->monster = malloc(sizeof(Monster));
  for (int i = 0; i < N_PLAYERS; i++){
    game->players[i] = create_new_player();
  }
  // Como no está inicializado el monstruo se cae al tratar de ver su current life
  while((game->remaining_players && game->monster->current_life) || !game->rounds){
    if (!game->rounds){
      // First turn
      printf("\n\n------------ Turno de jugador %s ----------------\n\n", game->players[turn]->name);
      turn_value = turn_choices(game, turn, N_PLAYERS);
      choose_monster(game, turn_value);
    } else {
      for (int turn = 0; turn < game->n_players; turn++){
        if (!game->players[turn]->retired){
          printf("\n\n------------ Turno de jugador %s ----------------\n\n", game->players[turn]->name);
          turn_value = turn_choices(game, turn, N_PLAYERS);
        }
      }
      printf("\n\n------------ Turno de monstruo ----------------\n\n");
      use_monster_skills(game);
    }
    /* Hacer algo con current_skill y current_target seteado en el jugador del turno*/
    update_round(game);
    game->rounds ++;
  }
  /* Liberar memoria */
  for (int i = 0; i < N_PLAYERS; i++){
    free(game->players[i]);
  }
  free(game->monster);
  free(game);
  return 0;

    
  }
