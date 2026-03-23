#ifndef CARTE_H
#define CARTE_H

#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>
#include "couleur.h"
#include "monstre.h"
#include "perso.h"
#include "maths.h"
#include "commun.h"
#include "combat.h"
#include "carte_types.h"
#include "option.h"

/* Prototypes des fonctions de logique */
void init_carte(case_t ** carte);
void generer_eau(case_t ** carte);
void generer_biomes(case_t ** carte);
void placer_batiments(case_t ** carte);
void remplir_zone(case_t ** carte, biome_t biome, coordonnee_t dep, coordonnee_t arr);
int test_etat_case(case_t ** carte, int x, int y);
void afficher_carte(case_t ** carte); // Console
void devoiler_brouillard_rayon(case_t ** carte, int x, int y, int rayon);
void liberer_memoire_carte(case_t *** carte, int taille_carte);
void placer_monstres(case_t ** carte);
void ajout_obstacles(case_t ** carte);
booleen_t deplacement_possible(case_t ** carte, int x, int y);
booleen_t case_occupee(case_t ** carte, int x, int y, int persX, int persY);
void coords_case_libre(case_t ** carte, int *x, int *y);
void souris_vers_case(int mouseX, int mouseY, int *carte_x, int *carte_y, int tailleCase, perso_t *perso, SDL_Renderer *renderer);
int chemin_valide(case_t **carte, int taille_carte, int xDepart, int yDepart, int xCible, int yCible, int pts_deplacement_max, perso_t *perso, int *distance);
booleen_t a_un_voisin_monstre(case_t ** carte, int cx, int cy);
void faire_apparaitre_groupe(case_t ** carte, type_monstre_t type, coordonnee_t cases_dispos[], int nb_cases_dispos);
void deplacer_monstres(SDL_Renderer * rendererPrincipal, case_t ** carte, perso_t * perso, combat_t ** combat_actuel,int *vies_globales);

// placer des sanctuaires
void placer_sanctuaires(case_t ** carte);

#endif 
