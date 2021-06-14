#include <stdbool.h>

enum TypeMonster {JagRuz, Ruzalos, Ruiz};
enum TypePlayer {Hunter, Doctor, Hacker};


typedef struct player {
  char name[20];
  bool retired;
  int life;
  int type;
  int current_life;
  int turns_with_x2;
  bool is_reprobate;
  int turns_reprobate;
  int brute_force;
  int rounds_with_spine;
  int venom_damage;
  int current_skill;
  int current_target;
} Player;

typedef struct monster {
  int life;
  int type;
  int current_life;
  int n_of_stabs;
  int used_jump;
  int brute_force;
  int ddos;
  int ddos_counter;
  bool distracted;
  Player* player_distracted;
} Monster;
