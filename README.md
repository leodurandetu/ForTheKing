# ForTheKing

Ceci est le répertoire GitHub d'un projet de Licence 2 Informatique à l'université du Mans. Ce projet commence le 5 janvier 2026 avec le choix du sujet et se termine le 16 avril 2026, la veille d'une soutenance orale. Il s'agit d'un jeu en 2D programmé en C.

## Membres du Groupe
- MOHAMED Saandi
- DURAND Leo
- TSAMARAYEV Massoud

## Description du Jeu

Notre projet est basé sur le jeu For The King.
For The King est un jeu de rôle et d’aventure au tour par tour.  
  
Le but du jeu est de sauver le royaume de Fahrul, plongé dans le chaos après la mort du roi. Le joueur contrôle un groupe de un à trois héros qui doivent explorer le monde, accomplir des quêtes, combattre des ennemis et vaincre une menace finale.  
  
Le monde est généré aléatoirement à chaque partie, ce qui rend chaque aventure différente. Les déplacements se font sur une carte où l’on découvre des villes, des donjons et des événements. Les combats sont au tour par tour et reposent sur des statistiques et des jets de dés.  
  
Le jeu comporte une dimension de survie : si tous les héros meurent, la partie s’arrête. L’objectif principal est donc d’explorer, progresser, devenir plus fort et rétablir la paix dans le royaume.

## Fonctionnalités détaillées

**Composition de l’équipe**  
Le joueur contrôle une équipe de 1 à 3 personnages. Chaque personnage a une classe, des statistiques et un rôle différent.

**Déplacements**  
Les personnages se déplacent tour par tour sur la carte. Chaque déplacement consomme des points, et certains terrains sont plus dangereux que d’autres.

**Exploration**  
La carte est partiellement cachée au début. En se déplaçant, les joueurs découvrent des villes, des donjons, des sanctuaires et des événements aléatoires.

**Combats**  
Les combats se déroulent au tour par tour. Les attaques et défenses utilisent des jets de dés. La réussite dépend des statistiques, de l’équipement et du hasard.

**Équipement et objets**  
Les héros peuvent acheter ou trouver des armes, des armures et des objets. L’équipement améliore les statistiques et influence les combats.

**Or et ressources**  
L’or est partagé entre les héros et sert à acheter de l’équipement ou des services. Des objets de soin et de survie sont nécessaires pour continuer l’aventure.

**Menace du chaos**  
Le niveau de chaos augmente avec le temps. S’il devient trop élevé, le monde devient plus dangereux avec des ennemis plus puissants.

**Mort des personnages**  
Si un personnage meurt, il peut parfois être ressuscité. Si toute l’équipe meurt, la partie est perdue.

**Objectif final**  
Pour gagner, il faut accomplir les quêtes principales et vaincre le boss final afin de sauver le royaume.

## Outils Techniques utilisés
Nous utilisons la librairie SDL2 pour gérer l'interface graphique.  
Gcc est notre choix pour compiler les programmes en C.  
Le makefile est utilisé pour compiler un projet en plusieurs parties, et de recompiler seulement les fichiers modifiés.  
Nous utilisons également Git pour pouvoir travailler en équipe sur un projet informatique.