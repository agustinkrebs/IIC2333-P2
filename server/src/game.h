#include "player.h"

typedef struct game {
  Player** players;
} Game;

int turn_choices(Game* game, int player_turn, int n_players);