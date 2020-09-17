#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "gameObjects.h"
#include "movementlogic.h"
#include "renderfunctions.h"
#include "myMacros.h"

void draw_asteroids(Asteroid *headAsteroid)
{
    if (headAsteroid == NULL || headAsteroid->data == NULL)
        return;

    for (size_t i = 0; i < NUM_OF_COLUMNS(headAsteroid->data->body); i += 1)
    {
        int endOfLine = i + 1 < NUM_OF_COLUMNS(headAsteroid->data->body) ? i + 1 : 0;
        al_draw_line(
            headAsteroid->data->body[0][i], headAsteroid->data->body[1][i],
            headAsteroid->data->body[0][endOfLine], headAsteroid->data->body[1][endOfLine],
            headAsteroid->data->color, 3.0);
    }

    draw_asteroids(headAsteroid->next);
}

// void add_asteroid(Asteroid **headAsteroid, float direction, float vx, float vy)
// {
//     if (*headAsteroid == NULL)
//         return;

//     Asteroid *currentAsteroid = *headAsteroid;
//     Asteroid *newAsteroid = malloc(sizeof(Asteroid));
//     AsteroidData *newAsteroidData = malloc(sizeof(AsteroidData));
//     if (newAsteroid == NULL || newAsteroidData == NULL)
//     {
//         puts("Can't allocate memory for newAsteroid or newAsteroidData!");
//         exit(2);
//     }

//     newAsteroidData->color = al_map_rgb(0, 128, 0);

//     /* Get a random angle between 0 and 360 */
//     newAsteroidData->direction = direction;

//     newAsteroidData->twist = 10.0;
//     newAsteroidData->isHitTwice = false;
//     newAsteroidData->isDuplicate = false;
//     newAsteroidData->vx = vx;
//     newAsteroidData->vy = vy;

//     for (size_t i = 0; i < NUM_OF_COLUMNS(currentAsteroid->data->body); i += 1)
//     {
//         newAsteroidData->body[0][i] = currentAsteroid->data->body[0][i] + vx;
//         newAsteroidData->body[1][i] = currentAsteroid->data->body[1][i] + vy;
//     }
//     newAsteroidData->centerOfRotation[0][0] = currentAsteroid->data->centerOfRotation[0][0] + vx;
//     newAsteroidData->centerOfRotation[1][0] = currentAsteroid->data->centerOfRotation[1][0] + vy;

//     while (currentAsteroid->next != NULL)
//         currentAsteroid = currentAsteroid->next;

//     newAsteroid->data = newAsteroidData;
//     newAsteroid->next = NULL;
//     currentAsteroid->next = newAsteroid;
// }


void add_asteroid(Asteroid **headAsteroid, float direction, float vx, float vy)
{
    if (*headAsteroid == NULL)
        return;

    Asteroid *currentAsteroid = *headAsteroid;
    Asteroid *newAsteroid = malloc(sizeof(Asteroid));
    AsteroidData *newAsteroidData = malloc(sizeof(AsteroidData));
    if (newAsteroid == NULL || newAsteroidData == NULL)
    {
        puts("Can't allocate memory for newAsteroid or newAsteroidData!");
        exit(2);
    }

    newAsteroidData->color = al_map_rgb(0, 128, 0);

    /* Get a random angle between 0 and 360 */
    newAsteroidData->direction = direction;

    newAsteroidData->twist = 10.0;
    newAsteroidData->isHitTwice = false;
    newAsteroidData->isDuplicate = false;
    newAsteroidData->vx = vx;
    newAsteroidData->vy = vy;

    for (size_t i = 0; i < NUM_OF_COLUMNS(currentAsteroid->data->body); i += 1)
    {
        newAsteroidData->body[0][i] = currentAsteroid->data->body[0][i] + vx;
        newAsteroidData->body[1][i] = currentAsteroid->data->body[1][i] + vy;
    }
    newAsteroidData->centerOfRotation[0][0] = currentAsteroid->data->centerOfRotation[0][0] + vx;
    newAsteroidData->centerOfRotation[1][0] = currentAsteroid->data->centerOfRotation[1][0] + vy;

    while (currentAsteroid->next != NULL)
        currentAsteroid = currentAsteroid->next;

    newAsteroid->data = newAsteroidData;
    newAsteroid->next = NULL;
    currentAsteroid->next = newAsteroid;
}


void rotate_asteroid(Asteroid **headAsteroid)
{
    if (*headAsteroid == NULL)
        return;

    float oldX;
    float oldY;

    for (size_t i = 0; i < NUM_OF_COLUMNS((*headAsteroid)->data->body); i += 1)
    {
        oldX = (*headAsteroid)->data->body[0][i];
        oldY = (*headAsteroid)->data->body[1][i];

        (*headAsteroid)->data->body[0][i] = rotate_x_cw(oldX, oldY, (*headAsteroid)->data->centerOfRotation);
        (*headAsteroid)->data->body[1][i] = rotate_y_cw(oldX, oldY, (*headAsteroid)->data->centerOfRotation);
    }
    (*headAsteroid)->data->direction = (*headAsteroid)->data->direction > 360 ? (*headAsteroid)->data->direction - 360 : (*headAsteroid)->data->direction;
    (*headAsteroid)->data->direction = (*headAsteroid)->data->direction < 0 ? (*headAsteroid)->data->direction + 360 : (*headAsteroid)->data->direction;

    rotate_asteroid(&(*headAsteroid)->next);
}

Asteroid *add_dup_asteroid(Asteroid **originalAsteroid, float vx, float vy)
{
    if (*originalAsteroid == NULL)
        return NULL;

    Asteroid *curr = *originalAsteroid;

    Asteroid *newAsteroid = malloc(sizeof(Asteroid));
    AsteroidData *newAsteroidData = malloc(sizeof(AsteroidData));

    if (newAsteroid == NULL || newAsteroidData == NULL)
        puts("Can't allocate memory for newAsteroid or newAsteroidData!");

    newAsteroidData->color = al_map_rgb(0, 128, 0);

    /* Get a random angle between 0 and 360 */
    newAsteroidData->direction = (*originalAsteroid)->data->direction;

    newAsteroidData->twist = 10.0;
    newAsteroidData->isHitTwice = false;
    newAsteroidData->isDuplicate = true;

    for (size_t i = 0; i < NUM_OF_COLUMNS((*originalAsteroid)->data->body); i += 1)
    {
        newAsteroidData->body[0][i] = (*originalAsteroid)->data->body[0][i] + vx;
        newAsteroidData->body[1][i] = (*originalAsteroid)->data->body[1][i] + vy;
    }

    newAsteroidData->centerOfRotation[0][0] = (*originalAsteroid)->data->centerOfRotation[0][0] + vx;
    newAsteroidData->centerOfRotation[1][0] = (*originalAsteroid)->data->centerOfRotation[1][0] + vy;

    newAsteroid->data = newAsteroidData;
    newAsteroid->next = NULL;

    while (curr->next != NULL)
        curr = curr->next;

    curr->next = newAsteroid;

    return newAsteroid;
}

void translate_asteroid(Asteroid **headAsteroid, MainWindow window)
{
    if (headAsteroid == NULL || *headAsteroid == NULL)
        return;

    Asteroid *curr = *headAsteroid;
    Asteroid *prev = NULL;

    while (curr != NULL)
    {
        /**
         * A variable that's either 0 when all the X axes of the vertices of an asteroid's body are still within the display's borders,
         * or it's the value that should be added or subtracted from these axes to get them back within the display's borders
         */
        float isXoffScreen = 0;

        /**
         * A variable that's either 0 when all the Y axes of the vertices of an asteroid's body are still within the display's borders,
         * or it's the value that should be added or subtracted from these axes to get them back within the display's borders
         */
        float isYoffScreen = 0;

        bool isAsteroidCompletelyOff = true;

        AsteroidData *currData = curr->data;
        Asteroid *next = curr->next;

        determine_direction(currData->direction, &(currData->vx), &(currData->vy), 1.0);
 
        /* Translate the asteroid and check for collisions with the borders */
        for (size_t i = 0; i < NUM_OF_COLUMNS(currData->body); i += 1)
        {
            currData->body[0][i] += currData->vx;
            currData->body[1][i] += currData->vy;

            if ((currData->body[0][i] < 0 || currData->body[0][i] > window.width) || (currData->body[1][i] < 0 || currData->body[1][i] > window.height))
            {
                if (currData->body[0][i] < 0)
                {
                    isXoffScreen = window.width;
                }
                else if (currData->body[0][i] > window.width)
                {
                    isXoffScreen = -1 * window.width;
                }

                if (currData->body[1][i] < 0)
                {
                    isYoffScreen = window.height;
                }
                else if (currData->body[1][i] > window.height)
                {
                    isYoffScreen = -1 * window.height;
                }
            }
            else
            {
                isAsteroidCompletelyOff = false;
            }
        }
        currData->centerOfRotation[0][0] += currData->vx;
        currData->centerOfRotation[1][0] += currData->vy;

        /** 
         * If all the asteroid's body is off screen, then delete it and free the memory
         */
        if (isAsteroidCompletelyOff)
        {
            if (prev != NULL)
            {
                prev->next = next;
            }
            else
            {
                *headAsteroid = next;
            }

            free(currData);
            free(curr);

            curr = next;
        }
        else
        {
            /** 
             * If only some of the asteroid's vertices are off screen, then create a duplicate asteroid
             */
            if ((isXoffScreen || isYoffScreen) && !(currData->isDuplicate))
            {
                add_dup_asteroid(&curr, isXoffScreen, isYoffScreen);
            }

            prev = curr;
            curr = curr->next;
        }
    }
}