#include "methods.h"
#include <stdlib.h>

/*---PLAYER---*/

void use_player_skills(Player* player);
void use_hounter_skills(Player* player);
void use_doctor_skills(Player* player);
void use_hackers_skills(Player* player);

int generate_random (int lower, int upper) {
  if (upper - lower <= 0){
    return lower;
  }
  int random = (rand()) % (upper - lower);
  int number = random + lower;
  return number;
}

void reduce_monster_life(Monster* monster, int hurt){
  printf("La vida del monstruo es %i pero ha sido atacado con un poder que disminuye %i\n", monster->current_life, hurt);
  if (monster->current_life - hurt < 0){
    monster->current_life = 0;
  } else {
    monster->current_life -= hurt;
  }
  printf("El monstruo queda con %i de vida\n", monster->current_life);
};

void increase_monster_life(Monster* monster, int value){
  if (monster->current_life + value >= monster->life){
    monster->current_life = monster->life;
  } else {
    monster->current_life += value;
  }
}

void monster_check_ddos(Monster* monster){
  if (monster->ddos_counter){
    monster->ddos_counter --;
    if (!monster->ddos_counter){
      monster->ddos = 0;
    }
  }
}

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
    player->current_life += value;
    if (player->current_life < 0){
      player->current_life = 0;
      player->retired = true;
      printf("Jugador %s ha sido eliminado\n", player->name);
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
  //en este caso estocada retorna exito si el mountro todavia no tiene estocadas, sino fracaso para que el jugador pueda seleccionar otra habilidad.
  int hurt = get_damage(player, 1000);
  reduce_monster_life(monster, hurt);
  printf("%s le infringe una estocado al mounstro\n", player->name);
  if (monster->n_of_stabs < 3){
    monster->n_of_stabs ++;
    printf("Mounstro queda con %i estocadas\n", monster->n_of_stabs);
  }
  else {
    printf("Mounstro ya tenía 3 estocadas\n");
  }
};

/* Corte Cruzado */
void use_cross_cut(Player* player, Monster* monster){
  int hurt = get_damage(player, 3000);
  reduce_monster_life(monster, hurt);
  printf("%s le infringe un corte cruzado al mounstro\n", player->name);
};

/* Distraer */
void use_distract(Player* player, Monster* monster){
  monster->distracted = true;
  monster->player_distracted = player;
  printf("%s distrae al mounstro\n", player->name);
};


/*---Skills de Médico---*/

/* Curar */
void use_heal(Player* player){
  printf(" %s, recuperando 2000 de vida\n", player->name);
  update_player_life(player, 2000);
};

/* Destello Regenerador */
void use_regenerative_flash(Player* player1, Player* player2, Monster* monster){
  int hurt = generate_random(750, 2000);
  hurt = get_damage(player1, hurt);
  reduce_monster_life(monster, hurt);
  int add_life = hurt/2 + hurt % 2;
  update_player_life(player2, add_life);
  printf("%s recupera %i de vida\n", player2->name, add_life);
};

/* Descarga Vital */ 
void use_vital_discharge(Player* player, Monster* monster){
  int hurt = (player->life - player->current_life) * 2;
  hurt = get_damage(player, hurt);
  reduce_monster_life(monster, hurt);
};


/*---Skills de Hacker---*/

/* Inyección SQL */
void use_sql_injection(Player* player){
  printf("Inyección de sql sobre %s\n", player->name);
  player->turns_with_x2 = 2;
};

/* Ataque DDOS */
void use_ddos_attack(Player* player, Monster* monster){
  int hurt = get_damage(player, 1500);
  printf("%s realiza un ataque DDOS realizando %i de daño\n", player->name, hurt);
  reduce_monster_life(monster, hurt);
};

/* Fuerza Bruta */
void use_brute_force(Player* player, Monster* monster){
  player->brute_force ++;
  printf("%s utiliza su fuerza bruta por %i vez\n", player->name, player->brute_force);
  if (player->brute_force == 3){
      printf("%s realiza 10000 de daño\n", player->name);
      int hurt = get_damage(player, 10000);
      reduce_monster_life(monster, hurt);
      player->brute_force = 0;
  }
};

/*---MONSTER---*/

void use_jagruz_skills(Monster* monster);
void use_ruzalos_skills(Monster* monster);
void use_ruiz_skilss(Monster* monster);


/*---Skills de JagRuz---*/

/* Ruzgar */
void use_ruzgar(Player* player){
  printf("RUZGAAAAAR sobre %s\n", player->name);
  update_player_life(player, -1000);
};

/* Coletazo */
void use_coletazo(Player* player){
  printf("COLETAZO sobre %s\n", player->name);
  update_player_life(player, -500);
};

/*---Skills de Ruzalos---*/

/* Salto */
void use_jump(Monster* monster, Player* player){
  printf("JUUUUUMP sobre %s\n", player->name);
  update_player_life(player, -1500);
  monster->used_jump = 1;
};

/* Espina Venenosa */
void use_poisonous_thorn(Monster* monster, Player* player){
  printf("ESPINA VENENOSA sobre %s\n", player->name);
  monster->used_jump = 0;
  int hurt;
  if (player->rounds_with_spine){
      player->venom_damage = 500;
  } else {
      player->venom_damage = 400;
  }
  player->rounds_with_spine = 3;
};

/*---Ruiz---*/

/* Caso Copia */
void use_copy_case(Monster* monster, Player* player, int rounds){
  int attack;
  printf("COPY CASE sobre %s\n", player->name);
  int random = generate_random(0,8);
  if (random == 0){/* Estocada TESTED*/
    monster->blood ++;
    if (monster->blood <=3){
      player->blood ++;
      printf("%s sangra %i\n",player->name,player->blood);
    }
    attack = -1000;
    attack = monster->ddos ? - attack * 2 : - attack;
    printf("Monstruo aplica una ESTOCADA a %s con daño %i\n", player->name, attack);
    update_player_life(player, attack + player->blood * 500); 
  } else if (random == 1){/* Corte Cruzado TESTED*/
    attack  = monster->ddos ? -3000 * 2 : -3000;
    printf("Monstruo aplica un CORTE CRUZADO a %s con daño %i\n", player->name, attack);
    update_player_life(player, attack + player->blood * 500);
  } else if  (random == 2){/* Curar TESTED*/
    printf("Monstruo aplica CURAR y aumenta su vida en 2000\n");
    increase_monster_life(monster, 2000);
  } else if (random == 3){/* Destello Regenerador TESTED*/
    random = generate_random(750, 2000);
    attack = monster->ddos ? - random * 2: - random;
    printf("Monstruo aplica un DESTELLO REGENERADOR a %s con daño %i y aumenta su vida en %i\n",player->name, attack, (int) random/2 + random % 2);
    update_player_life(player, attack + player->blood * 500);
    increase_monster_life(monster, (int) random/2 + random % 2);
  } else if (random == 4){/* Descarga Vital TESTED*/
    attack = monster->ddos ? -2 * (monster->life - monster->current_life) * 2 : -2 * (monster->life - monster->current_life);
    printf("Monstruo aplica una DESCARGA VITAL a %s con daño %i\n",player->name, attack);
    update_player_life(player, attack + player->blood * 500);
  } else if (random == 5){/* Inyeccion SQL TESTED*/
    printf("Monstruo aplica una INYECCIÓN SQL\n");
    monster->ddos_counter = 2;
    monster->ddos = 1;
  } else if (random == 6){/* Ataque DDOS TESTED*/
    attack = monster->ddos ?  -1500 * 2: - 1500;
    printf("Monstruo aplica un ATAQUE DDOS a %s con daño %i\n",player->name, attack);
    update_player_life(player, attack + player->blood * 500);
  } else if (random == 7){/* Fuerza Bruta TESTED*/
    printf("Monstruo aplica FUERZA BRUTA\n");
    monster->brute_force ++;
    if (monster->brute_force == 3){
      attack = monster->ddos ? - 10000 * 2 : - 10000;
      printf("Monstruo aplica esta habilidad por tercera vez con un daño de %i a %s\n",attack, player->name);
      update_player_life(player, attack + player->blood * 500);
      monster->brute_force = 0;
    }
  } else {/* Distraer TESTED*/
    printf("Monstruo aplica DISTRAER (NO HACE NADA)\n");
  }
  monster_check_ddos(monster);
};

/* Reprobaton-9000 */
void use_reprobaton_9000(Monster* monster, Player* player){
  printf("REPROBATON sobre %s\n", player->name);
  player->is_reprobate = true;
  monster_check_ddos(monster);
};

/* sudo rm -rf */
void use_sudo_rm(Monster* monster, Player** players, int n_players, int rounds){
  int attack = monster->ddos ? -100 * rounds * 2 : -100 * rounds;
  printf("SUDO RM sobre todos los jugadores con daño de %i\n", attack);
  for (int i = 0; i < n_players; i++){
    Player* player = players[i];
    if (!player->retired){
      update_player_life(player, attack + player->blood * 500);
    }
  }
  monster_check_ddos(monster);
};