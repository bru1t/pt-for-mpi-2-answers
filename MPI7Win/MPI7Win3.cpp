#include "pt4.h"
#include "mpi.h"

void Solve()
{
    Task("MPI7Win3");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int* arr = new int[size - 1];
    
    MPI_Win win;
    int n = 0;
    MPI_Win_create(arr, rank * sizeof(int), sizeof(int), MPI_INFO_NULL, MPI_COMM_WORLD, &win);

    if (rank == 0)
    {
        for (size_t i = 0; i < size - 1; i++)
        {
            pt >> arr[i];
        }
    }
    
    MPI_Win_fence(0, win);

    if (rank != 0) {
        MPI_Get(&n, 1, MPI_INT, 0, size - rank - 1, 1, MPI_INT, win);
    }

    MPI_Win_fence(0, win);

    if (rank != 0) {
        pt << n;
    }

    MPI_Win_free(&win);

}
