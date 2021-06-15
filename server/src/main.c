#include <unistd.h>
#include <stdio.h>
#include "comunication.h"
#include "conection.h"
#include "game.h"
#include <sys/time.h>
#include <string.h>

int MAX_N_PLAYERS = 4;

//////////////////////////////////// CODIGOS DE ENVIO DEL SERVIDOR HACIA EL CLIENTE

/*
(1) Mensaje de texto (no espera respuesta)
(2) Ingresar datos personales de juego
(3) Tienes la facultad de comenzar el juego
(4) Líder: Recibir estado del resto de los jugadores
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
            Player* player = malloc(sizeof(Player));
            game->players[number_clients] = player;
            game->players[number_clients]->socket = add_client(server_socket);
            FD_SET(game->players[number_clients]->socket, &current_sockets);
            strcpy(message, "Bienvenido al Juego!\n");
            server_send_message(game->players[number_clients]->socket, 1, message);

            if (number_clients == 0){
              strcpy(message, "Eres el primer jugador en conectarse, por lo que serás el jefe!\n");
              server_send_message(game->players[0]->socket, 1, message);
            } else {
              strcpy(message,  "------------- Alerta ---------------- \nSe acaba de conectar un nuevo jugador\nEstá en en el lobby eligiendo su nombre y clase\n------------- Fin Alerta ---------------- \n");
              server_send_message(game->players[0]->socket, 1, message);
            }

            strcpy(message, "Vamos a registrarnos!\n");
            server_send_message(game->players[number_clients]->socket, 2, message);
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
                strcpy(game->players[found]->name, token);
                // *game->players[found]->name = *token;
                printf("SERVER: main | NAME: %s\n", game->players[found]->name);
                *name = *token;
                token = strtok(NULL, " ");
                printf("SERVER: main | CLASS: %s\n", token);
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
                printf("SERVER: main | El cliente %d dice: %s\n", i, client_message);
                strcpy(message, "Tu personaje fue creado! Esperando que se registren los demás jugadores ...\n");
                server_send_message(game->players[found]->socket, 1, message);
                if (found == 0){
                  strcpy(message, "Puedes iniciar el juego cuando desees\n");
                  server_send_message(game->players[found]->socket, 3, message);
                } else {
                  // Se notifica al lider de la inclusión 
                  strcpy(message, "------------- Alerta ---------------- \n");
                  server_send_message(game->players[0]->socket, 1, message);
                  strcpy(message, "Nuevo jugador con nombre");
                  server_send_message(game->players[0]->socket, 1, message);
                  server_send_message(game->players[0]->socket, 1, name);
                  strcpy(message, "Loggeo con clase");
                  server_send_message(game->players[0]->socket, 1, message);
                  server_send_message(game->players[0]->socket, 1, class_preference_name);
                  strcpy(message, "\n------------- Fin Alerta ---------------- \n");
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
                strcpy(message, "¿Contra qué monstruo quieren luchar? \n");
                server_send_message(game->players[0]->socket, 55, message);
              } else {
                strcpy(message, "Hay jugadores que aun no declaran su nombre y clase! Intenta iniciar el juego en unos momentos más!\n\n");
                server_send_message(game->players[found]->socket, 3, message);
              }
              //free(client_message);
            }

            if (msg_code == 55){
              // Se recibe la elección del monstruo
              char * client_message = server_receive_payload(i);
              printf("SERVER: main | Se recepciona la elección del monstruo\n");
              int monster = atoi(client_message);
              choose_monster(game, monster);
              printf("SERVER: main | Comienza el juego!\n");
              strcpy(message, "Comienza el juego!\n");
              for (i = 0; i < number_clients; i ++) {
                server_send_message(game->players[i]->socket, 1, message);
              }   
              //free(client_message);  
              lobby = 0;
              break;
            }
          }
        }
      }
    }
    printf("okkk\n");


    // Como no está inicializado el monstruo se cae al tratar de ver su current life
    while((game->remaining_players && game->monster->current_life) || !game->rounds){
      if (game->rounds){
        for (int turn = 0; turn < game->n_players; turn++){
          if (!game->players[turn]->retired){
            printf("SERVER: main | \n\n------------ Turno de jugador %s ----------------\n\n", game->players[turn]->name);
            turn_value = turn_choices(game, turn, MAX_N_PLAYERS);
          }
        }
        if (game->remaining_players){
          printf("SERVER: main | \n\n------------ Turno de monstruo ----------------\n\n");
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
      }
      game->rounds ++;
    }
    /* Liberar memoria */
    for (int i = 0; i < MAX_N_PLAYERS; i++){
      free(game->players[i]);
    }
    //free(IP);
    free(message);
    //free(name); 
    //free(clase);
    free(game->players);
    free(game->monster);
    free(game);
    printf("SERVER: main | Volver a jugar:\n1)Si\n2)No\n"); /*Aca mas que nada se deberia ver si se termina la conexion con el socket */
    scanf("SERVER: main | %i",&play);
    if (play != 1){
      break;
    }
  }
  return 0;
}
