#include <unistd.h>
#include <stdio.h>
#include "comunication.h"
#include "conection.h"
#include "game.h"

int N_PLAYERS = 2;

int main(int argc, char *argv[]){
  int turn = 0;
  int turn_value;
  Game* game = malloc(sizeof(Game));
  game->players = malloc(N_PLAYERS*sizeof(Player));
  game->n_players = N_PLAYERS;
  game->remaining_players = N_PLAYERS;
  game->players_connected = 0;
  game->rounds = 0;
  game->monster = malloc(sizeof(Monster));
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
