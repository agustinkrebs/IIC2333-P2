#include "player.h"

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