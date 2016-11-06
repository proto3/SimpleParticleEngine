# $@ 	Le nom de la cible
# $< 	Le nom de la première dépendance
# $^ 	La liste des dépendances
# $? 	La liste des dépendances plus récentes que la cible
# $* 	Le nom du fichier sans suffixe

CC=gcc
CFLAGS=-g -Wall -Werror -lSDL -lpthread -lm
LDFLAGS=
EXEC=main

all: $(EXEC)

main: main.c physic.c structures.c graphic.c constants.h
	@$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	@rm -rf *.o *~ $(EXEC)
