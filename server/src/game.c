#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>     /* Symbolic Constants */
#include <sys/types.h>  /* Primitive System Data Types */
#include <stdlib.h>     /* General Utilities */
#include "game.h"
#include "comunication.h"

int player_life[] = {5000, 3000, 2500};
int monster_life[] = {10000, 20000, 25000};

Player* get_random_player(Game* game){
    int current_players_index[game->remaining_players];
    int j = 0;
    for (int i = 0; i < game->n_players; i++){
        if (!game->players[i]->is_retired){
            current_players_index[j] = i;
            j++;
        }
    }

    j = generate_random(0, game->remaining_players);
    return  game->players[current_players_index[j]];
}

void choose_monster(Game* game, int selection){
    Monster* monster = game->monster;
    if (selection == 4) {
        printf("SERVER: choose_monster | Seleccionando monstruo al azar...\n");
        selection = generate_random(1,3);
    }
    if (selection == 1){
        monster->type = JagRuz;
        printf("SERVER: choose_monster | Has seleccionado al monstruo JagRuz con %i de vida\n", monster_life[selection - 1]);
    } else if (selection == 2){
        monster->type = Ruzalos;
        printf("SERVER: choose_monster | Has seleccionado al monstruo Ruzalo con %i de vida\n", monster_life[selection - 1]);
    } else if (selection == 3){
        monster->type = Ruiz;
        printf("SERVER: choose_monster | Has seleccionado al monstruo Ruiz con %i de vida\n", monster_life[selection - 1]);
    }
    monster->life = monster_life[selection - 1];
    monster->current_life = monster_life[selection - 1];
    monster->n_of_stabs = 0;
    monster->used_jump = 0;
    monster->distracted = false;
    monster->brute_force = 0;
    monster->ddos = 0;
    monster->ddos_counter = 0;
    monster->blood = 0;
    monster->player_distracted = NULL;
    game->monster = monster;
};

Player* create_new_player(Player* player, int class) {
    /*
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
    scanf("%i", &class);
    */
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
    player->is_retired = false;
    player->is_reprobate = false;
    player->brute_force = 0;
    player->turns_with_x2 = 0;
    player->rounds_with_spine = 0;
    player->venom_damage = 0;
    player->current_skill = -1;
    player->current_target = -1;
    player->turns_reprobate = 0;
    player->blood = 0;
    return player;
}

static void choose_skills(Player* player) {
    int i;
    printf("SERVER: choose_skills | ---Elegir Habilidad---\n");
    if (player->type == Hunter){
        printf("SERVER: choose_skills | 1) Estocada\n2) Corte Cruzado\n3) Distraer");
    } else if (player->type == Doctor){
        printf("SERVER: choose_skills | 1) Curar\n2) Destello Regenerador\n3) Descarga Vital");
    } else if (player->type == Hacker){
        printf("SERVER: choose_skills | 1) Inyección SQL\n2) Ataque DDOS\n3)Fuerza Bruta");
    }
    scanf("%i",&i);
    player->current_skill = i - 1;
}

int turn_choices(Game* game, int player_turn){
    /*
    1) Si player_turn es 0, primer turno de todo el juego se retorna monstruo seleccionado
    2) Si es lider se imprime situacion de los jugadores ( no estoy seguro si es para el lder o todos)
    3) Se pregunta si se desea render, se retorna -1 en ese caso
    4) Se elige habilidad y se setea valor en current_skill
    5) Se elige  Objetivo si corresponde ( si no es el monstruo) y se setea valor en current_target (ver si current_target puede ser uno mismo)
    6) se retorna cero
    */
    int i;
    // char message[255];
    char* message = malloc(256);
    // char* response = malloc(256);
    // char response[255];
    Player* player = game->players[player_turn % game->n_players];

    if (player->current_life > 0 && !player->is_retired) {
        printf("SERVER: turn_choices | Turno de %s\n", player->name);
        strcpy(message, "-");
        server_send_message(player->socket, 99, message);
        printf("SERVER: turn_choices | Esperando para ver si quiere retirarse o continuar jugando\n");
        char* response = server_receive_payload(player->socket);
        if (response[1] == '-' && response[2] == '1') {
            free(response);
            player->is_retired = true;
            printf("SERVER: turn_choices | %s se ha retirado del juego\n", player->name);
            game->remaining_players --;
            if (player->is_leader && game->remaining_players) {
                Player* new_leader = get_random_player(game);
                printf("SERVER: turn_choices | El líder se ha retirado. El nuevo líder es %s\n", new_leader->name);
                strcpy(message, "El líder se ha retirado y has sido elegido como nuevo líder!");
                server_send_message(new_leader->socket, 1, message);
                new_leader->is_leader = true;
            }

            free(message);
            return i;
        };
        free(response);
        printf("SERVER: turn_choices | Jugador decide continuar jugando\n");
        strcpy(message, "---Elegir Habilidad---");
        server_send_message(player->socket, 1, message);
        // choose_skills(player);

        if (player->type == Hunter){
            strcpy(message, "1) Estocada");
            server_send_message(player->socket, 1, message);
            strcpy(message, "2) Corte Cruzado");
            server_send_message(player->socket, 1, message);
            strcpy(message, "3) Distraer");
            server_send_message(player->socket, 1, message);
        } else if (player->type == Doctor){
            strcpy(message, "1) Curar");
            server_send_message(player->socket, 1, message);
            strcpy(message, "2) Destello Regenerador");
            server_send_message(player->socket, 1, message);
            strcpy(message, "3) Descarga Vital");
            server_send_message(player->socket, 1, message);
        } else if (player->type == Hacker){
            strcpy(message, "1) Inyección SQL");
            server_send_message(player->socket, 1, message);
            strcpy(message, "2) Ataque DDOS");
            server_send_message(player->socket, 1, message);
            strcpy(message, "3) Fuerza Bruta");
            server_send_message(player->socket, 1, message);
        }
        strcpy(message, "-");
        server_send_message(player->socket, 40, message);
        char* skill_selection_response = server_receive_payload(player->socket);
        player->current_skill = skill_selection_response[1] - '0' - 1; // Aquí asumimos que el usuario responde un input válido
        printf("SERVER: turn_choices | %s ha decidio utilizar habilidad: %i\n", player->name, player->current_skill);
        free(skill_selection_response);
        if (player->type == Doctor){
            if (player->current_skill == 0){
                strcpy(message, "---Elegir Objetivo---");
                server_send_message(player->socket, 1, message);

                for (int j = 0; j < game->n_players; j++){
                    if (game->players[j]->current_life > 0 && !game->players[j]->is_retired && player->socket != game->players[j]->socket) {
                        sprintf(message, "%i) Jugador: %s", j+1, game->players[j]->name);
                        server_send_message(player->socket, 1, message);
                    }
                }
                strcpy(message, "-");
                server_send_message(player->socket, 40, message);
                char * selected_player_index = server_receive_payload(player->socket);
                player->current_target = selected_player_index[1] - '0' - 1; // Aquí asumimos que el usuario responde un input válido
                free(selected_player_index);
            }
            else if (player->current_skill == 1){
                //elegir jugador al azar
                int index_player = player_turn;
                while ((index_player == player_turn) || (game->players[index_player]->is_retired) ) {//numero random distinto al actual|| (game->players[index_player]->is_retired)
                    index_player = generate_random(0, game->n_players);
                }
                player->current_target = index_player;
            }
        } else if (player->type == Hacker){
            if(player->current_skill == 0){
                strcpy(message, "---Elegir Objetivo---");
                server_send_message(player->socket, 1, message);

                for (int j = 0; j < game->n_players; j++){
                    if (game->players[j]->current_life > 0 && !game->players[j]->is_retired && player->socket != game->players[j]->socket) {
                        sprintf(message, "%i) Jugador: %s", j+1, game->players[j]->name);
                        server_send_message(player->socket, 1, message);
                    }
                }
                strcpy(message, "-");
                server_send_message(player->socket, 40, message);
                char * selected_player_index = server_receive_payload(player->socket);
                player->current_target = selected_player_index[1] - '0' - 1; // Aquí asumimos que el usuario responde un input válido
                free(selected_player_index);
            }
        }
        use_skills(player, game);
    }

    free(message);
    return 0;
}


void send_last_action_to_clients(Game* game, char* message){
    for (int turn = 0; turn < game->n_players; turn++) {
        if (!game->players[turn]->is_retired) {
            // printf("game->players[turn]->socket: %i\n", game->players[turn]->socket);
            server_send_message(game->players[turn]->socket, 1, message);
        }
    }
}

void use_skills(Player* player, Game* game){
    char message_to_send[255];
    if (player->type == Hunter){
        if (player->current_skill == 0) {
            use_lunge(player, game->monster);
            sprintf(message_to_send, "Jugador %s ha utilizado 'lunge' sobre el monstruo", player->name);
        }
        else if (player->current_skill == 1) {
            use_cross_cut(player, game->monster);
            sprintf(message_to_send, "Jugador %s ha utilizado 'cross_cut' sobre el monstruo", player->name);
        }
        else {
            use_distract(player, game->monster);
            sprintf(message_to_send, "Jugador %s ha utilizado 'distract' sobre el monstruo", player->name);
        }
    }
    else if (player->type == Doctor) {
        if (player->current_skill == 0 ) {
            printf("SERVER: use_skills | %s cura a", player->name);
            use_heal(game->players[player->current_target]);
            printf("SERVER: use_skills | El poder va sobre el jugador: %s\n", game->players[player->current_target]->name);
            sprintf(message_to_send, "Jugador %s ha utilizado 'heal' sobre %s", player->name, game->players[player->current_target]->name);
        }
        else if (player->current_skill == 1) {
            printf("SERVER: use_skills | %s utiliza el rayo regenerador. ", player->name);
            printf("SERVER: use_skills | El poder va sobre el jugador: %s\n", game->players[player->current_target]->name);
            use_regenerative_flash(player, game->players[player->current_target], game->monster);
            sprintf(message_to_send, "Jugador %s ha utilizado 'regenerative_flash' sobre el monstruo", player->name);
        }
        else {
            use_vital_discharge(player, game->monster);
            sprintf(message_to_send, "Jugador %s ha utilizado 'vital_discharge' sobre el monstruo", player->name);
        }
    }
    else {
        if (player->current_skill == 0) {
            printf("SERVER: use_skills | El poder va sobre el jugador: %s\n", game->players[player->current_target]->name);
            use_sql_injection(game->players[player->current_target]);
            sprintf(message_to_send, "Jugador %s ha utilizado 'sql_injection' sobre %s", player->name, game->players[player->current_target]->name);
        }
        else if (player->current_skill == 1) {
            use_ddos_attack(player, game->monster);
            sprintf(message_to_send, "Jugador %s ha utilizado 'ddos_attack' sobre el monstruo", player->name);
        }
        else {
            use_brute_force(player, game->monster);
            sprintf(message_to_send, "Jugador %s ha utilizado 'brute_force' sobre el monstruo", player->name);
        }
    }
    send_last_action_to_clients(game, message_to_send);
}

void use_monster_skills(Game* game) {
    char message_to_send[255];
    bool already_sent_message = false;
    int prob = generate_random(0, 100);
    printf("SERVER: use_monster_skills | Probabilidad obtenida: %i\n", prob);
    Player* selected_player;
    if (game->monster->distracted) { //mounstro distraido ataca al cazador
        printf("SERVER: use_monster_skills | Mounstro distraido, ataca a ultimo cazador que lo distrajo\n");
        selected_player = game->monster->player_distracted;
        game->monster->distracted = false; //restablecer distraido
    }
    else {
        selected_player = get_random_player(game);
    }
    if (game->monster->type == JagRuz){
        if (prob < 50) {
            use_ruzgar(selected_player);
            sprintf(message_to_send, "Monstruo ha utilizado 'ruzgar' sobre jugador %s", selected_player->name);
        } else {
            for (int i = 0; i < game->n_players; i++){
                if (!game->players[i]->is_retired){
                    use_coletazo(game->players[i]);
                    sprintf(message_to_send, "Monstruo ha utilizado 'coletazo' sobre jugador %s", game->players[i]->name);
                    send_last_action_to_clients(game, message_to_send);
                }
            }
            already_sent_message = true;
        }
    }
    else if (game->monster->type == Ruzalos) {
        if (prob < 40 && !game->monster->used_jump) {
            use_jump(game->monster, selected_player);
            sprintf(message_to_send, "Monstruo ha utilizado 'jump' sobre jugador %s", selected_player->name);
        } else {
            use_poisonous_thorn(game->monster, selected_player);
            sprintf(message_to_send, "Monstruo ha utilizado 'poisonous_thorn' sobre jugador %s", selected_player->name);
        }
    }
    else {
        if (prob < 40) {
            Player* selected_player_skill = get_random_player(game);
            use_copy_case(game->monster, selected_player, game->rounds);
            sprintf(message_to_send, "Monstruo ha utilizado 'copy_case' sobre jugador %s", selected_player->name);
        } else if (prob < 60){
            use_reprobaton_9000(game->monster, selected_player);
            sprintf(message_to_send, "Monstruo ha utilizado 'reprobaton_9000' sobre jugador %s", selected_player->name);
        } else {
            use_sudo_rm(game->monster, game->players, game->n_players, game->rounds);
            sprintf(message_to_send, "Monstruo ha utilizado 'sudo_rm' sobre todos los jugadores");
        }
    }
    if (!already_sent_message) {
        send_last_action_to_clients(game, message_to_send);
    }
}

void update_round(Game* game){
    printf("SERVER: update_round | ------------ OTROS DE LA RONDA ----------------\n");
    if (game->monster->n_of_stabs){
        printf("SERVER: update_round | Debido a las %i estocadas que tiene el monstruo, este pierde %i de vida\n", game->monster->n_of_stabs, game->monster->n_of_stabs * 500);
        reduce_monster_life(game->monster, game->monster->n_of_stabs * 500);
    }
    game->remaining_players = game->n_players;
    for (int i = 0; i < game->n_players; i++){
        if (!game->players[i]->is_retired){
            if (game->players[i]->turns_with_x2){
                game->players[i]->turns_with_x2 --;
                printf("SERVER: update_round | Al jugador %s ahora le quedan %i turnos con x2\n", game->players[i]->name, game->players[i]->turns_with_x2);
            }
            if (game->players[i]->rounds_with_spine){
                printf("SERVER: update_round | Debido al veneno, el jugador %s pierde %i de vida\n", game->players[i]->name, game->players[i]->venom_damage);
                update_player_life(game->players[i], -game->players[i]->venom_damage);
                game->players[i]->rounds_with_spine --;
                printf("SERVER: update_round | Le quedan %i rondas sufriendo esto.... :(\n", game->players[i]->rounds_with_spine);
            }
            if (game->players[i]-> is_reprobate) {
                if (game->players[i]->turns_reprobate > 0) {
                    printf("SERVER: update_round | %s recupera el estado de no reprobado\n", game->players[i]->name);
                    game->players[i]->turns_reprobate = 0;
                    game->players[i]->is_reprobate = false;
                }
                else {
                    game->players[i]->turns_reprobate ++;
                }
            }
        } else {
            game->remaining_players --;
        }
    }
    if (!game->remaining_players){
        printf("SERVER: update_round | El monstruo ha ganado...\n");
    }
}

void send_stats(Game* game, Player* player) {
    char* message = malloc(256);
    strcpy(message, "-----COMIENZA UNA NUEVA RONDA-----");
    server_send_message(player->socket, 1, message);
    strcpy(message, "---Situación de los Jugadores---");
    server_send_message(player->socket, 1, message);
    for (int j = 0; j < game->n_players; j++) {
        Player* current_player = game->players[j];
        if (!current_player->is_retired) {
            if (current_player->is_leader) {
                sprintf(message, "Lider[%i]-%s -> Vida %i / %i", current_player->type, current_player->name, current_player->current_life, current_player->life);
                server_send_message(player->socket, 1, message);
                //printf("Lider-%s[%i] -> Vida %i / %i\n", player_list->name, player_list->type, player_list->current_life, player_list->life);
            }
            else {
                sprintf(message, "%s[%i] -> Vida %i / %i", current_player->name, current_player->type, current_player->current_life, current_player->life);
                server_send_message(player->socket, 1, message);
                // printf("%s[%i] -> Vida %i / %i\n", player_list->name, player_list->type, player_list->current_life, player_list->life);
            }
        }
    }
    strcpy(message, "---Situación del monstruo---");
    server_send_message(player->socket, 1, message);
    sprintf(message, "Monstruo -> Vida %i / %i", game->monster->current_life, game->monster->life);
    server_send_message(player->socket, 1, message);
    free(message);
}