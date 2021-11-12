#include "pt4.h"
#include "mpi.h"

void Solve()
{

    Task("MPI3Coll27");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int sz = size + 5;
    std::vector<double> n(sz);

    for (int i = 0; i < sz; ++i)
		    pt >> n[i];

    std::vector<double> re(sz);
    MPI_Scan(&n[0], &re[0], sz, MPI_DOUBLE, MPI_PROD, MPI_COMM_WORLD);

    for (auto& i : re)
		    pt << i;
        
}
