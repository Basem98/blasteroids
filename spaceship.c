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

#define PI 3.14159265359

/**
 * Convert the angle to radiants before calling sin or cos
 */
#define COS(x) cosf(x *(PI / 180))
#define SIN(x) sinf(x *(PI / 180))

#define TRANSLATION_FACTOR_X 640.0
#define TRANSLATION_FACTOR_Y 360.0

/**
 * Draw the Spaceship using the lines coordinates in the Spaceship struct
 */
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
 * Rotation
 */
float rotate_x_right(float oldX, float oldY, float rotationAngle)
{
    float old_x = oldX - TRANSLATION_FACTOR_X;
    float old_y = oldY - TRANSLATION_FACTOR_Y;
    return (old_x * COS(rotationAngle)) - (old_y * SIN(rotationAngle));
}
float rotate_y_right(float oldX, float oldY, float rotationAngle)
{
    float old_x = oldX - TRANSLATION_FACTOR_X;
    float old_y = oldY - TRANSLATION_FACTOR_Y;
    return (-1 * old_x * SIN(rotationAngle)) + (old_y * COS(rotationAngle));
}
void rotate_ship_right(Spaceship *ship, float rotationAngle)
{
    for (int i = 0; i < 4; i += 1)
    {
        float oldX1 = ship->spaceLine[i]->x1;
        float oldY1 = ship->spaceLine[i]->y1;
        float oldX2 = ship->spaceLine[i]->x2;
        float oldY2 = ship->spaceLine[i]->y2;
        printf("oldY1: %f\n", oldY1);
        ship->spaceLine[i]->x1 = rotate_x_right(oldX1, oldY1, rotationAngle) + TRANSLATION_FACTOR_X;
        ship->spaceLine[i]->y1 = rotate_y_right(oldX1, oldY1, rotationAngle) + TRANSLATION_FACTOR_Y;
        ship->spaceLine[i]->x2 = rotate_x_right(oldX2, oldY2, rotationAngle) + TRANSLATION_FACTOR_X;
        ship->spaceLine[i]->y2 = rotate_y_right(oldX2, oldY2, rotationAngle) + TRANSLATION_FACTOR_Y;
        printf("newY1: %f\n", ship->spaceLine[i]->y1);
    }
}

/**
 * Translation
 */

/**
 * Translate the ship's head to origin
 */

/**
 * 1. Move the ship forward
 */
