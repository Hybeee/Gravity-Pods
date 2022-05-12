#ifndef DRAW_H_INCLUDED
#define DRAW_H_INCLUDED
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <math.h>

#include "debugmalloc.h"
#include "file.h"
#include "charge.h"

extern int WINDOW_HEIGHT;

void sdl_init(char const *felirat, int szeles, int magas, SDL_Window **pwindow, SDL_Renderer **prenderer);
void drawGun(SDL_Renderer* renderer, double angle);
void drawWalls(SDL_Renderer *renderer, int **walls, int s);
void drawRetryCount(SDL_Renderer *renderer, int c);
void drawHole(SDL_Renderer *renderer, Sphere sphere);
void drawCharge(SDL_Renderer *renderer, Charge charge);
void drawLinkedCharge(SDL_Renderer *renderer, Linked_Charges *b);

#endif // DRAW_H_INCLUDED
