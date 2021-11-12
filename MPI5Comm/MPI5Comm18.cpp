#include "pt4.h"
#include "mpi.h"
void Solve()
{
    Task("MPI5Comm18");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int n = size / 3;
	int* numbers = new int[n];

	if (rank == 0 || rank == 1 || rank == 2)
		for (int i = 0; i < n; ++i)
			pt >> numbers[i];

	int* dims = new int[2];
	dims[0] = n;
	dims[1] = 3;

	int* periods = new int[2];
	periods[0] = 0;
	periods[1] = 0;

	MPI_Comm dcomm;
	MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, false, &dcomm);

	MPI_Comm colcomm;
	int remain_dims[2] = { 1, 0 };
	MPI_Cart_sub(dcomm, remain_dims, &colcomm);

	int x;
	MPI_Scatter(numbers, 1, MPI_INT, &x, 1, MPI_INT, 0, colcomm);
	pt << x;
}
