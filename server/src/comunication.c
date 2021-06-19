#include "comunication.h"

//LINKS REFERENCIAS
//https://pubs.opengroup.org/onlinepubs/009695399/functions/recv.html
//https://pubs.opengroup.org/onlinepubs/009695399/functions/send.html

int server_receive_id(int client_socket){
  // Se obtiene solamente el ID del mensaje
  int id = 0;
  recv(client_socket, &id, 1, 0);
  // printf("SERVER: server_receive_id | Recibí código de mensaje: %i\n", id);
  return id;
}

char * server_receive_payload(int client_socket){
  // Se obtiene el largo del payload
  int len = 0;
  recv(client_socket, &len, 1, 0);
  // Se obtiene el payload
  char * payload = malloc(len);
  int received = recv(client_socket, payload, len, 0);
  // Se retorna
  // printf("SERVER: server_receive_payload | Recibí payload: %s\n", payload);
  return payload;
}

void server_send_message(int client_socket, int pkg_id, char * message){
  int payloadSize = strlen(message) + 1;
  //printf("payload size: %d\n", payloadSize);
  // Se arma el paquete
  char msg[1+1+payloadSize];
  msg[0] = pkg_id;
  msg[1] = payloadSize;
  memcpy(&msg[2], message, payloadSize);
  // Se envía el paquete
  send(client_socket, msg, 2+payloadSize, 0);
  // printf("SERVER: server_send_message | Envié código de mensaje: %i\n", pkg_id);
}
