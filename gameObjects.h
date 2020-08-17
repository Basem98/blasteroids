/**
 * The dimensions for the game's display window
 */
typedef struct MainWindow
{
    const int width;
    const int height;
} MainWindow;

/**
 * The coordinates for each line
 * that will be used to draw the Spaceship
 */
typedef struct SpaceLine
{
    float x1;
    float y1;
    float x2;
    float y2;
} SpaceLine;

/**
 * The Spaceship's head
 */
typedef struct SpaceHead
{
    float x;
    float y;
} SpaceHead;

/**
 * The Spaceship
 */
typedef struct Spaceship
{
    SpaceLine *spaceLine[4];
    SpaceHead *head;
    ALLEGRO_COLOR color;
} Spaceship;
