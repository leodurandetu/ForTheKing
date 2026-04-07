# ForTheKing

Ceci est le répertoire GitHub d'un projet de Licence 2 Informatique à l'université du Mans. Ce projet commence le 5 janvier 2026 avec le choix du sujet et se termine le 16 avril 2026, la veille d'une soutenance orale. Il s'agit d'un jeu en 2D programmé en C.

## 👥 Membres du Groupe

- MOHAMED Saandi
- DURAND Leo
- TSAMARAYEV Massoud

## 📌 Description du Jeu

Notre projet est basé sur le jeu For The King.
For The King est un jeu de rôle et d’aventure au tour par tour.

Le but du jeu est de sauver le royaume de Fahrul, plongé dans le chaos après la mort du roi. Le joueur contrôle un groupe de un à trois héros qui doivent explorer le monde, accomplir des quêtes, combattre des ennemis et vaincre une menace finale.

Le monde est généré aléatoirement à chaque partie, ce qui rend chaque aventure différente. Les déplacements se font sur une carte où l’on découvre des villes, des donjons et des événements. Les combats sont au tour par tour et reposent sur des statistiques et des jets de dés.

Le jeu comporte une dimension de survie : si tous les héros meurent, la partie s’arrête. L’objectif principal est donc d’explorer, progresser, devenir plus fort et rétablir la paix dans le royaume.

## ⚙️ Fonctionnalités détaillées

**Composition de l’équipe**  
Le joueur contrôle un personnage. Le personnage a une classe, des statistiques et un rôle différent selon le personnage choisi.

**Déplacements**  
Le personnage se déplace tour par tour sur la carte. Chaque déplacement consomme des points, et certains terrains sont plus dangereux que d’autres.

**Exploration**  
La carte est partiellement cachée au début. En se déplaçant, le joueur découvre des campements, des sanctuaires.

**Combats**  
Les combats se déroulent au tour par tour. Les attaques et les défenses se basent sur un système aléatoire et les statistiques du personnage. Il y a deux choix d'attaque : attaque lourde et légère.

**Équipement et objets**  
Le héros peut trouver des objets comme des kits de soins. Les objets peuvent être utilisés pour regagner des statistiques par exemple.

**Mort des personnages**  
Le personnage possède trois vies. Si le personnage perd ses trois vies, il meurt et la partie est perdue.

**Objectif final**  
Pour gagner, il faut vaincre le boss final afin de sauver le royaume.

## 🛠️ Outils Techniques utilisés

- 🎮 **SDL2** : Interface graphique  
- 🧠 **Langage C** : Langage de programmation
- ⚙️ **GCC** : Compilation en C 
- 🧩 **Makefile** : Compilation automatique
- ⚙️ **Valgrind** : Pour vérifier les fuites de mémoire
- 🌐 **Git** : Travail collaboratif
- 📄 **Doxygen** : Génération automatique de documentation
- 📅 **Diagramme de Gantt** : Pour planifier les tâchese
- 📑 **Latex** : Rédaction du rapport de projet

## 🚀 Comment Installer le Jeu

Remarque : ce guide est valable pour les machines Unix ou Linux.

**Initialisation du dépôt git**  
cd chemin/ou/stocker/le/projet  
git clone https://github.com/leodurandetu/ForTheKing.git  
cd ForTheKing

**Installation des librairies nécessaires**  
Ce projet utilise SDL2.  
Voici un court guide pour l'installer sur Unix ou Linux :  
sudo apt update  
sudo apt install -y libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev<br/>
_sudo apt install gcc (si vous n'avez pas installé de compilateur gcc)_

**Compilation du projet**  
Important : il faut être à la racine du projet (ForTheKing/)  
Important : pour compiler le projet, vous devez avoir le compilateur C gcc.  
make clean all

**Test du projet**  
Important : il faut être à la racine du projet (ForTheKing/)  
_./bin/menu_  : lance le menu du jeu pour choisir le personnage ou accéder aux réglages.  
_./bin/fortheking_ : lance directement le jeu avec un mage comme personnage par défaut.

## 🎮 Comment Jouer au Jeu

Si vous êtes dans le menu, cliquez sur "JOUER" uniquement.  

#### _Voici les commandes pour jouer au jeu :_

|        Touche ⌨️       | Action          | Déscription                                                                               |
| :------------------: | :-------------- | :---------------------------------------------------------------------------------------- |
|   **<kbd>Z</kbd>**   | Avancer         | Se déplacer vers l'avant                                                                  |
|   **<kbd>Q</kbd>**   | Aller à gauche  | Se déplacer vers la gauche                                                                |
|   **<kbd>S</kbd>**   | Reculer         | Se déplacer vers l'arrière                                                                |
|   **<kbd>D</kbd>**   | Aller à droite  | Se déplacer vers la droite                                                                |
|   **<kbd>N</kbd>**   | Remplir         | Réinitialiser le compteur de point de déplacement pour permettre au joueur de se déplacer |
| **<kbd>ECHAP</kbd>** | Mettre en pause | Ouvrir le menu pause                                                                      |

|            Souris  🖱️          | Action            | Déscription                                                                                                                                                                                        |
| :--------------------------: | :---------------- | :------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
|    **<kbd>Molette</kbd>**    | Zoomer / Dézoomer | La molette de la souris vous permet de zoomer/dézoomer sur la carte                                                                                                                                  |
| **<kbd>Clic Gauche</kbd>** | Se Déplacer       | Le clic gauche de la souris vous permet de vous déplacer sur la case que vous avez cliqué. <br>Vous pouvez vous déplacer uniquement sur les cases avec un carré au milieu (qui représente un point de déplacement). |


