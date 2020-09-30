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

Asteroid *create_new_asteroid(float (*centerOfRotation)[1], float direction)
{
    if (*centerOfRotation == NULL)
        return NULL;

    Asteroid *newAsteroid = malloc(sizeof(Asteroid));
    AsteroidData *newAsteroidData = malloc(sizeof(AsteroidData));
    if (newAsteroid == NULL || newAsteroidData == NULL)
    {
        puts("Can't allocate memory for newAsteroid or newAsteroidData!");
        exit(2);
    }

    /**
     * The coordinates schema for the initial asteroid that all the asteroids should follow
     */
    float body[2][12] = {
        {10 / 1.5, 5 / 1.5, 5 / 1.5, 20 / 1.5, 25 / 1.5, 35 / 1.5, 50 / 1.5, 50 / 1.5, 30 / 1.5, 50 / 1.5, 40 / 1.5, 30 / 1.5},
        {50 / 1.5, 35 / 1.5, 20 / 1.5, 20 / 1.5, 10 / 1.5, 10 / 1.5, 20 / 1.5, 25 / 1.5, 30 / 1.5, 40 / 1.5, 50 / 1.5, 45 / 1.5}};

    float translationFactor_x = body[0][8] - centerOfRotation[0][0];
    float translationFactor_y = body[1][8] - centerOfRotation[1][0];
    translationFactor_x *= -1;
    translationFactor_y *= -1;

    for (size_t i = 0; i < NUM_OF_COLUMNS(body); i += 1)
    {
        newAsteroidData->body[0][i] = body[0][i] + translationFactor_x;
        newAsteroidData->body[1][i] = body[1][i] + translationFactor_y;
    }

    /**
     * The center of the asteroid's rotation represented as a vector
     */
    newAsteroidData->centerOfRotation[0][0] = centerOfRotation[0][0];
    newAsteroidData->centerOfRotation[0][1] = centerOfRotation[1][0];

    newAsteroidData->direction = direction;
    newAsteroidData->twist = 10.0;
    newAsteroidData->isHit = false;
    newAsteroidData->isDupe = false;
    newAsteroidData->hasBeenDuped = false;
    newAsteroidData->color = al_map_rgb(0, 128, 0);

    newAsteroid->data = newAsteroidData;
    newAsteroid->next = NULL;
    return newAsteroid;
}

void append_asteroid(Asteroid **headAsteroid)
{
    if (*headAsteroid == NULL)
        return;

    Asteroid *currentAsteroid = *headAsteroid;
    float centerOfRotation[2][1] = {
        {320},
        {0}};
    Asteroid *newAsteroid = create_new_asteroid(centerOfRotation, rand() % 361);

    while (currentAsteroid->next != NULL)
        currentAsteroid = currentAsteroid->next;

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

void append_dup_asteroid(Asteroid **originalAsteroid, float vx, float vy)
{
    if (*originalAsteroid == NULL)
        return;

    Asteroid *curr = *originalAsteroid;
    float centerOfRotation[2][1] = {
        {curr->data->centerOfRotation[0][0] + vx},
        {curr->data->centerOfRotation[1][0] + vy}};
    Asteroid *newAsteroid = create_new_asteroid(centerOfRotation, curr->data->direction);

    newAsteroid->data->isDupe = true;

    while (curr->next != NULL)
        curr = curr->next;

    curr->next = newAsteroid;
}

void split_in_half(Asteroid **originalAsteroid)
{
    if (*originalAsteroid == NULL)
        return;

    Asteroid *curr = *originalAsteroid;
    float centerOfRotation[2][1] = {
        {curr->data->centerOfRotation[0][0]},
        {curr->data->centerOfRotation[1][0]}};
    float firstDirection = curr->data->direction - 45 >= 0 ? curr->data->direction - 45
                                                           : curr->data->direction - 45 + 360;
    float secDirection = curr->data->direction + 45 <= 360 ? curr->data->direction + 45
                                                              : curr->data->direction + 45 - 360;
    Asteroid *firstAst = create_new_asteroid(centerOfRotation, firstDirection);
    Asteroid *secondAst = create_new_asteroid(centerOfRotation, secDirection);

    firstAst->data->isHit = (secondAst->data->isHit = true);

    while (curr->next != NULL)
        curr = curr->next;

    curr->next = firstAst;
    firstAst->next = secondAst;
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
         * 2 variables that are either 0 when all the X or Y axes of the vertices of an asteroid's body are still within the display's borders,
         * or it's the value that should be added or subtracted from these axes to get them back within the display's borders
         */
        float isXoffScreen = 0;
        float isYoffScreen = 0;

        bool isCompletelyOnScreen = true;
        bool isCompletelyOffScreen = true;

        AsteroidData *currData = curr->data;

        determine_direction(currData->direction, &(currData->vx), &(currData->vy), 1.0);

        /* Translate the asteroid and check for collisions with the borders */
        for (size_t i = 0; i < NUM_OF_COLUMNS(currData->body); i += 1)
        {
            currData->body[0][i] += currData->vx;
            currData->body[1][i] += currData->vy;

            if ((currData->body[0][i] < 0 || currData->body[0][i] > window.width) || (currData->body[1][i] < 0 || currData->body[1][i] > window.height))
            {
                isCompletelyOnScreen = false;

                isXoffScreen = currData->body[0][i] < 0 ? window.width
                                                        : currData->body[0][i] > window.width ? -1 * window.width : 0;

                isYoffScreen = currData->body[1][i] < 0 ? window.height
                                                        : currData->body[1][i] > window.height ? -1 * window.height : 0;

                continue;
            }
            isCompletelyOffScreen = false;
        }
        currData->centerOfRotation[0][0] += currData->vx;
        currData->centerOfRotation[1][0] += currData->vy;

        /**
         * If the dupe asteroid is completely on screen now, stop treating it as a dupe,
         * in order to duplicate when it itself starts getting off the screen
         */
        if (isCompletelyOnScreen)
        {
            currData->isDupe = false;
            currData->hasBeenDuped = false;
        }

        /** 
         * If all of the asteroid's body is off screen, then delete it and free the memory
         */
        if (isCompletelyOffScreen)
        {
            Asteroid *next = curr->next;

            if (prev != NULL)
                prev->next = next;
            else
                *headAsteroid = next;

            free(currData);
            free(curr);

            curr = next;
            continue;
        }

        /** 
         * If only some of the asteroid's vertices are off screen, then create a duplicate asteroid
         */
        if ((isXoffScreen || isYoffScreen) && (!(currData->hasBeenDuped) && !(currData->isDupe)))
        {
            append_dup_asteroid(&curr, isXoffScreen, isYoffScreen);
            currData->hasBeenDuped = true;
        }

        prev = curr;
        curr = curr->next;
    }
}