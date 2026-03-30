CC=gcc
FLAGS=-Wall -g
SDL_DIR=${HOME}/SDL2
SDL_LIB_DIR=${SDL_DIR}/lib
SDL_INC_DIR=${SDL_DIR}/include
LIBS=-L${SDL_LIB_DIR} -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lm
INCS=-I${SDL_INC_DIR}
PROG=bin/fortheking
MENU=bin/menu

all: mon_prog mon_menu test_deplacements

mon_prog: src/fortheking.c
	${CC} -o ${PROG} src/fortheking.c src/init_sdl.c src/ressources.c src/niveau.c src/perlin_noise.c src/adapter_niveau.c src/inventaire.c src/objet.c src/combat.c src/affichage_carte.c src/affichage_combat.c src/affichage_commun.c src/maths.c src/pause_menu.c src/carte.c src/couleur.c src/perso.c src/monstre.c src/affichage_infos.c src/option.c ${LIBS} ${INCS} ${FLAGS}

mon_menu: src/menu.c
	${CC} -o ${MENU} src/menu.c ${LIBS} ${INCS} ${FLAGS}

test_deplacements: test/test_deplacements.c
	${CC} -o bin/test_deplacements test/test_deplacements.c src/adapter_niveau.c src/niveau.c src/perlin_noise.c src/carte.c src/monstre.c ${LIBS} ${INCS} ${FLAGS}

clean:
	rm -f ${PROG}
	rm -f ${MENU}
	rm -f *.o
