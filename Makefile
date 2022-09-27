# Makefile

CC = gcc
CPPFLAGS =
CFLAGS = -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image` -lSDL_gfx
LDFLAGS =
LDLIBS = `pkg-config --libs sdl2 SDL2_image`

all: gaussianfilter

SRC = gaussian_filter.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

gaussianfilter: gaussian_filter.o
.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}

# END
