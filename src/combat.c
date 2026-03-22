#include "../lib/combat.h"

/* Leo
 * Cette fonction permet d'ouvrir
 * une fenêtre de combat, et donc
 * également de commencer un combat
 * en parallèle.
 */
void ouvrir_fenetre_combat(SDL_Renderer * rendererPrincipal, combat_t ** combat, case_t carte[TAILLE_CARTE][TAILLE_CARTE],int *vies_globales) {
    (*combat)->pFenetre = (SDL_CreateWindow("For The King - Combat", 
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            640, 480, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE));

    if (!((*combat)->pFenetre)) {
        fprintf(stderr, "Erreur fenêtre : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // Création du moteur de rendu (accéléré par la carte graphique)
    (*combat)->renderer = (SDL_CreateRenderer((*combat)->pFenetre, -1, SDL_RENDERER_ACCELERATED));
    if (!((*combat)->renderer)) {
        fprintf(stderr, "Erreur renderer : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // Chargement du sprite du personnage
    (*combat)->texture_perso = NULL;
    SDL_Surface *img_perso = IMG_Load("img/mage.png");
    if (img_perso) {
        // Rend la couleur blanche (255, 255, 255) transparente
        SDL_SetColorKey(img_perso, SDL_TRUE, SDL_MapRGB(img_perso->format, 255, 255, 255));
        (*combat)->texture_perso = (SDL_CreateTextureFromSurface((*combat)->renderer, img_perso));
        SDL_SetTextureBlendMode((*combat)->texture_perso, SDL_BLENDMODE_BLEND);
        SDL_FreeSurface(img_perso);
    }
 
    // Chargement du sprite du monstre en fonction de son type(troll et squelette pour le moment)
    (*combat)->texture_monstre = NULL;
    SDL_Surface *img_monstre = NULL;

    if ((*combat)->monstre->type == SQUELETTE) {
        img_monstre = IMG_Load("img/squelette.png");
    } else if ((*combat)->monstre->type == TROLL) {
        img_monstre = IMG_Load("img/troll.png");
    }

    if (img_monstre) {
        // SDL_SetColorKey(img_monstre, SDL_TRUE, SDL_MapRGB(img_monstre->format, 255, 255, 255));
        (*combat)->texture_monstre = SDL_CreateTextureFromSurface((*combat)->renderer, img_monstre);
        SDL_SetTextureBlendMode((*combat)->texture_monstre, SDL_BLENDMODE_BLEND);
        SDL_FreeSurface(img_monstre);
    } else {
        fprintf(stderr, "Erreur chargement image monstre : %s\n", IMG_GetError());
    }
    
    // Chargement du sprite du fond d'écran
    (*combat)->texture_fond_ecran = NULL;
    SDL_Surface *img_fond_foret = IMG_Load("img/fond_foret.png");
    if (img_fond_foret) {
        (*combat)->texture_fond_ecran = (SDL_CreateTextureFromSurface((*combat)->renderer, img_fond_foret));
        SDL_SetTextureBlendMode((*combat)->texture_fond_ecran, SDL_BLENDMODE_BLEND);
        SDL_FreeSurface(img_fond_foret);
    }

    // Chargement des symboles de l'attaque legère
    (*combat)->texture_attaque_legere = NULL;
    SDL_Surface *img_legere = IMG_Load("img/attaque_l.png"); 
    if (img_legere) {
        (*combat)->texture_attaque_legere = SDL_CreateTextureFromSurface((*combat)->renderer, img_legere);
        SDL_SetTextureBlendMode((*combat)->texture_attaque_legere, SDL_BLENDMODE_BLEND);
        SDL_FreeSurface(img_legere);
    }

    // Chargement des symboles de l'attaque lourde
    (*combat)->texture_attaque_lourde = NULL;
    SDL_Surface *img_lourde = IMG_Load("img/attaque_L.png"); 
    if (img_lourde) {
        (*combat)->texture_attaque_lourde = SDL_CreateTextureFromSurface((*combat)->renderer, img_lourde);
        SDL_SetTextureBlendMode((*combat)->texture_attaque_lourde, SDL_BLENDMODE_BLEND);
        SDL_FreeSurface(img_lourde);
    }

    TTF_Init();
    (*combat)->font = TTF_OpenFont("Fonts/Enchanted Land.otf", 24);

    SDL_Color blanc = {255, 255, 255, 255}; // blanc

    // Texte bouton léger
    SDL_Surface *surf_leger = TTF_RenderUTF8_Blended((*combat)->font, "Attaque Légère", blanc);

    if (surf_leger) {
        (*combat)->texture_texte_leger = SDL_CreateTextureFromSurface((*combat)->renderer, surf_leger);
        (*combat)->rect_texte_leger.w = surf_leger->w;
        (*combat)->rect_texte_leger.h = surf_leger->h;
        SDL_FreeSurface(surf_leger);
    }

    // Texte bouton lourd
    SDL_Surface *surf_lourd = TTF_RenderUTF8_Blended((*combat)->font, "Attaque Lourde", blanc);
    if (surf_lourd) {
        (*combat)->texture_texte_lourd = SDL_CreateTextureFromSurface((*combat)->renderer, surf_lourd);
        (*combat)->rect_texte_lourd.w = surf_lourd->w;
        (*combat)->rect_texte_lourd.h = surf_lourd->h;
        SDL_FreeSurface(surf_lourd);
    }

    vainqueur_t vainqueur = PAS_DE_VAINQUEUR;
    int running = 1;
    int majAffichage = 1;
    int clic_gauche = 0;
    (*combat)->tour = TOUR_JOUEUR;
    SDL_Point point;

    while (running) {
        SDL_Event e;

        while (SDL_PollEvent(&e)) {

            switch (e.type) {
                case SDL_QUIT:
                    running = 0;
                    break;

                case SDL_MOUSEMOTION:
                    point.x = e.motion.x;
                    point.y = e.motion.y;

                    (*combat)->survole_bouton_leger = SDL_PointInRect(&point, &((*combat)->bouton_leger));
                    (*combat)->survole_bouton_lourd = SDL_PointInRect(&point, &((*combat)->bouton_lourd));

                    majAffichage = 1;
                    break;

                case SDL_MOUSEBUTTONDOWN:

                    if (e.button.button == SDL_BUTTON_LEFT) {
                        clic_gauche = 1;
                    }

                    point.x = e.button.x;
                    point.y = e.button.y;

                    if((*combat)->tour == TOUR_JOUEUR){
                        if (SDL_PointInRect(&point, &((*combat)->bouton_leger))) {
                            printf("Attaque légère\n");
                            attaque_legere(*combat);
                            (*combat)->tour = changer_tour(*combat);
                            printf("Le joueur a joué\n");
                            printf("santé monstre : %d",(*combat)->monstre->sante);
                            

                        } else if (SDL_PointInRect(&point, &((*combat)->bouton_lourd))) {
                            printf("Attaque lourde\n");
                            attaque_lourde(*combat);
                            (*combat)->tour = changer_tour(*combat);
                            printf("Le joueur a joué\n");
                            printf("santé monstre : %d",(*combat)->monstre->sante);
                        }

                    }

                    if ((*combat)->tour == TOUR_MONSTRE) {
                        printf("\nTour du monstre\n");
                        choix_attaque_monstre(*combat);
                        (*combat)->tour = changer_tour(*combat);
                        printf("Le monstre a joué\n");
                        printf("Santé joueur : %d\n",(*combat)->perso->sante);

                        majAffichage = 1;
                    }

                    /* Vérification de la mort du joueur */
                    if((*combat)->perso->sante <= 0) {
                        
                        (*vies_globales)--; 

                        if (*vies_globales >= 0) {
                            // S'il reste des vies, on ressuscite directement dans le combat
                            afficher_message_combat(*combat, "Une vie est utilisee...");
                            SDL_Delay(1500);
                            
                            // On lui redonne ses 75% de PV max
                            (*combat)->perso->sante = (int)(0.75f * (*combat)->perso->sante_max);
                            
                            majAffichage = 1; 
                        } else {
                            // Plus de vies du tout : Fin de l'aventure
                            afficher_message_combat(*combat, "Combat terminé.");
                            SDL_Delay(3000);
                            
                            vainqueur = MONSTRE_VAINQUEUR;
                            running = 0; // on ferme la fenêtre
                        }
                    } 
                    /* Vérification de la mort du monstre */
                    else if((*combat)->monstre->sante <= 0) {
                        vainqueur = JOUEUR_VAINQUEUR;
                        running = 0;
                    }
                    
                    break;

                case SDL_WINDOWEVENT:

                    if (e.window.event == SDL_WINDOWEVENT_RESIZED
                        || e.window.event == SDL_WINDOWEVENT_EXPOSED) {
                        majAffichage = 1;
                    }

                    if (e.window.event == SDL_WINDOWEVENT_CLOSE) {
                        running = 0;
                    }

                    break;

            }

        }

        if (majAffichage) {
            maj_affichage_fenetre_combat(*combat, clic_gauche);
            majAffichage = 0;
        }

        clic_gauche = 0;
    }

    combat_termine(rendererPrincipal, combat, carte, vainqueur,vies_globales);
    detruire_fenetre_combat(combat);
}

/* Leo
 * Cette fonction permet de mettre à
 * jour l'XP du personnage après
 * un combat ainsi que supprimer le
 * monstre sur la carte si il a perdu
 */
void combat_termine(SDL_Renderer * rendererPrincipal, combat_t ** combat, case_t carte[TAILLE_CARTE][TAILLE_CARTE], vainqueur_t vainqueur,int *vies_globales) {
    perso_t * perso = (*combat)->perso;

    /* si le joueur est le vainqueur,
     * on supprime le monstre de la carte,
     * et on lui donne de l'XP
     */
    if (vainqueur == JOUEUR_VAINQUEUR) {
        monstre_t * combat_monstre = (*combat)->monstre;
        int y = combat_monstre->y;
        int x = combat_monstre->x;

        if (combat_monstre != NULL) {
            monstre_t * monstre = carte[y][x].monstre;

            if (monstre != NULL) {
                carte[y][x].batiment.type = TOMBE;
                carte[y][x].monstre = NULL;
                free(monstre);
            }

        }

        monstre_tue(perso);

        objet_t kit_de_soins = creer_kit_de_soins(rendererPrincipal);
        ajouter_objet_inventaire(&(perso->inventaire), kit_de_soins);
    } else if (vainqueur == MONSTRE_VAINQUEUR || perso->sante <= 0) {

            // Si on arrive ici, c'est que le combat est perdu et qu'il n'y a plus de vies.
            // La résurrection est gérée directement pendant le combat.
            perso->sante = 0;
            perso->mort = 1; 
        }

}

/* Leo
 * Cette fonction libère la mémoire d'un combat,
 * et de sa fenêtre et des éléments graphiques.
 */
void detruire_fenetre_combat(combat_t ** combat) {

    if (combat != NULL && *combat != NULL) {

        if ((*combat)->texture_attaque_legere != NULL) {
            SDL_DestroyTexture((*combat)->texture_attaque_legere);
            (*combat)->texture_attaque_legere = NULL;
        }

        if ((*combat)->texture_attaque_lourde != NULL) {
            SDL_DestroyTexture((*combat)->texture_attaque_lourde);
            (*combat)->texture_attaque_lourde = NULL;
        }

        if ((*combat)->texture_monstre != NULL) {
            SDL_DestroyTexture((*combat)->texture_monstre);
            (*combat)->texture_monstre = NULL;
        }

        if ((*combat)->texture_perso != NULL) {
            SDL_DestroyTexture((*combat)->texture_perso);
            (*combat)->texture_perso = NULL;
        }

        if ((*combat)->texture_fond_ecran != NULL) {
            SDL_DestroyTexture((*combat)->texture_fond_ecran);
            (*combat)->texture_fond_ecran = NULL;
        }

        if (((*combat)->renderer) != NULL) {
            SDL_DestroyRenderer((*combat)->renderer);
            ((*combat)->renderer) = NULL;
        }

        if (((*combat)->pFenetre) != NULL) {
            SDL_DestroyWindow((*combat)->pFenetre);
            ((*combat)->pFenetre) = NULL;
        }

        if (((*combat)->texture_texte_leger) != NULL) {
            SDL_DestroyTexture((*combat)->texture_texte_leger);
            (*combat)->texture_texte_leger = NULL;
        }

        if (((*combat)->texture_texte_lourd) != NULL) {
            SDL_DestroyTexture((*combat)->texture_texte_lourd);
            (*combat)->texture_texte_lourd = NULL;
        }

        if ((*combat)->font != NULL) {
            TTF_CloseFont((*combat)->font);
            (*combat)->font = NULL;
        }

        free(*combat);
        *combat = NULL;
    }

}



/* Massoud 
 * Cette fonction est faite pour alterner le tour.
*/
tour_t changer_tour(combat_t *combat){
    return (combat->tour == TOUR_JOUEUR) ? TOUR_MONSTRE : TOUR_JOUEUR;
}


/* Massoud
 * Fonctions d'attaque du combat.
 * Chaque attaque génère un pourcentage aléatoire, puis applique
 * ce pourcentage aux dégâts de base de l'attaquant pour calculer
 * les dégâts réels infligés à la cible.
 * La santé de la cible est ensuite diminuée, sans jamais descendre sous 0.
 *
 * Attaque légère :
 *   - Joueur  : entre 20 % et 40 % des dégâts.
 *   - Monstre : entre 5 % et 30 % des dégâts.
 *
 * Attaque lourde :
 *   - Joueur  : entre 30 % et 60 % des dégâts.
 *   - Monstre : entre 20 % et 50 % des dégâts.
 */

void attaque_legere(combat_t *combat){
    int pourcentage = 20 + rand() % 21; // 20% à 40%
    int vrai_degats = (pourcentage * combat->perso->degats)/100;

    combat->monstre->sante -= vrai_degats;

    if (combat->monstre->sante < 0){
        combat->monstre->sante = 0;
    }
}

void attaque_lourde(combat_t *combat){
    int pourcentage = 30 + rand() % 31; // 30% à 60%
    int vrai_degats = (pourcentage * combat->perso->degats)/100;

    combat->monstre->sante -= vrai_degats;

    if (combat->monstre->sante < 0){
        combat->monstre->sante = 0;
    }
}

void attaque_legere_monstre(combat_t *combat){
    int pourcentage = 5 + rand() % 26; // 5% à 30%
    int vrai_degats = (pourcentage * combat->monstre->degats)/100;

    combat->perso->sante -= vrai_degats;

    if(combat->perso->sante < 0){
        combat->perso->sante = 0;
    }
}

void attaque_lourde_monstre(combat_t *combat){
    int pourcentage = 20 + rand() % 31; // 20% à 50%
    int vrai_degats = (pourcentage * combat->monstre->degats)/100;

    combat->perso->sante -= vrai_degats;

    if(combat->perso->sante < 0){
        combat->perso->sante = 0;
    }
}

/* Massoud
 * Cette fonction choisit l'attaque du monstre en fonction de la situation
 * du combat, en gardant une part de hasard pour rendre le comportement
 * moins prévisible.
 *
 * - Si le monstre a peu de vie (≤ 30 % de sa santé maximale), il devient
 *   plus agressif et privilégie l'attaque lourde.
 * - Si le joueur a beaucoup de vie (> 60 % de sa santé maximale), le
 *   monstre attaque fort (préférence pour l'attaque lourde).
 * - Si le joueur a peu de vie (≤ 30 % de sa santé maximale), le monstre
 *   agit plus prudemment (préférence pour l'attaque légère).
 * - Dans tous les autres cas, le monstre choisit de façon équilibrée
 *   entre attaque légère et lourde, avec 50 % de chance pour chacune.
 */
void choix_attaque_monstre(combat_t *combat){
    int hasard = rand() % 100;

    int sante_joueur = combat->perso->sante;
    int sante_max_joueur = combat->perso->sante_max;

    int sante_monstre = combat->monstre->sante;
    int sante_max_monstre = combat->monstre->sante_max;

    /* Si le monstre a peu de vie -> il devient agressif */
    if(sante_monstre <= (sante_max_monstre * 30) / 100){
        if(hasard < 80){
            attaque_lourde_monstre(combat);
        } else {
            attaque_legere_monstre(combat);
        }
    }

    /* Si le joueur a beaucoup de vie -> le monstre attaque fort */
    else if(sante_joueur > (sante_max_joueur * 60) / 100){
        if(hasard < 70){
            attaque_lourde_monstre(combat);
        } else {
            attaque_legere_monstre(combat);
        }
    }

    /* Si le joueur a peu de vie -> le monstre joue plus prudemment */
    else if(sante_joueur <= (sante_max_joueur * 30) / 100){
        if(hasard < 70){
            attaque_legere_monstre(combat);
        } else {
            attaque_lourde_monstre(combat);
        }
    }

    /* Comportement équilibré */
    else {
        if(hasard < 50){
            attaque_legere_monstre(combat);
        } else {
            attaque_lourde_monstre(combat);
        }
    }
}
/*Saandi*/
/**
* Fonction qui creer un combat
*/
combat_t* creer_combat(perso_t *perso, monstre_t *monstre) {
    combat_t *combat = malloc(sizeof(combat_t));

    combat->perso = perso;
    combat->monstre = monstre;
    combat->pFenetre = NULL;
    combat->renderer = NULL;
    combat->texture_monstre = NULL;
    combat->texture_perso = NULL;
    combat->texture_fond_ecran = NULL;

    return combat;
}

void detruire_combat(combat_t ** combat) {
    free(*combat);
    *combat = NULL;
}
