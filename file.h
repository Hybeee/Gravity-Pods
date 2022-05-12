#ifndef FILE_H_INCLUDED
#define FILE_H_INCLUDED
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "debugmalloc.h"

extern int WINDOW_HEIGHT;

///* Cel, mely egy file-bol lesz kiolvasva */
typedef struct Sphere{
   int c_pos_x, c_pos_y;
   int radius;
} Sphere;

#include "game.h"

void free2dimchar(char** names, int c);
void free2dimint(int** walls, int c);
char* numToString(int c);
char* getFileName(const char *level, const char *number);
int** readMap(int level, Sphere *sphere, int* wallcount);
void writeLeaderboard(int level, Player *p, int attempt);
void readLeaderboard(int level);
Player getPlayerData(char* player_name);
void updatePlayerFile(Player current_player);
int** getWallsArray(int c);
void fillUpArray(int** walls, int c, FILE* in_file);
Player writeToFile(char** names, int* player_levels, char* player_name, int c);

#endif // FILE_H_INCLUDED
