#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include "gameObjects.h"
#include "movementlogic.h"
#include "renderfunctions.h"
#include "myMacros.h"

/**
 * Basic error handler
 */
void error(char *err)
{
    fprintf(stderr, "%s: %s", err, strerror(errno));
    exit(2);
}

int main()
{
    /**
     * Initialize Allegro's core library and all the addons to be used in the project
     */
    if (!al_init())
        error("Can't initialize Allegro");
    if (!al_install_keyboard())
        error("Can't initialize Allegro");
    if (!al_init_primitives_addon())
        error("Can't initialize primtives addon");
    if (!al_init_font_addon())
        error("Can't initialize font addon");

    /**
     * Create the game's main timer that ticks 60 tick per second
     */
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60.0);
    if (!timer)
        error("Can't create game timer");

    /** 
     * Create the event queue that will recieve any event
     * that comes from a registered event source,
     * then push that event in a FIFO manner to an ALLEGRO_EVENT structure
     */
    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    if (!queue)
        error("Can't create event queue");

    ALLEGRO_FONT *font = al_create_builtin_font();
    if (!font)
        error("Can't create font");

    /**
     * The dimensions for the initial display
     */
    Display mainDisplay = {.width = 640, .height = 540};
    Display gameDisplay = {.width = 640, .height = 480};
    ALLEGRO_DISPLAY *display;

    if (!(display = al_create_display(mainDisplay.width, mainDisplay.height)))
        error("Can't draw main display");

    /**
     * Antialiasing
     */
    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

    /**
     * Notify the event queue of the sources, from which it should listen for dispatched events
     */
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_display_event_source(display));

    /* Player Info */
    int currentScore = 0;
    char scoreInfo[100];
    float currentSpeed = 0.5;
    char lifeInfo[100];
    int currentAsteroids = 1;
    char asteroidsInfo[100];

    /* The Spaceship */
    Spaceship ship = {
        .vx = 5,
        .vy = 5,
        /**
         * The coordinates for the spaceship's body represented by a 2x8 matrix
         */
        .body = {
            {168.0, 160.0, 152.0, 160.0, 154.0, 159.0, 166.0, 161.0},
            {140.0, 120.0, 140.0, 120.0, 135.0, 135.0, 135.0, 135.0}},
        /**
         * The point where the spaceship's head is headed represented as a vector
         */
        .head = {{160.0}, {120.0}},
        /**
         * The center of the spaceship's rotation represented as a vector
         */
        .centerOfRotation = {{160.0}, {135.0}},
        .direction = 0.0,
        .color = al_map_rgb(255, 255, 255)};

    /* The base asteroid */
    float centerOfRotation[2][1] = {
        {19},
        {19}};
    Asteroid *headAsteroid = create_new_asteroid(centerOfRotation, 135.0);

    /* The blasts */
    Blast *headBlast = NULL;

    /**
     * The ALLEGRO_EVENT structure that will recieve any dispatched event
     */
    ALLEGRO_EVENT event;

    /**
     * Start the game's timer
     */
    al_start_timer(timer);

    bool wannaExit = false;
    bool reRender = true;

    /**
     * The game's main loop
     */
    while (1)
    {
        /* This function pauses your program at this point
         * until an event is fired from any registered event source
         */
        al_wait_for_event(queue, &event);

        switch (event.type)
        {
        case ALLEGRO_EVENT_TIMER:
        {
            /**
             * The game's logic is calculated here exactly every 1/60th of a second (60 ticks per sec)
             */
            rotate_asteroid(&headAsteroid);
            translate_asteroid(&headAsteroid, gameDisplay, &currentAsteroids, &currentSpeed, currentScore);
            translate_blast(&headBlast, &headAsteroid, gameDisplay, &currentScore, currentAsteroids);
            sprintf(scoreInfo, "Score: %i", currentScore);
            sprintf(lifeInfo, "Current Speed Of Asteroids: %f", currentSpeed);
            sprintf(asteroidsInfo, "Number of Asteroids: %i", currentAsteroids);

            reRender = true;
            break;
        }
        case ALLEGRO_EVENT_KEY_CHAR:
        {
            if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                wannaExit = true;
            if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT)
            {
                rotate_ship(&ship, 1);
            }
            if (event.keyboard.keycode == ALLEGRO_KEY_LEFT)
            {
                rotate_ship(&ship, -1);
            }
            if (event.keyboard.keycode == ALLEGRO_KEY_UP)
            {
                translate_ship(&ship, gameDisplay);
            }
            if (event.keyboard.keycode == ALLEGRO_KEY_SPACE)
            {
                add_blast(ship, &headBlast);
            }

            break;
        }
        case ALLEGRO_EVENT_DISPLAY_CLOSE:
        {
            wannaExit = true;
            break;
        }
        }

        if (wannaExit)
            break;

        if (reRender && al_event_queue_is_empty(queue))
        {
            /**
             * This is where the rendering happens. 30 frames are rendered per second
             */
            al_clear_to_color(al_map_rgb(0, 0, 0));
            draw_ship(&ship);
            draw_asteroids(headAsteroid);
            draw_blasts(headBlast);
            al_draw_filled_rectangle(0, gameDisplay.height, mainDisplay.width, mainDisplay.height, al_map_rgba(255, 255, 255, 1));
            al_draw_text(font, al_map_rgb(0, 0, 0), 20, gameDisplay.height + 20, 0, scoreInfo);
            al_draw_text(font, al_map_rgb(0, 0, 0), 20, gameDisplay.height + 40, 0, asteroidsInfo);
            al_draw_text(font, al_map_rgb(0, 0, 0), (mainDisplay.width - (8 * (int) strlen(lifeInfo)) - 20), gameDisplay.height + 20, 0, lifeInfo);
            al_flip_display();
            reRender = false;
        }
    }

    /**
     * Never forget to free the heap
     * from all the dynamic memory that your program used throughout its life cycle!!!
     */
    freeLinkedLists(&headAsteroid, &headBlast);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    al_destroy_display(display);
    al_destroy_font(font);

    return 0;
}
