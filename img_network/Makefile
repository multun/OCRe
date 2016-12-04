CFLAGS += -std=c99 -Wconversion -pedantic -Wall -Wextra -Wformat=2 \
	-Wswitch-enum -Wcast-align -Wpointer-arith -Wbad-function-cast \
	-Wstrict-overflow=4 -Wstrict-prototypes -Winline -Wundef \
	-Wnested-externs -Wcast-qual -Wshadow -Wunreachable-code -Wlogical-op \
	-Wfloat-equal -Wstrict-aliasing=2 -Wredundant-decls -Wold-style-definition \
	-Werror -ggdb3 -O0 -fno-omit-frame-pointer -fstrict-aliasing

CFLAGS_BUILD += -O3 -Wall -pedantic -std=c99 -Wextra -fomit-frame-pointer

LDFLAGS += -lm

EXEC=train_net

SRC += main.c sets.c training.c
SRC += $(wildcard ../neural_network/*.c)
SRC += $(wildcard ../base_structs/*.c)
SRC += $(wildcard ../signal_running.c)
#special file
SRC += charset.c


OBJ = $(SRC:.c=.o)
DEP = ${SRC:.c=.d}


all: $(EXEC)


fuckit: clean
	$(CC) -o $@ main.c -g -DFUCKIT $(CFLAGS_BUILD) $(LDFLAGS)


$(EXEC): $(OBJ)
	@echo "Linking the main binary file..."
	@$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	@echo "Building $@..."
	@$(CC) -MMD -o $@ -c $< $(CFLAGS)

charset.c:
	bash generate_char_table.sh $@

-include ${DEP}
charset.h: charset.c


.PHONY: clean nclean raise_limits

raise_limits:
	sudo sh -c 'echo 500000 > /proc/sys/vm/max_map_count'
	sudo sysctl -w fs.file-max=20000000
clean:
	rm -f fuckit $(EXEC) $(DEP) $(OBJ)
run: all
	@mkdir -p xor_data
	./$(EXEC) $(ARGS)

nclean:
	rm -f xor_data/*
