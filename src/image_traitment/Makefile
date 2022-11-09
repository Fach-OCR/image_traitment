CC = gcc -I../../include/
CFLAGS = -Wall -Wextra -Werror -std=c99 -pedantic-errors `pkg-config --cflags sdl2 SDL2_image` -g
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

valim:
	valgrind --leak-check=full --show-leak-kinds=all -s ./main image_01.jpeg

test:
	-mv images/* .
	./main image_01.jpeg && \
	./main image_02.jpeg && \
	./main image_03.jpeg && \
	./main image_04.jpeg && \
	./main image_05.jpeg && \
	./main image_06.jpeg && \
	mv image_0* images/
	feh *.jpeg


format:
	find -name "*.[ch]" -exec clang-format --verbose -i {} \;


$(BUILD)/%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(LDLFLAGS) $(CPPFLAGS) $(LDLIBS)

clean:
	rm -rf $(BUILD)
	$(RM) $(EXEC)
	${RM} res_*
	${RM} double_tresh_res_image_0* hysteris_res_image_0* non_max_res_image_0* sobel_res_image_0* gaussian_blur_res_i* gray_scale_res_ima* brightness_res_im* contrast_res_ima* res_hough_res_ima*
	${RM} *.swp

