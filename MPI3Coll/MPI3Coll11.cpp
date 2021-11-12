#include "pt4.h"
#include "mpi.h"

void Solve()
{

    Task("MPI3Coll11");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double n;
    pt >> n;
    double* result = new double[size];

    MPI_Allgather(&n, 1, MPI_DOUBLE, result, 1, MPI_DOUBLE, MPI_COMM_WORLD);

    for (int i = 0; i < size; ++i)
        pt << result[i];
        
}
