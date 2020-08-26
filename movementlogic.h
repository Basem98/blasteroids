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
 * Determine where the ship's head is headed based on the rotationAngle
*/
void determine_direction(Spaceship *ship, float velocity);

/**
 * Translate the ship in the direction it's headed
 */
void translate_ship(Spaceship *ship, MainWindow window);