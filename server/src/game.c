#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h> 
#include <unistd.h>     /* Symbolic Constants */
#include <sys/types.h>  /* Primitive System Data Types */ 
#include <stdlib.h>     /* General Utilities */
#include "game.h"
#include "methods.h"
#include "classes.h"

int player_life[] = {5000, 3000, 2500};
int monster_life[] = {10000, 20000, 25000};

void choose_monster(Game* game, int selection){
    Monster* monster = malloc(sizeof(Monster));
    if (selection == 1){
        monster->type = JagRuz;
        printf("\nHas seleccionado al monstruo JagRuz con %i de vida\n\n", monster_life[selection - 1]);
    } else if (selection == 2){
        monster->type = Ruzalos;
        printf("\nHas seleccionado al monstruo Ruzalo con %i de vida\n\n", monster_life[selection - 1]);
    } else if (selection == 3){
        monster->type = Ruiz;
        printf("\nHas seleccionado al monstruo Ruiz con %i de vida\n\n", monster_life[selection - 1]);
    }
    monster->life = monster_life[selection - 1];
    monster->current_life = monster_life[selection - 1];
    monster->n_of_stabs = 0;
    monster->used_jump = 0;
    monster->was_distracted = 0;
    game->monster = monster;
};

//ESTA FUNCION LA PODEMOS SACAR
void choose_player_type(Player* player){
    int selection;
    printf("---Inicio de Juego---\n");
    printf("Tipos:\n");
    printf("1) Cazador\n");
    printf("2) Médico\n");
    printf("3) Hacker\n");
    printf("Selecciona tu personaje:\n");
    scanf("%i", &selection);
    if (selection == 1){
        player->type = Hunter;
        printf("\nHas seleccionado Cazador con %i de vida\n\n", player_life[selection - 1]);
    } else if (selection == 2){
        player->type = Ruzalos;
        printf("\nHas seleccionado Médico con %i de vida\n\n", player_life[selection - 1]);
    } else if (selection == 3){
        player->type = Ruiz;
        printf("\nHas seleccionado Hacker con %i de vida\n\n", player_life[selection - 1]);
    }
    player->life = player_life[selection - 1];
    player->brute_force = 0;
    player->current_life = player_life[selection - 1];
    player->current_skill = 0;
    player->current_target = 0;
    player->is_reprobate = 0;
    player->rounds_with_spine = 0;
    player->turns_with_x2 = 0;
} 

Player* create_new_player() {
    Player* player = malloc(sizeof(Player));
    int class;
    printf("--- Bienvenido a Monster Hunt Ruz! --- \n");
    printf("Introduzca su nombre: ");
    scanf("%s", player->name);
    printf("%s estas son las clases disponibles:\n", player->name);
    printf("1) Cazador\n");
    printf("2) Medico\n");
    printf("3) Hacker\n");
    printf("Selecciona la clase con la que quieres jugar: \n");
    scanf("%i", class);
    player->type = class - 1;
    if (player->type == Hunter) {
        player->life = 5000;
        player->current_life = 5000;
    }
    else if (player->type == Doctor) {
        player->life = 3000;
        player->current_life = 3000;
    }
    else {
        player->life = 2500;
        player->current_life = 2500;
    }
    player->retired = false;
    player->is_reprobate = 0;
    player->brute_force = 0;
    player->turns_with_x2 = 0;
    player->rounds_with_spine = 0;
    player->current_skill = -1;
    player->current_target = -1;
    return player;
}



int turn_choices(Game* game, int player_turn, int n_players){
    /*
    1) Si player_turn es 0, primer turno de todo el juego se retorna monstruo seleccionado
    2) Si es lider se imprime situacion de los jugadores ( no estoy seguro si es para el lder o todos)
    3) Se pregunta si se desea render, se retorna -1 en ese caso
    4) Se elige habilidad y se setea valor en current_skill
    5) Se elige  Objetivo si corresponde ( si no es el monstruo) y se setea valor en current_target (ver si current_target puede ser uno mismo)
    6) se retorna cero
    */
    int i;
    if (!player_turn) {
        printf("---Inicio de Juego---\n");
        printf("Lider estos son los monstruos con los que puedes combatir:\n");
        printf("1) Great JagRuz\n");
        printf("2) Ruzalos\n");
        printf("3) Ruiz, el Gemelo Malvado del Profesor Ruz\n");
        printf("4) Aleatorio\n");
        printf("Lider selecciona el monstruo con el que deseas combatir:\n");
        scanf("%i",&i);
        return i;
    } else {
        if (!(player_turn % n_players)) {
            printf("---Situación de los Jugadores---\n");
            for (int j = 0; j < n_players; j++) {
                Player* player_list = game->players[j];
                if (!j) {
                    printf("Lider-%s[%i] -> Vida %i / %i\n", player_list->name, player_list->type, player_list->current_life, player_list->life);
                }
                printf("%s[%i] -> Vida %i / %i\n", player_list->name, player_list->type, player_list->current_life, player_list->life);
            }
        }
        Player* player = game->players[player_turn % n_players];
        if (player->current_life > 0 && !player->retired) {
            printf("Turno de %s \n", player->name);
            printf("¿ Deseas rendirte ?");
            printf("Presiona -1 si quieres rendirte");
            scanf("%i",&i);
            if (i == -1) {
                player->retired = true;
                printf("%s se ha retirado del juego", player->name);
                return i;
            };
            printf("---Elegir Habilidad---\n");
            if (player->type == Hunter){
                printf("1) Estocada\n2) Corte Cruzado\n3) Distraer\n");
            } else if (player->type == Doctor){
                printf("1) Curar\n2) Destello Regenerador\n3) Descarga Vital\n");
            } else if (player->type == Hacker){
                printf("1) Inyección SQL\n2) Ataque DDOS\n3)Fuerza Bruta");
            }
            scanf("%i",&i);
            player->current_skill = i - 1;
            if (player->type == Doctor){
                if (player->current_skill == 0){
                    printf("---Elegir Objetivo---\n"); /* Hay que cachar si las habilidades se puede aplicar a uno mismo*/
                    for (int j = 0; j < n_players;j++){
                        printf("%i) Jugador %i", j + 1, j +1);
                    }
                    scanf("%i",&i);
                    player->current_target = i;
                }
                else if (player->current_skill == 1){
                    //elegir jugador al azar
                    int index_player = player_turn;
                    while (index_player == player_turn); {//numero random distinto al actual
                        int index_player = generate_random(1, 4);
                    }   
                    player->current_target = index_player;
                }
            } else if (player->type == Hacker){
                if(player->current_skill == 0){
                    pritnf("---Elegir Objetivo---\n"); /* Hay que cachar si las habilidades se puede aplicar a uno mismo*/
                    for (int j = 0; j < n_players;j++){
                        printf("%i) Jugador %i", j + 1, j +1);
                    }
                    scanf("%i",&i);
                    player->current_target = i;
                }
            }
            use_skills(player, game);
        }

    }
    return 0;
}

void use_skills(Player* player, Game* game){
    if (player->type == Hunter){
        if (player->current_skill == 0) {
            use_lunge(player, game->monster);
        }
        else if (player->current_skill == 1) {
            use_cross_cut(player, game->monster);
        }
        else {
            use_distract(player, game->monster);
        }
    }
    else if (player->type == Doctor) {
        if (player->current_skill == 0 ) {
            printf("%s cura a", player->name);
            use_heal(game->players[player->current_target]);

        }
        else if (player->current_skill == 1) {
            printf("%s utiliza el rayo regenerador. ", player->name);
            use_regenerative_flash(player, game->players[player->current_target], game->monster);
        }
        else {
            use_vital_discharge(player, game->monster);
        }
    }
    else {
        if (player->current_skill == 0) {
            use_sql_injection(game->players[player->current_target]);
        }
        else if (player->current_skill == 1) {
            use_ddos_attack(player, game->monster);
        }
        else {
            use_brute_force(player, game->monster);
        }  
    }
}