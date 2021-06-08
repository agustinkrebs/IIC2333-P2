enum TypeMonster {JagRuz, Ruzalos, Ruiz};

typedef struct monster {
  int life;
  int type;
  int current_life;
  int n_of_stabs;
  int used_jump;
  int was_distracted;
} Monster;

void use_monster_skills(Monster* monster);
void use_jagruz_skills(Monster* monster);
void use_ruzalos_skills(Monster* monster);
void use_ruiz_skilss(Monster* monster);

/*---Skills de JagRuz---*/

/* Ruzgar */
void use_ruzgar(Player* player);

/* Coletazo */
void use_coletazo();

/*---Skills de Ruzalos---*/

/* Salto */
void use_jump(Monster* monster, Player* player);

/* Espina Venenosa */
void use_poisonous_thorn(Monster* monster, Player* player);

/*---Ruiz---*/

/* Caso Copia */
void use_copy_case(Monster* monster, Player* player_copy, Player* player_paste);

/* Reprobaton-9000 */
void use_reprobaton_9000(Player* player);

/* sudo rm -rf */
void use_sudo_rm();
