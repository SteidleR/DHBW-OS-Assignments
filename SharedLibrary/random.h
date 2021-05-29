#ifndef __RANDOM_H
#define __RANDOM_H

#define VECTOR_LENGTH 624
#define VECTOR_M 397 

#include <time.h>
#include <math.h>

/* Some links to the Mersenne Twister algorithm and C Code
 * https://en.wikipedia.org/wiki/Mersenne_Twister
 * https://github.com/ESultanik/mtwister
 */

typedef struct tagMT {
    unsigned long mt[VECTOR_LENGTH];
    int index;
} MT;

MT seed(unsigned long);
unsigned long srandomLong(MT*);
double srandom(MT*);

// with generated seed
unsigned long randomLong();
double random();

// randomInt, randomRange: for prn between a given range
// python-oriented function naming
int srandomInt(MT*, int, int);
#define srandomRange(rand, lower, upper) srandomInt(rand, lower, upper)

// with generated seed
int randomInt(int, int);
#define randomRange(lower, upper) randomInt(lower, upper)

#endif