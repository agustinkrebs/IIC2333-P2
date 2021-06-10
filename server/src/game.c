#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h> 
#include <unistd.h>     /* Symbolic Constants */
#include <sys/types.h>  /* Primitive System Data Types */ 
#include <stdlib.h>     /* General Utilities */
#include "game.h"
#include "monster.h"
#include "player.c"

void main() {
    Game* game = create_lobby(4);
    for (int i = 0; i < 4; i++) {
        Player* player = create_new_player();
        game->players[i] = player;
        game->players_connected += 1;
    }
    int player_turn = 0;
    
}

Game* create_lobby(int n_players) {
    Game* game = malloc(sizeof(Game));
    game->players = malloc(n_players*sizeof(Player));
    game->n_players = n_players;
    game->players_connected = 0;
    return game;
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
        if (player->current_life > 0) {
            printf("Turno de %s \n", player->name);
            printf("¿ Deseas rendirte ?");
            printf("Presiona -1 si quieres rendirte");
            scanf("%i",&i);
            if (i == -1) return i;
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
        }

    }
    return 0;
}

void use_skills(Player* player, Monster* monster, Game* game){
    if (player->type == Hunter){
        if (player->current_skill == 0) {
            use_lunge(player, monster);
        }
        else if (player->current_skill == 1) {
            use_cross_cut(player, monster);
        }
        else {
            use_distract(player, monster);
        }
    }
    else if (player->type == Doctor) {
        if (player->current_skill == 0 ) {
            use_heal(game->players[player->current_target]);
        }
        else if (player->current_skill == 1) {
            use_regenerative_flash(player, game->players[player->current_target], monster);
        }
        else {
            use_vital_discharge(player, monster);
        }
    }
    else {
        if (player->current_skill == 0) {
            use_sql_injection(game->players[player->current_target]);
        }
        else if (player->current_skill == 1) {
            use_ddos_attack(player, monster);
        }
        else {
            use_brute_force(player, monster);
        }  
    }
}