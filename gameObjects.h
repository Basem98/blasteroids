/**
 * The structures that will represent the blueprint for all the objects in the game
*/


/* The dimensions for any display  */
typedef struct Display
{
    const int width;
    const int height;
} Display;


/* The Spaceship */
typedef struct Spaceship
{
    float vx, vy;
    float body[2][8];
    float head[2][1];
    float centerOfRotation[2][1];
    float direction;
    ALLEGRO_COLOR color;
} Spaceship;


/* The Blasts */
typedef struct BlastData
{
    float vx, vy;
    float body[2][2];
    float direction;
    ALLEGRO_COLOR color;
} BlastData;


/* The singly-linked list that will store all the current blasts */
typedef struct Blast {
    BlastData *data;
    struct Blast *next;
} Blast;


/* The Asteroids */
typedef struct AsteroidData
{
    float vx, vy;
    float body[2][12];
    float centerOfRotation[2][1];
    float direction;
    float twist;
    bool isHit;
    bool isDupe;
    bool hasBeenDuped;

    ALLEGRO_COLOR color;
} AsteroidData;


/* The singly-linked list that will store all the current asteroids */
typedef struct Asteroid {
    AsteroidData *data;
    struct Asteroid *next;
} Asteroid;