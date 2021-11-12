#include "pt4.h"
#include "mpi.h"

void Solve()
{

    Task("MPI2Send12");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int a;

    if (rank == 0) {

        pt >> a;
        MPI_Ssend(&a, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
        MPI_Recv(&a, 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
        pt << a;

    }
    else if (rank == size - 1) {

        MPI_Recv(&a, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
        pt << a;
        pt >> a;
        MPI_Ssend(&a, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

    }
    else {

        MPI_Recv(&a, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
        pt << a;
        pt >> a;
        MPI_Ssend(&a, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);

    }
    
}
