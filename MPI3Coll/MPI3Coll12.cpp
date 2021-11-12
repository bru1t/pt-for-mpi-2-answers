#include "pt4.h"
#include "mpi.h"

void Solve()
{
	
	Task("MPI3Coll12");
	int flag;
	MPI_Initialized(&flag);
	if (flag == 0)
		return;
	int rank, size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int* numbers = new int[4];

	for (int i = 0; i < 4; ++i)
		pt >> numbers[i];

	int res_size = size * 4;
	int* result = new int[res_size];

	MPI_Allgather(numbers, 4, MPI_INT, result, 4, MPI_INT, MPI_COMM_WORLD);

	for (int i = 0; i < res_size; ++i)
		pt << result[i];

}
