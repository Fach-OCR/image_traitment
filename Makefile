CC = gcc -Iinclude/
CFLAGS = -Wall -Wextra -Werror `pkg-config --cflags sdl2 SDL2_image` -O3 -std=c99 -pedantic
LDLIBS = `pkg-config --libs sdl2 SDL2_image` -lSDL2 -lm
LDFLAGS = -g #-fsanitize=address

BUILD := build
SOURCE_DIR := src

SRC = $(shell find $(SOURCE_DIR) -name "*.c" ! -name "main.c")
OBJ = $(SRC:%.c=$(BUILD)/%.o)
DEP = $(SRC:%.c=$(BUILD)/%.d)

all: init main clear

clear:
	find . -type d -empty -delete

init:
	$(shell mkdir -p $(BUILD))
	$(shell mkdir -p $(SRC:%.c=$(BUILD)/%))

main: $(OBJ)
	gcc -o $@ $(CFLAGS) $^ $(LDLFLAGS) $(LDLIBS)

image_traitment:
	make -C src/image_traitment

solver:
	make -C src/sudoku_solver

$(BUILD)/%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(LDLFLAGS) $(CPPFLAGS) $(LDLIBS)

clean:
	${RM} ${OBJ}
	rm -rf $(BUILD)
	${RM} ${EXE}
	${RM} gaussian
	${RM} res_*
	${RM} .ut*
	${RM} .gauss*
	${RM} .Make*
	${RM} .main*
	${RM} .sob*
