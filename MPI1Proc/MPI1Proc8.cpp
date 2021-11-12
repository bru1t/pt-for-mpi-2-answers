#include "pt4.h"
#include "mpi.h"

void Solve()
{

    Task("MPI1Proc8");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int n;
    float el, rez = 0;

    pt >> n;

    for (int i = 0; i < n; i++) {
        pt >> el;
        rez += el;
    }

    rank % 2 == 0 ? pt << rez : pt << rez / n;

}
