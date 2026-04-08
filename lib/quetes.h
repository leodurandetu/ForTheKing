#ifndef QUETES_H
#define QUETES_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "perso.h"
#include "monstre.h"

/**
 * @def QUETE_OBJECTIF
 * @brief Nombre d'ennemis à tuer pour valider chaque quête.
 */
#define QUETE_OBJECTIF 5

/**
 * @enum quete_id_t
 * @brief Identifiant de chaque quête annexe, dans l'ordre où elles se débloquent.
 */
typedef enum {
    QUETE_TROLLS = 0,     /**< Tuer 5 trolls (+2 dégâts, +2 santé) */
    QUETE_SQUELETTES,     /**< Tuer 5 squelettes (+1 dégât, +1 santé) */
    QUETE_YETI,           /**< Tuer 5 yétis (+1 dégât, +2 santé) */
    QUETE_TOUTES_FINIES   /**< Plus aucune quête disponible */
} quete_id_t;

/**
 * @struct info_quete_t
 * @brief Structure contenant les informations de base pour une quête.
 */
typedef struct {
    const char * intitule;       /**< Libellé de la quête affiché sur le panneau */
    const char * ennemi_nom;     /**< Nom de l'ennemi ciblé par la quête */
    type_monstre_t type_cible;   /**< Type de monstre à tuer pour avancer */
    int bonus_degats;            /**< Bonus de dégâts */
    int bonus_sante;             /**< Bonus de santé */
} info_quete_t;

/**
 * @struct quetes_t
 * @brief Structure de suivi de l'avancement global des quêtes annexes.
 */
typedef struct {
    quete_id_t actuelle;  /**< Identifiant de la quête actuellement en cours */
    int nb_tues;          /**< Nombre d'ennemis ciblés déjà tués */
} quetes_t;

/**
 * @brief Initialise la structure de quêtes.
 * @param quetes Pointeur sur la structure de quêtes à initialiser.
 */
void init_quetes(quetes_t * quetes);

/**
 * @brief Met à jour la quête en cours après la mort d'un monstre.
 * @param quetes Pointeur sur la structure de quêtes.
 * @param type_monstre_tue Type du monstre qui vient d'être vaincu.
 * @return 1 si la quête vient d'être complétée, 0 sinon.
 */
int notifier_mort_monstre(quetes_t * quetes, type_monstre_t type_monstre_tue);

/**
 * @brief Affiche le panneau de quête en cours (en haut à droite).
 * @param renderer Le renderer SDL utilisé pour l'affichage.
 * @param font La police utilisée pour générer les textes.
 * @param quetes Pointeur sur la structure de quêtes.
 */
void afficher_quetes(SDL_Renderer * renderer, TTF_Font * font, const quetes_t * quetes);

/**
 * @brief Affiche le bandeau "Quête accomplie !", applique les bonus, puis passe à la suivante.
 * @param renderer Le renderer SDL utilisé pour l'affichage.
 * @param font La police utilisée pour générer les textes.
 * @param perso Pointeur sur le personnage afin d'appliquer les récompenses.
 * @param quetes Pointeur sur la structure de quêtes.
 */
void afficher_quete_accomplie(SDL_Renderer * renderer, TTF_Font * font, perso_t * perso, quetes_t * quetes);

#endif