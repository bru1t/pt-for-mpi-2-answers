#include "pt4.h"
#include "mpi.h"

void Solve()
{

    Task("MPI7Win5");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int* a = new int[size - 1];
    int n, b, c;
    if (rank == 0)
        for (int i = 0; i < size - 1; i++)
            pt >> a[i];
    else
    {
        pt >> n;
        pt >> b;
    }

    MPI_Win win;
    MPI_Win_create(a, (size - 1) * 4, 4, MPI_INFO_NULL, MPI_COMM_WORLD, &win);

    MPI_Win_fence(0, win);
    if (rank != 0)
        MPI_Accumulate(&b, 1, MPI_INT, 0, n, 1, MPI_INT, MPI_PROD, win);

    MPI_Win_fence(0, win);
    if(rank == 0)
        for (int i = 0; i < size - 1; i++)
            pt << a[i];

    MPI_Win_free(&win);

    delete[] a;
    
}
