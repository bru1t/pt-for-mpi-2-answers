#include "pt4.h"
#include "mpi.h"

void Solve()
{

	Task("MPI3Coll2");
	int flag;
	MPI_Initialized(&flag);
	if (flag == 0)
		return;
	int rank, size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	double numbers[5];

	if (rank == 0)
		for (int i = 0; i < 5; ++i)
			pt >> numbers[i];

	MPI_Bcast(&numbers, 5, MPI_DOUBLE, rank, MPI_COMM_WORLD);

	if (rank != 0)
		for (int i = 0; i < 5; ++i)
			pt << numbers[i];
			
}
