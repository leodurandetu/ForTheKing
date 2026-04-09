#ifndef INVENTAIRE_H
#define INVENTAIRE_H

#include "objet.h"
#include "commun.h"

/**
 * @file inventaire.h
 * @author Léo
 * @brief Gestion du système d'inventaire et du stockage des objets du personnage.
 */

/* --- Définitions --- */
#define TAILLE_INVENTAIRE 10

/**
 * @author Léo
 * @brief Structure représentant le sac à dos du personnage.
 * @details Contient un tableau statique d'objets. La gestion des emplacements 
 * vides se fait généralement par la vérification de la quantité ou du type de l'objet.
 */
typedef struct {
    objet_t contenu[TAILLE_INVENTAIRE]; /**< Tableau de stockage des objets. */
} inventaire_t;

/* --- Prototypes des fonctions --- */

/**
 * @author Léo
 * @brief Compte le nombre d'objets d'un certain type dans l'inventaire.
 * @param inventaire Pointeur vers l'inventaire cible.
 * @param objet L'instance de l'objet à compter dans le sac à dos.
 * @return int La quantité totale de ce type d'objet dans l'inventaire.
 */
int compter_objet_inventaire(inventaire_t * inventaire, objet_type_t objet_type);

/**
 * @author Léo
 * @brief Ajoute une certaine quantité à un type d'objet dans l'inventaire.
 * @param renderer Le moteur de rendu SDL.
 * @param inventaire Pointeur vers l'inventaire cible.
 * @param objet_type Le type de l'objet
 * @param quantite La quantité à ajouter
 */
void ajouter_quantite_inventaire(SDL_Renderer * renderer, inventaire_t * inventaire, objet_type_t objet_type, int quantite);

/**
 * @author Léo
 * @brief Enlève une certaine quantité à un type d'objet dans l'inventaire.
 * @param inventaire Pointeur vers l'inventaire cible.
 * @param objet_type Le type de l'objet
 * @param quantite La quantité à enlever
 */
void enlever_quantite_inventaire(inventaire_t * inventaire, objet_type_t objet_type, int quantite);

/**
 * @author Léo
 * @brief Tente d'ajouter un objet dans le premier emplacement libre de l'inventaire.
 * @param inventaire Pointeur vers l'inventaire cible.
 * @param objet L'instance de l'objet à copier dans le sac à dos.
 * @return booleen_t VRAI si l'objet a été ajouté avec succès, FAUX si l'inventaire est plein.
 */
booleen_t ajouter_objet_inventaire(inventaire_t * inventaire, objet_t objet);

/**
 * @author Léo
 * @brief Supprime un objet de l'inventaire en fonction de son type.
 * @details Utile pour retirer des objets de quête ou des consommables spécifiques sans connaître leur indice.
 * @param inventaire Pointeur vers l'inventaire cible.
 * @param objet_type La catégorie de l'objet à rechercher et à supprimer.
 * @return booleen_t VRAI si l'objet a été trouvé et supprimé, FAUX sinon.
 */
booleen_t enlever_objet_inventaire(inventaire_t * inventaire, objet_type_t objet_type);

/**
 * @author Léo
 * @brief Consomme ou utilise un objet situé à un indice précis du tableau.
 * @param inventaire Pointeur vers l'inventaire contenant l'objet.
 * @param indice_objet Position de l'objet dans le tableau (de 0 à TAILLE_INVENTAIRE - 1).
 * @param perso Pointeur vers le personnage qui utilise l'objet et subit ses effets.
 * @param maj_affichage Pointeur vers un entier permettant de notifier l'interface qu'un rafraîchissement est nécessaire.
 * @return booleen_t VRAI si l'objet existait et a pu être utilisé, FAUX sinon.
 */
booleen_t utiliser_objet_inventaire(inventaire_t * inventaire, int indice_objet, perso_t * perso, int * maj_affichage);

/**
 * @author Léo
 * @brief Prépare l'inventaire en le vidant de tout contenu (initialisation par défaut).
 * @param inventaire Pointeur vers l'inventaire à initialiser.
 */
void initialiser_inventaire(inventaire_t * inventaire);

/**
 * @author Léo
 * @brief Libère les ressources éventuellement allouées par les objets présents dans l'inventaire.
 * @details Parcourt le tableau et appelle les fonctions de destruction pour chaque objet valide.
 * @param inventaire Pointeur vers l'inventaire à nettoyer avant la suppression du personnage.
 */
void liberer_memoire_inventaire(inventaire_t * inventaire);

#endif
