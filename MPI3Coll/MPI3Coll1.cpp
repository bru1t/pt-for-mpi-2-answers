#include "pt4.h"
#include "mpi.h"

void Solve()
{

    Task("MPI3Coll1");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int n;
    if (!rank)
        pt >> n;

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank)
        pt << n;
        
}
