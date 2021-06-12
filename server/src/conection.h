#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "comunication.h"

//http://manpages.ubuntu.com/manpages/trusty/man7/sys_socket.h.7posix.html
//https://pubs.opengroup.org/onlinepubs/009695399/basedefs/sys/types.h.html
//https://pubs.opengroup.org/onlinepubs/009695399/basedefs/netinet/in.h.html
//https://pubs.opengroup.org/onlinepubs/7908799/xns/arpainet.h.html
//https://pubs.opengroup.org/onlinepubs/7908799/xns/netdb.h.html

typedef struct players_info{
  int *clients;
  int number_clients;
} PlayersInfo;

int prepare_socket(char * IP, int port);
PlayersInfo * get_team_leader(char server_socket);
int add_client(int server_socket);
