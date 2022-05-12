#include "game.h"

///* Toltes hozzaadasa a listahoz(egy kattintasra) */
Linked_Charges* addCharge(Linked_Charges *b, int x, int y){
    //*b = begin -> az elso elemre mutato pointer a linked list-ben
    Linked_Charges *newcharge = (Linked_Charges*) malloc(sizeof(Linked_Charges));

    newcharge->pos_x = x;
    newcharge->pos_y = y;
    newcharge->next = b;

    //Ha a lista ures, az uj toltesre mutat a pointer
    //Mas esetben az uj toltest hozzafuzi a program a lista vegere
    b = newcharge;

    return b;
}

///* Pozicio alapjan adott toltes elvetele a listabol egy kattintasra */
Linked_Charges* removeCharge(Linked_Charges *b, int x, int y){
    int e = 15; //epszilon ertek, igy a jatekosnak nem kell pixel-pontosan a toltesre kattintania
    Linked_Charges *l = NULL; //lemarado pointer
    for(Linked_Charges *m = b; m != NULL; m = m->next){
        double distance = sqrt((x - m->pos_x) * (x - m->pos_x) + (y - m->pos_y) * (y - m->pos_y));
        //kiszamolja a toltes kp-janak es a kattintas pontjanak a tavolsagat, ennek kell epszilonon belul lennie.
        if(distance < e){
            //Dokumentacio infoc-n <3
            if(l == NULL){
                Linked_Charges *new_b = m->next;
                free(m);
                b = new_b;
            } else {
                l->next = m->next;
                free(m);
            }

            return b;
        }
        l = m;
    }

    return b;
}
///* Lecheckolja, hogy a jatekos alapjan kilott toltes elerte-e a celt */
bool checkCollisionWithHole(GameObjects *gameObjects){
    Charge charge = gameObjects->charge;
    Sphere sphere = gameObjects->sphere;
    double distance_x = charge.x_pos - sphere.c_pos_x;
    double distance_y = charge.y_pos - sphere.c_pos_y;
    double distance = sqrt(distance_x * distance_x + distance_y * distance_y);

    if(sphere.radius > distance){
        return true;
    }

    return false;
}
/* Lecheckolja, hogy a toltes osszeutkozott-e barmelyik fallal */
void checkCollisionWithWalls(GameObjects *gameObjects, int s){
    int** walls = gameObjects->walls;
    Charge *charge = &gameObjects->charge;
    for(int i = 0; i < s; i++){
        //checkolas arra, hogy egy felso falat talal el
        if(walls[i][1] == 0){
            if((charge->x_pos > walls[i][0] && charge->x_pos < walls[i][0] + walls[i][2]) && (charge->y_pos > walls[i][1] && charge->y_pos < walls[i][1] + walls[i][3])){
                /* a toltest kiviszi a kepernyorol es kovetkezo inicializalasig ott tartja(kovetkezo loves parancsig) */
                charge->x_pos = -200;
                charge->y_pos = -200;
                charge->x_vel = 0;
                charge->y_vel = 0;
            }
        //checkolas arra, hogy egy also falat talal el
        } else if(walls[i][1] == WINDOW_HEIGHT){
            if((charge->x_pos > walls[i][0] && charge->x_pos < walls[i][0] + walls[i][2]) && (charge->y_pos < walls[i][1] && charge->y_pos > walls[i][1] - walls[i][3])){
                /* a toltest kiviszi a kepernyorol es kovetkezo inicializalasig ott tartja(kovetkezo loves parancsig) */
                charge->x_pos = -200;
                charge->y_pos = -200;
                charge->x_vel = 0;
                charge->y_vel = 0;
            }
        }
    }
}

///* Kirajzolo fuggvenyek */
void drawSDLFunctions(SDL_Renderer *renderer, int walls_size, int retry, GameObjects *gameObject){
    boxRGBA(renderer, 0, 0, WINDOW_HEIGHT * 16 / 9, WINDOW_HEIGHT, 0, 0, 0, 255);
    drawGun(renderer, gameObject->angle);
    drawWalls(renderer, gameObject->walls, walls_size);
    drawHole(renderer, gameObject->sphere);
    drawCharge(renderer, gameObject->charge);
    drawLinkedCharge(renderer, gameObject->first);
    drawRetryCount(renderer, retry);
}

///* Tolteshez kapcsolodo ujraszamolo fuggvenyek */
void calcFunctions(GameObjects *gameObjects){
    calcNewVelocity(&gameObjects->charge, 8, calcNewAccelaration(&gameObjects->charge, gameObjects->first));
    calcNewPos(&gameObjects->charge, 8);
}

///* User inputokkal foglalkozo fuggvenyek */
bool keyHandler(SDL_Event ev, bool *didComplete, int *retry, GameObjects *gameObjects){
    if(ev.key.type == SDL_KEYDOWN){
            switch(ev.key.keysym.sym){
            case SDLK_UP:
                gameObjects->angle -= 2.5;
                break;
            case SDLK_DOWN:
                gameObjects->angle += 2.5;
                break;
            case SDLK_SPACE:
                chargeInit(&gameObjects->charge, gameObjects->angle);
                *retry += 1;
                break;
            case SDLK_ESCAPE:
                SDL_Quit();
                *didComplete = false;
                return false;
            }
        }
    return true;
}

///* A lerakott toltesekkel foglalkozo fuggveny */
void linkedChargeHandle(SDL_Event ev, GameObjects *gameObjects){
    if(ev.button.button == SDL_BUTTON_LEFT){
        bool vane = false;
        for(Linked_Charges *m = gameObjects->first; m != NULL; m = m->next){
            if(ev.button.x == m->pos_x && ev.button.y == m->pos_y){
                vane = true;
            }
        }
        if(!vane){
            gameObjects->first = addCharge(gameObjects->first, ev.button.x, ev.button.y);
        }
    }
    if(ev.button.button == SDL_BUTTON_RIGHT){
        gameObjects->first = removeCharge(gameObjects->first, ev.button.x, ev.button.y);
    }
}

///* Visszater azzal, hogy hanyszor probalkozott ujra a jatekos az adott szinten */
int startGame(int **walls, int walls_size, Sphere sphere, bool *didComplete){
    /* ablak letrehozasa */
    SDL_Window *window;
    SDL_Renderer *renderer;
    sdl_init("Gravity Pods", WINDOW_HEIGHT * 16 / 9, WINDOW_HEIGHT, &window, &renderer);

    //segedvaltozok
    double angle = 0; // Agyu allasahoz szukseges szok erteke
    Linked_Charges *first = NULL;
    Charge charge;
    int retry = 0;

    GameObjects gameObjects;
    gameObjects.walls = walls;
    gameObjects.angle = angle;
    gameObjects.charge = charge;
    gameObjects.first = first;
    gameObjects.sphere = sphere;

    SDL_Event ev;
    SDL_PollEvent(&ev);

    /* Eventek leirasa
     Masodik feltetel a while ciklus futasat nezi, escape gomb lenyomasa eseten keyHandler = false, igy a loop leall  */
    while (ev.type != SDL_QUIT && keyHandler(ev, didComplete, &retry, &gameObjects)) {
        SDL_PollEvent(&ev);
        SDL_RenderClear(renderer);

        //Lerakott tolteseket kezelo fuggveny
        linkedChargeHandle(ev, &gameObjects);

        /* Kirajzolo fuggvenyek */
        drawSDLFunctions(renderer, walls_size, retry, &gameObjects);
        calcFunctions(&gameObjects);

        checkCollisionWithWalls(&gameObjects, walls_size);
        if(checkCollisionWithHole(&gameObjects)){
            SDL_Quit();
            freeLinkedCharges(gameObjects.first);
            *didComplete = true;
            return retry;
        }

        SDL_RenderPresent(renderer);
    }
    SDL_Quit();
    freeLinkedCharges(gameObjects.first);
    *didComplete = false;
    return -1;
}
