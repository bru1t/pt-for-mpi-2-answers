#include "mpi.h"
#include "pt4.h"

void Solve()
{

    Task("MPI2Send16");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int N;
    float num;
    MPI_Status status;

    pt >> N;

    if (N == 0) {
        MPI_Recv(&num, 1, MPI_FLOAT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
        pt << num;
    }
    else {

		    int ARR_SIZE = size - 1;
		    float arr[ARR_SIZE];

		    for (int i = 0; i < ARR_SIZE; i++)
            pt >> arr[i];

        if (rank == 0) {
            for (int i = 1; i < size; i++)
                MPI_Send(&arr[i-1], 1, MPI_FLOAT, i, 0, MPI_COMM_WORLD);
		    }
		    else {
            int index = 0;
            for (int i = 0; i < rank; i++)
                MPI_Send(&arr[index++], 1, MPI_FLOAT, i, 0, MPI_COMM_WORLD);
            for (int i = rank + 1; i < size; i++)
                MPI_Send(&arr[index++], 1, MPI_FLOAT, i, 0, MPI_COMM_WORLD);
        }

    }

}
