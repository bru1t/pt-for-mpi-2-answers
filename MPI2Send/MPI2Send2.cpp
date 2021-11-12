#include "pt4.h"
#include "mpi.h"

void Solve()
{

    Task("MPI2Send2");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double n;
    if (rank == 0) {
        for (int i = size - 1; i > 0; --i) {
            MPI_Recv(&n, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
            pt << n;
        }
    }
    else {

        pt >> n;
        int bfs = sizeof(double) + MPI_BSEND_OVERHEAD;
        int* bf = (int*)malloc(bfs);
        
        MPI_Buffer_attach(bf, bfs);
        MPI_Bsend(&n, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
        MPI_Buffer_detach(bf, &bfs);

    }

}
