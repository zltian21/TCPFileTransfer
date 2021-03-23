#include <stdlib.h>
#include <stdio.h>

struct pkt { // total bytes = 81 + 2 + 2 = 85
    char data[81]; //80 printable character and 1 null terminating character 
    short count;
    short seq;
    char padding[3];
};