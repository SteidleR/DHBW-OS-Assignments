#include "random.h"
#include <stdio.h>


void main() {
    printf("Random Number between 0 and 1:   %f\n", random());
    printf("Random Number between 0 and 100: %i\n", randomInt(0, 100));
}