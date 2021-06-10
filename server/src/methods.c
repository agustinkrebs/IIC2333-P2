#include "methods.h"
#include <stdlib.h>

/*---PLAYER---*/

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
    if (player->is_reprobate){
      value = value * 1.5;
    }
    player->current_life -= value;
    if (player->current_life < 0){
      player->current_life = 0;
    }
  }
};

int get_damage(Player* player, int value){
  if (player->is_reprobate){
    value = value / 2;
  }
  if (player->turns_with_x2){
    value = value * 2;
  }
  return value;
}

/*---Skills de Cazador---*/

/* Estocada */
void use_lunge(Player* player, Monster* monster){
  int hurt = get_damage(player, 1000);
  if (monster->n_of_stabs < 3){
    monster->n_of_stabs ++;
  }
  reduce_monster_life(monster, hurt);
};

/* Corte Cruzado */
void use_cross_cut(Player* player, Monster* monster){
  int hurt = get_damage(player, 3000);
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
  hurt = get_damage(player1, hurt);
  reduce_monster_life(monster, hurt);
  int add_life = hurt/2 + hurt % 2;
  update_player_life(player2, add_life);
};

/* Descarga Vital */ 
void use_vital_discharge(Player* player, Monster* monster){
  int hurt = player->life - player->current_life;
  hurt = get_damage(player, hurt);
  reduce_monster_life(monster, hurt);
};


/*---Skills de Hacker---*/

/* Inyección SQL */
void use_sql_injection(Player* player){
  player->turns_with_x2 = 2;
};

/* Ataque DDOS */
void use_ddos_attack(Player* player, Monster* monster){
  int hurt = get_damage(player, 1500);
  reduce_monster_life(monster, hurt);
};

/* Fuerza Bruta */
void use_brute_force(Player* player, Monster* monster){
  player->brute_force ++;
  if (player->brute_force == 3){
      int hurt = get_damage(player, 10000);
      reduce_monster_life(monster, hurt);
      player->brute_force = 0;
  }
};

/*---MONSTER---*/

void use_monster_skills(Monster* monster);
void use_jagruz_skills(Monster* monster);
void use_ruzalos_skills(Monster* monster);
void use_ruiz_skilss(Monster* monster);


/*---Skills de JagRuz---*/

/* Ruzgar */
void use_ruzgar(Player* player){
    update_player_life(player, -1000);
};

/* Coletazo */
void use_coletazo(){
    // Hacer un for.
};

/*---Skills de Ruzalos---*/

/* Salto */
void use_jump(Monster* monster, Player* player){
    update_player_life(player, -1500);
    monster->used_jump = 1;
};

/* Espina Venenosa */
void use_poisonous_thorn(Monster* monster, Player* player){
    monster->used_jump = 0;
    int hurt;
    if (player->rounds_with_spine){
        hurt = 500;
    } else {
        hurt = 400;
    }
    player->rounds_with_spine = 3;
    update_player_life(player, -hurt);
};

/*---Ruiz---*/

/* Caso Copia */
void use_copy_case(Monster* monster, Player* player_copy, Player* player_paste){

};

/* Reprobaton-9000 */
void use_reprobaton_9000(Player* player){
    player->is_reprobate = 1;
};

/* sudo rm -rf */
void use_sudo_rm();