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

#define NUM_OF_COLUMNS(x) sizeof(*(x)) / sizeof(float)
#define PI 3.14159265359
#define ROTATION_ANGLE 30.0

/**
 * Convert the angle to radiants before calling sin or cos
 */
#define COS(x) cosf(x *(PI / 180))
#define SIN(x) sinf(x *(PI / 180))

/**
 * The transformation matrices for clock-wise and counter clock-wise rotation
*/
float T_cw[2][2] = {
    COS(ROTATION_ANGLE), -1 * SIN(ROTATION_ANGLE),
    SIN(ROTATION_ANGLE), COS(ROTATION_ANGLE)
};

float T_ccw[2][2] = {
    COS(ROTATION_ANGLE), SIN(ROTATION_ANGLE),
    -1 * SIN(ROTATION_ANGLE), COS(ROTATION_ANGLE)
};



void draw_ship(Spaceship *ship)
{
    for (int i = 0; i < NUM_OF_COLUMNS(ship->body); i += 2)
    {
        if (i + 1 < NUM_OF_COLUMNS(ship->body))
            al_draw_line(ship->body[0][i], ship->body[1][i], ship->body[0][i + 1], ship->body[1][i + 1], ship->color, 3.0);
    }
}

/**
 * Functions that will handle the 2D transformation
 */

/**
 * Rotation around the ship->centerOfRotation point:
 */

float rotate_x_cw(float x, float y, float (*centerOfRotation)[1])
{
    /**
     * Translate the points back to the origin before rotating
    */
    float tx = x - centerOfRotation[0][0];
    float ty = y - centerOfRotation[1][0];

    return ((tx * T_cw[0][0]) + (ty * T_cw[0][1])) + centerOfRotation[0][0];
}
float rotate_y_cw(float x, float y, float (*centerOfRotation)[1])
{
    float tx = x - centerOfRotation[0][0];
    float ty = y - centerOfRotation[1][0];

    return ((tx * T_cw[1][0]) + (ty * T_cw[1][1])) + centerOfRotation[1][0];
}

float rotate_x_ccw(float x, float y, float (*centerOfRotation)[1])
{
    float tx = x - centerOfRotation[0][0];
    float ty = y - centerOfRotation[1][0];

    return ((tx * T_ccw[0][0]) + (ty * T_ccw[0][1])) + centerOfRotation[0][0];
}

float rotate_y_ccw(float x, float y, float (*centerOfRotation)[1])
{
    float tx = x - centerOfRotation[0][0];
    float ty = y - centerOfRotation[1][0];

    return ((tx * T_ccw[1][0]) + (ty * T_ccw[1][1])) + centerOfRotation[1][0];
}

void rotate_ship(Spaceship *ship, short rotationDirection)
{
    /**
     * Determine the direction of the rotation and the function to use based on the sign of the rotationDirection argument
    */
    float (*rotate_x)(float, float, float(*)[1]) = rotationDirection > 0 ? rotate_x_cw : rotate_x_ccw;
    float (*rotate_y)(float, float, float(*)[1]) = rotationDirection > 0 ? rotate_y_cw : rotate_y_ccw;

    float oldHeadX = ship->head[0][0];
    float oldHeadY = ship->head[1][0];
    ship->head[0][0] = rotate_x(oldHeadX, oldHeadY, ship->centerOfRotation);
    ship->head[1][0] = rotate_y(oldHeadX, oldHeadY, ship->centerOfRotation);

    float oldX;
    float oldY;

    for (int i = 0; i < NUM_OF_COLUMNS(ship->body); i += 1)
    {
        oldX = ship->body[0][i];
        oldY = ship->body[1][i];
        ship->body[0][i] = rotate_x(oldX, oldY, ship->centerOfRotation);
        ship->body[1][i] = rotate_y(oldX, oldY, ship->centerOfRotation);
    }

    ship->direction += (rotationDirection * ROTATION_ANGLE);
    ship->direction = ship->direction > 360 ? ship->direction - 360 : ship->direction;
    ship->direction = ship->direction < 0 ? ship->direction + 360 : ship->direction;
}

/**
 * Translation
*/
void determine_direction(Spaceship *ship, float velocity)
{
    float direction = ship->direction;
    if (direction == 0 || direction == 360)
    {
        ship->vx = 0;
        ship->vy = -1 * velocity;
    }
    if (direction == 90)
    {
        ship->vx = velocity;
        ship->vy = 0;
    }
    if (direction == 180)
    {
        ship->vx = 0;
        ship->vy = velocity;
    }
    if (direction == 270)
    {
        ship->vx = -1 * velocity;
        ship->vy = 0;
    }
    if ((direction > 0 && direction < 90))
    {
        ship->vx = velocity;
        ship->vy = -1 * velocity;
    }
    if ((direction > 90 && direction < 180))
    {
        ship->vx = velocity;
        ship->vy = velocity;
    }
    if ((direction > 180 && direction < 270))
    {
        ship->vx = -1 * velocity;
        ship->vy = velocity;
    }
    if ((direction > 270 && direction < 360))
    {
        ship->vx = -1 * velocity;
        ship->vy = -1 * velocity;
    }
}
void translate_ship(Spaceship *ship, MainWindow window)
{
    determine_direction(ship, 5);
    /**
     * Check if the head of the ship has reached any of the display's edges before translating it any further
    */
    if (((ship->head[0][0] + ship->vx) > window.width || (ship->head[0][0] + ship->vx) < 0) || ((ship->head[1][0] + ship->vy) > window.height || (ship->head[1][0] + ship->vy) < 0))
        return;

    ship->head[0][0] += ship->vx;
    ship->head[1][0] += ship->vy;
    ship->centerOfRotation[0][0] += ship->vx;
    ship->centerOfRotation[1][0] += ship->vy;

    for (int i = 0; i < NUM_OF_COLUMNS(ship->body); i += 1)
    {
        ship->body[0][i] += ship->vx;
        ship->body[1][i] += ship->vy;
    }
}