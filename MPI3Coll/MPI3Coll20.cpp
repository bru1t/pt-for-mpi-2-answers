#include "pt4.h"
#include "mpi.h"

void Solve()
{

	Task("MPI3Coll20");
	int flag;
	MPI_Initialized(&flag);
	if (flag == 0)
		return;
	int rank, size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	double* nums = new double[size + 5];
	for (int i = 0; i < size + 5; ++i)
		pt >> nums[i];

	double* result = new double[size + 5];

	MPI_Reduce(nums, result, size + 5, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);

	if (rank == 0)
		for (int i = 0; i < size + 5; ++i)
			pt << result[i];
			
}
