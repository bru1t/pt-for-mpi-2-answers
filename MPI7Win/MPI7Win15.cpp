#include "pt4.h"
#include "mpi.h"
void Solve()
{
    Task("MPI7Win15");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double* n = new double[size];
    for (int i = 0; i < size; i++)
        pt >> n[i];

    MPI_Win win;
    MPI_Win_create(n, size * 8, 8, MPI_INFO_NULL, MPI_COMM_WORLD, &win);

    MPI_Win_fence(0, win);
    for (int i = rank + 1; i < size; i++)
        MPI_Put(&n[i], 1, MPI_DOUBLE, i, rank, 1, MPI_DOUBLE, win);

    MPI_Win_fence(0, win);
    for (int i = rank + 1; i < size; i++)
        n[i] = 0.0f;

    MPI_Win_fence(0, win);
    for (int i = 0; i < size; i++)
        pt << n[i];

    MPI_Win_free(&win);
    delete[] n;
}
