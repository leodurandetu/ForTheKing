#include <stdio.h>
#include "couleur.h"

// Change uniquement la couleur du texte (foreground)
void couleur_texte(int code) {
    printf("\033[%dm", code);
}

// Change uniquement la couleur de fond (background)
void couleur_fond(int code) {
    printf("\033[%dm", code);
}

// Réinitialise tout (texte + fond + style)
void couleur_reset(void) {
    printf("\033[0m");
}

// Change texte ET fond en même temps
// Exemple : couleur(RED_TEXT, YELLOW_BG);
void couleur(int texte, int fond) {
    printf("\033[%d;%dm", texte, fond);
}