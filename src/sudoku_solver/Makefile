# Simple Makefile

CC=gcc -I../../include
CFLAGS= -Wall -Wextra -Werror -O3 -std=c99 -pedantic -g

SRC =  $(shell find -name "*.c")
OBJ = $(SRC:%.c=$(BUILD)/%.o)
DEP = $(SRC:%.c=$(BUILD)/%.d)

BUILD := build
EXEC := main


all: init solver

init:
	$(shell mkdir -p $(BUILD))
	$(shell mkdir -p $(SRC:%=$(BUILD)/%))

solver: $(OBJ)
	gcc -o $(EXEC) $(CFLAGS) $^

$(BUILD)/%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(LDLFLAGS) $(CPPFLAGS) $(LDLIBS)


clean:
	${RM} ${OBJ}
	rm -rf $(BUILD)
	$(RM) $(EXEC)
	${RM} ${DEP}
	${RM} grid.result
	${RM} vg*
	${RM} .Ma*
	${RM} .sudo*

# END
