#include "pt4.h"
#include "mpi.h"

void Solve()
{

    Task("MPI2Send13");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int n, k;
    pt >> n;
    int pre_rank = (rank == 0) ? size - 1 : rank - 1;
    int next_rank = (rank + 1) % size;
    MPI_Status status;

    if (rank % 2) {

        MPI_Ssend(&n, 1, MPI_INT, pre_rank, 0, MPI_COMM_WORLD);
        MPI_Recv(&k, 1, MPI_INT, next_rank, 0, MPI_COMM_WORLD, &status);

    }
    else {
        MPI_Recv(&k, 1, MPI_INT, next_rank, 0, MPI_COMM_WORLD, &status);
        MPI_Ssend(&n, 1, MPI_INT, pre_rank, 0, MPI_COMM_WORLD);

    }
    
    pt << k;

}
