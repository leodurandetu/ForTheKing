#include "../lib/sauvegarde.h"

int sauvegarder_partie(const char * nomFichier, perso_t * perso, case_t ** carte, SDL_Renderer * renderer) {
    FILE *fichier = fopen(nomFichier, "wb");

    if (!fichier) {
        perror("Erreur lors de l'ouverture du fichier de sauvegarde");
        return 0;
    }

    /* Sauvegarde du personnage */
    if (fwrite(&(perso->x), sizeof(int), 1, fichier) != 1) {
        fprintf(stderr, "Erreur lors de l'écriture des données du personnage.\n");
        fclose(fichier);
        return 0;
    }

    if (fwrite(&(perso->y), sizeof(int), 1, fichier) != 1) {
        fprintf(stderr, "Erreur lors de l'écriture des données du personnage.\n");
        fclose(fichier);
        return 0;
    }

    if (fwrite(&(perso->degats), sizeof(int), 1, fichier) != 1) {
        fprintf(stderr, "Erreur lors de l'écriture des données du personnage.\n");
        fclose(fichier);
        return 0;
    }

    if (fwrite(&(perso->force), sizeof(int), 1, fichier) != 1) {
        fprintf(stderr, "Erreur lors de l'écriture des données du personnage.\n");
        fclose(fichier);
        return 0;
    }

    if (fwrite(&(perso->sante), sizeof(int), 1, fichier) != 1) {
        fprintf(stderr, "Erreur lors de l'écriture des données du personnage.\n");
        fclose(fichier);
        return 0;
    }

    if (fwrite(&(perso->sante_max), sizeof(int), 1, fichier) != 1) {
        fprintf(stderr, "Erreur lors de l'écriture des données du personnage.\n");
        fclose(fichier);
        return 0;
    }

    if (fwrite(&(perso->intelligence), sizeof(int), 1, fichier) != 1) {
        fprintf(stderr, "Erreur lors de l'écriture des données du personnage.\n");
        fclose(fichier);
        return 0;
    }

    if (fwrite(&(perso->rapidite), sizeof(int), 1, fichier) != 1) {
        fprintf(stderr, "Erreur lors de l'écriture des données du personnage.\n");
        fclose(fichier);
        return 0;
    }

    if (fwrite(&(perso->evasion), sizeof(int), 1, fichier) != 1) {
        fprintf(stderr, "Erreur lors de l'écriture des données du personnage.\n");
        fclose(fichier);
        return 0;
    }

    if (fwrite(&(perso->pts_deplacements), sizeof(int), 1, fichier) != 1) {
        fprintf(stderr, "Erreur lors de l'écriture des données du personnage.\n");
        fclose(fichier);
        return 0;
    }

    if (fwrite(&(perso->niveau), sizeof(int), 1, fichier) != 1) {
        fprintf(stderr, "Erreur lors de l'écriture des données du personnage.\n");
        fclose(fichier);
        return 0;
    }

    if (fwrite(&(perso->exp), sizeof(int), 1, fichier) != 1) {
        fprintf(stderr, "Erreur lors de l'écriture des données du personnage.\n");
        fclose(fichier);
        return 0;
    }

    if (fwrite(&(perso->mort), sizeof(int), 1, fichier) != 1) {
        fprintf(stderr, "Erreur lors de l'écriture des données du personnage.\n");
        fclose(fichier);
        return 0;
    }

    if (fwrite(&(perso->inventaire), sizeof(inventaire_t), 1, fichier) != 1) {
        fprintf(stderr, "Erreur lors de l'écriture des données du personnage.\n");
        fclose(fichier);
        return 0;
    }

    if (fwrite(&(perso->type), sizeof(perso_type_t), 1, fichier) != 1) {
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
            int monstreExiste = (monstre != NULL);

            if (fwrite(&monstreExiste, sizeof(int), 1, fichier) != 1) {
                fprintf(stderr, "Erreur lors de l'écriture de la case [%d][%d].\n", i, j);
                fclose(fichier);
                return 0;
            }

            if (monstreExiste) {

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

            }

        }

    }

    fclose(fichier);
    printf("Partie sauvegardée avec succès dans le fichier \"%s\"\n", nomFichier);
    return 1;
}

int charger_partie(const char * nomFichier, perso_t * perso, case_t ** carte, SDL_Renderer * renderer) {
    FILE *fichier = fopen(nomFichier, "rb");

    if (!fichier) {
        perror("Erreur lors de l'ouverture du fichier de chargement");
        return 0;
    }

    /* Chargement du personnage */
    int x;

    if (fread(&x, sizeof(int), 1, fichier) != 1) {
        fprintf(stderr, "Erreur lors de la lecture des données du personnage.\n");
        fclose(fichier);
        return 0;
    }

    perso->x = x;
    int y;

    if (fread(&y, sizeof(int), 1, fichier) != 1) {
        fprintf(stderr, "Erreur lors de la lecture des données du personnage.\n");
        fclose(fichier);
        return 0;
    }

    perso->y = y;
    int degats;

    if (fread(&degats, sizeof(int), 1, fichier) != 1) {
        fprintf(stderr, "Erreur lors de la lecture des données du personnage.\n");
        fclose(fichier);
        return 0;
    }

    perso->degats = degats;
    int force;

    if (fread(&force, sizeof(int), 1, fichier) != 1) {
        fprintf(stderr, "Erreur lors de la lecture des données du personnage.\n");
        fclose(fichier);
        return 0;
    }

    perso->force = force;
    int sante;

    if (fread(&sante, sizeof(int), 1, fichier) != 1) {
        fprintf(stderr, "Erreur lors de la lecture des données du personnage.\n");
        fclose(fichier);
        return 0;
    }

    perso->sante = sante;
    int sante_max;

    if (fread(&sante_max, sizeof(int), 1, fichier) != 1) {
        fprintf(stderr, "Erreur lors de la lecture des données du personnage.\n");
        fclose(fichier);
        return 0;
    }

    perso->sante_max = sante_max;
    int intelligence;

    if (fread(&intelligence, sizeof(int), 1, fichier) != 1) {
        fprintf(stderr, "Erreur lors de la lecture des données du personnage.\n");
        fclose(fichier);
        return 0;
    }

    perso->intelligence = intelligence;
    int rapidite;

    if (fread(&rapidite, sizeof(int), 1, fichier) != 1) {
        fprintf(stderr, "Erreur lors de la lecture des données du personnage.\n");
        fclose(fichier);
        return 0;
    }

    perso->rapidite = rapidite;
    int evasion;

    if (fread(&evasion, sizeof(int), 1, fichier) != 1) {
        fprintf(stderr, "Erreur lors de la lecture des données du personnage.\n");
        fclose(fichier);
        return 0;
    }

    perso->evasion = evasion;
    int pts_deplacements;

    if (fread(&pts_deplacements, sizeof(int), 1, fichier) != 1) {
        fprintf(stderr, "Erreur lors de la lecture des données du personnage.\n");
        fclose(fichier);
        return 0;
    }

    perso->pts_deplacements = pts_deplacements;
    int niveau;

    if (fread(&niveau, sizeof(int), 1, fichier) != 1) {
        fprintf(stderr, "Erreur lors de la lecture des données du personnage.\n");
        fclose(fichier);
        return 0;
    }

    perso->niveau = niveau;
    int exp;

    if (fread(&exp, sizeof(int), 1, fichier) != 1) {
        fprintf(stderr, "Erreur lors de la lecture des données du personnage.\n");
        fclose(fichier);
        return 0;
    }

    perso->exp = exp;
    int mort;

    if (fread(&mort, sizeof(int), 1, fichier) != 1) {
        fprintf(stderr, "Erreur lors de la lecture des données du personnage.\n");
        fclose(fichier);
        return 0;
    }

    perso->mort = mort;
    inventaire_t inventaire;

    if (fread(&inventaire, sizeof(inventaire_t), 1, fichier) != 1) {
        fprintf(stderr, "Erreur lors de la lecture des données du personnage.\n");
        fclose(fichier);
        return 0;
    }

    perso->inventaire = inventaire;
    perso_type_t type;

    if (fread(&type, sizeof(perso_type_t), 1, fichier) != 1) {
        fprintf(stderr, "Erreur lors de la lecture des données du personnage.\n");
        fclose(fichier);
        return 0;
    }

    perso->type = type;

    for (int k = 0; k < TAILLE_INVENTAIRE; k++) {
        remplir_texture_objet(&(perso->inventaire.contenu[k]), renderer);
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

            int monstreExiste;

            if (fread(&monstreExiste, sizeof(int), 1, fichier) != 1) {
                fprintf(stderr, "Erreur lors de la lecture de la case [%d][%d].\n", i, j);
                fclose(fichier);
                return 0;
            }
            
            monstre_t * monstre = NULL;

            /* si le monstre existe */
            if (monstreExiste) {
                monstre = malloc(sizeof(monstre_t));

                type_monstre_t type_monstre;

                if (fread(&type_monstre, sizeof(type_monstre_t), 1, fichier) != 1) {
                    fprintf(stderr, "Erreur lors de la lecture de la case [%d][%d].\n", i, j);
                    fclose(fichier);
                    return 0;
                }

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
            }

            temp_case.monstre = monstre;
            carte[i][j] = temp_case;
        }

    }

    fclose(fichier);
    printf("Partie chargée avec succès depuis le fichier \"%s\"\n", nomFichier);
    return 1;
}