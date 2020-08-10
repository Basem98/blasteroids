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
// #include "renderFunctions.h"


/* Draw the Spaceship using the lines coordinates in the Spaceship struct */
void draw_ship(Spaceship *ship)
{
    ship->color = al_map_rgb(0, 128, 0);
    al_draw_line(ship->leftLine->x1, ship->leftLine->y1, ship->leftLine->x2, ship->leftLine->y2, ship->color, 3.0);
    al_draw_line(ship->rightLine->x1, ship->rightLine->y1, ship->rightLine->x2, ship->rightLine->y2, ship->color, 3.0);
    al_draw_line(ship->leftTale->x1, ship->leftTale->y1, ship->leftTale->x2, ship->leftTale->y2, ship->color, 3.0);
    al_draw_line(ship->rightTale->x1, ship->rightTale->y1, ship->rightTale->x2, ship->rightTale->y2, ship->color, 3.0);
}

/* Functions that will handle the 2D transformation */
/* Rotation */
// Convert the angle of rotation from degree to radiants
double degToRad(double degree)
{
    return degree * (PI / 180);
}

void rotate_ship_right(Spaceship *ship)
{
    
}
/* Translation */
/* 1. Move the ship forward */
void move_forward()