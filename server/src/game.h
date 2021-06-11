#include "methods.h"

typedef struct game {
  Player** players;
  Monster* monster;
  int n_players;
  int remaining_players;
  int players_connected;
  int rounds;
} Game;

Player* create_new_player();
int turn_choices(Game* game, int player_turn, int n_players);
void choose_monster(Game* game, int selection);
void choose_player_type(Player* player);
void use_skills(Player* player, Game* game);
void use_monster_skills(Game* game);
void update_round(Game* game);