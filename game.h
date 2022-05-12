#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED
#include <stdbool.h>
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <stdlib.h>
#include <math.h>

#include "debugmalloc.h"
#include "charge.h"

///* Jatekoshoz szukseges adatokat tarolo struktura */
typedef struct Player{
    char name[50];
    int currentlevel; //Mindig az a szint amit meg NEM teljesitett a jatekos
} Player;


#include "draw.h"
#include "file.h"

typedef struct GameObjects{
    int angle;
    int** walls;
    Sphere sphere;
    Charge charge;
    Linked_Charges *first;
}GameObjects;

Linked_Charges* addCharge(Linked_Charges *b, int x, int y);
Linked_Charges* removeCharge(Linked_Charges *b, int x, int y);
bool checkCollisionWithHole(GameObjects *gameObjects);
void checkCollisionWithWalls(GameObjects *gameObjects, int s);
int startGame(int **walls, int walls_size, Sphere sphere, bool *didComplete);
void drawSDLFunctions(SDL_Renderer *renderer, int walls_size, int retry, GameObjects *gameObject);
void calcFunctions(GameObjects *gameObjects);
bool keyHandler(SDL_Event ev, bool *didComplete, int *retry, GameObjects *gameObjects);
void linkedChargeHandle(SDL_Event ev, GameObjects *gameObjects);

#endif // GAME_H_INCLUDED
