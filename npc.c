#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "npc.h"
#include "object.h"
#include "location.h"

int numNPCs = 3;
NPC *npcs[3];

FILE *npcFile;
FILE *locsFile;

void generateNPCs() {
	npcFile = fopen("npcs.txt", "r");
	locsFile = fopen("locations.txt", "r");

	char *locLine = NULL;
	size_t m;

	char tags[numLocs][16];
	for (int j=0; j<numLocs; j++) {
		getline(&locLine, &m, locsFile);
		strtok(locLine, "/");
		strcpy(tags[j], strtok(NULL, "/"));
	}
	fclose(locsFile);

	for (int i=0; i<numNPCs; i++) {
		npcs[i] = malloc(sizeof(NPC));
		npcs[i]->super = malloc(sizeof(Location));

		char *line = NULL;
		size_t n;

		char *article;
		char *tag;
		char *intro;
		char *description;
		char *voiceline;
		int connections[4];
		char directions[4];
		char *capacity;
		char *health;
		char *aggression;
		char *locTag;
		int locIndex;

		getline(&line, &n, npcFile);

		article = strtok(line, "/");
		tag = strtok(NULL, "/");
		intro = strtok(NULL, "/");
		description = strtok(NULL, "/");
		voiceline = strtok(NULL, "/");
		char *tempConns[] = {strtok(NULL, "/"), strtok(NULL, "/"), strtok(NULL, "/"), strtok(NULL, "/")};
		for (int i=0; i<4; i++) {
			char *tempP;
			long ret;

			ret = strtol(tempConns[i], &tempP, 10);
			connections[i] = ret;
		}

		directions[0] = (char) *strtok(NULL, "/");
		directions[1] = (char) *strtok(NULL, "/");
		directions[2] = (char) *strtok(NULL, "/");
		directions[3] = (char) *strtok(NULL, "/");
		capacity = strtok(NULL, "/");
		int intCapacity = atoi(capacity);
		health = strtok(NULL, "/");
		int intHealth = atoi(health);
		aggression = strtok(NULL, "/");
		int intAggro = atoi(aggression);
		locTag = strtok(NULL, "\n");
		locTag[strlen(locTag)-1] = '\0';

		for (int j=0; j<numLocs; j++) {
			if (!strcmp(tags[j], locTag)) {
				locIndex = j;
				break;
			}
		}

		strcpy(npcs[i]->super->article, article);
		strcpy(npcs[i]->super->tag, tag);
		strcpy(npcs[i]->super->intro, intro);
		strcpy(npcs[i]->super->description, description);
		strcpy(npcs[i]->voiceline, voiceline);
		memcpy(npcs[i]->super->connections, connections, sizeof(int)*4);
		strcpy(npcs[i]->super->directions, directions);
		npcs[i]->super->capacity = intCapacity;
		npcs[i]->health = intHealth;
		npcs[i]->location = locs[locIndex];
		npcs[i]->aggression = intAggro;
		npcs[i]->alive = true;
	}

	fclose(npcFile);
}

void talk(char *noun) {
	bool talked = false;
	if (noun != NULL) {
		for (int i=0; i<numNPCs; i++) {
			if (npcs[i]->alive && !strcmp(noun, npcs[i]->super->tag) && !strcmp(npcs[i]->location->tag, player->location->tag)) {
				printf("\"%s\"\n", npcs[i]->voiceline);
				talked = true;
			} else if (!npcs[i]->alive) {
				printf("Unfortunately the %s is dead, and so doesn't respond.\n", npcs[i]->super->tag);
				talked = true;
			} else if (strcmp(npcs[i]->location->tag, player->location->tag) && !strcmp(noun, npcs[i]->super->tag)) {
				printf("I can't see a %s nearby.\n", noun);
				talked = true;
			}
		}
		if (!talked) {
			printf("The %s doesn't respond.\n", noun);
		}
	} else {
		printf("You talk to yourself.\n");
	}
}

void playerAttack(char *noun) {
	bool found = false;
	bool attacked = false;
	if (noun != NULL) {
		for (int i=0; i<numNPCs; i++) {
			if (npcs[i]->alive && !strcmp(noun, npcs[i]->super->tag) && !strcmp(npcs[i]->location->tag, player->location->tag)) {
				found = true;
				char weapon[16];
				bool gotWeapon = false;

				printf("with what?\n\n>>> ");
				fgets(weapon, 16, stdin);
				weapon[strlen(weapon)-1] = '\0';

				for (int j=0; j<numObjs; j++) {
					if (!strcmp(weapon, objs[j]->tag) && !gotWeapon) {
						if (!strcmp(objs[j]->location->tag, "player")) {
							gotWeapon = true;
							if (objs[j]->damage == 0) {
								printf("You hit the %s with the %s.\nIt doesn't do much damage.\n", npcs[i]->super->tag, objs[j]->tag);
							} else {
								printf("You attack the %s with the %s.\nIt does %i damage.\n", npcs[i]->super->tag, objs[j]->tag, objs[j]->damage);
								npcs[i]->health -= objs[j]->damage;
								if (npcs[i]->aggression > 4) {
									npcs[i]->aggression *= 2;
								} else {
									npcs[i]->aggression = 5;
								}
								attacked = true;
							}
						} else {
							printf("You're not holding %s.\n", weapon);
						}
					}
				}
				if (!gotWeapon) {
					char useFist[4];
					if (!strcmp(weapon, "fists") || !strcmp(weapon, "fist") || !strcmp(weapon, "hands") || !strcmp(weapon, "hand")) {
						strcpy(useFist, "y");
					} else {
						printf("You don't have a %s.\nUse your fists instead? (y/n)\n\n>>> ", weapon);
						fgets(useFist, 4, stdin);
						useFist[strlen(useFist)-1] = '\0';
					}

					if (!strcmp(useFist, "y")) {
						printf("You hit the %s with your fists.\nIt does 5 damage.\n", npcs[i]->super->tag);
						npcs[i]->health -= 5;
						npcs[i]->aggression++;
						attacked = true;
					}
				}
			} else if (!npcs[i]->alive && !strcmp(noun, npcs[i]->super->tag) && !strcmp(npcs[i]->location->tag, player->location->tag)) {
				printf("The %s is dead.\nI don't think it would be wise to attack it anymore.\n", npcs[i]->super->tag);
				found = true;
			} else if (!strcmp(noun, npcs[i]->super->tag)) {
				printf("There isn't a %s nearby.\n", noun);
				found = true;
			}
			if (attacked) {
				if (npcs[i]->health <= 0) {
					npcs[i]->alive = false;
					bool hasWeapon = false;
					for (int j=0; j<numObjs; j++) {
						if (!strcmp(objs[j]->location->tag, npcs[i]->super->tag)) {
							hasWeapon = true;
							printf("The %s dies, dropping its %s to the ground.\n", npcs[i]->super->tag, objs[j]->tag);
							objs[j]->location = player->location;
						}
					}
					if (!hasWeapon) {
						printf("The %s dies.\n", npcs[i]->super->tag);
					}
				}
			}
		}
		if (!found) {
			printf("You attack the %s, nothing happens...\n", noun);
		}
	} else {
		printf("You start attacking the air. Nothing happens...\n");
	}
}

bool npcAttack() {
	for (int i=0; i<numNPCs; i++) {
		if (!strcmp(npcs[i]->location->tag, player->location->tag) && npcs[i]->alive) {
			int seed = rand() % (11);  // Random number from 1 to 10
			if (seed < npcs[i]->aggression) {
				// Attack the player
				bool hasWeapon = false; 
				for (int j=0; j<numObjs; j++) {
					if (!strcmp(objs[j]->location->tag, npcs[i]->super->tag) && objs[j]->damage != 0) {
						printf("The %s attacks you with %s %s.\nYou take %i damage.\n", npcs[i]->super->tag, objs[j]->article, objs[j]->tag, objs[j]->damage);
						player->health -= objs[j]->damage;
						hasWeapon = true;
					}
				}
				if (!hasWeapon) {
						printf("The %s punches you, dealing 5 damage.\n", npcs[i]->super->tag);
						player->health -= 5;
				}
			}
		}
	}
	if (player->health <= 0) {
		printf("\nWith a groan, you sink to the ground, too weak to carry on.\n\nYou die.\n\n");
		return false;
	}
	return true;
}