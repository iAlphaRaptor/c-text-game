#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "input.h"
#include "parser.h"
#include "location.h"
#include "object.h"
#include "npc.h"
#include "dictionary.h"
#include "system.h"

char *verb, *verbSyn, *noun, *nounSyn;

bool parse(char* input) {
	if (strcmp(input, "\n") && input[0] != 32) {

		verb = strtok(input, " \n");	
		getSynonyms(&verb);
		noun = strtok(NULL, " \n");
		getSynonyms(&noun);

		if (!strcmp(verb, "quit")) {
			return false;
		} else if (!strcmp(verb, "help")) {
			printf("Type in any command to navigate the world, and hope that the parser knows how to handle it.\n");
			printf("At the moment, commands should be in the form '<noun> <verb>', for example 'look book'.\n");
			printf("This will be improved in a further update.\n");
		} else if (!strcmp(verb, "look")) {
			if (noun == NULL || !strcmp(noun, "around")) {
				lookAround();
			} else {
				look(noun);
			}
		} else if (!strcmp(verb, "go")) {
			go(noun);
		} else if (!strcmp(verb, "take")) {
			take(noun);
		} else if (!strcmp(verb, "drop")) {
			drop(noun);
		} else if (!strcmp(verb, "inventory")) {
			inventory();
		} else if (!strcmp(verb, "talk")) {
			talk(noun);
		} else if (!strcmp(verb, "fight")) {
			playerAttack(noun);
		} else if (!strcmp(verb, "eat")) {
			eat(noun);
		} else if (!strcmp(verb, "open")) {
			interactDoor(noun, "open");
		} else if (!strcmp(verb, "close")) {
			interactDoor(noun, "close");
		} else if (!strcmp(verb, "unlock")) {
			interactDoor(noun, "unlock");
		} else if (!strcmp(verb, "lock")) {
			interactDoor(noun, "lock");
		} else if (!strcmp(verb, "clear")) {
			system("clear");
		} else if (!strcmp(verb, "health")) {
			printf("HP : %i\n", player->health);
		} else if (!strcmp(verb, "make")) {
			system("make");
			return false;
		} else if (!strcmp(verb, "save")) {
			save(commands);
		} else if (!strcmp(verb, "load")) {
			load();
		} else {
			printf("I'm not sure how to %s.\n", verb);
		}
	}
	return npcAttack();
}
