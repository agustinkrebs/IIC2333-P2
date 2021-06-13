#include <unistd.h>
#include <stdio.h>
#include "comunication.h"
#include "conection.h"
#include "game.h"
#include <sys/time.h>
#include <string.h>

int N_PLAYERS = 4;

//////////////////////////////////// CODIGOS DE ENVIO DEL SERVIDOR HACIA EL CLIENTE

/*
(1) Mensaje de texto (no espera respuesta)
(2) Ingresar datos personales de juego
(3) Tienes la facultad de comenzar el juego
(55) Ingresa elección de monstruo
(99) Notificar a un cliente de que empezó su turno y preguntarle si desea retirarse.
(97) Solicitarle al cliente que debe elegir su habilidad para hacer daño.
(95) Doctor o Hacker: elegir a otro jugador sobre el cual aplicar el poder selecciondo

*/

//////////////////////////////////// CODIGOS DE ENVIO DEL CLIENTE HACIA EL SERVIDOR

/*
(2) Se envían datos del juego
(3) Quiero comenzar el juego
(55) Recepción de monstruo elegido
(98) Elección de si quiere retirarse o no en este turno
(96) Elección del poder a utilizar
(94) Doctor: Elección del jugador elegido para sanar
*/

int prepare_socket(char * IP, int port);
PlayersInfo * get_team_leader(char server_socket);
int add_client(int server_socket);
void choose_monster(Game* game, int selection);

int main(int argc, char *argv[]){
  // Ejemplo de input de consola: ./server -i 0.0.0.0 -p tcp_8080
  
  Game* game = malloc(sizeof(Game));
  
  game->players = malloc(N_PLAYERS*sizeof(Player));

  game->n_players = N_PLAYERS;
  game->remaining_players = N_PLAYERS;
  game->players_connected = 0;
  game->rounds = 0;
  game->monster = malloc(sizeof(Monster));

  // Se define una IP y un puerto
  char * IP = argv[2];
  int PORT = atoi(argv[4]);

  printf("main | ip_address: %s\n", argv[2]);
  printf("main | tcp_port: %s\n", argv[4]);

  // Se crea un mensaje generico
  char *message[255];
  char *name[20];
  char *clase[20];

  // Estructuras para los clientes
  int number_clients = 0;
  int registered_clients = 0;
  int lobby = 1;
  int i; 
  

  // Se crea el servidor y se obtienen los sockets de ambos clientes.
  int server_socket = prepare_socket(IP, PORT);
  
  printf("Se comienza a escuchar clientes\n");
  

  fd_set current_sockets, ready_sockets;
  FD_ZERO(&current_sockets);
  FD_SET(server_socket, &current_sockets);

  while (lobby){

    ready_sockets = current_sockets;
    if (select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL) < 0){
      exit(-1);
    }

    for (i = 0; i < FD_SETSIZE; i ++){
      if (FD_ISSET(i, &ready_sockets)) {
        if (i == server_socket){
          Player* player = malloc(sizeof(Player));
          game->players[number_clients] = player;
          game->players[number_clients]->socket = add_client(server_socket);
          FD_SET(game->players[number_clients]->socket, &current_sockets);
          *message = "Bienvenido al Juego!\n";
          server_send_message(game->players[number_clients]->socket, 1, *message);

          if (number_clients == 0){
            *message = "Eres el primer jugador en conectarse, por lo que serás el jefe!\n";
            server_send_message(game->players[0]->socket, 1, *message);
          } else {
            *message = "------------- Alerta ---------------- \n";
            server_send_message(game->players[0]->socket, 1, *message);
            *message = "Se acaba de conectar un nuevo jugador\nEstá en en el lobby eligiendo su nombre y clase";
            server_send_message(game->players[0]->socket, 1, *message);
            *message = "\n------------- Fin Alerta ---------------- \n";
            server_send_message(game->players[0]->socket, 1, *message);
          }

          *message = "Vamos a registrarnos!\n";
          server_send_message(game->players[number_clients]->socket, 2, *message);
          number_clients ++;
        } else {

          // Primero debemos saber qué socket me habló
          int found = 0;
          for (int k = 0; k < 4; k ++){
            if (game->players[k]->socket == i){
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
              *game->players[found]->name = *token;
              *name = token;
              token = strtok(NULL, " ");
              printf("%s\n", token);
              int class_preference = atoi(token);
              char class_preference_name[20];
              game->players[found] = create_new_player(game->players[found], class_preference);
              if (class_preference == 1){
                  strcpy(class_preference_name, "Cazador");
              } else if (class_preference == 2){
                  strcpy(class_preference_name, "Médico");
              } else {
                  strcpy(class_preference_name, "Hacker");
              }
              registered_clients ++;
              printf("El cliente %d dice: %s\n", i, client_message);
              *message = "Tu personaje fue creado! Esperando que se registren los demás jugadores ...\n";
              server_send_message(game->players[found]->socket, 1, *message);
              if (found == 0){
                *message = "Puedes iniciar el juego cuando desees\n";
                server_send_message(game->players[found]->socket, 3, *message);
              } else {
                // Se notifica al lider de la inclusión 
                *message = "------------- Alerta ---------------- \n";
                server_send_message(game->players[0]->socket, 1, *message);
                *message = "Nuevo jugador con nombre";
                server_send_message(game->players[0]->socket, 1, *message);
                server_send_message(game->players[0]->socket, 1, *name);
                *message = "Loggeo con clase";
                server_send_message(game->players[0]->socket, 1, *message);
                server_send_message(game->players[0]->socket, 1, class_preference_name);
                *message = "\n------------- Fin Alerta ---------------- \n";
                server_send_message(game->players[0]->socket, 1, *message);
              }
              free(class_preference_name);
              free(token);
            }
            free(client_message);
          }
          // Caso en que el usuario quiera empezar el juego
          if (msg_code == 3){
            char * client_message = server_receive_payload(i);
            printf("El Jefe quiere comenzar el juego\n");
            if (number_clients == registered_clients) {
              printf("Se procede a preguntarle al lider qué monstruo quiere utilizar!\n");
              *message = "¿Contra qué mounstro quieren luchar? \n";
              server_send_message(game->players[0]->socket, 55, *message);
            } else {
              *message = "Hay jugadores que aun no declaran su nombre y clase! Intenta iniciar el juego en unos momentos más!\n\n";
              server_send_message(game->players[found]->socket, 3, *message);
            }
            free(client_message);
          }

          if (msg_code == 55){
            // Se recibe la elección del monstruo
            char * client_message = server_receive_payload(i);
            printf("Se recepciona la elección del monstruo\n");
            int monster = atoi(client_message);
            choose_monster(game, monster);
            printf("Comienza el juego!\n");
            *message = "Comienza el juego!\n";
            for (i = 0; i < number_clients; i ++) {
              server_send_message(game->players[i]->socket, 1, *message);
            }   
            free(client_message);  
          }
        }
      }
    }
  }
  printf("okkk\n");
  


  // Recepción de datos de los jugadores
  int players_data = 0;
  int attention = 0;
  while (players_data < number_clients){
    char * client_message = server_receive_payload(game->players[attention]->socket);
    printf("El cliente %d dice: %s\n", attention+1, client_message);
    players_data ++;
    attention ++;
    if (attention == 4){
      attention = 0;
    }
  }

  int turn = 0;
  int turn_value;
  
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
  
  free(IP);
  free(message);
  free(name); 
  free(clase);

  free(game->monster);
  free(game);
  return 0;

    
  }
