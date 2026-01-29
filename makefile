CC=gcc
FLAGS=-Wall -g
SDL_DIR=${HOME}/SDL2
SDL_LIB_DIR=${SDL_DIR}/lib
SDL_INC_DIR=${SDL_DIR}/include
LIBS=-L${SDL_LIB_DIR} -lSDL2
INCS=-I${SDL_INC_DIR}
PROG=fortheking
all: sdl_text
sdl_text: src/fortheking.c
	${CC} -o ${PROG} src/fortheking.c ${LIBS} ${INCS} ${FLAGS}
clean:
	rm -f ${PROG}
	rm -f *.o