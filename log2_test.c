#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "log2_lshift16.h"
#include "log2fix.h"


#define PRECISION 16
#define XEEOF -1
#define SAMPLES 10000

int main()
{
    FILE *file = fopen("log2.csv", "w");
    double scale = 1U << PRECISION;
    double scale1 = 1U << 1;

    if (file == NULL) {
        printf("fail to open\n");
        return 1;
    }

    for (int i = 1; i <= SAMPLES; i++) {
        fprintf(file, "%d %f %f %f\n", i, log2(i),
                log2fix(i * scale, 16) / scale,
                log2fix(i * scale1, 1) / scale1);
    }
    fclose(file);

    return 0;
}