CC=gcc
FLAGS=-Wall -g
SDL_DIR=${HOME}/SDL2
SDL_LIB_DIR=${SDL_DIR}/lib
SDL_INC_DIR=${SDL_DIR}/include
LIBS=-L${SDL_LIB_DIR} -lSDL2 -lSDL2_image -lSDL2_ttf
INCS=-I${SDL_INC_DIR}
PROG=bin/fortheking
all: mon_prog

mon_prog: src/fortheking.c
	${CC} -o ${PROG} src/fortheking.c src/carte.c src/couleur.c ${LIBS} ${INCS} ${FLAGS}

test_carte:
	${CC} -o bin/test_carte src/test_carte.c src/carte.c src/couleur.c ${LIBS} ${INCS} ${FLAGS}

clean:
	rm -f ${PROG}
	rm -f bin/test_carte
	rm -f *.o