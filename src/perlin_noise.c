#include <math.h>
#include <stdlib.h>
#include "../lib/perlin_noise.h"


/* Massoud */

/* Table de permutation ; 256 valeurs mélangées */
static int p[512];

/* Calcule la courbe de lissage pour des transitions douces */
static float fade(float t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

/* Interpolation linéaire entre a et b */
static float lerp(float t, float a, float b) {
    return a + t * (b - a);
}

/* Génère un gradient pseudo-aléatoire selon le hash */
static float grad(int hash, float x, float y) {
    switch (hash & 3) {
        case 0: return  x + y;
        case 1: return -x + y;
        case 2: return  x - y;
        case 3: return -x - y;
        default: return 0;
    }
}

/* Initialise la table de permutation avec une graine (mélange Fisher-Yates) */
void perlin_init(unsigned int graine) {
    srand(graine);
    int perm[256];
    for (int i = 0; i < 256; i++) perm[i] = i;

    // Mélange des valeurs
    for (int i = 255; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = perm[i]; perm[i] = perm[j]; perm[j] = tmp;
    }
    
    // Remplit la table doublée
    for (int i = 0; i < 512; i++) p[i] = perm[i & 255];
}

/* Génère une valeur de bruit cohérent entre -1.0 et 1.0 pour les coordonnées x,y */
float perlin_2d(float x, float y) {
    // Trouve les coordonnées de la cellule de la grille
    int xi = (int)floorf(x) & 255;
    int yi = (int)floorf(y) & 255;
    
    // Coordonnées relatives dans la cellule [0,1]
    float xf = x - floorf(x);
    float yf = y - floorf(y);
    
    // Application des coefficients de lissage
    float u = fade(xf);
    float v = fade(yf);

    // Hachage des coordonnées des 4 coins du carré
    int aa = p[p[xi] + yi];
    int ab = p[p[xi] + yi + 1];
    int ba = p[p[xi + 1] + yi];
    int bb = p[p[xi + 1] + yi + 1];

    // Combinaison des résultats par interpolation bilinéaire
    return lerp(v,
        lerp(u, grad(aa, xf, yf), grad(ba, xf-1, yf)),
        lerp(u, grad(ab, xf, yf-1), grad(bb, xf-1, yf-1))
    );
}