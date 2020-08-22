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
#define COS(x) cosf(fabs(x) * (PI / 180))
#define SIN(x) sinf(fabs(x) * (PI / 180))

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

float rotate_x_cw(float oldX, float oldY, SpaceHead *centerOfRotation, float rotationAngle)
{
    float old_x = oldX - centerOfRotation->x;
    float old_y = oldY - centerOfRotation->y;

    return ((old_x * COS(rotationAngle)) - (old_y * SIN(rotationAngle))) + centerOfRotation->x;
}
float rotate_y_cw(float oldX, float oldY, SpaceHead *centerOfRotation, float rotationAngle)
{
    float old_x = oldX - centerOfRotation->x;
    float old_y = oldY - centerOfRotation->y;

    return ((old_x * SIN(rotationAngle)) + (old_y * COS(rotationAngle))) + centerOfRotation->y;
}

float rotate_x_anticw(float oldX, float oldY, SpaceHead *centerOfRotation, float rotationAngle)
{
    float old_x = oldX - centerOfRotation->x;
    float old_y = oldY - centerOfRotation->y;

    return ((old_x * COS(rotationAngle)) + (old_y * SIN(rotationAngle))) + centerOfRotation->x;
}

float rotate_y_anticw(float oldX, float oldY, SpaceHead *centerOfRotation, float rotationAngle)
{
    float old_x = oldX - centerOfRotation->x;
    float old_y = oldY - centerOfRotation->y;

    return ((-1 * old_x * SIN(rotationAngle)) + (old_y * COS(rotationAngle))) + centerOfRotation->y;
}

void rotate_ship(Spaceship *ship, float rotationAngle, float (*rotate_x)(float, float, SpaceHead *, float), float (*rotate_y)(float, float, SpaceHead *, float))
{
    float oldHeadX = ship->head->x;
    float oldHeadY = ship->head->y;
    ship->head->x = rotate_x(oldHeadX, oldHeadY, ship->centerOfRotation, rotationAngle);
    ship->head->y = rotate_y(oldHeadX, oldHeadY, ship->centerOfRotation, rotationAngle);

    for (int i = 0; i < 4; i += 1)
    {
        float oldX1 = ship->spaceLine[i]->x1;
        float oldY1 = ship->spaceLine[i]->y1;
        float oldX2 = ship->spaceLine[i]->x2;
        float oldY2 = ship->spaceLine[i]->y2;
        ship->spaceLine[i]->x1 = rotate_x(oldX1, oldY1, ship->centerOfRotation, rotationAngle);
        ship->spaceLine[i]->y1 = rotate_y(oldX1, oldY1, ship->centerOfRotation, rotationAngle);
        ship->spaceLine[i]->x2 = rotate_x(oldX2, oldY2, ship->centerOfRotation, rotationAngle);
        ship->spaceLine[i]->y2 = rotate_y(oldX2, oldY2, ship->centerOfRotation, rotationAngle);
    }

    ship->direction += rotationAngle;
    ship->direction = ship->direction > 360 ? ship->direction - 360 : ship->direction;
    ship->direction = ship->direction < 0 ? ship->direction + 360 : ship->direction;
}

/**
 * Translation
*/
void determine_direction(Spaceship *ship)
{
    float direction = ship->direction;
    if (direction == 0 || direction == 360)
    {
        ship->vx = 0;
        ship->vy = -5;
    }
    if (direction == 90)
    {
        ship->vx = 5;
        ship->vy = 0;
    }
    if (direction == 180)
    {
        ship->vx = 0;
        ship->vy = 5;
    }
    if (direction == 270)
    {
        ship->vx = -5;
        ship->vy = 0;
    }
    if ((direction > 0 && direction < 90))
    {
        ship->vx = 5;
        ship->vy = -5;
    }
    if ((direction > 90 && direction < 180))
    {
        ship->vx = 5;
        ship->vy = 5;
    }
    if ((direction > 180 && direction < 270))
    {
        ship->vx = -5;
        ship->vy = 5;
    }
    if ((direction > 270 && direction < 360))
    {
        ship->vx = -5;
        ship->vy = -5;
    }
}
void translate_ship(Spaceship *ship, MainWindow window)
{
    determine_direction(ship);
    /**
     * Check if the head of the ship has reached any of the display's edges before translating it any further
    */
    if (((ship->head->x + ship->vx) > window.width || (ship->head->x + ship->vx) < 0) || ((ship->head->y + ship->vy) > window.height || (ship->head->y + ship->vy) < 0))
        return;

    ship->head->x += ship->vx;
    ship->head->y += ship->vy;
    ship->centerOfRotation->x += ship->vx;
    ship->centerOfRotation->y += ship->vy;
    for (int i = 0; i < 4; i += 1)
    {
        ship->spaceLine[i]->x1 += ship->vx;
        ship->spaceLine[i]->y1 += ship->vy;
        ship->spaceLine[i]->x2 += ship->vx;
        ship->spaceLine[i]->y2 += ship->vy;
    }
}