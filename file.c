#include "file.h"

///* Az alabbi ket fuggveny a ket 2dim array-t szabaditja fel, melyek a program soran hasznalatra kerulnek */
///* Parameterben a pontos neve, amit felszabadit */
void free2dimchar(char** names, int c){
    for(int i = 0; i < c; i++){
        free(names[i]);
    }
    free(names);
}
void free2dimint(int** walls, int c){
    for(int i = 0; i < c; i++){
        free(walls[i]);
    }
    free(walls);
}

///* Adott int szamot egy karakter tombbe alakitja at */
///* draw.c-ben es a filenev letrehozasaban van haszna */
char* numToString(int c){
    int tempcount = c;
    int stringcount = 0;
    while(tempcount > 0){
        tempcount /= 10;
        stringcount++;
    }

    //memoria felszabaditasa azon fuggvenyekben melyek meghivjak
    char* array = (char*) malloc(sizeof(char) * stringcount + 1);
    for(int i = stringcount - 1; i >= 0; i--){
        array[i] = c % 10 + '0';
        c /= 10;
    }
    array[stringcount] = '\0';

    return array;
}

///* Letrehozza a ketdimenzios tombot es visszareturnoli */
int** getWallsArray(int c){
    int** walls = (int**) malloc(sizeof(int*) * c);
    if(walls == NULL){
        printf("Memoria lefoglalasa sikertelen. \n");
        return NULL;
    }
    for(int i = 0; i < c; i++){
        walls[i] = (int*) malloc(sizeof(int) * 4);
        if(walls[i] == NULL){
            printf("Memoria lefoglalasa sikertelen. \n");
            return NULL;
        }
    }

    return walls;
}

void fillUpArray(int** walls, int c, FILE* in_file){
    int x, y, w, h; //x, y, szelesseg, magassag
    double ratio = (double)WINDOW_HEIGHT / 800;
    //adatok beolvasasa a resztombokbe
    for(int g = 0; g < c; g++){
        fscanf(in_file, "%d %d %d %d", &x, &y, &w, &h);
        walls[g][0] = x * ratio;
        walls[g][1] = y * ratio;
        walls[g][2] = w * ratio;
        walls[g][3] = h * ratio;
    }
}

///* Letrehozza a file nevet tartalmazo stringet. Ez minden szintnel valtozik */
char* getFileName(const char *level, const char *number){
    //Harom reszbol all a szintet tartalmazo filenev ezeket fuzi ossze:
    //level + hanyadik + .txt = levelx.txt
    char* fstring = (char*) malloc(strlen(level) + strlen(number) + strlen(".txt") + 1);

    strcpy(fstring, level);
    strcat(fstring, number);
    strcat(fstring, ".txt");

    return fstring;
}

///* Kiolvassa az adott szinthez tartozo .txt file-bol a palya tulajdonsagait */
int** readMap(int level, Sphere *sphere, int* wallcount){
    char* num = numToString(level);
    //free(filename) a fuggveny vegen
    char* filename = getFileName("level", num);

    FILE *in_file = fopen(filename, "rt");
    if(in_file == NULL){
        printf("Error! Nem lehet megnyitni a file-t.");
        return NULL;
    }
    //file megnyitasa utan nincs rajuk szukseg
    free(filename);
    free(num);

    int count;
    fscanf(in_file, "%d", &count); //hany darab fal lesz, file legelso sora

    //ketdimenzios tomb lefoglalasa
    int** walls = getWallsArray(count);
    fillUpArray(walls, count, in_file);

    //A celkor adatainak beolvasasa a kovetkezo sorbol
    int c_pos_x, c_pos_y, radius;
    double ratio = (double)WINDOW_HEIGHT / 800;
    fscanf(in_file, "%d %d %d", &c_pos_x, &c_pos_y, &radius);
    sphere->c_pos_x = c_pos_x * ratio;
    sphere->c_pos_y = c_pos_y * ratio;
    sphere->radius = radius * ratio;

    fclose(in_file);

    *wallcount = count;
    return walls;
}

///* A leaderboardx.txt file-t modositja a level alapjan es a jatekos pontszamanak fuggvenyeben */
void writeLeaderboard(int level, Player *p, int attempt){
    char* num = numToString(level);
    char* filename = getFileName("leaderboard", num);
    FILE *in_file = fopen(filename, "rt");
    if(in_file == NULL){
        printf("Error! Nem lehet megnyitni a file-t.");
        return;
    }

    int attempts[5];
    char names[5][50];
    for(int i = 0; i < 5; i++) fscanf(in_file, "%d %s", &attempts[i], names[i]);

    fclose(in_file);

    //A leaderboard atirasat megvalosito logika

    if(attempts[4] <= attempt && attempts[4] != -1){
        free(filename);
        free(num);
        return;
    }

    int new_attempts[5]; //segedtomb
    char *new_names[5]; //segedtomb
    int k = 0; //a segedtombok indexe
    char *pname = p->name; //jatekos nevet elmento tombre mutato pointer
    bool isNew = true;
    for(int j = 0; j < 5; j++){
        if(isNew && (attempt < attempts[j] || attempts[j] == -1)){
            //uj elemek beszurasa
            new_attempts[k] = attempt;
            new_names[k++] = pname;
            if(j != 4){
                new_attempts[k] = attempts[j];
                new_names[k++] = names[j];
            }
            isNew = false;
        } else if(j != 4){
            //egyebkent csak beszur
            new_attempts[k] = attempts[j];
            new_names[k++] = names[j];
        }
    }

    FILE *out_file = fopen(filename, "wt");
    free(filename);
    free(num);


    //file-ba valo visszairas
    for(int i = 0; i < 5; i++){
        fprintf(out_file, "%d %s\n", new_attempts[i], new_names[i]);
    }

    fclose(out_file);
}

///* Kiirja a konzolra a leaderboardot */
void readLeaderboard(int level){
    char* num = numToString(level);
    //free(filename) a fuggveny vegen
    char* filename = getFileName("leaderboard", num);

    FILE *in_file = fopen(filename, "rt");

    if(in_file == NULL){
        printf("Error! Nem lehet megnyitni a file-t.");
        return;
    }

    free(filename);
    free(num);

    char name[50];
    int attempt;

    for(int i = 0; i < 5; i++){
        fscanf(in_file, "%d %s", &attempt, name);
        printf("%s: %d \n", name, attempt);
    }

    fclose(in_file);
}

Player writeToFile(char** names, int* player_levels, char* player_name, int c){
    FILE *out_file = fopen("players.txt", "wt");
    if(out_file == NULL){
        printf("Error! Nem lehet megnyitni a file-t.");
        //pl program kilep
    }
    fprintf(out_file, "%d\n", c + 1);
    for(int i = 0; i < c; i++){
        fprintf(out_file, "%s %d\n", names[i], player_levels[i]);
    }
    fprintf(out_file, "%s %d", player_name, 1);
    fclose(out_file);
    free(player_levels);
    free2dimchar(names, c);

    Player new_player;
    strcpy(new_player.name, player_name);
    new_player.currentlevel = 1;

    printf("Udv a jatekban %s \n", new_player.name);

    return new_player;
}

///* Inditasnal, miutan a jatekos beirta a nevet ez a fuggveny hivodik meg */
///* Ha jatekos szerepel a file-ban visszaterit egy jatekos strukturat a nevevel es a file-ban levo adataval */
///* Ha nincs benne beleirja a file vegebe es visszaterit egy "default" jatekos strukturat */
Player getPlayerData(char* player_name){
    FILE *in_file = fopen("players.txt", "rt");
    if(in_file == NULL){
        printf("Error! Nem lehet megnyitni a file-t.");
    }

    int count;
    fscanf(in_file, "%d", &count);

    char** names = (char**) malloc(sizeof(char*) * count);
    for(int i = 0; i < count; i++){
        names[i] = (char*) malloc(sizeof(char) * 50);
    }
    int* player_levels = (int*) malloc(sizeof(int) * count);

    /* For loop a segedtombok feltoltesehez es a jatekos checkolasahoz */
    for(int i = 0; i < count; i++){
        fscanf(in_file, "%s %d", names[i], &player_levels[i]);
        if(strcmp(player_name, names[i]) == 0){
            /* Megegyezes eseten visszaterul a jatekos strukturaja */
            printf("Udv ujra, %s! \n", player_name);

            Player new_player;
            strcpy(new_player.name, player_name);
            new_player.currentlevel = player_levels[i];

            fclose(in_file);
            free(player_levels);
            free2dimchar(names, count);
            return new_player;
        }
    }

    fclose(in_file);
    /* ha nem volt talalat akkor a file vegere irjuk a jatekos nevet default(1) levellel */
    Player new_player = writeToFile(names, player_levels, player_name, count);

    return new_player;
}

///* A program kilepese elott hivodik meg */
///* A jatekos file-ben adott jatekos level progressionjet updateli. Pl.: futas elott: ...XY 3... futas utan: ...XY 5... ha XY eljutott az 5os szintre */
void updatePlayerFile(Player current_player){
    FILE *in_file = fopen("players.txt", "rt");
    if(in_file == NULL){
        printf("Error! Nem lehet megnyitni a file-t.");
    }

    int count;
    fscanf(in_file, "%d", &count);

    char** names = (char**) malloc(sizeof(char*) * count);
    for(int i = 0; i < count; i++){
        names[i] = (char*) malloc(sizeof(char) * 50);
    }
    int* player_levels = (int*) malloc(sizeof(int) * count);

    for(int i = 0; i < count; i++){
        fscanf(in_file, "%s %d", names[i], &player_levels[i]);
        if(strcmp(names[i], current_player.name) == 0){
            if(current_player.currentlevel < 5){
                player_levels[i] = current_player.currentlevel + 1;
            } else {
                player_levels[i] = current_player.currentlevel;
            }
        }
    }
    fclose(in_file);

    FILE *out_file = fopen("players.txt", "wt");
    if(out_file == NULL){
        printf("Error! Nem lehet megnyitni a file-t.");
    }
    fprintf(out_file, "%d\n", count);
    for(int i = 0; i < count; i++){
        fprintf(out_file, "%s %d\n", names[i], player_levels[i]);
    }

    fclose(out_file);
    free(player_levels);
    free2dimchar(names, count);
}
