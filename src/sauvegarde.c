#include "../lib/sauvegarde.h"

int sauvegarder_partie(const char * nomFichier, perso_t * perso, case_t ** carte) {
    FILE *fichier = fopen(nomFichier, "wb");

    if (!fichier) {
        perror("Erreur lors de l'ouverture du fichier de sauvegarde");
        return 0;
    }

    /* Sauvegarde du personnage */
    if (fwrite(perso, sizeof(perso_t), 1, fichier) != 1) {
        fprintf(stderr, "Erreur lors de l'écriture des données du personnage.\n");
        fclose(fichier);
        return 0;
    }

    /* Sauvegarde de la carte */
    for (int i = 0; i < TAILLE_CARTE; i++) {

        for (int j = 0; j < TAILLE_CARTE; j++) {
            /* Sauvegarde de la structure de base de la case */
            if (fwrite(&carte[i][j], sizeof(case_t), 1, fichier) != 1) {
                fprintf(stderr, "Erreur lors de l'écriture de la case [%d][%d].\n", i, j);
                fclose(fichier);
                return 0;
            }

        }

    }

    fclose(fichier);
    printf("Partie sauvegardée avec succès dans le fichier \"%s\"\n", nomFichier);
    return 1;
}

int charger_partie(const char * nomFichier, perso_t * perso, case_t ** carte) {
    FILE *fichier = fopen(nomFichier, "rb");

    if (!fichier) {
        perror("Erreur lors de l'ouverture du fichier de chargement");
        return 0;
    }

    /* Chargement du personnage */
    if (fread(perso, sizeof(perso_t), 1, fichier) != 1) {
        fprintf(stderr, "Erreur lors de la lecture des données du personnage.\n");
        fclose(fichier);
        return 0;
    }

    /* IMPORTANT : L'inventaire contient des pointeurs (nom, texture, fonction utiliser).
     * On les remet à zéro pour éviter d'utiliser des adresses mémoires périmées.
     */
    for (int k = 0; k < TAILLE_INVENTAIRE; k++) {
        perso->inventaire.contenu[k].nom = NULL;
        perso->inventaire.contenu[k].texture = NULL;
        perso->inventaire.contenu[k].utiliser = NULL;
    }

    /* Chargement de la carte */
    for (int i = 0; i < TAILLE_CARTE; i++) {

        for (int j = 0; j < TAILLE_CARTE; j++) {
            /* Charger les données de base de la case */
            if (fread(&carte[i][j], sizeof(case_t), 1, fichier) != 1) {
                fprintf(stderr, "Erreur lors de la lecture de la case [%d][%d].\n", i, j);
                fclose(fichier);
                return 0;
            }

            carte[i][j].monstre = NULL;
        }

    }

    fclose(fichier);
    printf("Partie chargée avec succès depuis le fichier \"%s\"\n", nomFichier);
    return 1;
}