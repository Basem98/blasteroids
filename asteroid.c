#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <math.h>
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
     * The coordinates schema that all the asteroids should follow
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
    newAsteroidData->color = al_map_rgb(255, 0, 0);

    newAsteroid->data = newAsteroidData;
    newAsteroid->next = NULL;
    return newAsteroid;
}

void append_asteroid(Asteroid **headAsteroid)
{
    if (*headAsteroid == NULL)
        return;

    Asteroid *currentAsteroid = *headAsteroid;

    /**
     * Create a list of 4 different angles
     * each one with an index corresponding to the number of a column,
     * which represents its appropriate vertex in the 2x4 possibleCenters matrix
     */
    float possibleAngles[4] = {45, 135, 225, 315};
    float possibleCenters[2][4] = {
        {20, 20, 620, 620},
        {460, 20, 20, 460}};
    int index = rand() % 4;
    float angle;
    float centerOfRotation[2][1];
    centerOfRotation[0][0] = possibleCenters[0][index];
    centerOfRotation[1][0] = possibleCenters[1][index];
    angle = possibleAngles[index];

    Asteroid *newAsteroid = create_new_asteroid(centerOfRotation, angle);

    while (currentAsteroid->next != NULL)
        currentAsteroid = currentAsteroid->next;

    currentAsteroid->next = newAsteroid;
}

void rotate_asteroid(Asteroid **headAsteroid)
{
    if (*headAsteroid == NULL)
        return;

    Asteroid *curr = *headAsteroid;
    while (curr != NULL)
    {
        float oldX;
        float oldY;

        for (size_t i = 0; i < NUM_OF_COLUMNS((*headAsteroid)->data->body); i += 1)
        {
            oldX = curr->data->body[0][i];
            oldY = curr->data->body[1][i];

            curr->data->body[0][i] = rotate_x_cw(oldX, oldY, curr->data->centerOfRotation);
            curr->data->body[1][i] = rotate_y_cw(oldX, oldY, curr->data->centerOfRotation);
        }
        curr->data->direction = curr->data->direction > 360 ? curr->data->direction - 360 : curr->data->direction;
        curr->data->direction = curr->data->direction < 0 ? curr->data->direction + 360 : curr->data->direction;

        curr = curr->next;
    }
}

void add_dup_asteroid(Asteroid **originalAsteroid)
{
    if (*originalAsteroid == NULL)
        return;

    Asteroid *curr = *originalAsteroid;
    float direction = curr->data->direction;
    float dupeCenterOfRotation[2][1] = {
        {fabs(640 - curr->data->centerOfRotation[0][0])},
        {fabs(480 - curr->data->centerOfRotation[1][0])}};

    /* Make sure the asteroid keeps moving in a straight line if the original direction is 0, 90, 180, 270 or 360 */
    dupeCenterOfRotation[0][0] = fabs(COS(direction)) == 1 ? curr->data->centerOfRotation[0][0] : dupeCenterOfRotation[0][0];
    dupeCenterOfRotation[1][0] = fabs(SIN(direction)) == 1 ? curr->data->centerOfRotation[1][0] : dupeCenterOfRotation[1][0];

    /* Make the asteroid appear gradually on the other side of the screen, instead of just popping out of nowhere */
    determine_direction(direction, &(curr->data->vx), &(curr->data->vy), 25.0);
    dupeCenterOfRotation[0][0] += ((curr->data->vx / fabs(SIN(direction))));
    dupeCenterOfRotation[1][0] += ((curr->data->vy / fabs(COS(direction))));

    if (dupeCenterOfRotation[0][0] >= 640 || dupeCenterOfRotation[0][0] <= 0 || dupeCenterOfRotation[1][0] >= 640 || dupeCenterOfRotation[1][0] <= 0)
        return;
    else
    {
        dupeCenterOfRotation[0][0] += (-2 * (curr->data->vx / fabs(SIN(direction))));
        dupeCenterOfRotation[1][0] += (-2 * (curr->data->vy / fabs(COS(direction))));
    }

    Asteroid *newAsteroid = create_new_asteroid(dupeCenterOfRotation, direction);
    curr->data->hasBeenDuped = true;
    newAsteroid->data->isDupe = true;

    newAsteroid->next = curr->next;
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

    secondAst->next = curr->next;
    firstAst->next = secondAst;
    curr->next = firstAst;
}

void translate_asteroid(Asteroid **headAsteroid, MainWindow window)
{
    if (headAsteroid == NULL || *headAsteroid == NULL)
        return;

    Asteroid *curr = *headAsteroid;
    Asteroid *prev = NULL;

    while (curr != NULL)
    {
        bool isCompletelyOnScreen = true;
        bool isCompletelyOffScreen = true;

        AsteroidData *currData = curr->data;

        determine_direction(currData->direction, &(currData->vx), &(currData->vy), 1.0);

        /* Translate the asteroid and check for collisions with the borders */
        for (size_t i = 0; i < NUM_OF_COLUMNS(currData->body); i += 1)
        {
            currData->body[0][i] += currData->vx;
            currData->body[1][i] += currData->vy;

            if ((currData->body[0][i] <= 0 || currData->body[0][i] >= window.width) || (currData->body[1][i] <= 0 || currData->body[1][i] >= window.height))
            {
                isCompletelyOnScreen = false;
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
        }

        /** 
         * If all of the asteroid's body is off screen, then delete it and free the memory
         */
        if ((isCompletelyOffScreen && currData->hasBeenDuped) || ((currData->centerOfRotation[0][0] + 19 < 0 || currData->centerOfRotation[0][0] - 19 > window.width) || (currData->centerOfRotation[1][0] + 19 < 0 || currData->centerOfRotation[1][0] - 19 > window.height)))
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
        if (!isCompletelyOnScreen && !((currData->hasBeenDuped) || (currData->isDupe)))
        {
            add_dup_asteroid(&curr);
        }

        prev = curr;
        curr = curr->next;
    }
}