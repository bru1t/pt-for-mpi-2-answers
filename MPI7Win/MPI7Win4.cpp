#include "pt4.h"
#include "mpi.h"

void Solve()
{

    Task("MPI7Win4");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double* n = new double[size + 3];
    double m[5];
    if (rank == 0)
        for (int i = 0; i < size + 3; i++)
            pt >> n[i];
    MPI_Win win;
    MPI_Win_create(n, (size + 3) * 8, 8, MPI_INFO_NULL, MPI_COMM_WORLD, &win);

    MPI_Win_fence(0, win);
    if(rank != 0)
        MPI_Get(&m, 5, MPI_DOUBLE, 0, rank - 1, 5, MPI_DOUBLE, win);

    MPI_Win_fence(0, win);
    if(rank != 0)
        for (int i = 0; i < 5; i++)
            pt << m[i];

    MPI_Win_free(&win);
    delete[] n;
    
}
