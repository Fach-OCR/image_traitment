C = gcc -I../../include/
CFLAGS = -Wall -Wextra -Werror -std=c99 `pkg-config --cflags sdl2 SDL2_image` -g
LDLIBS = `pkg-config --libs sdl2 SDL2_image` -lSDL2 -lm
LDFLAGS = #-fsanitize=address


SRC =  $(shell find -name "*.c")
OBJ = $(SRC:%.c=$(BUILD)/%.o)
DEP = $(SRC:%.c=$(BUILD)/%.d)

BUILD := build
EXEC := main


all: init imagery


clear:
	find . -type d -empty -delete


init:
	$(shell mkdir -p $(BUILD))
	$(shell mkdir -p $(SRC:%=$(BUILD)/%))


imagery: $(OBJ)
	gcc -o $(EXEC) $(CFLAGS) $^ $(LDFLAGS) $(LDLIBS)


test:
	mv images/* .
	./main image_01.jpeg && \
	./main image_02.jpeg && \
	./main image_03.jpeg && \
	./main image_04.jpeg && \
	./main image_05.jpeg && \
	./main image_06.jpeg && \
	mv image_0* images/
	feh res_*


format:
	find -name "*.[ch]" -exec clang-format --verbose -i {} \;


$(BUILD)/%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(LDLFLAGS) $(CPPFLAGS) $(LDLIBS)

clean:
	rm -rf $(BUILD)
	$(RM) $(EXEC)
	${RM} res_*

