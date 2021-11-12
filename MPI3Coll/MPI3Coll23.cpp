#include "pt4.h"
#include "mpi.h"
#include <vector>

struct MINLOC_Data {
    double a;
    int n;
};

void Solve()
{

    Task("MPI3Coll23");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<MINLOC_Data> d(size + 5);
    std::vector<MINLOC_Data> res(size + 5);

    for (int i = 0; i < size + 5; ++i) {
        pt >> d[i].a;
        d[i].n = rank;
    }

    MPI_Allreduce(&d[0], &res[0], size + 5, MPI_DOUBLE_INT, MPI_MINLOC, MPI_COMM_WORLD);

    for (int i = 0; i < size + 5; ++i)
        if (rank == 0)
            pt << res[i].a;
        else
            pt << res[i].n;
            
}
