/* Structures */

typedef enum {

    BRUTE,
    CHASSEUR,
    ASSASSIN,
    MAGE

} perso_type_t;

typedef struct {

    int x, y;
    int force;
    int sante;
    int intelligence;
    int rapidite;
    int evasion;
    
    perso_type_t type;

} perso_t;


/* Prototypes des fonctions*/

/* Massoud */
void afficher_personnage(SDL_Renderer *renderer,SDL_Texture *texture,perso_t *pers);
/* Leo */
perso_t * init_perso(perso_type_t persoType);