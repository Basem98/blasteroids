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

/**
 * The blasts
*/
typedef struct BlastData
{
    float vx, vy;
    float body[2][2];
    float direction;
    ALLEGRO_COLOR color;
} BlastData;

/**
 * The singly-linked list that will store all the current blasts
*/
typedef struct Blast {
    BlastData *blastData;
    struct Blast *next;
} Blast;