CC = gcc
CFLAGS = -Wall -Wextra `pkg-config --cflags sdl2 SDL2_image` -g
LDLIBS = `pkg-config --libs sdl2 SDL2_image` -lSDL2

SRC = gaussian_filter.c utilis_image.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

gaussian: gaussian_filter.o utilis_image.o
	${CC} ${CFLAGS} $^ -o $@ ${LDLIBS}
#	./gaussian test.png

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}
	${RM} gaussian
	${RM} res_*
