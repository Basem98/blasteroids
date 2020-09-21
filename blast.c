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

void draw_blasts(Blast *headBlast)
{
    if (headBlast == NULL || headBlast->data == NULL)
        return;

    for (size_t i = 0; i + 1 < NUM_OF_COLUMNS(headBlast->data->body); i += 1)
        al_draw_line(
            headBlast->data->body[0][i], headBlast->data->body[1][i],
            headBlast->data->body[0][i + 1], headBlast->data->body[1][i + 1],
            headBlast->data->color, 3.0);

    draw_blasts(headBlast->next);
}

void translate_blast(Blast **headBlast, Asteroid **asteroids, MainWindow window)
{
    if (*headBlast == NULL)
        return;

    Blast *cur = *headBlast;
    Blast *prev = NULL;

    while (cur != NULL)
    {
        BlastData *curData = cur->data;
        /* Check if any collision happened between the blast and any of the display's edges */
        if (((curData->body[0][1] < 0 || curData->body[0][1] > window.width) || (curData->body[1][1] < 0 || curData->body[1][1] > window.height)) || (blast_asteroid_coll(&cur, asteroids)))
        {
            Blast *nextBlast = cur->next;
            if (prev != NULL)
            {
                /* If there's a previous blast, then assign the next blast to its next field */
                prev->next = nextBlast;
            }
            else
            {
                /* If there's no previous blast, then make the next blast the head */
                *headBlast = nextBlast;
            }
            /* Delete the blast that collided with the display's edges */
            free(curData);
            free(cur);
            cur = nextBlast;
            continue;
        }

        determine_direction(curData->direction, &(curData->vx), &(curData->vy), 5.0);
        for (int i = 0; i < 2; i += 1)
        {
            /* Translate both of the blast's axes */
            curData->body[0][i] += curData->vx;
            curData->body[1][i] += curData->vy;
        }

        prev = cur;
        cur = cur->next;
    }
}

void add_blast(Spaceship ship, Blast **headBlast)
{
    Blast **currentBlast = headBlast;
    Blast *newBlast = malloc(sizeof(Blast));
    BlastData *newBlastData = malloc(sizeof(BlastData));

    if (newBlast == NULL || newBlastData == NULL)
    {
        puts("Unable to allocate memory for newBlast or newBlastData!\n");
        exit(2);
    }

    newBlastData->vx = 5.0;
    newBlastData->vy = 5.0;
    newBlastData->direction = ship.direction;

    determine_direction(newBlastData->direction, &(newBlastData->vx), &(newBlastData->vy), 3.0);

    newBlastData->body[0][0] = ship.head[0][0];
    newBlastData->body[0][1] = ship.head[0][0] + newBlastData->vx;
    newBlastData->body[1][0] = ship.head[1][0];
    newBlastData->body[1][1] = ship.head[1][0] + newBlastData->vy;

    newBlastData->color = al_map_rgb(0, 128, 0);

    newBlast->data = newBlastData;
    newBlast->next = NULL;

    while (*currentBlast != NULL)
        currentBlast = &(*currentBlast)->next;

    *currentBlast = newBlast;
}

bool blast_asteroid_coll(Blast **currBlast, Asteroid **asteroids)
{
    if (*currBlast == NULL || *asteroids == NULL)
        return false;

    bool collision = false;
    Asteroid *currAst = *asteroids;
    Asteroid *prevAst = NULL;
    float px = (*currBlast)->data->body[0][1];
    float py = (*currBlast)->data->body[1][1];
    while (currAst != NULL)
    {
        AsteroidData *currData = currAst->data;
        for (size_t currVertex = 0; currVertex < NUM_OF_COLUMNS(currData->body); currVertex += 1)
        {
            int nextVertex = currVertex + 1 < NUM_OF_COLUMNS(currData->body) ? currVertex + 1 : 0;
            float vc_x = currData->body[0][currVertex];
            float vc_y = currData->body[1][currVertex];
            float vn_x = currData->body[0][nextVertex];
            float vn_y = currData->body[1][nextVertex];
            if (((vc_y > py) != (vn_y > py)) && (px < (vn_x - vc_x) * (py - vc_y) / (vn_y - vc_y) + vc_x))
            {
                collision = !collision;
            }
        }
        if (collision)
            break;

        prevAst = currAst;
        currAst = currAst->next;
    }
    if (collision)
    {
        if (!currAst->data->isHit)
            split_in_half(&currAst);

        if (prevAst)
        {
            prevAst->next = currAst->next;
        }
        else
        {
            *asteroids = currAst->next;
        }
        free(currAst->data);
        free(currAst);
    }
    return collision;
}
