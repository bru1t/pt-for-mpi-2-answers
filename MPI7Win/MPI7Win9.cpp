#include "pt4.h"
#include "mpi.h"

void Solve()
{

    Task("MPI7Win9");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int* a = new int[size];
    int* b = new int[size];
    for (int i = 0; i < size; i++)
        pt >> a[i];

    MPI_Win win;
    MPI_Win_create(a, size * 4, 4, MPI_INFO_NULL, MPI_COMM_WORLD, &win);

    MPI_Win_fence(0, win);
    for (int i = 0; i < size; i++)
        MPI_Get(&b[i], 1, MPI_INT, size - 1 - i, rank, 1, MPI_INT, win);

    MPI_Win_fence(0, win);
    for (int i = 0; i < size; i++)
        pt << b[i];

    MPI_Win_free(&win);
    delete[] a;
    
}
