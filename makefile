CC=gcc
FLAGS=-Wall -g
SDL_DIR=${HOME}/SDL2
SDL_LIB_DIR=${SDL_DIR}/lib
SDL_INC_DIR=${SDL_DIR}/include
LIBS=-L${SDL_LIB_DIR} -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
INCS=-I${SDL_INC_DIR}
PROG=bin/fortheking
all: mon_prog

mon_prog: src/fortheking.c
	${CC} -o ${PROG} src/fortheking.c src/carte.c src/couleur.c src/perso.c ${LIBS} ${INCS} ${FLAGS}

clean:
	rm -f ${PROG}
	rm -f *.o
