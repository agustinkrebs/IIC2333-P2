#include <unistd.h>
#include <stdio.h>
#include "conection.h"
#include "comunication.h"

char * get_input(){
  char * response = malloc(20);
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
  //Se obtiene la ip y el puerto donde está escuchando el servidor (la ip y puerto de este cliente da igual)
  char * IP = "0.0.0.0";
  int PORT = 8080;

  // Se prepara el socket
  int server_socket = prepare_socket(IP, PORT);

  // Se inicializa un loop para recibir todo tipo de paquetes y tomar una acción al respecto
  while (1){
    int msg_code = client_receive_id(server_socket);

    if (msg_code == 1) { //Recibimos un mensaje del servidor
      char * message = client_receive_payload(server_socket);
      printf("%s\n", message);
      free(message);
    }

    if (msg_code == 2) { //Recibimos un mensaje que proviene del otro cliente
      char * message = client_receive_payload(server_socket);
      printf("%s\n", message);
      free(message);

      printf("Ingrese su nombre:\n");
      char * name = get_input();

      printf("(1) Cazador\n");
      printf("(2) Medico \n");
      printf("(3) Hacker\n");
      printf("Ingrese el numero de la clase de su personaje:\n");
      char * class = get_input();
      char buf[256];
      snprintf(buf, sizeof(buf), "%s|%s", name, class);
      client_send_message(server_socket, 2, buf);
      free(class);
      free(name);
      free(buf);
    }

    if (msg_code == 3) { //Recibimos un mensaje que proviene del otro cliente
      char * message = client_receive_payload(server_socket);
      printf("%s\n", message);
      free(message);

      printf("Ingresa cualquier caracter para comenzar el juego:\n");
      char * x = get_input();
      client_send_message(server_socket, 3, x);
      free(message);
      free(x);
    }

    if (msg_code == 55) { //Recibimos un mensaje que proviene del otro cliente
      char * message = client_receive_payload(server_socket);
      printf("%s\n", message);
      free(message);

      printf("(1) Great JagRuz\n");
      printf("(2) Ruzalos \n");
      printf("(3) Ruiz, el Gemelo Malvado del Profesor Ruz\n");
      printf("(4) Mounstro Random\n");
      printf("Ingrese el numero correspondiente al mounstro que quieren combatir:\n");
      char * x = get_input();
      client_send_message(server_socket, 55, x);
      free(x);
    }

  }

  // Se cierra el socket
  close(server_socket);
  free(IP);

  return 0;
}
