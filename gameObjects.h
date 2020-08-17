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
    double x1;
    double y1;
    double x2;
    double y2;
} SpaceLine;

/**
 * The Spaceship's head
 */
typedef struct SpaceHead
{
    double x;
    double y;
} SpaceHead;

/**
 * The Spaceship
 */
typedef struct Spaceship
{
    SpaceLine *spaceLine[4];
    SpaceHead *head;
    SpaceHead *centerOfRotation;
    double rotationAngle;
    ALLEGRO_COLOR color;
} Spaceship;