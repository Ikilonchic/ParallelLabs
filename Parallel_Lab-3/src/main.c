#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <mpi.h>

void PrintVector(int vector[], const int length) {
    printf("Vector:\t");
    for (int i = 0; i < length; ++i) {
        printf("%d ", vector[i]);
    }
    printf("\n");
}

int Worker(int rank, const int size) {
    printf("Processor - start: %d/%d\n", rank, size - 1);

    const int length = 64;
    
    int vector[length], sum;

    switch (rank) {
        case 1:
            MPI_Recv(vector, length, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            for (int i = 0; i < length; ++i) {
                sum += vector[i];
            }

            for (int i = 0; i < size; ++i) {
                if (i != rank) {
                    MPI_Send(&sum, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
                }
            }

            printf("Processor - sum: %i (%d/%d)\n", sum, rank, size - 1);

            break;
        case 0:
            for (int i = 0; i < length; ++i) {
                vector[i] = rand() % 50;
            }

            PrintVector(vector, length);

            MPI_Send(vector, length, MPI_INT, 1, 0, MPI_COMM_WORLD);
        default:
            MPI_Recv(&sum, 1, MPI_INT, 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Processor - sum: %i (%d/%d)\n", sum, rank, size - 1);
            break;
    }

    printf("Processor - finish: %d/%d\n", rank, size - 1);

    return 0;
}

int main(int argc, char** argv) {
    srand(time(0));

    if (MPI_Init(&argc, &argv) != 0) {
		fprintf(stderr, "Failed to init MPI!");
		return EXIT_FAILURE;
	}

    int size, rank, nameLength;
    char processorName[MPI_MAX_PROCESSOR_NAME];

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Get_processor_name(processorName, &nameLength);

    if (rank == 0) {
        printf("Processor name: %s\n", processorName);
    }

    if (size < 2) {
        printf("Not enough processors.\n");
        MPI_Finalize();
        return EXIT_SUCCESS;
    }

    Worker(rank, size);
    MPI_Finalize();

    return EXIT_SUCCESS;
}
