/*
** This is my own version of Good ol' Blasteroids
** This is built only using the C standard library and the Allegro 5 framework
*/
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>

/* The dimensions for the game's display window */
typedef struct {
    const int width;
    const int height;
} MainWindow;

/* Basic error handler */
void error(char* err)
{
    fprintf(stderr, "%s: %s", err, strerror(errno));
    exit(2);
}

int main(int argc, char** argv)
{
    /* Initialize Allegro's core library and all the addons to be used in the project
    */
    if (!al_init())
        error("Can't initialize Allegro");
    if (!al_install_keyboard())
        error("Can't initialize Allegro");
    if (!al_init_primitives_addon())
        error("Can't initialize primtives addon");
    if (!al_init_font_addon())
        error("Can't initialize font addon");
    if (!al_init_acodec_addon())
        error("Can't initialize acodec addon");
    if (!al_install_audio())
        error("Can't initialize audio addon");
    if (!al_reserve_samples(16))
        error("Can't reserve audio samples");


    /* Create the game's main timer that ticks 30 tick per second */
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 30.0);
    if (!timer)
        error("Can't create game timer");

    /* Create the event queue that will recieve any event
    ** that comes from a registered event source,
    ** then push that event in a FIFO manner to an ALLEGRO_EVENT structure
    */
    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    if (!queue)
        error("Can't create event queue");

    ALLEGRO_FONT *font = al_create_builtin_font();
    if (!font)
        error("Can't create font");

    MainWindow window ={ 1280, 720 };
    ALLEGRO_DISPLAY *display;

    if (!(display = al_create_display(window.width, window.height)))
        error("Can't draw main window");

    /* Notify the event queue of the sources, from which it should listen for dispatched events */
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_display_event_source(display));


    /* The ALLEGRO_EVENT structure that will recieve any dispatched event */
    ALLEGRO_EVENT event;

    /* Start the game's timer */
    al_start_timer(timer);

    bool wannaExit = false;
    bool reRender = true;

    /* The game's main loop */
    while (1)
    {
        /* This function pauses your program at this point
        ** until an event is fired from any registered event source,
        */
        al_wait_for_event(queue, &event);

        switch (event.type)
        {
        case ALLEGRO_EVENT_TIMER:
        {
            /* The game's logic is calculated here exactly every 0.0333 second */
            reRender = true;
            break;
        }
        case ALLEGRO_EVENT_KEY_DOWN:
        {
            if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                wannaExit = true;
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
            /* This is where the rendering happens. 30 frames are rendered per second */
        }
    }

    /* Never forget to free the heap from all the dynamic memory that your program used throughout its life cycle!!! */
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    al_destroy_display(display);
    al_destroy_font(font);

    return 0;
}