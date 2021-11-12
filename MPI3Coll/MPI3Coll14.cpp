#include "pt4.h"
#include "mpi.h"

void Solve()
{

    Task("MPI3Coll14");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<double> n(size);

    for (int i = 0; i < size; ++i)
		    pt >> n[i];

    std::vector<double> re(size);

    MPI_Alltoall(&n[0], 1, MPI_DOUBLE, &re[0], 1, MPI_DOUBLE, MPI_COMM_WORLD);

    for (auto& i : re)
		    pt << i;

}
