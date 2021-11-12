#include "pt4.h"
#include "mpi.h"

void Solve()
{

	Task("MPI3Coll4");
	int flag;
	MPI_Initialized(&flag);
	if (flag == 0)
		return;
	int rank, size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int sendarray[5];
	int* rbuf;

	for (int i = 0; i < 5; ++i)
		pt >> sendarray[i];

	rbuf = (int*)malloc(size * 5 * sizeof(int));

	MPI_Gather(sendarray, 5, MPI_INT, rbuf, 5, MPI_INT, 0, MPI_COMM_WORLD);

	if (rank == 0)
		for (int i = 0; i < size * 5; ++i)
			pt << rbuf[i];
			
}
