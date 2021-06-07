enum TypePlayer {Hunter, Doctor, Hacker};

typedef struct player {
  int life;
  int type;
} Player;

void use_player_skills(Player* player);
void use_hounter_skills(Player* player);
void use_doctor_skills(Player* player);
void use_hackers_skills(Player* player);

/*---Skills de Cazador---*/

/* Estocada */
void use_lunge(Player* player);

/* Corte Cruzado */
void use_cross_cut(Player* player);

/* Distraer */
void use_distract(Player* player);

/*---Skills de Médico---*/

/* Curar */
void use_heal(Player* player);

/* Destello Regenerador */
void use_regenerative_flash(Player* player);

/* Descarga Vital */ 
void use_vital_discharge(Player* player);

/*---Skills de Hacker---*/

/* Inyección SQL */
void use_sql_injection(Player* player);

/* Ataque DDOS */
void use_ddos_attack(Player* player);

/* Fuerza Bruta */
void use_brute_force(Player* player);



