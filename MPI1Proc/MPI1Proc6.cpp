#include "pt4.h"
#include "mpi.h"

void Solve()
{

    Task("MPI1Proc6");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank % 2 && rank) {

        double n;

        pt >> n;
        pt << 2 * n;

    }
    else if (!(rank % 2) && rank) {

        int n;

        pt >> n;
        pt << 2 * n;

    }

}
