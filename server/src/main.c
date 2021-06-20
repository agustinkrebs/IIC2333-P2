#include <unistd.h>
#include <stdio.h>
#include "comunication.h"
#include "conection.h"
#include "game.h"
#include <sys/time.h>
#include <string.h>
#include <stdbool.h>

int MAX_N_PLAYERS = 4;

//////////////////////////////////// CODIGOS DE ENVIO DEL SERVIDOR HACIA EL CLIENTE

/*
(1) Mensaje de texto (no espera respuesta)
(40) Sin mensaje pero espera respuesta.

(2) Ingresar datos personales de juego
(3) Tienes la facultad de comenzar el juego
(99) Notificar a un cliente de que empezó su turno y preguntarle si desea retirarse.

*/

//////////////////////////////////// CODIGOS DE ENVIO DEL CLIENTE HACIA EL SERVIDOR

/*
(2) Se envían datos del juego
(3) Quiero comenzar el juego
*/


int main(int argc, char *argv[]){
  // Ejemplo de input de consola: ./server -i 0.0.0.0 -p 8080
  
  int turn;
  int turn_value;
  int play;
  while (1) {
    Game* game;

    turn = 0;
    game = malloc(sizeof(Game));
    game->players = malloc(MAX_N_PLAYERS*sizeof(Player));
    game->n_players = 0;
    game->remaining_players = 0;
    game->players_connected = 0;
    game->rounds = 0;
    game->monster = malloc(sizeof(Monster));
    game->monster->current_life = -1;

    // Se define una IP y un puerto
    char IP[20];
    strcpy(IP, argv[2]);
    int PORT = atoi(argv[4]);
    // char * IP = "0.0.0.0";
    // int PORT = 8080;

    printf("SERVER: main | ip_address: %s\n", IP);
    printf("SERVER: main | tcp_port: %i\n", PORT);

    // Se crea un mensaje generico
    // char message[255];
    char* message = malloc(256);
    char name[20];
    char* clase[20];

    // Estructuras para los clientes
    int number_clients = 0;
    int registered_clients = 0;
    int lobby = 1;
    int i; 
    

    // Se crea el servidor y se obtienen los sockets de ambos clientes.
    int server_socket = prepare_socket(IP, PORT);
    
    printf("SERVER: main | Se comienza a escuchar clientes\n");
    

    fd_set current_sockets, ready_sockets;
    FD_ZERO(&current_sockets);
    FD_SET(server_socket, &current_sockets);

    while (lobby){
      printf("SERVER: main | Estamos en el lobby\n");
      ready_sockets = current_sockets;
      if (select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL) < 0){
        exit(-1);
      }

      for (i = 0; i < FD_SETSIZE; i ++){
        if (FD_ISSET(i, &ready_sockets)) {
          if (i == server_socket){
            // Player* player = malloc(sizeof(Player));
            game->players[number_clients] = malloc(sizeof(Player));
            game->players[number_clients]->socket = add_client(server_socket);
            FD_SET(game->players[number_clients]->socket, &current_sockets);
            strcpy(message, "Bienvenido al Juego!");
            server_send_message(game->players[number_clients]->socket, 1, message);

            if (number_clients == 0){
              game->players[0]->is_leader = true;
              strcpy(message, "Eres el primer jugador en conectarse, por lo que serás el jefe!");
              server_send_message(game->players[0]->socket, 1, message);
            } else {
              strcpy(message,  "------------- Alerta -------------");
              server_send_message(game->players[0]->socket, 1, message);
              strcpy(message,  "Se acaba de conectar un nuevo jugador. Está en en el lobby eligiendo su nombre y clase");
              server_send_message(game->players[0]->socket, 1, message);
              strcpy(message,  "----------- Fin Alerta -----------");
              server_send_message(game->players[0]->socket, 1, message);
            }

            strcpy(message, "Vamos a registrarnos!");
            server_send_message(game->players[number_clients]->socket, 2, message);
            number_clients ++;
          } else {

            // Primero debemos saber qué socket me habló
            int found = 0;
            for (int k = 0; k < MAX_N_PLAYERS; k++) {
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
                strcpy(game->players[found]->name, token);
                *name = *token;
                token = strtok(NULL, " ");
                if (found != 0) {
                  game->players[found]->is_leader = false;
                }
                // printf("SERVER: main | CLASS: %s\n", token);
                int class_preference = atoi(token);
                char class_preference_name[20];
                game->players[found] = create_new_player(game->players[found], class_preference);
                game->n_players ++;
                game->remaining_players ++;
                if (class_preference == 1){
                    strcpy(class_preference_name, "Cazador");
                } else if (class_preference == 2){
                    strcpy(class_preference_name, "Médico");
                } else {
                    strcpy(class_preference_name, "Hacker");
                }
                registered_clients ++;
                // printf("SERVER: main | El cliente %d dice: %s\n", i, client_message);
                strcpy(message, "Tu personaje fue creado! Esperando que se registren los demás jugadores ...");
                server_send_message(game->players[found]->socket, 1, message);
                if (found == 0){
                  strcpy(message, "Puedes iniciar el juego cuando desees");
                  server_send_message(game->players[found]->socket, 3, message);
                } else {
                  // Se notifica al lider de la inclusión 
                  strcpy(message,  "------------- Alerta -------------");
                  server_send_message(game->players[0]->socket, 1, message);
                  sprintf(message, "Nuevo jugador con nombre %s", game->players[found]->name);
                  server_send_message(game->players[0]->socket, 1, message);
                  sprintf(message, "Loggeo con clase %s", class_preference_name);
                  server_send_message(game->players[0]->socket, 1, message);
                  strcpy(message,  "----------- Fin Alerta -----------");
                  server_send_message(game->players[0]->socket, 1, message);
                }
                //free(class_preference_name);
                //free(token);
              }
              //free(client_message);
            }
            // Caso en que el usuario quiera empezar el juego
            if (msg_code == 3){
              char * client_message = server_receive_payload(i);
              printf("SERVER: main | El Jefe quiere comenzar el juego\n");
              if (number_clients == registered_clients) {
                printf("SERVER: main | Se procede a preguntarle al lider qué monstruo quiere utilizar!\n");
                strcpy(message, "¿Contra qué monstruo quieren luchar?");
                server_send_message(game->players[0]->socket, 1, message);
                strcpy(message, "1) Great JagRuz");
                server_send_message(game->players[0]->socket, 1, message);
                strcpy(message, "2) Ruzalos");
                server_send_message(game->players[0]->socket, 1, message);
                strcpy(message, "3) Ruiz, el Gemelo Malvado del Profesor Ruz");
                server_send_message(game->players[0]->socket, 1, message);
                strcpy(message, "4) Monstruo Random");
                server_send_message(game->players[0]->socket, 1, message);
                strcpy(message, "Ingrese el numero correspondiente al mounstro que quieren combatir:");
                server_send_message(game->players[0]->socket, 1, message);
                strcpy(message, "-");
                server_send_message(game->players[0]->socket, 40, message);
                printf("SERVER: main | Se recepciona la elección del monstruo\n");
                char * monster_selection_response = server_receive_payload(i);
                int monster = monster_selection_response[1] - '0';
                free(monster_selection_response);
                choose_monster(game, monster);
                printf("SERVER: main | Comienza el juego!\n");
                strcpy(message, "Comienza el juego!");
                for (i = 0; i < number_clients; i ++) {
                  server_send_message(game->players[i]->socket, 1, message);
                }   
                lobby = 0;
                break;
              } else {
                strcpy(message, "Hay jugadores que aun no declaran su nombre y clase! Intenta iniciar el juego en unos momentos más!\n\n");
                server_send_message(game->players[found]->socket, 3, message);
              }
                free(client_message);  
            }
          }
        }
      }
    }


    
    // Como no está inicializado el monstruo se cae al tratar de ver su current life
    while(game->remaining_players > 0 && game->monster->current_life > 0){
      printf("Nueva ronda\n");

      // Enviamos stats actualizadas a todos los jugadores y seteamos su estado "already_played" como falso.
      for (int turn = 0; turn < game->n_players; turn++) {
        if (!game->players[turn]->is_retired) {
          game->players[turn]->already_played = false;
          printf("Enviaré stats a player con turn = %i\n", turn);
          send_stats(game, game->players[turn % game->n_players]);
        }
      }
      
      // Primero juega el líder
      for (int turn = 0; turn < game->n_players; turn++) {
        if (!game->players[turn]->is_retired && game->players[turn]->is_leader){
          printf("SERVER: main | ------------ Turno de líder %s ----------------\n", game->players[turn]->name);
          turn_value = turn_choices(game, turn);
          game->players[turn]->already_played = true;
          break;
        }
      }

      // Luego juega el resto
      for (int turn = 0; turn < game->n_players; turn++){
          printf("SERVER: main | TURN: %i\n", turn);
        if (!game->players[turn]->is_retired && !game->players[turn]->already_played){
          printf("SERVER: main | ------------ Turno de jugador %s ----------------\n", game->players[turn]->name);
          turn_value = turn_choices(game, turn);
          game->players[turn]->already_played = true;
        }
      }
      if (game->remaining_players){
        printf("SERVER: main | ------------ Turno de monstruo ----------------\n");
        use_monster_skills(game);
      }
      /* Hacer algo con current_skill y current_target seteado en el jugador del turno*/
      if (game->remaining_players && game->monster->current_life) {
        update_round(game);
      }
      if (game->remaining_players == 0 ) {
        printf("SERVER: main | No quedan jugadores. Monstruo gano\n");
      }
      else if (!game->monster->current_life) {
        printf("SERVER: main | Felicidades! Han logrado vencer al monstruo\n");
      }
      game->rounds ++;
    }
    printf("SERVER: main | Terminó el juego.\n");
    /* Liberar memoria */
    for (int i = 0; i < game->n_players; i++){
      free(game->players[i]);
    }
    //free(IP);
    free(message);
    //free(clase);
    free(game->players);
    free(game->monster);
    free(game);
    printf("SERVER: main | Volver a jugar:\n1)Si\n2)No\n"); /*Aca mas que nada se deberia ver si se termina la conexion con el socket */
    scanf("SERVER: main | %i",&play);
    if (play != 1){
      close(server_socket);
      break;
    }
  }
  return 0;
}
