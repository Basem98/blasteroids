#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <math.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include "gameObjects.h"
#include "movementlogic.h"
#include "renderfunctions.h"

#define PI 3.14159265359

/**
 * Convert the angle to radiants before calling sin or cos
 */
#define COS(x) cos(x *(PI / 180))
#define SIN(x) sin(x *(PI / 180))


void draw_ship(Spaceship *ship)
{
    for (int i = 0; i < 4; i += 1)
    {
        al_draw_line(ship->spaceLine[i]->x1, ship->spaceLine[i]->y1, ship->spaceLine[i]->x2, ship->spaceLine[i]->y2, ship->color, 3.0);
    }
}

/**
 * Functions that will handle the 2D transformation
 */

/**
 * Rotation around the ship->centerOfRotation point:
 */

double rotate_x_cw(double oldX, double oldY, SpaceHead *centerOfRotation, double rotationAngle)
{
    double old_x = oldX - centerOfRotation->x;
    double old_y = oldY - centerOfRotation->y;

    return ((old_x * COS(rotationAngle)) - (old_y * SIN(rotationAngle))) + centerOfRotation->x;
}
double rotate_y_cw(double oldX, double oldY, SpaceHead *centerOfRotation, double rotationAngle)
{
    double old_x = oldX - centerOfRotation->x;
    double old_y = oldY - centerOfRotation->y;

    return ((old_x * SIN(rotationAngle)) + (old_y * COS(rotationAngle))) + centerOfRotation->y;
}

double rotate_x_anticw(double oldX, double oldY, SpaceHead *centerOfRotation, double rotationAngle)
{
    double old_x = oldX - centerOfRotation->x;
    double old_y = oldY - centerOfRotation->y;

    return ((old_x * COS(rotationAngle)) + (old_y * SIN(rotationAngle))) + centerOfRotation->x;
}

double rotate_y_anticw(double oldX, double oldY, SpaceHead *centerOfRotation, double rotationAngle)
{
    double old_x = oldX - centerOfRotation->x;
    double old_y = oldY - centerOfRotation->y;

    return ((-1 * old_x * SIN(rotationAngle)) + (old_y * COS(rotationAngle))) + centerOfRotation->y;
}

void rotate_ship(Spaceship *ship, double (*rotate_x)(double, double, SpaceHead *, double), double (*rotate_y)(double, double, SpaceHead *, double))
{
    double oldHeadX = ship->head->x;
    double oldHeadY = ship->head->y;
    ship->head->x = rotate_x(oldHeadX, oldHeadY, ship->centerOfRotation, ship->rotationAngle);
    ship->head->y = rotate_y(oldHeadX, oldHeadY, ship->centerOfRotation, ship->rotationAngle);
    for (int i = 0; i < 4; i += 1)
    {
        double oldX1 = ship->spaceLine[i]->x1;
        double oldY1 = ship->spaceLine[i]->y1;
        double oldX2 = ship->spaceLine[i]->x2;
        double oldY2 = ship->spaceLine[i]->y2;
        ship->spaceLine[i]->x1 = rotate_x(oldX1, oldY1, ship->centerOfRotation, ship->rotationAngle);
        ship->spaceLine[i]->y1 = rotate_y(oldX1, oldY1, ship->centerOfRotation, ship->rotationAngle);
        ship->spaceLine[i]->x2 = rotate_x(oldX2, oldY2, ship->centerOfRotation, ship->rotationAngle);
        ship->spaceLine[i]->y2 = rotate_y(oldX2, oldY2, ship->centerOfRotation, ship->rotationAngle);
    }
}