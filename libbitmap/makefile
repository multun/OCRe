CC=gcc

CFLAGS=-std=c99 -pedantic -Wall -Wno-missing-braces -Wextra -Wno-missing-field-initializers -Wformat=2 -Wswitch-default -Wswitch-enum -Wcast-align -Wpointer-arith -Wbad-function-cast -Wstrict-overflow=5 -Wstrict-prototypes -Winline -Wundef -Wnested-externs -Wcast-qual -Wshadow -Wunreachable-code -Wlogical-op -Wfloat-equal -Wstrict-aliasing=2 -Wredundant-decls -Wold-style-definition -Werror -ggdb3 -O0 -fno-omit-frame-pointer -ffloat-store -fno-common -fstrict-aliasing -lm        -fpic

CFLAGS_BUILD=-O3 -Wall -pedantic -std=c99 -Wextra

#pthread pour le threading, et ld pour les modules
LDFLAGS= #-lpthread -ldl
EXEC=libbitmap.so

SRC= $(wildcard *.c)

#SRC= push_swap.c main.c

#on construis la liste des .o à partir des .c
OBJ= $(SRC:.c=.o)

##	$@ Le nom de la cible
##      $< Le nom de la première dépendance
##      $^ La liste des dépendances
##      $?  La liste des dépendances plus récentes que la cible $* Le nom
##	du fichier sans suffixe

#la directive all doit lister tous les executables à produire
all: $(EXEC)

build: CFLAGS=$(CFLAGS_BUILD)
build: clean all


$(EXEC): $(OBJ)
	@echo "Building the main binary file..."
	@$(CC) -shared -o $@ $^ $(LDFLAGS)

#dépendances additionnelles ; le fichier est reconstruit si une dépendance est modifiée.


# règle générique pour tous les dépendances en .o, avec un pipe pour les arguments de comp.
%.o: %.c
	@echo "Building $@..."
	@$(CC) -fPIC -o $@ -c $< $(CFLAGS)
#.phony reconstruit toujours ses dépendances
.PHONY: clean mrproper
#vire tous les fichiers intermédiaires
clean:
#	rm -rf *.o  ne marche pas récursivement
	find ./ -type f -name '*.o' -delete
mrproper: clean
	rm -rf $(EXEC)
