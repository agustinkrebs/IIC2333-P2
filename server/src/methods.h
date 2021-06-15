#include "classes.h"
#include <stdio.h>

int generate_random (int lower, int upper);

/*---PLAYER---*/

void use_player_skills(Player* player);
void use_hounter_skills(Player* player);
void use_doctor_skills(Player* player);
void use_hackers_skills(Player* player);

void reduce_monster_life(Monster* monster, int hurt);
void increase_monster_life(Monster* monster, int value);
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


/*---MONSTERS---*/

void use_jagruz_skills(Monster* monster);
void use_ruzalos_skills(Monster* monster);
void use_ruiz_skilss(Monster* monster);

/*---Skills de JagRuz---*/

/* Ruzgar */
void use_ruzgar(Player* player);

/* Coletazo */
void use_coletazo(Player* player);

/*---Skills de Ruzalos---*/

/* Salto */
void use_jump(Monster* monster, Player* player);

/* Espina Venenosa */
void use_poisonous_thorn(Monster* monster, Player* player);

/*---Ruiz---*/

/* Caso Copia */
void use_copy_case(Monster* monster, Player* player, int rounds);

/* Reprobaton-9000 */
void use_reprobaton_9000(Monster* monster, Player* player);

/* sudo rm -rf */
void use_sudo_rm(Monster* monster, Player** players, int n_players, int rounds);
