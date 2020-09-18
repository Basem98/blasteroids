/**
 * Functions that handle the 2D transformations logic
 */


/**
 * Rotation formulas for any vector around a point (centerOfRotation) other than the origin
 */

/* Rotate the x axis of a vector in a clock-wise direction around the centerOfRotation vector */
float rotate_x_cw(float oldX, float oldY, float (*centerOfRotation)[1]);

/* Rotate the y axis of a vector in a clock-wise direction around the centerOfRotation vector */
float rotate_y_cw(float oldX, float oldY, float (*centerOfRotation)[1]);

/* Rotate the x axis of a vector in a counter clock-wise direction around the centerOfRotation vector */
float rotate_x_ccw(float oldX, float oldY, float (*centerOfRotation)[1]);

/* Rotate the y axis of a vector in a counter clock-wise direction around the centerOfRotation vector */
float rotate_y_ccw(float oldX, float oldY, float (*centerOfRotation)[1]);


/**
 * Rotate the ship either clock-wise or anti-clock-wise.
 * The direction is defined based on the sign of the rotationDirection argument
 */
void rotate_ship(Spaceship *ship, short rotationDirection);


/**
 * Determine where a space object is headed based on its rotation angle by changing the direction of its velocity
 */
void determine_direction(float direction, float *vx, float *vy, float velocity);


/**
 * Spaceship-specific functionality
 */

/* Translate the ship in the direction it's headed */
void translate_ship(Spaceship *ship, MainWindow window);


/**
 * Blast-specific functionality
 */

/* Translate each blast in the direction it's headed */
void translate_blast(Blast **headBlast, MainWindow window);

/* Add another blast! */
void add_blast(Spaceship ship, Blast **headBlast);


/**
 * Asteroid-specific functionality
 */

/* Add another asteroid to the end of the list */
void add_asteroid(Asteroid **headAsteroid, float direction, float vx, float vy);

/**
 * Add a duplicate asteroid that matches the originalAsteroid to the end of the list,
 * but the only visible parts of it will be the parts that are off screen in the originalAsteroid
 */
void add_dup_asteroid(Asteroid **originalAsteroid, float vx, float vy);

/* Rotate each asteroid around itself based on its twist angle */
void rotate_asteroid(Asteroid **headAsteroid);

/* Translate each asteroid in the same direction, which is defined in the direction property */
void translate_asteroid(Asteroid **headAsteroid, MainWindow window);