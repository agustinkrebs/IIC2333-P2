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
  char IP[20];
  strcpy(IP, argv[2]);
  int PORT = atoi(argv[4]);
  // char * IP = "0.0.0.0";
  // int PORT = 8080;

  printf("CLIENT: main | ip_address: %s\n", IP);
  printf("CLIENT: main | tcp_port: %i\n", PORT);


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

    if (msg_code == 99) { 
      printf("CLIENT: main | ¡Comenzó tu turno!\n");
      printf("CLIENT: main | ¿Deseas rendirte?\n");
      printf("CLIENT: main | Presiona -1 si quieres rendirte. Cualquier otro número en caso contrario\n");
      char * response = get_input();
      client_send_message(server_socket, 98, response); // el número no importa
      if (response[0] == '-' && response[1] == '1') {
        break;
      }
      free(response);
    }

    if (msg_code == 40) { 
      char * response = get_input();
      client_send_message(server_socket, 94, response); // el número no importa
      free(response);
    }

    if (msg_code == 30) { 
      printf("¿Quieres seguir jugando?\n(1) Sí\n(2) No\n");
      char * response = get_input();
      printf("Enviando la respuesta\n");
      client_send_message(server_socket, 94, response); // el número no importa
      free(response);
    }

    if (msg_code == 69) { 
      char * message = client_receive_payload(server_socket);
      printf("CLIENT: main | Adios y que te vaya bien!\n");
      break;
    }
    
    
  }

  // Se cierra el socket
  close(server_socket);
  //free(IP);

  return 0;
}
