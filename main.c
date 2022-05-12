#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "debugmalloc.h"
#include "draw.h"
#include "file.h"
#include "charge.h"

int WINDOW_HEIGHT = 800;



int main(int argc, char *argv[]) {

    Sphere game_sphere;

    char name[50];
    printf("Ird be a neved: ");
    scanf("%s", name);



    printf(" ----------------\n");
    printf("|  GRAVITY PODS  | \n");
    printf(" ----------------\n");

    Player new_player = getPlayerData(name);

    int m_m_choice; // a fomenuben valo lepegeteshez kell

    bool isRunning = true;
    while(isRunning){
        printf("1. Jatek \n");
        printf("2. Dicsoseglista \n");
        printf("3. Kilepes \n");
        scanf("%d", &m_m_choice);
        switch(m_m_choice){
        case 1:
            printf("Szintek: 1 2 3 4 5 \n");
            printf("Visszalepeshez uss be -1-t, egyebkent valassz egy szintet, amin jatszani akarsz: ");
            int g_level;

            while(1){
                scanf("%d", &g_level);
                if(g_level > new_player.currentlevel && g_level < 6){
                    printf("Meg nem jarsz a %d szinten. Teljesitsd sikeresen elobb az elozo palyakat! \n", g_level);
                } else if(g_level <= new_player.currentlevel && g_level > 0){
                    //jatekkezdeshez
                    bool didComplete;
                    int wallcount;
                    int** walls = readMap(g_level, &game_sphere, &wallcount);
                    int player_attempt = startGame(walls, wallcount, game_sphere, &didComplete);
                    //amikor a jateknak vege lett
                    if(didComplete){
                        writeLeaderboard(g_level, &new_player, player_attempt);
                        updatePlayerFile(new_player);
                        new_player.currentlevel += 1; //igy erheto el, hogy a jatekos futasidoben tudjon egybol a kovetkezo szintre menni
                    }
                    free2dimint(walls, wallcount);
                } else if(g_level == -1){
                    break;
                } else{
                    printf("Nem jo input, a szintek 1 es 5 kozott vannak. \n");
                    printf("-1 a visszalepeshez. \n");
                }

                printf("Szintek: 1 2 3 4 5 \n");
            }

            break;
        case 2:
            printf("Ird be, hogy melyik szint dicsoseglistajat szeretned latni: \n");
            printf("Szintek: 1 2 3 4 5 \nNyomj -1-t a menube valo visszalepeshez: ");
            int l_level;
            scanf("%d", &l_level);

            if(l_level > 0 && l_level < 6){
                readLeaderboard(l_level);
            } else if(l_level != -1){
                printf("Rossz formatum. \n");
            }

            break;
        case 3:
            printf("Easy volt nem? :c. \n");
            isRunning = false;
            break;
        default:
            printf("Bolcsesz vagy? EeeGy eS hArOm KoZoottTT Irj... \n");
        }
    }

    return 0;
}
