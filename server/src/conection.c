#include "conection.h"

//LINKS REFERENCIAS:
//https://www.man7.org/linux/man-pages/man2/socket.2.html
//https://man7.org/linux/man-pages/man7/socket.7.html
//https://www.man7.org/linux/man-pages/man3/setsockopt.3p.html
//https://man7.org/linux/man-pages/man2/setsockopt.2.html
//https://linux.die.net/man/3/htons
//https://linux.die.net/man/3/inet_aton
//https://www.howtogeek.com/225487/what-is-the-difference-between-127.0.0.1-and-0.0.0.0/
//https://www.man7.org/linux/man-pages/man2/bind.2.html
//https://www.man7.org/linux/man-pages/man2/accept.2.html


int prepare_socket(char * IP, int port) {

  // Se define la estructura para almacenar info del socket del servidor al momento de su creación
  struct sockaddr_in server_addr;

  // Se solicita un socket al SO, que se usará para escuchar conexiones entrantes
  int server_socket = socket(AF_INET, SOCK_STREAM, 0);

  // Se configura el socket a gusto (recomiendo fuertemente el REUSEPORT!)
  int opt = 1;
  int ret = setsockopt(server_socket, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));

  // Se guardan el puerto e IP en la estructura antes definida
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  inet_aton(IP, &server_addr.sin_addr);
  server_addr.sin_port = htons(port);

  // Se le asigna al socket del servidor un puerto y una IP donde escuchar
  int ret2 = bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
  return server_socket;

}

PlayersInfo * get_team_leader(char server_socket){
  

  // Se coloca el socket en modo listening
  printf("get_team_leader | Empezaré a escuchar a un líder\n");
  int ret3 = listen(server_socket, 1);

  // Se definen las estructuras para almacenar info sobre los sockets de los clientes
  struct sockaddr_in client1_addr;
  socklen_t addr_size = sizeof(client1_addr);
  // Se inicializa una estructura propia para guardar los n°s de sockets de los clientes.
  PlayersInfo * sockets_clients = malloc(sizeof(PlayersInfo));

  // Se crea un arreglo en donde se guardarán a los clientes
  int arr[4];
  sockets_clients->clients = arr;

  // Se aceptan a al primer cliente que llega. "accept" retorna el n° de otro socket asignado para la comunicación
  sockets_clients->clients[0] = accept(server_socket, (struct sockaddr *)&client1_addr, &addr_size);
  sockets_clients->number_clients = 1;


  return sockets_clients;
}


int add_client(int server_socket){
  
  int client;

  // Se coloca el socket en modo listening
  int ret3 = listen(server_socket, 1);

  // Se definen las estructuras para almacenar info sobre los sockets de los clientes
  struct sockaddr_in client_addr;
  socklen_t addr_size = sizeof(client_addr);

  // Se aceptan a los primeros 2 clientes que lleguen. "accept" retorna el n° de otro socket asignado para la comunicación
  client = accept(server_socket, (struct sockaddr *)&client_addr, &addr_size);
  printf("add_client | Se ha conectado un cliente de forma existosa\n");

  return client;
}
