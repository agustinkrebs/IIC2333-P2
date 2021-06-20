#include "methods.h"

typedef struct game {
  Player** players;
  Monster* monster;
  int n_players;
  int remaining_players;
  int players_connected;
  int rounds;
} Game;

Player* create_new_player(Player* player, int class);
int turn_choices(Game* game, int player_turn);
void choose_monster(Game* game, int selection);
void choose_player_type(Player* player);
void use_skills(Player* player, Game* game);
void use_monster_skills(Game* game);
void update_round(Game* game);
void send_stats(Game* game, Player* player);
Player* get_random_player(Game* game);
void send_last_action_to_clients(Game* game, char* message);