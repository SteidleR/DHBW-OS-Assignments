#include "random.h"
#include <time.h>
#include <math.h>

#define UPPER_MASK		0x80000000
#define LOWER_MASK		0x7fffffff
#define TEMPERING_MASK_B	0x9d2c5680
#define TEMPERING_MASK_C	0xefc60000


inline static void m_seed(MT* rand, unsigned long seed) {
    rand->mt[0] = seed & 0xffffffff;
    for(rand->index=1; rand->index<VECTOR_LENGTH; rand->index++) {
        rand->mt[rand->index] = (6069 * rand->mt[rand->index-1]) & 0xffffffff;
    }
}

/*
* Creates a new random generator from the given seed
*/
MT seed(unsigned long seed) {
    MT rand;
    m_seed(&rand, seed);
    return rand;
}

/*
* pseudo-random generator for type long
*/
unsigned long srandomLong(MT* rand) {
    unsigned long y;
    static unsigned long mag[2] = {0x0, 0x9908b0df}; /* mag[x] = x * 0x9908b0df for x = 0,1 */
    if(rand->index >= VECTOR_LENGTH || rand->index < 0) {
        /* generate STATE_VECTOR_LENGTH words at a time */
        int kk;
        if(rand->index >= VECTOR_LENGTH+1 || rand->index < 0) {
            m_seed(rand, 4357);
        }
        for(kk=0; kk<VECTOR_LENGTH-VECTOR_M; kk++) {
            y = (rand->mt[kk] & UPPER_MASK) | (rand->mt[kk+1] & LOWER_MASK);
            rand->mt[kk] = rand->mt[kk+VECTOR_M] ^ (y >> 1) ^ mag[y & 0x1];
        }
        for(; kk<VECTOR_LENGTH-1; kk++) {
            y = (rand->mt[kk] & UPPER_MASK) | (rand->mt[kk+1] & LOWER_MASK);
            rand->mt[kk] = rand->mt[kk+(VECTOR_M-VECTOR_LENGTH)] ^ (y >> 1) ^ mag[y & 0x1];
        }
        y = (rand->mt[VECTOR_LENGTH-1] & UPPER_MASK) | (rand->mt[0] & LOWER_MASK);
        rand->mt[VECTOR_LENGTH-1] = rand->mt[VECTOR_M-1] ^ (y >> 1) ^ mag[y & 0x1];
        rand->index = 0;
    }
    y = rand->mt[rand->index++];
    y ^= (y >> 11);
    y ^= (y << 7) & TEMPERING_MASK_B;
    y ^= (y << 15) & TEMPERING_MASK_C;
    y ^= (y >> 18);
    return y;
}

// srandomLong with generated seed
unsigned long randomLong() {
    MT r = seed(clock()^time(0));
    return srandomLong(&r);
}

/*
 * Generates a pseudo-random number between 0 and 1
 */
double srandom(MT* rand) {
    return ((double)randomLong(rand) / (unsigned long)0xffffffff);
}

// srandom with generated seed
double random() {
    MT r = seed(clock()^time(0));
    return srandom(&r);
}

/*
 * Generates a prn between lower and upper
 */
int srandomInt(MT* rand, int lower, int upper) {
    long r = randomLong(rand);
    return (r % (upper-lower)) + lower;
}

void main() {}