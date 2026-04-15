# ForTheKing

Ceci est le répertoire GitHub d'un projet de Licence 2 Informatique à l'université du Mans. Ce projet commence le 5 janvier 2026 avec le choix du sujet et se termine le 16 avril 2026, la veille d'une soutenance orale. Il s'agit d'un jeu en 2D programmé en C.

## 👥 Membres du Groupe

- MOHAMED Saandi
- DURAND Leo
- TSAMARAYEV Massoud

## 📌 Description du Jeu

Notre projet est basé sur le jeu For The King.
For The King est un jeu de rôle et d’aventure au tour par tour.

Le but du jeu est de sauver le royaume de Fahrul, plongé dans le chaos après la mort du roi. Le joueur contrôle un personnage qui doit explorer le monde, accomplir des quêtes, combattre des ennemis et vaincre une menace finale.

Le monde est généré aléatoirement à chaque partie, ce qui rend chaque aventure différente. Les déplacements se font sur une carte où l’on découvre des villes, des donjons et des événements. Les combats sont au tour par tour et reposent sur des statistiques et des jets de dés.

Le jeu comporte une dimension de survie : si le personnage meurt, la partie s’arrête. L’objectif principal est donc d’explorer, progresser, devenir plus fort et rétablir la paix dans le royaume.

## ⚙️ Fonctionnalités détaillées

**Composition de l’équipe**  
Le joueur contrôle un personnage. Le personnage a des statistiques (points de vie, dégâts, force etc.) différentes selon la classe choisie.

**Déplacements**  
Le personnage se déplace tour par tour sur la carte. Chaque déplacement consomme des points de déplacement. Les cases où on peut se déplacer ont un carré noir au milieu. Il faut passer son tour pour récupérer des points. Sur certaines cases comme l'eau, la boue, la montagne et les arbres on ne peut pas se déplacer.

**Exploration**  
La carte est partiellement cachée au début par un brouillard. En se déplaçant, le joueur découvre des campements et des sanctuaires.

**Combats**  
Les combats se déroulent au tour par tour. Les attaques et les défenses se basent sur un système aléatoire et les statistiques du personnage. Il y a deux choix d'attaque : attaque lourde et légère. Le personnage a deux chance sur trois de réussir à infliger une attaque lourde au monstre.

**Équipement et objets**  
Le héros peut trouver des objets comme des kits de soins. Les objets peuvent être utilisés pour regagner des statistiques pour le personnage. Il peut aussi augmenter
ses statistiques (dégâts, force, intélligence, rapidité et expérience) en récupérant des points de chaque statistique dans différents sanctuaires. Le joueur a la possibilité de se reposer dans des campements afin de récupérer soit une partie de ses points de vie ou soit la totalité. Pour cela il devra dépenser des pièces d'or.

**Expérience et niveau**  
Le personnage gagne de l'expérience en tuant des monstres ou en faisant des quêtes.
Une fois la barre d'expérience pleine, le personnage gagne un niveau.
Ce gain de niveau fait gagner des statistiques bonus.

**Statistiques des personnages**  
- Force (En rouge) : augmente les dégâts (5 Force = 1 Dégât)
- Intelligence (En bleu) : pas implémenté
- Rapidité (En vert) : pas implémenté
- Evasion (En jaune/or) : pas implémenté
- Barre rouge : santé du personnage
- Barre bleue : expérience du personnage

**Mort des personnages**  
Le personnage possède trois vies. Si le personnage perd ses trois vies, il meurt et la partie est perdue.

**Objectif final**  
Pour gagner, il faut vaincre le boss final afin de sauver le royaume. 
Ce boss final peut être attaqué dans une des 4 tours situées proches des quatres coins de la carte.

## 🛠️ Outils Techniques utilisés

- 🎮 **SDL2** : Interface graphique  
- 🧠 **Langage C** : Langage de programmation
- ⚙️ **GCC** : Compilation en C 
- 🧩 **Makefile** : Compilation automatique
- ⚙️ **Valgrind** : Pour vérifier les fuites de mémoire
- 🌐 **Git** : Travail collaboratif
- 📄 **Doxygen** : Génération automatique de documentation
- 📅 **Diagramme de Gantt** : Pour planifier les tâches
- 📑 **Latex** : Rédaction du rapport de projet

## 🚀 Comment Installer le Jeu

Remarque : ce guide est valable pour les machines Unix ou Linux.

**Initialisation du dépôt git**  
cd chemin/ou/stocker/le/projet  
_Récupérer le jeu :_ 
```bash 
git clone https://github.com/leodurandetu/ForTheKing.git
```
_Se déplacer dans le répertoire qui contient le jeu :_ 
```bash
cd ForTheKing
```

**Installation des librairies nécessaires**  
Ce projet utilise SDL2.  
Voici un court guide pour l'installer sur Unix ou Linux :  
_sudo apt update_  
```bash 
sudo apt install -y libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev_  
```
_Si vous n'avez pas installé de compilateur gcc :_
```bash
sudo apt install gcc
```

**Compilation du projet**  
Important : il faut être à la racine du projet (ForTheKing/)  
Important : pour compiler le projet, vous devez avoir le compilateur C gcc.  
```bash 
make all
```

**Test du projet**  
Important : il faut être à la racine du projet (ForTheKing/)  
Cette commande lance le menu du jeu pour choisir le personnage ou accéder aux réglages.  
```bash 
./bin/menu 
```

## 🎮 Comment Jouer au Jeu

Si vous êtes dans le menu, cliquez sur "Nouvelle Aventure", puis choisissez un personnage et cliquez sur "Lancer l'Aventure".  

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


