#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "gameObjects.h"
#include "movementlogic.h"
#include "renderfunctions.h"

void draw_blasts(Blast *headBlast)
{
    if (headBlast == NULL || headBlast->blastData == NULL)
        return;

    al_draw_line(
        headBlast->blastData->body[0][0], headBlast->blastData->body[1][0],
        headBlast->blastData->body[0][1], headBlast->blastData->body[1][1],
        headBlast->blastData->color, 3.0);

    draw_blasts(headBlast->next);
}


void translate_blast(Blast **headBlast, MainWindow window)
{
    if (*headBlast == NULL)
        return;

    Blast *cur = *headBlast;
    Blast *prev = NULL;

    while (cur != NULL)
    {
        BlastData *curData = cur->blastData;
        if ((curData->body[0][1] < 0 || curData->body[0][1] > window.width)
        || (curData->body[1][1] < 0 || curData->body[1][1] > window.height))
        {
            Blast *nextBlast = cur->next;
            if (prev != NULL)
            {
                prev->next = nextBlast;
            } else {
                *headBlast = nextBlast;
            }
            free(curData);
            free(cur);
            cur = nextBlast;
            continue;
        }

        determine_direction(curData->direction, &(curData->vx), &(curData->vy), 5.0);
        for (int i = 0; i < 2; i += 1)
        {
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
        puts("Unable to allocate memory for newBlast or newBlastData!\n");

    newBlastData->vx = 5.0;
    newBlastData->vy = 5.0;
    newBlastData->direction = ship.direction;

    determine_direction(newBlastData->direction, &(newBlastData->vx), &(newBlastData->vy), 3.0);

    newBlastData->body[0][0] = ship.head[0][0];
    newBlastData->body[0][1] = ship.head[0][0] + newBlastData->vx;
    newBlastData->body[1][0] = ship.head[1][0];
    newBlastData->body[1][1] = ship.head[1][0] + newBlastData->vy;

    newBlastData->isGone = false;
    newBlastData->color = al_map_rgb(0, 128, 0);

    newBlast->blastData = newBlastData;
    newBlast->next = NULL;

    while (*currentBlast != NULL)
        currentBlast = &(*currentBlast)->next;

    *currentBlast = newBlast;
}