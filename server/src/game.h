#include "player.h"

typedef struct game {
  Player** players;
  Monster* monster;
  int n_players;
  int players_connected;
} Game;

int turn_choices(Game* game, int player_turn, int n_players);