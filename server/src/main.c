#include <unistd.h>
#include <stdio.h>
#include "comunication.h"
#include "conection.h"
#include "game.h"
#include <sys/time.h>

int N_PLAYERS = 4;

//////////////////////////////////// CODIGOS DE ENVIO DEL SERVIDOR

/*
(1) Mensaje de texto (no espera respuesta)
(2) Ingresar datos personales de juego
(3) Tienes la facultad de comenzar el juego

*/

//////////////////////////////////// CODIGOS DE ENVIO DEL CLIENTE

/*
(2) Se envían datos del juego
(3) Quiero comenzar el juego
*/

int prepare_socket(char * IP, int port);
PlayersInfo * get_team_leader(char server_socket);
int add_client(int server_socket);

int main(int argc, char *argv[]){

  Game* game = malloc(sizeof(Game));
  game->players = malloc(N_PLAYERS*sizeof(Player));
  game->n_players = N_PLAYERS;
  game->remaining_players = N_PLAYERS;
  game->players_connected = 0;
  game->rounds = 0;
  game->monster = malloc(sizeof(Monster));

  // Se define una IP y un puerto
  char * IP = "0.0.0.0";
  int PORT = 8080;

  // Se crea un mensaje generico
  char *message[1000000];

  // Estructuras para los clientes
  int number_clients = 0;
  int registered_clients = 0;
  int lobby = 1;
  int i; 
  struct player players[4];

  // Se crea el servidor y se obtienen los sockets de ambos clientes.
  int server_socket = prepare_socket(IP, PORT);
  
  printf("Se comienza a escuchar clientes\n");
  

  fd_set current_sockets, ready_sockets;
  FD_ZERO(&current_sockets);
  FD_SET(server_socket, &current_sockets);
  printf("Se comienza a escuchar clientes\n");

  while (lobby){

    ready_sockets = current_sockets;
    if (select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL) < 0){
      exit(-1);
    }

    for (i = 0; i < FD_SETSIZE; i ++){
      if (FD_ISSET(i, &ready_sockets)) {
        if (i == server_socket){
          printf("Entramos\n");
          players[number_clients].socket = add_client(server_socket);
          FD_SET(players[number_clients].socket, &current_sockets);
          *message = "Bienvenido al Juego!\n";
          server_send_message(players[number_clients].socket, 1, *message);

          if (number_clients == 0){
            *message = "Eres el primer jugador en conectarse, por lo que serás el jefe!\n";
            server_send_message(players[0].socket, 1, *message);
          }

          server_send_message(players[number_clients].socket, 2, *message);
          number_clients ++;
        } else {

          // Primero debemos saber qué socket me habló
          int found = 0;
          for (int k = 0; k < 4; k ++){
            if (players[k].socket == i){
              found = k;
              break;
            }
          }
          int msg_code = server_receive_id(i);

          // Caso en que me den los datos para inicializar al jugador
          if (msg_code == 2){
            char * client_message = server_receive_payload(i);
            if (client_message){
              char * token = strtok(client_message, "|");
              printf("%s\n", token);
              *players[found].name = *token;
              token = strtok(NULL, " ");
              printf("%s\n", token);
              if (!strncmp(token, "1", 2)){
                printf("era un 1\n");
                players[found].type = 1;
              } else if (!strncmp(token, "2", 2)) {
                printf("era un 2\n");
                players[found].type = 2;
              } else if (!strncmp(token, "3", 2)) {
                printf("era un 3\n");
                players[found].type = 3;
              } else {
                printf("numero invalido\n");
              }
              registered_clients ++;
              printf("El cliente %d dice: %s\n", i, client_message);
              *message = "Tu personaje fue creado! Esperando que se registren los demás jugadores ...\n";
              server_send_message(players[found].socket, 1, *message);
              printf("%i\n", found);
              if (found == 0){
                printf("aa%i\n", found);
                *message = "Puedes iniciar el juego cuando desees\n";
                server_send_message(players[found].socket, 3, *message);
              }
            }
          }
          // Caso en que el usuario quiera empezar el juego
          if (msg_code == 3){
            printf("El Jefe quiere comenzar el juego\n");
            if (number_clients == registered_clients) {
              printf("Se comienza el juego!\n");
              lobby = 0;
              printf("hola\n");
            } else {
              *message = "Hay jugadores que aun no declaran su nombre y clase! Intenta iniciar el juego en unos momentos más!\n\n";
              server_send_message(players[found].socket, 3, *message);
            }
          }
        printf("hola\n");
        }
      }
    }
  }
  printf("okkk\n");


  // Recepción de datos de los jugadores
  int players_data = 0;
  int attention = 0;
  while (players_data < number_clients){
    char * client_message = server_receive_payload(players[attention].socket);
    printf("El cliente %d dice: %s\n", attention+1, client_message);
    players_data ++;
    attention ++;
    if (attention == 4){
      attention = 0;
    }
  }

  while (1) {
    ;
  }

  int turn = 0;
  int turn_value;
  
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
