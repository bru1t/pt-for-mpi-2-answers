#include "pt4.h"
#include "mpi.h"

void Solve()
{

    Task("MPI2Send19");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double buf;
    pt >> buf;
    int out_rank = size - rank - 1;
    MPI_Sendrecv_replace(&buf, 1, MPI_DOUBLE, out_rank, 0, out_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    pt << buf;
    
}
