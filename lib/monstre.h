#ifndef MONSTRE_H
#define MONSTRE_H

/* Structures */
typedef enum{
    SQUELETTE,
    TROLL
}type_monstre_t;

typedef struct {

    type_monstre_t type;
    int x, y;
    int degats;
    int force;
    int sante;
    int sante_max;
    int intelligence;
    int rapidite;
    int evasion;
    int niveau;
    
} monstre_t;

/* Prototypes des fonctions */
monstre_t * creer_monstre_aleatoire(type_monstre_t type,int x, int y);

#endif
