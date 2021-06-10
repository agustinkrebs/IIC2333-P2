#include "methods.h"

typedef struct game {
  Player** players;
  Monster* monster;
} Game;

int turn_choices(Game* game, int player_turn, int n_players);

void choose_monster(Game* game, int selection);
void choose_player_type(Player* player);