#include "mpi.h"
#include "pt4.h"

void Solve()
{

    Task("MPI3Coll25");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int ARR_SIZE = 10;
    float arr1[ARR_SIZE], arr2[ARR_SIZE];
    memset(arr1, 0, sizeof(arr1));
    memset(arr2, 0, sizeof(arr2));
    int recvcounts[6] = {2, 2, 2, 2, 2, 2};

    for (int i = 0; i < 2 * size; i++)
    	pt >> arr1[i];

    MPI_Reduce_scatter(arr1, arr2, recvcounts, MPI_FLOAT, MPI_MAX, MPI_COMM_WORLD);

    for (int i = 0; i < ARR_SIZE; i++)
    	if (arr2[i] > 0)
    		pt << arr2[i];

}
