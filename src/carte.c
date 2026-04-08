#include "../lib/carte.h"
#include "../lib/perlin_noise.h"
/* ========================================================================*/

/* Leo */
/*
 * Cette fonction renvoie les coordonnées
 * d'une case libre sur la carte choisie aléatoirement
 * (sans monstres, batiments et pas dans l'eau)
*/
void coords_case_libre(case_t ** carte, int *x, int *y) {
    *x = rand() % TAILLE_CARTE;
    *y = rand() % TAILLE_CARTE;

    while (!deplacement_possible(carte, *x, *y)) {
        *x = rand() % TAILLE_CARTE;
        *y = rand() % TAILLE_CARTE;
    }

}

/* Leo */
/*
 * Cette fonction retourne VRAI si le personnage
 * peut se déplacer sur cette case, FAUX sinon.
 */
booleen_t deplacement_possible(case_t ** carte, int x, int y) {
    //  On vérifie les bords D'ABORD -> sinon seg fault
    if (x < 0 || x >= TAILLE_CARTE || y < 0 || y >= TAILLE_CARTE) {
        return FAUX;
    }

    case_t maCase = carte[y][x];

    if (maCase.biome == EAU) {
        return FAUX;
    } else if (maCase.terrain != PAS_DE_TERRAIN) {
        return FAUX;
    } else if (maCase.estVisible == 0) {
        return FAUX;
    } else if (maCase.sanctuaires != PAS_DE_SANCTUAIRE) {
        return FAUX;
    } else {
        return VRAI;
    }

}

/* Leo */
/*
 * Cette fonction retourne VRAI si la case
 * est occupée soit par un batiment,
 * soit par un monstre,
 * soit par un obstacle,
 * soit par le joueur,
 * soit si la case est en dehors de la carte
 */
booleen_t case_occupee(case_t ** carte,
    int x, int y, int persX, int persY) {

    if (x < 0 || x >= TAILLE_CARTE || y < 0 || y >= TAILLE_CARTE) {
        return VRAI;
    } else if (y == persY && x == persX) {
        return VRAI;
    } else {
        case_t maCase = carte[y][x];

        if (maCase.monstre != NULL || maCase.batiment.type != PAS_DE_BATIMENT
            || maCase.terrain != PAS_DE_TERRAIN) {
            return VRAI;
        } else {
            return FAUX;
        }

    }

}


/* Leo et Saandi */
/*
 * Cette fonction permet de dévoiler le brouillard sur la carte,
 * à partir d'un point donné d'abscisse x et d'ordonnée y,
 * et avec un rayon donné.
 */
void devoiler_brouillard_rayon(case_t ** carte, int x, int y, int rayon) {
     int i, j;

    for (i = 0; i < TAILLE_CARTE; i++) {
        for (j = 0; j < TAILLE_CARTE; j++) {

            // Calcul de la distance entre la case (i, j) et le centre (x, y)
            int dx = i - x;
            int dy = j - y;
            int distanceCarree = dx * dx + dy * dy;

            // Si la case est dans le cercle, on la dévoile
            if (distanceCarree <= rayon * rayon) {
                carte[j][i].estVisible = 1;
            }
        }
    }

}

/* Leo */
/*
 * Cette fonction permet d'initialiser la carte,
 * c'est à dire d'initialiser toutes les cases
 * à une case de terre, vide, cachée par le brouillard,
 * sans monstre ni batiment.
 */
void init_carte(case_t ** carte) {
    int x, y;

    for (x = 0; x < TAILLE_CARTE; x++) {

        for (y = 0; y < TAILLE_CARTE; y++) {
            case_t maCase;
            batiment_t monBatiment;

            monBatiment.type = PAS_DE_BATIMENT;

            maCase.biome = TERRE;
            maCase.batiment = monBatiment;

            /* Par défaut, toutes les cases sont cachées par le brouillard,
             * sauf celles à un rayon de 1 des tours du boss
             */

            maCase.estVisible = 1;

            maCase.monstre = NULL;
            maCase.terrain = PAS_DE_TERRAIN;
            maCase.sanctuaires = PAS_DE_SANCTUAIRE;
            maCase.sanctuaire_pris = 0;

            carte[x][y] = maCase;
        }

    }

}

/* Leo */
/*
 * Cette fonction ajoute du brouillard
 * sur toutes les cases de la carte
 */
void remplir_brouillard(case_t ** carte) {
    int x, y;

    for (x = 0; x < TAILLE_CARTE; x++) {

        for (y = 0; y < TAILLE_CARTE; y++) {
            carte[x][y].estVisible = 0;

        }

    }

}

/* Massoud */
/*
 * Cette fonction permet de placer des cases d'eau
 * sur la carte, de manière aléatoire an utilisant
 * l'algorithme de Perlin Noise.
 */
void generer_eau(case_t **carte) {
    float echelle = 0.08f;

    for (int y = 0; y < TAILLE_CARTE; y++) {
        for (int x = 0; x < TAILLE_CARTE; x++) {
            float val = perlin_2d(x * echelle, y * echelle);
            /* Les zones avec val > 0.25 deviennent de l'eau */
            if (val > 0.25f) {
                carte[y][x].biome = EAU;
            }
        }
    }
}

/*Saandi*/
void remplir_zone(case_t ** carte, biome_t biome, coordonnee_t dep, coordonnee_t arr) {
    int x_min = fmax(0, fmin(dep.x, arr.x));
    int x_max = fmin(TAILLE_CARTE - 1, fmax(dep.x, arr.x));
    int y_min = fmax(0, fmin(dep.y, arr.y));
    int y_max = fmin(TAILLE_CARTE - 1, fmax(dep.y, arr.y));

    for (int i = y_min; i <= y_max; i++) {
        for (int j = x_min; j <= x_max; j++) {
            carte[i][j].biome = biome;
        }
    }
}
/* Saandi */
void generer_biomes(case_t **carte) {
    float echelle = 0.06f;

    for (int y = 0; y < TAILLE_CARTE; y++) {
        for (int x = 0; x < TAILLE_CARTE; x++) {
            if (carte[y][x].biome == EAU) continue;

            /* Trois échantillons décalés dans l'espace */
            float foret  = perlin_2d(x * echelle + 50.0f, y * echelle);
            float desert = perlin_2d(x * echelle + 100.0f, y * echelle + 30.0f);
            float neige  = perlin_2d(x * echelle, y * echelle + 80.0f);

            /* On prend le biome dont le bruit est le plus fort */
            if (foret > desert && foret > neige && foret > 0.1f)
                carte[y][x].biome = FORET;
            else if (desert > neige && desert > 0.1f)
                carte[y][x].biome = DESERT;
            else if (neige > 0.1f)
                carte[y][x].biome = NEIGE;
            /* sinon on garde TERRE */
        }
    }
}


// Massoud
// Vérifie à un rayon de 2 cases pour éviter que les monstres s'agglutinent
booleen_t a_un_voisin_monstre(case_t ** carte, int cx, int cy) {
    // On scanne un petit carré de 5x5 autour de la case cible
    for (int y = cy - 2; y <= cy + 2; y++) {
        for (int x = cx - 2; x <= cx + 2; x++) {

            // On vérifie qu'on ne sort pas de la carte
            if (x >= 0 && x < TAILLE_CARTE && y >= 0 && y < TAILLE_CARTE) {

                // Si ce n'est pas la case centrale ET qu'il y a un monstre
                if ((x != cx || y != cy) && carte[y][x].monstre != NULL) {

                    // Calcul simplifié de la distance sur grille hexagonale
                    int diff_x = abs(x - cx);
                    int diff_y = abs(y - cy);

                    // Si c'est dans un rayon de  environ 2 cases -> on refuse
                    if (diff_x + diff_y <= 3) {
                        return VRAI;
                    }
                }
            }
        }
    }
    return FAUX;
}

// Massoud
void faire_apparaitre_groupe(case_t ** carte, type_monstre_t type, coordonnee_t cases_dispos[], int nb_cases_dispos) {
    // Minimum 15 cases de biome (évite les monstres isolés sur les bords)
    if (nb_cases_dispos < 15) return;

    // 1 monstre pour 12 cases libres + un léger aléatoire)
    int nb_a_placer = (nb_cases_dispos / 20) + (rand() % 2);
    if (nb_a_placer > nb_cases_dispos) nb_a_placer = nb_cases_dispos;

    int monstres_places = 0;

    while (monstres_places < nb_a_placer && nb_cases_dispos > 0) {
        int index = rand() % nb_cases_dispos;
        int cx = cases_dispos[index].x;
        int cy = cases_dispos[index].y;

        if (!a_un_voisin_monstre(carte, cx, cy)) {
            carte[cy][cx].monstre = creer_monstre_aleatoire(type, cx, cy);
            monstres_places++;
        }

        // On retire la case testée de la liste pour ne pas boucler dessus à l'infini
        cases_dispos[index] = cases_dispos[nb_cases_dispos - 1];
        nb_cases_dispos--;
    }
}

/* Leo, améliorée par Massoud */
/*
 * Cette fonction permet de placer un certain nombre
 * de monstres sur la carte de manière aléatoire
 */
void placer_monstres(case_t ** carte) {
    // Taille du secteur scanné (12x12 cases)
    int taille_secteur = 12;

    for (int i = 0; i < TAILLE_CARTE; i += taille_secteur) {
        for (int j = 0; j < TAILLE_CARTE; j += taille_secteur) {
            coordonnee_t cases_foret[144];
            coordonnee_t cases_desert[144];
            coordonnee_t cases_neige[144];
            int nb_foret = 0, nb_desert = 0, nb_neige = 0;

            // Scan du secteur pour lister les cases libres par biome
            for (int y = i; y < i + taille_secteur && y < TAILLE_CARTE; y++) {
                for (int x = j; x < j + taille_secteur && x < TAILLE_CARTE; x++) {

                    if (carte[y][x].monstre == NULL && carte[y][x].terrain == PAS_DE_TERRAIN &&
                        carte[y][x].biome != EAU && carte[y][x].batiment.type == PAS_DE_BATIMENT) {

                        if (carte[y][x].biome == FORET) {
                            cases_foret[nb_foret].x = x;
                            cases_foret[nb_foret].y = y;
                            nb_foret++;
                        } else if (carte[y][x].biome == DESERT) {
                            cases_desert[nb_desert].x = x;
                            cases_desert[nb_desert].y = y;
                            nb_desert++;
                        } else if (carte[y][x].biome == NEIGE) {
                            cases_neige[nb_neige].x = x;
                            cases_neige[nb_neige].y = y;
                            nb_neige++;
                        }

                    }

                }

            }

            faire_apparaitre_groupe(carte, TROLL, cases_foret, nb_foret);
            faire_apparaitre_groupe(carte, SQUELETTE, cases_desert, nb_desert);
            faire_apparaitre_groupe(carte, YETI, cases_neige, nb_neige);
        }
    }

}

/* Leo */
/*
 * Cette fonction permet de libérer la mémoire à la fin
 * du programme, tout ce qui est relié à la carte.
 * Notamment les pointeurs vers les monstres qui sont
 * stockés (ou pas) sur chaque case de la carte.
 */
void liberer_memoire_carte(case_t *** carte, int taille_carte) {
    int i, j;

    if (carte != NULL) {

        for (i = 0; i < taille_carte; i++) {

            for (j = 0; j < taille_carte; j++) {
                monstre_t *monstre = (*carte)[i][j].monstre;

                if (monstre != NULL) {
                    free(monstre);
                }

            }

            free((*carte)[i]);
            (*carte)[i] = NULL;
        }

        free(*carte);
        *carte = NULL;
    }

}

/* Leo */
/*
 * Cette fonction place des batiments aléatoirement
 * sur la carte : campements et magasins.
 * Cette fonction place également les 4 tours du boss
 * aux coins de la carte
 */
void placer_batiments(case_t ** carte) {
    int i;
    int nb = 60 + rand() % 38;

    for (i = 0; i < nb; i++) {
        int x = rand() % TAILLE_CARTE;
        int y = rand() % TAILLE_CARTE;

        /* On recommence jusqu'à trouver un endroit convenable,
         * C'est à dire sans monstre, et pas dans l'eau.
         */
        while (carte[x][y].monstre != NULL || carte[x][y].biome == EAU || carte[x][y].terrain != PAS_DE_TERRAIN) {
            x = rand() % TAILLE_CARTE;
            y = rand() % TAILLE_CARTE;
        }

        carte[x][y].batiment.type = CAMPEMENT;
    }

    carte[2][2].batiment.type = TOUR_DU_BOSS;
    carte[2][TAILLE_CARTE - 3].batiment.type = TOUR_DU_BOSS;

    carte[TAILLE_CARTE - 3][2].batiment.type = TOUR_DU_BOSS;
    carte[TAILLE_CARTE - 3][TAILLE_CARTE - 3].batiment.type = TOUR_DU_BOSS;
}

/* Crée par Massoud, transformée en fonction sans modifier le contenu par Léo */
/*
 * Cette fonction permet de détecter la case
 * qui est à la position de la souris à l'écran
 */
void souris_vers_case(int mouseX, int mouseY,
    int *carte_x, int *carte_y, int tailleCase,
    perso_t *perso, SDL_Renderer *renderer) {

    int fenetre_taille_x, fenetre_taille_y;
    SDL_GetRendererOutputSize(renderer, &fenetre_taille_x, &fenetre_taille_y);

    // Mathématiques de la grille "flat-top"
    float rayon = tailleCase / 2.0f;
    float hex_w = 2.0f * rayon;
    float hex_h = sqrtf(3) * rayon;
    float espacement_colonnes = hex_w * 0.75f;

    // Décalage de la caméra centrée sur le joueur
    float camX = perso->x * espacement_colonnes;
    float camY = perso->y * hex_h + (perso->x % 2 ? hex_h / 2 : 0);
    int decalageX = (int)(fenetre_taille_x / 2 - camX - hex_w / 2);
    int decalageY = (int)(fenetre_taille_y / 2 - camY - hex_h / 2);

    int result_x = -1;
    int result_y = -1;

    // Détection du clic hexagonal par calcul de distance
    float dist_min_carree = rayon * rayon;

    // Approximation de la zone pour ne pas vérifier les milliers de cases
    int approx_x = (int)((mouseX - decalageX) / espacement_colonnes);
    int approx_y = (int)((mouseY - decalageY) / hex_h);

    // On teste un carré de 3x3 autour de la souris
    for (int i = approx_y - 1; i <= approx_y + 1; i++) {
        for (int j = approx_x - 1; j <= approx_x + 1; j++) {
            if (i >= 0 && i < TAILLE_CARTE && j >= 0 && j < TAILLE_CARTE) {
                float cx = j * espacement_colonnes + hex_w / 2 + decalageX;
                float cy = i * hex_h + (j % 2 ? hex_h / 2 : 0) + hex_h / 2 + decalageY;

                // Théorème de Pythagore pour trouver le centre le plus proche
                float dx = mouseX - cx;
                float dy = mouseY - cy;
                float dist_carree = dx * dx + dy * dy;

                if (dist_carree < dist_min_carree) {
                    dist_min_carree = dist_carree;
                    result_x = j;
                    result_y = i;
                }
            }
        }
    }

    *carte_x = result_x;
    *carte_y = result_y;
}

/* Leo */
/*
 * Attention : ne pas appeler cette fonction directement,
 * mais plutôt chemin_valide
 *
 * Cette fonction retourne la distance d'un chemin, si il y a un
 * chemin constitué de cases valides (pas d'eau, pas de monstre,
 * pas de batiment par-dessus) pour arriver à une case
 * destination, ou INFINI sinon.
 *
 * Elle est récursive pour l'instant.
 */
static int peut_atteindre_rec(case_t ** carte,
    int taille_carte, int xDepart, int yDepart, int xCible, int yCible,
    int pts_deplacement_restants, perso_t *perso)
{

    /* Cas d'arrêt : on est arrivé à destination */
    if (xDepart == xCible && yDepart == yCible) {
        return 0;
    }

    /* Si l'un des chemins va en dehors de la carte */
    if (xCible < 0 || xDepart < 0 || xCible >= taille_carte || xDepart >= taille_carte
        || yCible < 0 || yDepart < 0 || yCible >= taille_carte || yDepart >= taille_carte) {
        return INFINI;
    }

    if (pts_deplacement_restants <= 0) {
        /* on renvoie INFINI quand on ne peut pas atteindre la case finale */
        return INFINI;
    }

    int meilleure_dist = INFINI;

    int dx[2][6] = {
        { +1,  0, -1, -1,  0, +1 },
        { +1,  0, -1, -1,  0, +1 }
    };

    int dy[2][6] = {
        {  0, +1,  0, -1, -1, -1 },
        { +1, +1, +1,  0, -1,  0 }
    };

    int cout_deplacement[2][6] = {
        { 1,  1,  1,  1,  1,  1 },  // pair
        { 1,  1,  1,  1,  1,  1 }   // impair
    };

    int direction;

    for (direction = 0; direction < 6; direction++) {
        int nx = xDepart + dx[xDepart % 2][direction];
        int ny = yDepart + dy[xDepart % 2][direction];
        int cout = cout_deplacement[xDepart % 2][direction];

        if (nx >= 0 && nx < taille_carte && ny >= 0 && ny < taille_carte) {

            if (deplacement_possible(carte, nx, ny)) {

                if (cout <= pts_deplacement_restants) {
                    /* Appel récursif */
                    int distance = peut_atteindre_rec(carte, taille_carte, nx, ny, xCible, yCible, pts_deplacement_restants - cout, perso);

                    if (distance != INFINI) {

                        if (distance + cout < meilleure_dist) {
                            meilleure_dist = distance + cout;
                        }

                    }

                }

            }

        }

    }

    return meilleure_dist;
}

/* Leo */
/*
 * Cette fonction permet de savoir si il y a un chemin
 * constitué de cases valides (pas d'eau, pas de monstre,
 * pas de batiment par-dessus) pour arriver à une case
 * destination.
 * Elle retourne également la distance du chemin, à travers
 * le paramètre pointeur sur entier *distance, ou -1 si il
 * n'y a pas de chemin valide.
 */
int chemin_valide(case_t ** carte, int taille_carte,
    int xDepart, int yDepart, int xCible, int yCible,
    int pts_deplacement_max, perso_t *perso, int *distance)
{

    if (pts_deplacement_max < 0) {
        return FAUX;
    }

    int resultat = peut_atteindre_rec(
        carte, taille_carte, xDepart, yDepart, xCible,
        yCible, pts_deplacement_max, perso
    );

    if (resultat != INFINI) {

        if (distance != NULL) {
            *distance = resultat;
        }

        return VRAI;
    } else {

        if (distance != NULL) {
            *distance = -1;
        }

        return FAUX;
    }

}


/* Massoud */
/*
 * Cette fonction ajoute des obstacles
 * pour chaque type de biome, tels que les arbres,
 * montagnes, cactus et la boue, aléatoirement.
 */
void ajout_obstacles(case_t ** carte){
    int i, j;

    for(i = 0; i < TAILLE_CARTE; i++){
        for(j = 0; j < TAILLE_CARTE; j++){

            carte[i][j].terrain = PAS_DE_TERRAIN;

            int r = rand() % 100;

            switch(carte[i][j].biome){

                case FORET:
                    if(r < 8){
                        carte[i][j].terrain = ARBRES;
                    }
                    break;

                case NEIGE:
                    if(r < 8){
                        carte[i][j].terrain = MONTAGNES;
                    }
                    break;

                case DESERT:
                    if(r < 8){
                        carte[i][j].terrain = CACTUS;
                    }
                    break;

                case TERRE:
                    if(r < 8){
                        carte[i][j].terrain = BASSIN_EAU;
                    }
                    break;

                default:
                    break;
            }
        }
    }
}

/* Leo
 * Cette fonction déplace tous les monstres d'une case
 * sur la carte dans une direction aléatoire
 */
void deplacer_monstres(SDL_Renderer * rendererPrincipal, case_t ** carte, perso_t * perso, combat_t ** combat_actuel,int *vies_globales) {
    /* on suppose qu'on a déjà fait srand(time(NULL)) dans le main */
    int x, y;

    int persX = perso->x;
    int persY = perso->y;

    int dejaDeplace[TAILLE_CARTE][TAILLE_CARTE] = {0};

    for (y = 0; y < TAILLE_CARTE; y++) {

        for (x = 0; x < TAILLE_CARTE; x++) {
            monstre_t * monstre = carte[y][x].monstre;

            if (monstre != NULL && !dejaDeplace[y][x]) {
                int direction = rand() % NB_DIRECTIONS;
                int nx = x, ny = y;

                switch (direction) {

                    case HAUT:
                        ny = y - 1;
                        break;

                    case BAS:
                        ny = y + 1;
                        break;

                    case GAUCHE:
                        nx = x - 1;
                        break;

                    case DROITE:
                        nx = x + 1;
                        break;

                }

                dejaDeplace[y][x] = 1;

                if (deplacement_possible(carte, nx, ny)
                    && !case_occupee(carte, nx, ny,  persX, persY) &&
                    carte[ny][nx].sanctuaires == PAS_DE_SANCTUAIRE) {

                    // On vérifie le type du monstre actuel et le biome de destination
                    int peut_bouger = 0;

                    if (monstre->type == SQUELETTE && carte[ny][nx].biome == DESERT) {
                        peut_bouger = 1;
                    } else if (monstre->type == TROLL && carte[ny][nx].biome == FORET) {
                        peut_bouger = 1;
                    } else if (monstre->type == YETI && carte[ny][nx].biome == NEIGE) {
                        peut_bouger = 1;
                    } else if (monstre->type != SQUELETTE && monstre->type != TROLL) {
                        // Pour les monstres sans restriction de biome
                        peut_bouger = 1;
                    }

                    if (peut_bouger) {
                        carte[ny][nx].monstre = monstre;
                        carte[y][x].monstre = NULL;

                        monstre->x = nx;
                        monstre->y = ny;

                        dejaDeplace[ny][nx] = 1;
                    }

                }

            }

        }

    }

}

/* Massoud
 * Cette fonction place 100(pour le moment) sanctuaires aléatoirement
 * sur des cases libres de la carte.
 */
void placer_sanctuaires(case_t ** carte){
    int nb_sanc_places = 0;

    while (nb_sanc_places < 100) {
        int x = rand() % TAILLE_CARTE;
        int y = rand() % TAILLE_CARTE;

        /* On vérifie que la case est libre : pas d'eau, pas d'obstacle,
         * qu'il n'y a pas de bâtiment, ni de monstre,
         * et qu'elle n'a pas déjà un sanctuaire.
         */
        if (deplacement_possible(carte, x, y) &&
            carte[y][x].monstre == NULL &&
            carte[y][x].batiment.type == PAS_DE_BATIMENT &&
            carte[y][x].sanctuaires == PAS_DE_SANCTUAIRE) {

            /* Choisir un type de sanctuaire au hasard entre 1 et 4,
             * 1=PUISSANCE, 2=RAPIDITE, 3=INTELLIGENCE, 4=EXPERIENCE
             */
            carte[y][x].sanctuaires = (rand() % 4) + 1;
            nb_sanc_places++;
        }
    }
}

/* Leo
 * Cette fonction permet de déplacer le
 * personnage d'un certain décalage si
 * possible, sinon elle ne fait rien.
 */
void deplacer_perso_si_possible(perso_t * perso, case_t ** carte, int dx, int dy, int * majAffichage, int * majBrouillard) {
    int dest_x = perso->x + dx;
    int dest_y = perso->y + dy;

    if (dest_x >= 0 && dest_x < TAILLE_CARTE
        && dest_y >= 0 && dest_y < TAILLE_CARTE
        && deplacement_possible(carte, dest_x, dest_y)
        && case_occupee(carte, dest_x, dest_y, perso->x, perso->y) == FAUX
        && perso->pts_deplacements > 0)
    {
        *majAffichage = 1;
        *majBrouillard = 1;
        perso->y = dest_y;
        perso->x = dest_x;
        perso->pts_deplacements--;
    }

}
