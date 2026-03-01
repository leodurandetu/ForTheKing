#ifndef PERSO_H
#define PERSO_H

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
    int pts_deplacements;
    
    perso_type_t type;

} perso_t;


/* Prototypes des fonctions*/

/* Massoud */
void afficher_personnage(SDL_Renderer * renderer, SDL_Texture * texture_perso, perso_t * perso, int tailleCase);
/* Leo */
perso_t * init_perso(perso_type_t persoType, int xDepart, int yDepart);
/* Leo */
void detruire_perso(perso_t ** perso);
/* Massoud */
int get_pers_movements_points(perso_t *perso);
// faire une fonction qui met à jour les pts de deplacement
#endif
