CC = gcc
CFLAGS = -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image`
LDLIBS = `pkg-config --libs sdl2 SDL2_image`
SRC = gaussian_filter.c

all:
	${CC} ${CFLAGS} -c -o gaussian_filter.o ${SRC}
	${CC} gaussian_filter.o ${LDLIBS} -o gaussianfilter
	gcc gaussian_filter.o `pkg-config --libs sdl2 SDL2_image` -o gaussianfilter


OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}
	${RM} gaussianfilter
