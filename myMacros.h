/**
 * Necessary macros used in multiple object files throughout the program
 */


/* A macro that gets the number of columns in a matrix */
#define NUM_OF_COLUMNS(x) sizeof(*(x)) / sizeof(float)

/**
 * A macrto that represents the value of PI
 */
#define PI 3.14159265359

/**
 * Convert the angle to radiants before calling sin or cos
 */
#define COS(x) cosf(x *(PI / 180))
#define SIN(x) sinf(x *(PI / 180))
