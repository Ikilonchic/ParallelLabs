#include "utils.h"
#include "vector.h"

#define THREADS_COUNT_INDEX 1
#define FIRST_MIN_VECTOR_VALUE_INDEX 2
#define FIRST_MAX_VECTOR_VALUE_INDEX 3

void main_config(const int threads_num) {
    srand((unsigned)time(NULL));

    omp_set_dynamic(DISABLE);
    omp_set_num_threads(threads_num);
}

int main(const int argc, const char** argv) {
    if (argc <= 3) {
        fprintf(stdout, "No args to launch the program...\n"
                        "Example: lab1.exe <threads_num> <min_vector_value> <max_vector_value>\n");
        return EXIT_SUCCESS;
    }

    const int threads_num = (int)strtol(argv[THREADS_COUNT_INDEX], NULL, RADIX);
    const double min_value = (double)strtol(argv[FIRST_MIN_VECTOR_VALUE_INDEX], NULL, RADIX);
    const double max_value = (double)strtol(argv[FIRST_MAX_VECTOR_VALUE_INDEX], NULL, RADIX);


    main_config(threads_num);

    vector* vec1 = vector_ctor(67108864, min_value, max_value);
    vector* vec2 = vector_ctor(67108864, min_value, max_value);

    double generate_time_1 = time_fn_for_vector(vector_random, vec1);
    double generate_time_2 = time_fn_for_vector(vector_random, vec2);

    double result_time = time_fn_for_two_vector(scalar, vec1, vec2);

    fprintf(stdout, "Time to generate first vector: %f\n"
                    "Time to generate second vector: %f\n"
                    "Time to result: %f\n", generate_time_1, generate_time_2, result_time);

    vector_free(vec1);
    vector_free(vec2);

    return EXIT_SUCCESS;
}
