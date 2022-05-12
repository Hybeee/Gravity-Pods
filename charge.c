#include "charge.h"

///* Charge struktura inicializalasa */
void chargeInit(Charge* charge, double angle){
    //lespawnolja a toltest az agyu vegebe a drawCharge fuggveny segitsegevel
    angle = (angle / 360) * 2 * 3.14;

    //parameterek az agyu helyzete alapjan. nemctrlcctrlv esku
    int length = WINDOW_HEIGHT / 10;
    int center_x = WINDOW_HEIGHT * 16 / 9 * 1 / 20;
    int center_y = WINDOW_HEIGHT / 2;

    //agyu vegebe kerul a toltes, amikor eloszor kirajzolodik
    charge->x_pos = cos(angle) * length + center_x;
    charge->y_pos = sin(angle) * length + center_y;

    //sebesseg x, y komponense attol fugg, hogy az agyu merre all.
    charge->x_vel = cos(angle) * 3 * WINDOW_HEIGHT / 800;
    charge->y_vel = sin(angle) * 3 * WINDOW_HEIGHT / 800;
}

//Alabbi ket fuggveny: ds = v * dt
//                   : dv = sum(a) * dt

///* toltes uj poziciojanak meghatarozasa a sebessege alapjan */
void calcNewPos(Charge *charge, double frame_time){
    charge->x_pos += charge->x_vel * frame_time * WINDOW_HEIGHT / 800;
    charge->y_pos += charge->y_vel * frame_time * WINDOW_HEIGHT / 800;
    if(charge->x_pos > WINDOW_HEIGHT * 16 / 9 + 1 || charge->y_pos > WINDOW_HEIGHT + 1 || charge->x_pos < 0 || charge->y_pos < 0){
        charge->x_vel = 0;
        charge->y_vel = 0;
    }
}

///* toltes sebessegenek meghatarozasa a gyorsulasvektor alapjan */
void calcNewVelocity(Charge* charge, double frame_time, double* a_vector){
    //frame-timeonkent valtoztatja meg a sebessegvektor nagysagat.
    //a_vector tomb: [ax, ay];
    charge->x_vel += a_vector[0] * frame_time;
    charge->y_vel += a_vector[1] * frame_time;
    free(a_vector);
}

///* Futasidoben kiszamolja az eredo gyorsulas x, y vektorat a lehelyezett toltesek alapjan */
double* calcNewAccelaration(Charge const *charge, Linked_Charges *b){
    //2 elemu tomb. elso elem: ax, masodik elem: ay
    double* a_sum = (double*) malloc(sizeof(double) * 2); //calcNewVelocity szabaditja fel
    a_sum[0] = 0;
    a_sum[1] = 0;

    //segedvaltozok a fuggvenyben
    double acc_vector = 0;
    double ax = 0;
    double ay = 0;

    //vegigloopol a lancolt listan, majd kiszamolja a lerakott es kilott toltes tavolsagat
    //ha ez kisebb mint egy ertek, akkor a gyorsulasvektorhoz hozzaadja a ket pontot osszekoto vektort
    for(Linked_Charges *m = b; m != NULL; m = m->next){
        //tavolsag kiszamitasa a gyorsulasvektor meghatarozasahoz
        double x_distance = -1 * (charge->x_pos - m->pos_x);
        double y_distance = -1 * (charge->y_pos - m->pos_y);
        double distance = sqrt((x_distance * x_distance) + (y_distance * y_distance));


        //Csak akkor hat ero a toltesre ha adott tavolsagon belul helyezkedik el
        if(distance <= WINDOW_HEIGHT / 4 && distance > 0){
            //Wikipedia-s Coulomb ero vektoros keplete
            acc_vector = (WINDOW_HEIGHT / 15) * (distance / (abs(distance * distance * distance)));
            double angle = atan2((double)y_distance, (double)x_distance);
            ax = acc_vector * cos(angle);
            ay = acc_vector * sin(angle);
        }
    }


    a_sum[0] += ax * WINDOW_HEIGHT / 80;
    a_sum[1] += ay * WINDOW_HEIGHT / 80;

    return a_sum;
}

///* Felszabaditja a tolteseket tartalmazo lancolt listat */
void freeLinkedCharges(Linked_Charges *b){
    Linked_Charges *m = b;

    while(m != NULL){
        Linked_Charges *next = m->next;
        free(m);
        m = next;
    }
}
