#include "mpi.h"
#include "pt4.h"

void Solve()
{

    Task("MPI1Proc2");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int n;

    pt >> n;
    pt << 2 * n;

    if (rank == 0)
    	pt << size;
    
}
