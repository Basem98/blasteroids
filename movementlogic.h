/**
 * Functions that handle the 2D transformations logic
*/

float rotate_x_cw(float oldX, float oldY, SpaceHead *centerOfRotation, float rotationAngle);

float rotate_y_cw(float oldX, float oldY, SpaceHead *centerOfRotation, float rotationAngle);

float rotate_x_anticw(float oldX, float oldY, SpaceHead *centerOfRotation, float rotationAngle);

float rotate_y_anticw(float oldX, float oldY, SpaceHead *centerOfRotation, float rotationAngle);

/**
 * Rotate the ship either clock-wise or anti-clock-wise.
 * The direction is defined based on the two functions
 * passed as arguments, which will handle the rotation of each axis
*/
void rotate_ship(Spaceship *ship,
                 float rotationAngle,
                 float (*rotate_x)(float, float, SpaceHead *, float),
                 float (*rotate_y)(float, float, SpaceHead *, float));

/**
 * Determine where the ship's head is headed based on the rotationAngle
*/
void determine_direction(Spaceship *ship);

/**
 * Translate the ship in the direction it's headed
 */
void translate_ship(Spaceship *ship, MainWindow window);