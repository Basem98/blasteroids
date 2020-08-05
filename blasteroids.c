#include <allegro5/allegro.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

typedef struct {
    const int width;
    const int height;
} MainWindow;

void error(char* err)
{
    fprintf(stderr, "%s: %s", err, strerror(errno));
    exit(2);
}

int main(int argc, char** argv)
{
    if (!al_init())
        error("Can't initialize Allegro");

    MainWindow window = { 1200, 600 };

    if (!al_create_display(window.width, window.height))
        error("Can't draw main window");

    return 0;
}