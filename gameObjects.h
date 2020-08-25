/**
 * The dimensions for the game's display window
 */
typedef struct MainWindow
{
    const int width;
    const int height;
} MainWindow;


/**
 * The Spaceship
 */
typedef struct Spaceship
{
    float vx, vy;
    float body[2][8];
    float head[2][1];
    float centerOfRotation[2][1];
    float direction;
    ALLEGRO_COLOR color;
} Spaceship;