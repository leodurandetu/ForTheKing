#ifndef MONSTRE_H
#define MONSTRE_H

/* Structures */

typedef struct {

    int x, y;
    int degats;
    int force;
    int sante;
    int sante_max;
    int intelligence;
    int rapidite;
    int evasion;
    
} monstre_t;

/* Prototypes des fonctions */
monstre_t * creer_monstre_aleatoire(int x, int y);

#endif
