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
void use_ruzgar(Monster* monster);

/* Coletazo */
void use_coletazo(Monster* monster);

/*---Skills de Ruzalos---*/

/* Salto */
void use_jump(Monster* monster);

/* Espina Venenosa */
void use_poisonous_thorn(Monster* monster);

/*---Ruiz---*/

/* Caso Copia */
void use_copy_case(Monster* monster);

/* Reprobaton-9000 */
void use_reprobaton_9000(Monster* monster);

/* sudo rm -rf */
void use_sudo_rm(Monster* monster);


