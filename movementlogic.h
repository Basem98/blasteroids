/**
 * Functions that handle the 2D transformations logic
*/

double rotate_x_cw(double oldX, double oldY, SpaceHead *centerOfRotation, double rotationAngle);

double rotate_y_cw(double oldX, double oldY, SpaceHead *centerOfRotation, double rotationAngle);

double rotate_x_anticw(double oldX, double oldY, SpaceHead *centerOfRotation, double rotationAngle);

double rotate_y_anticw(double oldX, double oldY, SpaceHead *centerOfRotation, double rotationAngle);

/**
 * Rotate the ship either clock-wise or anti-clock-wise.
 * The direction is defined based on the two functions
 * passed as arguments, which will handle the rotation of each axis
*/
void rotate_ship(Spaceship *ship,
                 double (*rotate_x)(double, double, SpaceHead *, double),
                 double (*rotate_y)(double, double, SpaceHead *, double));
