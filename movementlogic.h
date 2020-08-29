/**
 * Functions that handle the 2D transformations logic
*/

float rotate_x_cw(float oldX, float oldY, float (*centerOfRotation)[1]);

float rotate_y_cw(float oldX, float oldY, float (*centerOfRotation)[1]);

float rotate_x_ccw(float oldX, float oldY, float (*centerOfRotation)[1]);

float rotate_y_ccw(float oldX, float oldY, float (*centerOfRotation)[1]);

/**
 * Rotate the ship either clock-wise or anti-clock-wise.
 * The direction is defined based on the two functions
 * passed as arguments, which will handle the rotation of each axis
*/
void rotate_ship(Spaceship *ship, short rotationDirection);

/**
 * Determine where a space object is headed based on the rotationAngle
*/
void determine_direction(float direction, float *vx, float *vy, float velocity);

/**
 * Translate the ship in the direction it's headed
 */
void translate_ship(Spaceship *ship, MainWindow window);

/**
 * Translate each blast in the direction it's headed
*/
// void translate_blast(Blast **previousBlast, Blast **currentBlast, MainWindow window);
void translate_blast(Blast **headBlast, MainWindow window);


/**
 * Add another blast!
*/
void add_blast(Spaceship ship, Blast **headBlast);