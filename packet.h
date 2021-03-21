#include <stdlib.h>
#include <stdio.h>

struct pkt {
    char data[80];
    short count;
    short seq;
};