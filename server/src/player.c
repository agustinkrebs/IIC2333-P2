#include "player.h"
#include <stdlib.h>

#define RAND_MAX = 1250;

void use_player_skills(Player* player);
void use_hounter_skills(Player* player);
void use_doctor_skills(Player* player);
void use_hackers_skills(Player* player);

void reduce_monster_life(Monster* monster, int hurt){
  if (monster->current_life - hurt < 0){
    monster->current_life = 0;
  } else {
    monster->current_life -= hurt;
  }
};

void update_player_life(Player* player, int value){
  if (value > 0){
    player->current_life += value;
    if (player->current_life > player->life){
      player->current_life = player->life;
    }
  } else {
    player->current_life -= value;
    if (player->current_life < 0){
      player->current_life = 0;
    }
  }
};


/*---Skills de Cazador---*/

/* Estocada */
void use_lunge(Player* player, Monster* monster){
  int hurt = 1000;
  if (player->turns_with_x2) {
    hurt = hurt * 2;
  }
  if (monster->n_of_stabs < 3){
    monster->n_of_stabs ++;
  }
  reduce_monster_life(monster, hurt);
};

/* Corte Cruzado */
void use_cross_cut(Player* player, Monster* monster){
  int hurt = 3000;
  if (player->turns_with_x2) {
    hurt = hurt * 2;
  }
  reduce_monster_life(monster, hurt);
};

/* Distraer */
void use_distract(Player* player, Monster* monster){
  monster->was_distracted = 1;
  // Aqui debiese ir la función que guarda al player como el último en distraer al monstruo.
};


/*---Skills de Médico---*/

/* Curar */
void use_heal(Player* player){
  update_player_life(player, 2000);
};

/* Destello Regenerador */
void use_regenerative_flash(Player* player1, Player* player2, Monster* monster){
  int hurt = random() + 750;
  if (player1->turns_with_x2) {
    hurt = hurt * 2;
  }
  reduce_monster_life(monster, hurt);
  int add_life = hurt/2 + hurt % 2;
  update_player_life(player2, add_life);
};

/* Descarga Vital */ 
void use_vital_discharge(Player* player, Monster* monster){
  int hurt = player->life - player->current_life;
  if (player->turns_with_x2) {
    hurt = hurt * 2;
  }
  reduce_monster_life(monster, hurt);
};


/*---Skills de Hacker---*/

/* Inyección SQL */
void use_sql_injection(Player* player){
  player->turns_with_x2 = 2;
};

/* Ataque DDOS */
void use_ddos_attack(Player* player, Monster* monster){
  int hurt = 1500;
  if (player->turns_with_x2) {
    hurt = hurt * 2;
  }
  reduce_monster_life(monster, hurt);
};

/* Fuerza Bruta */
void use_brute_force(Player* player, Monster* monster){
  player->brute_force ++;
  if (player->brute_force == 3){
      int hurt = 10000;
      if (player->turns_with_x2) {
        hurt = hurt * 2;
      }
      reduce_monster_life(monster, hurt);
      player->brute_force = 0;
  }
};
