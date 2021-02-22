//
// Created by Ikilon on 2/16/2021.
//

#ifndef PARALLEL_VECTOR
#define PARALLEL_VECTOR

#include "utils.h"

typedef struct {
    double* value;
    size_t size;

    double min, max;
} vector;

vector* vector_ctor(const size_t size, const double min, const double max) {
    vector* self = (vector*)malloc(sizeof(vector));

    if (self) {
        self->size = size;
        self->min = min;
        self->max = max;
        self->value = calloc(size, sizeof(double));
    }

    return self;
}

void vector_free(vector* self) {
    free(self->value);
    free(self);

    self = NULL;
}

static long long i = 0ll;

void vector_random(const vector* vec) {
    if (vec) {
        #pragma omp parallel for default(none) shared(vec) private(i)
        for (i = 0ll; i < vec->size; i++) {
            vec->value[i] = rand_double(vec->min, vec->max);
        }
    }
}

double time_fn_for_vector(void (*fn)(const vector*), vector* self) {
    double begin, end;

    begin = omp_get_wtime();
    fn(self);
    end = omp_get_wtime();

    return end - begin;
}

double time_fn_for_two_vector(const double (*fn)(const vector*, const vector*), vector* first, vector* second) {
    double begin, end;

    begin = omp_get_wtime();
    fn(first, second);
    end = omp_get_wtime();

    return end - begin;
}

double scalar(const vector* first, const vector* second) {
    double result = 0.0;

    if (first->size != second->size)
        return 0.0;

    #pragma omp parallel for default(none) shared(first, second), private(i), reduction(+: result)
    for (i = 0; i < first->size; i++) {
        result += first->value[i] * second->value[i];
    }

    return result;
}

#endif
