#include <unistd.h>
#include <stdio.h>
#include "conection.h"
#include "comunication.h"

char * get_input(){
  char * response = malloc(255);
  int pos=0;
  while (1){
    char c = getchar();
    if (c == '\n') break;
    response[pos] = c;
    pos++;
  }
  response[pos] = '\0';
  return response;
}


int main (int argc, char *argv[]){
  // Ejemplo de input de consola: ./client -i 0.0.0.0 -p 8080

  //Se obtiene la ip y el puerto donde está escuchando el servidor (la ip y puerto de este cliente da igual)
  // char * IP = argv[2];
  // int PORT = atoi(argv[4]);
  char * IP = "0.0.0.0";
  int PORT = 8080;

  printf("CLIENT: main | ip_address: %s\n", argv[2]);
  printf("CLIENT: main | tcp_port: %s\n", argv[4]);

  // Se prepara el socket
  int server_socket = prepare_socket(IP, PORT);

  // Se inicializa un loop para recibir todo tipo de paquetes y tomar una acción al respecto
  while (1){
    int msg_code = client_receive_id(server_socket);

    if (msg_code == 1) { //Recibimos un mensaje del servidor
      char * message = client_receive_payload(server_socket);
      printf("CLIENT: main | %s\n", message);
      //free(message);
    }

    if (msg_code == 2) { 
      char * message = client_receive_payload(server_socket);
      printf("CLIENT: main | %s\n", message);
      //free(message);

      printf("CLIENT: main | Ingrese su nombre:\n");
      char * name = get_input();

      printf("CLIENT: main | (1) Cazador\n");
      printf("CLIENT: main | (2) Medico \n");
      printf("CLIENT: main | (3) Hacker\n");
      printf("CLIENT: main | Ingrese el numero de la clase de su personaje:\n");
      char * class = get_input();
      char buf[256];
      snprintf(buf, sizeof(buf), "%s|%s", name, class);
      client_send_message(server_socket, 2, buf);
      //free(class);
      //free(name);
    }

    if (msg_code == 3) {
      char * message = client_receive_payload(server_socket);
      printf("CLIENT: main | %s\n", message);
      //free(message);

      printf("CLIENT: main | Ingresa cualquier caracter para comenzar el juego:\n");
      char * x = get_input();
      client_send_message(server_socket, 3, x);
      //free(message);
      //free(x);
    }

    if (msg_code == 4) {
      char * message = client_receive_payload(server_socket);
      int n_players = atoi(message);
      printf("CLIENT: main | ---Situación de los Jugadores---\n");
      /*
      TODO: Imprimir esta información
      for (int j = 0; j < n_players; j++) {
          Player* player_list = game->players[j];
          if (j == 0) {
              printf("Lider-%s[%i] -> Vida %i / %i\n", player_list->name, player_list->type, player_list->current_life, player_list->life);
          }
          else {
              printf("%s[%i] -> Vida %i / %i\n", player_list->name, player_list->type, player_list->current_life, player_list->life);
          }
      }
      */
      printf("CLIENT: main | ---Situación del monstruo---\n");
      // printf("Monstruo -> Vida %i / %i\n", game->monster->current_life, game->monster->life);
      printf("CLIENT: main | -----------------------------------\n");
      //free(message);
    }

    if (msg_code == 55) {
      char * message = client_receive_payload(server_socket);
      printf("CLIENT: main | %s\n", message);
      //free(message);

      printf("CLIENT: main | (1) Great JagRuz\n");
      printf("CLIENT: main | (2) Ruzalos \n");
      printf("CLIENT: main | (3) Ruiz, el Gemelo Malvado del Profesor Ruz\n");
      printf("CLIENT: main | (4) Mounstro Random\n");
      printf("CLIENT: main | Ingrese el numero correspondiente al mounstro que quieren combatir:\n");
      char * response = get_input();
      client_send_message(server_socket, 55, response);
      //free(response);
    }

    if (msg_code == 99) { 
      printf("CLIENT: main | ¿Deseas rendirte?\n");
      printf("CLIENT: main | Presiona -1 si quieres rendirte. Cualquier otro número en caso contrario\n");
      char * response = get_input();
      client_send_message(server_socket, 98, response);
      //free(response);
    }

    if (msg_code == 97) { 
      char * message = client_receive_payload(server_socket);
      printf("CLIENT: main | ---Elegir Habilidad---\n");
      printf("CLIENT: main | %s", message);
      //free(message);
      char * response = get_input();
      client_send_message(server_socket, 96, response);
      //free(response);
    }

    if (msg_code == 95) { 
      char * message = client_receive_payload(server_socket);
      int n_players = atoi(message);
      printf("CLIENT: main | ---Elegir Objetivo---\n"); /* Hay que cachar si las habilidades se puede aplicar a uno mismo*/
      for (int j = 0; j < n_players; j++){
          printf("CLIENT: main | %i) Jugador %i\n", j + 1, j +1);
      }
      //free(message);
      char * response = get_input();
      client_send_message(server_socket, 94, response);
      //free(response);
    }
  }

  // Se cierra el socket
  close(server_socket);
  //free(IP);

  return 0;
}
