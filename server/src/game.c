#include "game.h"

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
                    print("Lider:\nVida:\n");
                }
                printf("Jugador %i\nVida:\n", j + 1);
            }
        }
        printf("¿ Deseas rendirte ?");
        printf("Presiona -1 si quieres rendirte");
        scanf("%i",&i);
        if (i == -1) return i;
        printf("---Elegir Habilidad---\n");
        if (game->players[player_turn % n_players]->type == Hunter){
            printf("1) Estocada\n2) Corte Cruzado\n3) Distraer\n");
        } else if (game->players[player_turn % n_players]->type == Doctor){
            printf("1) Curar\n2) Destello Regenerador\n3) Descarga Vital\n");
        } else if (game->players[player_turn % n_players]->type == Hacker){
            printf("1) Inyección SQL\n2) Ataque DDOS\n3)Fuerza Bruta");
        }
        scanf("%i",&i);
        game->players[player_turn % n_players]->current_skill = i;
        if (game->players[player_turn % n_players]->type == Doctor){
            if (game->players[player_turn % n_players]->current_skill == 0 || game->players[player_turn % n_players]->current_skill == 1){
                pritnf("---Elegir Objetivo---\n"); /* Hay que cachar si las habilidades se puede aplicar a uno mismo*/
                for (int j = 0; j < n_players;j++){
                    printf("%i) Jugador %i", j + 1, j +1);
                }
                scanf("%i",&i);
                game->players[player_turn % n_players]->current_target = i;
            }
        } else if (game->players[player_turn % n_players]->type == Hacker){
            if(game->players[player_turn % n_players]->current_skill == 0){
                pritnf("---Elegir Objetivo---\n"); /* Hay que cachar si las habilidades se puede aplicar a uno mismo*/
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