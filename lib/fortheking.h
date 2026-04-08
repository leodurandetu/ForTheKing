#ifndef FOR_THE_KING_H
#define FOR_THE_KING_H

#include <stdio.h>
#include <math.h> 
#include <time.h>
#include <string.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include "../lib/carte.h"
#include "../lib/perso.h"
#include "../lib/affichage_infos.h"
#include "../lib/combat.h"
#include "../lib/pause_menu.h"
#include "../lib/affichage_carte.h"
#include "../lib/option.h"
#include "../lib/inventaire.h"
#include "../lib/affichage_commun.h"
#include "../lib/perlin_noise.h"
#include "../lib/ressources.h"
#include "../lib/init_sdl.h"
#include "../lib/sauvegarde.h"
#include "../lib/sanctuaire_menu.h"
#include "../lib/commun.h"
#include "../lib/quetes.h"

#define TAILLE_CASE_MAXI 250
#define TAILLE_CASE_DEPART 150
#define TAILLE_CASE_MINI 100
#define RAYON_DECOUVERTE_BROUILLARD 5

#endif
