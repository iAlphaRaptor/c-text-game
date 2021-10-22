#include <string.h>
#include <stdio.h>
#include "dictionary.h"

char *dictionary[][13] = {{"look", "observe", NULL},
			 {"go", "move", "walk", "run", NULL},
			 {"quit", "exit", NULL},
			 {"n", "north", NULL},
			 {"e", "east", NULL},
			 {"w", "west", NULL},
			 {"s", "south", NULL},
			 {"inventory", "i", NULL},
			 {"look", "examine", "inspect", NULL},
			 {"fight", "kill", "attack", NULL},
			 {"help", "commands", "instructions", "rules", NULL},
			 {"health", "hp", NULL},
			 {"talk", "speak", "address", NULL}};

void getSynonyms(char **word) {
	if (*word != NULL) {
		for (int i=0; i<(sizeof(dictionary) / sizeof(dictionary[0])); i++) {
			for (int j=0; dictionary[i][j] != NULL; j++) {
				if (dictionary[i][j] != NULL && !strcmp(*word, dictionary[i][j])) {
					*word = dictionary[i][0];
				}
			}
		}
	}
}
