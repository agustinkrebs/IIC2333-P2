#include "monster.h"

enum TypePlayer {Hunter, Doctor, Hacker};

typedef struct player {
  int life;
  int type;
  int current_life;
  int turns_with_x2;
  int is_reprobate;
  int brute_force;
} Player;

void use_player_skills(Player* player);
void use_hounter_skills(Player* player);
void use_doctor_skills(Player* player);
void use_hackers_skills(Player* player);

void reduce_monster_life(Monster* monster, int hurt);
void update_player_life(Player* player, int value);

/*---Skills de Cazador---*/

/* Estocada */
void use_lunge(Player* player, Monster* monster);

/* Corte Cruzado */
void use_cross_cut(Player* player, Monster* monster);

/* Distraer */
void use_distract(Player* player, Monster* monster);


/*---Skills de Médico---*/

/* Curar */
void use_heal(Player* player);

/* Destello Regenerador */
void use_regenerative_flash(Player* player1, Player* player2, Monster* monster);

/* Descarga Vital */ 
void use_vital_discharge(Player* player, Monster* monster);


/*---Skills de Hacker---*/

/* Inyección SQL */
void use_sql_injection(Player* player);

/* Ataque DDOS */
void use_ddos_attack(Player* player, Monster* monster);

/* Fuerza Bruta */
void use_brute_force(Player* player, Monster* monster);
