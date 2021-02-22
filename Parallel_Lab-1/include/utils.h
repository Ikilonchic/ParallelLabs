//
// Created by Ikilon on 2/16/2021.
//

#ifndef PARALLEL_UTILS
#define PARALLEL_UTILS

#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <time.h>
#include <math.h>
#include <omp.h>

#define DISABLE 0
#define RADIX 10

double rand_double(double min, double max) {
    return min + (max - min) / RAND_MAX * rand();
}

#endif
