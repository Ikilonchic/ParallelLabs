#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>

#include "mpi.h"

int Sum(int array[], const int start, const int end) {
    int sum = 0;

    for (int i = start; i < end; ++i) {
        sum += array[i];
    }

    return  sum;
}

int Min(int array[], const int start, const int end) {
    int min = array[start];

    for (int i = start + 1; i < end; ++i) {
        if (array[i] < min)
            min = array[i];
    }

    return min;
}

int Max(int array[], const int start, const int end) {
    int max = array[start];

    for (int i = start + 1; i < end; ++i) {
        if (array[i] > max)
            max = array[i];
    }

    return max;
}

int Worker(int rank, int size) {
    printf("Processor - start: %d/%d\n", rank, size - 1);

    int block_size = 8,
        array_length = block_size * size,
        array[array_length],
        *gathered_array,
        gathered_array_length = size * array_length,
        block_start = rank * block_size,
        block_end = block_start + block_size;

    gathered_array = (int*)malloc(gathered_array_length * sizeof(int));

    int local_sum = 0, sum = 0, local_min, min, local_max, max;

    if (rank == 0) {
        // Shuffle array...
        for (int i = 0; i < array_length; ++i) {
            array[i] = rand() % 50;
        }
    }

    // Broadcast...
    MPI_Bcast(array, array_length, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Gather(array, array_length, MPI_INT, gathered_array, array_length, MPI_INT, 0, MPI_COMM_WORLD);

    // Operation...
    local_sum = Sum(array, block_start, block_end);
    MPI_Reduce(&local_sum, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    local_max = Max(array, block_start, block_end);
    MPI_Reduce(&local_max, &max, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

    local_min = Min(array, block_start, block_end);
    MPI_Reduce(&local_min, &min, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);

    // Output...
    if (rank == 0) {
        printf("Gathered array: ");
        for (int i = 0; i < gathered_array_length; ++i) {
            if (i % (block_size * size) == 0) {
                printf("\n");
            }
            printf("%i ", gathered_array[i]);
        }
        printf("\n");

        printf("Sum: %i, Max: %i, Min: %i\n", sum, max, min);
    }

    free(gathered_array);
    printf("Processor - finish: %d/%d\n", rank, size - 1);
    return 0;
}

int main(int argc, char* argv[]) {
    int status;

    if ((status = MPI_Init(&argc, &argv))) {
        printf("Failed to init MPI.\n");
        return EXIT_FAILURE;
    }

    int rank, size, host_len;
    char processorName[MPI_MAX_PROCESSOR_NAME];

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Get_processor_name(processorName, &host_len);

    if (rank == 0) {
        printf("Processor name: %s\n", processorName);
    }

    Worker(rank, size);

    MPI_Finalize();

    return EXIT_SUCCESS;
}
