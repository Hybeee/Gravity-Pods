#ifndef CHARGE_H_INCLUDED
#define CHARGE_H_INCLUDED
#include <math.h>
#include <stdlib.h>

#include "debugmalloc.h"

extern int WINDOW_HEIGHT;

typedef struct Linked_Charges{
    int pos_x, pos_y;

    struct Linked_Charges *next;
}Linked_Charges;

///Toltes tipus mindegyik fieldje egy vektort tarol
typedef struct Charge{
    double x_pos, y_pos;
    double x_vel, y_vel;
} Charge;

void chargeInit(Charge* charge, double angle);
void calcNewPos(Charge *charge, double frame_time);
void calcNewVelocity(Charge *charge, double frame_time, double *a_vector);
double* calcNewAccelaration(Charge const *charge, Linked_Charges *b);
void freeLinkedCharges(Linked_Charges *b);


#endif // CHARGE_H_INCLUDED
