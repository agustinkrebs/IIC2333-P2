#include "game.h"
#include <stdio.h>
#include <stdlib.h>

int player_life[] = {5000, 3000, 2500};
int monster_life[] = {10000, 20000, 25000};

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
        printf("Lider selecciona el monstruo con el que deseas combatir:\n");
        scanf("%i",&i);
        return i;
    } else {
        if (!(player_turn % n_players)) {
            printf("---Situación de los Jugadores---\n");
            for (int j = 0; j < n_players; j++) {
                if (!j) {
                    printf("Lider:\nVida:\n");
                }
                printf("Jugador %i\nVida:\n", j + 1);
            }
        }
        printf("¿Deseas rendirte?\n");
        printf("Presiona -1 si quieres rendirte. Cualqueir otra tecla en caos contrario\n");
        scanf("%i",&i);
        if (i == -1) return i;
        printf("---Elegir Habilidad---\n");
        if (game->players[player_turn % n_players]->type == Hunter){
            printf("1) Estocada\n2) Corte Cruzado\n3) Distraer\n");
        } else if (game->players[player_turn % n_players]->type == Doctor){
            printf("1) Curar\n2) Destello Regenerador\n3) Descarga Vital\n");
        } else if (game->players[player_turn % n_players]->type == Hacker){
            printf("1) Inyección SQL\n2) Ataque DDOS\n3)Fuerza Bruta\n");
        }
        scanf("%i",&i);
        game->players[player_turn % n_players]->current_skill = i;
        if (game->players[player_turn % n_players]->type == Doctor){
            if (game->players[player_turn % n_players]->current_skill == 0 || game->players[player_turn % n_players]->current_skill == 1){
                printf("---Elegir Objetivo---\n"); /* Hay que cachar si las habilidades se puede aplicar a uno mismo*/
                for (int j = 0; j < n_players;j++){
                    printf("%i) Jugador %i", j + 1, j +1);
                }
                scanf("%i",&i);
                game->players[player_turn % n_players]->current_target = i;
            }
        } else if (game->players[player_turn % n_players]->type == Hacker){
            if(game->players[player_turn % n_players]->current_skill == 0){
                printf("---Elegir Objetivo---\n"); /* Hay que cachar si las habilidades se puede aplicar a uno mismo*/
                for (int j = 0; j < n_players;j++){
                    printf("%i) Jugador %i", j + 1, j +1);
                }
                scanf("%i",&i);
                game->players[player_turn % n_players]->current_target = i;
            }
        }

    }
    return 0;
}

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