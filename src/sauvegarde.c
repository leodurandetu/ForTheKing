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

            if (fwrite(&(carte[i][j].sanctuaires), sizeof(type_sanc_t), 1, fichier) != 1) {
                fprintf(stderr, "Erreur lors de l'écriture de la case [%d][%d].\n", i, j);
                fclose(fichier);
                return 0;
            }

            if (fwrite(&(carte[i][j].sanctuaire_pris), sizeof(int), 1, fichier) != 1) {
                fprintf(stderr, "Erreur lors de l'écriture de la case [%d][%d].\n", i, j);
                fclose(fichier);
                return 0;
            }

            if (fwrite(&(carte[i][j].biome), sizeof(biome_t), 1, fichier) != 1) {
                fprintf(stderr, "Erreur lors de l'écriture de la case [%d][%d].\n", i, j);
                fclose(fichier);
                return 0;
            }

            if (fwrite(&(carte[i][j].terrain), sizeof(terrain_t), 1, fichier) != 1) {
                fprintf(stderr, "Erreur lors de l'écriture de la case [%d][%d].\n", i, j);
                fclose(fichier);
                return 0;
            }

            if (fwrite(&(carte[i][j].batiment), sizeof(batiment_t), 1, fichier) != 1) {
                fprintf(stderr, "Erreur lors de l'écriture de la case [%d][%d].\n", i, j);
                fclose(fichier);
                return 0;
            }

            if (fwrite(&(carte[i][j].estVisible), sizeof(int), 1, fichier) != 1) {
                fprintf(stderr, "Erreur lors de l'écriture de la case [%d][%d].\n", i, j);
                fclose(fichier);
                return 0;
            }

            /* Sauvegarde du monstre sur la case */
            monstre_t * monstre = carte[i][j].monstre;

            if (monstre != NULL) {

                if (fwrite(&(monstre->type), sizeof(type_monstre_t), 1, fichier) != 1) {
                    fprintf(stderr, "Erreur lors de l'écriture de la case [%d][%d].\n", i, j);
                    fclose(fichier);
                    return 0;
                }

                if (fwrite(&(monstre->x), sizeof(int), 1, fichier) != 1) {
                    fprintf(stderr, "Erreur lors de l'écriture de la case [%d][%d].\n", i, j);
                    fclose(fichier);
                    return 0;
                }

                if (fwrite(&(monstre->y), sizeof(int), 1, fichier) != 1) {
                    fprintf(stderr, "Erreur lors de l'écriture de la case [%d][%d].\n", i, j);
                    fclose(fichier);
                    return 0;
                }

                if (fwrite(&(monstre->degats), sizeof(int), 1, fichier) != 1) {
                    fprintf(stderr, "Erreur lors de l'écriture de la case [%d][%d].\n", i, j);
                    fclose(fichier);
                    return 0;
                }

                if (fwrite(&(monstre->force), sizeof(int), 1, fichier) != 1) {
                    fprintf(stderr, "Erreur lors de l'écriture de la case [%d][%d].\n", i, j);
                    fclose(fichier);
                    return 0;
                }

                if (fwrite(&(monstre->sante), sizeof(int), 1, fichier) != 1) {
                    fprintf(stderr, "Erreur lors de l'écriture de la case [%d][%d].\n", i, j);
                    fclose(fichier);
                    return 0;
                }

                if (fwrite(&(monstre->sante_max), sizeof(int), 1, fichier) != 1) {
                    fprintf(stderr, "Erreur lors de l'écriture de la case [%d][%d].\n", i, j);
                    fclose(fichier);
                    return 0;
                }

                if (fwrite(&(monstre->intelligence), sizeof(int), 1, fichier) != 1) {
                    fprintf(stderr, "Erreur lors de l'écriture de la case [%d][%d].\n", i, j);
                    fclose(fichier);
                    return 0;
                }

                if (fwrite(&(monstre->rapidite), sizeof(int), 1, fichier) != 1) {
                    fprintf(stderr, "Erreur lors de l'écriture de la case [%d][%d].\n", i, j);
                    fclose(fichier);
                    return 0;
                }

                if (fwrite(&(monstre->evasion), sizeof(int), 1, fichier) != 1) {
                    fprintf(stderr, "Erreur lors de l'écriture de la case [%d][%d].\n", i, j);
                    fclose(fichier);
                    return 0;
                }

                if (fwrite(&(monstre->niveau), sizeof(int), 1, fichier) != 1) {
                    fprintf(stderr, "Erreur lors de l'écriture de la case [%d][%d].\n", i, j);
                    fclose(fichier);
                    return 0;
                }

            } else {
                /* type du monstre */
                type_monstre_t typeMonstre = PAS_DE_MONSTRE;

                if (fwrite(&(typeMonstre), sizeof(type_monstre_t), 1, fichier) != 1) {
                    fprintf(stderr, "Erreur lors de l'écriture de la case [%d][%d].\n", i, j);
                    fclose(fichier);
                    return 0;
                }

                /* x */
                int x = 0;

                if (fwrite(&(x), sizeof(int), 1, fichier) != 1) {
                    fprintf(stderr, "Erreur lors de l'écriture de la case [%d][%d].\n", i, j);
                    fclose(fichier);
                    return 0;
                }

                /* y */
                int y = 0;

                if (fwrite(&(y), sizeof(int), 1, fichier) != 1) {
                    fprintf(stderr, "Erreur lors de l'écriture de la case [%d][%d].\n", i, j);
                    fclose(fichier);
                    return 0;
                }

                /* degats */
                int degats = 0;

                if (fwrite(&(degats), sizeof(int), 1, fichier) != 1) {
                    fprintf(stderr, "Erreur lors de l'écriture de la case [%d][%d].\n", i, j);
                    fclose(fichier);
                    return 0;
                }

                /* force */
                int force = 0;

                if (fwrite(&(force), sizeof(int), 1, fichier) != 1) {
                    fprintf(stderr, "Erreur lors de l'écriture de la case [%d][%d].\n", i, j);
                    fclose(fichier);
                    return 0;
                }

                /* sante */
                int sante = 0;

                if (fwrite(&(sante), sizeof(int), 1, fichier) != 1) {
                    fprintf(stderr, "Erreur lors de l'écriture de la case [%d][%d].\n", i, j);
                    fclose(fichier);
                    return 0;
                }

                /* sante max */
                int sante_max = 0;

                if (fwrite(&(sante_max), sizeof(int), 1, fichier) != 1) {
                    fprintf(stderr, "Erreur lors de l'écriture de la case [%d][%d].\n", i, j);
                    fclose(fichier);
                    return 0;
                }

                /* intelligence */
                int intelligence = 0;

                if (fwrite(&(intelligence), sizeof(int), 1, fichier) != 1) {
                    fprintf(stderr, "Erreur lors de l'écriture de la case [%d][%d].\n", i, j);
                    fclose(fichier);
                    return 0;
                }

                /* rapidite */
                int rapidite = 0;

                if (fwrite(&(rapidite), sizeof(int), 1, fichier) != 1) {
                    fprintf(stderr, "Erreur lors de l'écriture de la case [%d][%d].\n", i, j);
                    fclose(fichier);
                    return 0;
                }

                /* evasion */
                int evasion = 0;

                if (fwrite(&(evasion), sizeof(int), 1, fichier) != 1) {
                    fprintf(stderr, "Erreur lors de l'écriture de la case [%d][%d].\n", i, j);
                    fclose(fichier);
                    return 0;
                }

                /* niveau */
                int niveau = 1;

                if (fwrite(&(niveau), sizeof(int), 1, fichier) != 1) {
                    fprintf(stderr, "Erreur lors de l'écriture de la case [%d][%d].\n", i, j);
                    fclose(fichier);
                    return 0;
                }

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
            case_t temp_case;

            type_sanc_t sanctuaires;
            
            if (fread(&sanctuaires, sizeof(type_sanc_t), 1, fichier) != 1) {
                fprintf(stderr, "Erreur lors de la lecture de la case [%d][%d].\n", i, j);
                fclose(fichier);
                return 0;
            }

            temp_case.sanctuaires = sanctuaires;
            int sanctuairePris;
            
            if (fread(&sanctuairePris, sizeof(int), 1, fichier) != 1) {
                fprintf(stderr, "Erreur lors de la lecture de la case [%d][%d].\n", i, j);
                fclose(fichier);
                return 0;
            }

            temp_case.sanctuaire_pris = sanctuairePris;
            biome_t biome;
            
            if (fread(&biome, sizeof(biome_t), 1, fichier) != 1) {
                fprintf(stderr, "Erreur lors de la lecture de la case [%d][%d].\n", i, j);
                fclose(fichier);
                return 0;
            }

            temp_case.biome = biome;
            terrain_t terrain;
            
            if (fread(&terrain, sizeof(terrain_t), 1, fichier) != 1) {
                fprintf(stderr, "Erreur lors de la lecture de la case [%d][%d].\n", i, j);
                fclose(fichier);
                return 0;
            }

            temp_case.terrain = terrain;
            batiment_t batiment;
            
            if (fread(&batiment, sizeof(batiment_t), 1, fichier) != 1) {
                fprintf(stderr, "Erreur lors de la lecture de la case [%d][%d].\n", i, j);
                fclose(fichier);
                return 0;
            }

            temp_case.batiment = batiment;
            int estVisible;
            
            if (fread(&estVisible, sizeof(int), 1, fichier) != 1) {
                fprintf(stderr, "Erreur lors de la lecture de la case [%d][%d].\n", i, j);
                fclose(fichier);
                return 0;
            }

            temp_case.estVisible = estVisible;

            type_monstre_t type_monstre;

            if (fread(&type_monstre, sizeof(type_monstre_t), 1, fichier) != 1) {
                fprintf(stderr, "Erreur lors de la lecture de la case [%d][%d].\n", i, j);
                fclose(fichier);
                return 0;
            }
            
            monstre_t * monstre = NULL;

            /* si le monstre existe */
            if (type_monstre != PAS_DE_MONSTRE) {
                monstre = malloc(sizeof(monstre_t));

                monstre->type = type_monstre;

                int x;
                
                if (fread(&x, sizeof(int), 1, fichier) != 1) {
                    fprintf(stderr, "Erreur lors de la lecture de la case [%d][%d].\n", i, j);
                    fclose(fichier);
                    return 0;
                }

                monstre->x = x;
                int y;
                
                if (fread(&y, sizeof(int), 1, fichier) != 1) {
                    fprintf(stderr, "Erreur lors de la lecture de la case [%d][%d].\n", i, j);
                    fclose(fichier);
                    return 0;
                }

                monstre->y = y;
                int degats;
                
                if (fread(&degats, sizeof(int), 1, fichier) != 1) {
                    fprintf(stderr, "Erreur lors de la lecture de la case [%d][%d].\n", i, j);
                    fclose(fichier);
                    return 0;
                }

                monstre->degats = degats;
                int force;
                
                if (fread(&force, sizeof(int), 1, fichier) != 1) {
                    fprintf(stderr, "Erreur lors de la lecture de la case [%d][%d].\n", i, j);
                    fclose(fichier);
                    return 0;
                }

                monstre->force = force;
                int sante;
                
                if (fread(&sante, sizeof(int), 1, fichier) != 1) {
                    fprintf(stderr, "Erreur lors de la lecture de la case [%d][%d].\n", i, j);
                    fclose(fichier);
                    return 0;
                }

                monstre->sante = sante;
                int sante_max;
                
                if (fread(&sante_max, sizeof(int), 1, fichier) != 1) {
                    fprintf(stderr, "Erreur lors de la lecture de la case [%d][%d].\n", i, j);
                    fclose(fichier);
                    return 0;
                }

                monstre->sante_max = sante_max;
                int intelligence;
                
                if (fread(&intelligence, sizeof(int), 1, fichier) != 1) {
                    fprintf(stderr, "Erreur lors de la lecture de la case [%d][%d].\n", i, j);
                    fclose(fichier);
                    return 0;
                }

                monstre->intelligence = intelligence;
                int rapidite;
                
                if (fread(&rapidite, sizeof(int), 1, fichier) != 1) {
                    fprintf(stderr, "Erreur lors de la lecture de la case [%d][%d].\n", i, j);
                    fclose(fichier);
                    return 0;
                }

                monstre->rapidite = rapidite;
                int evasion;
                
                if (fread(&evasion, sizeof(int), 1, fichier) != 1) {
                    fprintf(stderr, "Erreur lors de la lecture de la case [%d][%d].\n", i, j);
                    fclose(fichier);
                    return 0;
                }

                monstre->evasion = evasion;
                int niveau;
                
                if (fread(&niveau, sizeof(int), 1, fichier) != 1) {
                    fprintf(stderr, "Erreur lors de la lecture de la case [%d][%d].\n", i, j);
                    fclose(fichier);
                    return 0;
                }

                monstre->niveau = niveau;
            } else {
                monstre = NULL;

                int tmp;

                for (int k = 0; k < 10; k++) {
                    fread(&tmp, sizeof(int), 1, fichier);
                }

            }

            temp_case.monstre = monstre;
            carte[i][j] = temp_case;
        }

    }

    fclose(fichier);
    printf("Partie chargée avec succès depuis le fichier \"%s\"\n", nomFichier);
    return 1;
}