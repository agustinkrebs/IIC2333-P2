void turn(int player_turn, int n_players) {
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
        /* mostrar habilidades dependiendo el tipo de jugador*/
        pritnf("---Elegir Objetivo---\n");

    }
}