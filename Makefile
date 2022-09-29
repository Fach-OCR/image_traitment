CC = gcc
CFLAGS = -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image` -g
LDLIBS = `pkg-config --libs sdl2 SDL2_image SDL_gfx`

SRC = gaussian_filter.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

gaussian: gaussian_filter.o
	${CC} ${CFLAGS} $^ -o $@ ${LDLIBS}

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}
	${RM} gaussian
	${RM} res_*
