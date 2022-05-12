#include "draw.h"
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>

///* ablak letrehozasa */
void sdl_init(char const *felirat, int szeles, int magas, SDL_Window **pwindow, SDL_Renderer **prenderer) {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        SDL_Log("Nem indithato az SDL: %s", SDL_GetError());
        exit(1);
    }
    SDL_Window *window = SDL_CreateWindow(felirat, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, szeles, magas, 0);
    if (window == NULL) {
        SDL_Log("Nem hozhato letre az ablak: %s", SDL_GetError());
        exit(1);
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        SDL_Log("Nem hozhato letre a megjelenito: %s", SDL_GetError());
        exit(1);
    }
    SDL_RenderClear(renderer);

    *pwindow = window;
    *prenderer = renderer;
}

///* agyu kirajzolasa */
void drawGun(SDL_Renderer *renderer, double angle){
    angle = (angle / 360) * 2 * 3.14;

    //agyu poziciojanak inicializalasa
    int center_x = WINDOW_HEIGHT * 16 / 9 * 1 / 20;
    int center_y = WINDOW_HEIGHT / 2;
    int radius = WINDOW_HEIGHT / 20;
    circleRGBA(renderer, center_x, center_y, radius, 57, 255, 20, 255);

    int length = WINDOW_HEIGHT / 10;
    //ket egymast lefedo teglalap: igy lesz olyan hatasa, mintha lenne ott egy cso
    //a csovek mindig arra allnak, amerre a jatekos celoz. Ez az angle alapjan van meghatarozva
    thickLineRGBA(renderer, center_x, center_y, cos(angle) * length + center_x, sin(angle) * length + center_y, WINDOW_HEIGHT / 45, 57, 255, 20, 255);
    thickLineRGBA(renderer, center_x, center_y, cos(angle) * length + center_x, sin(angle) * length + center_y, WINDOW_HEIGHT / 45 - 3, 0, 0, 0, 255);

    //lefedi a cso azon reszet, mely a koron belulre nyulna
    filledCircleRGBA(renderer, center_x, center_y, radius - 1, 0, 0, 0, 255);
}

///* falak kirajzolasa a file-bol beolvasott adatok alapjan */
void drawWalls(SDL_Renderer *renderer, int **walls, int s){
    //2dimenzios tomb a parameterben tartalmazza a fal adatait
    //[[balsarokx, balsaroky, szelesseg, magassag]]
    for(int i = 0; i < s; i++){
        //esetszetvalasztas balsaroky fuggvenyeben
        //ha ez 0, akkor balsaroky-hoz hozzaadjuk a magassagot, masik esetben kivonjuk
        if(walls[i][1] == 0){
            rectangleRGBA(renderer, walls[i][0], walls[i][1] - 2, walls[i][0] + walls[i][2], walls[i][1] + walls[i][3], 57, 255, 20, 255);
        } else if(walls[i][1] == WINDOW_HEIGHT){
            rectangleRGBA(renderer, walls[i][0], walls[i][1] + 2, walls[i][0] + walls[i][2], walls[i][1] - walls[i][3], 57, 255, 20, 255);
        }
    } //+- 2 a masodik koordinataknal, hogy a teglalapnak az alja/teteje ne latszodjon.
    //TODO: fuggveny ami felszabaditja double **walls memoriajat ide jon.
}

///* Probalkozasok szamanak kirajzolasa */
void drawRetryCount(SDL_Renderer *renderer, int c){
    char *countString = numToString(c);
    stringRGBA(renderer, 0, 0, countString, 57, 255, 20, 255);
    free(countString);
}

///* Cel kirajzolasa file-bol beolvasott adat alapjan */
void drawHole(SDL_Renderer *renderer, Sphere sphere){
    //TODO(?) : Ket ellipszis megvaltozasa, melyek nagysagban valtoznak. Igy egy animaciot letrehozva
    circleRGBA(renderer, sphere.c_pos_x, sphere.c_pos_y, sphere.radius, 171, 32, 253, 255);
}

///* Toltes kirajzolasa a Charge struktura fieldjei alapjan */
void drawCharge(SDL_Renderer *renderer, Charge charge){
    aacircleRGBA(renderer, charge.x_pos, charge.y_pos, WINDOW_HEIGHT / 200, 251, 255, 0, 255);
}

///* Kirajzolja a jatekos altal lehelyezett tolteseket */
void drawLinkedCharge(SDL_Renderer *renderer, Linked_Charges *b){
    for(Linked_Charges* m = b; m != NULL; m = m->next){
        aacircleRGBA(renderer, m->pos_x, m->pos_y, WINDOW_HEIGHT / 100, 173, 216, 230, 255);
    }
}
