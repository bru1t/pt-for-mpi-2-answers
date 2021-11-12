#include "pt4.h"
#include "mpi.h"

void Solve()
{

    Task("MPI2Send26");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int N, n;
    MPI_Request r;
    pt >> N;

    if (N) {

        for (int i = 0; i < size; i++) {
            if (i == rank) continue;
            pt >> n;
            MPI_Issend(&n, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &r);
            double ft, st;
            ft = MPI_Wtime();
            MPI_Wait(&r, MPI_STATUSES_IGNORE);
            st = MPI_Wtime();
            Show((st - ft) * 1000);
        }

    }
    else {

        MPI_Recv(&n, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
        pt << n;

    }

}
