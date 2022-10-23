CC = gcc
CFLAGS = -Wall -Wextra `pkg-config --cflags sdl2 SDL2_image` -O3
LDLIBS = `pkg-config --libs sdl2 SDL2_image` -lSDL2 -lm
LDFLAGS = -g #-fsanitize=address

SRC = gaussian_filter.c utilis_image.c sobel.c main.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

gaussian: gaussian_filter.o utilis_image.o sobel.o main.o
	${CC} ${CFLAGS} ${LDFLAGS} $^ -o $@ ${LDLIBS}
#	./gaussian test.png

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}
	${RM} gaussian
	${RM} res_*
	${RM} .ut*
	${RM} .gauss*
	${RM} .Make*
	${RM} .main*
	${RM} .sob*
