#include "pt4.h"
#include "mpi.h"

void Solve()
{

    Task("MPI3Coll3");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double n, res[10];
    pt >> n;

    MPI_Gather(&n, 1, MPI_DOUBLE, res, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank == 0)
        for (int i = 0; i < size; ++i)
            pt << res[i];
            
}
