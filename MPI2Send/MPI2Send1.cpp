#include "pt4.h"
#include "mpi.h"

void Solve()
{
  
    Task("MPI2Send1");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int n;

    if (!rank) {
        for (int i = 1; i < size; ++i) {
            MPI_Recv(&n, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            pt << n;
        }
    }
    else {
        pt >> n;
        MPI_Send(&n, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

}
