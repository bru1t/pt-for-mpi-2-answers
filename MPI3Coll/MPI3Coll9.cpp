#include "mpi.h"
#include "pt4.h"

void Solve()
{

    Task("MPI3Coll9");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int ARR_SIZE = 50;
    MPI_Status status;
    int cache1[ARR_SIZE], cache2[ARR_SIZE];
    int disp[5] = {0, 2, 5, 9, 14};
    int send[5] = {2, 3, 4, 5, 6};

    if (rank == 0)
    	for (int i = 0; i < size * (size + 3) / 2; i++)
    		pt >> cache1[i];

    MPI_Scatterv(cache1, send, disp, MPI_INT, cache2, ARR_SIZE, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < 6; i++)
    	if (cache2[i] > 0)
    		pt << cache2[i];

}
