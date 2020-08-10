/* The dimensions for the game's display window */
typedef struct {
    const int width;
    const int height;
} MainWindow;

/* The coordinates for each line
** that will be used to draw the spaceship
*/
typedef struct SpaceLine {
    float x1;
    float y1;
    float x2;
    float y2;
} spaceLine1;