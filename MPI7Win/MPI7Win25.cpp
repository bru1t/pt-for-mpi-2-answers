#include "mpi.h"
#include "pt4.h"

void Solve()
{

    Task("MPI7Win25");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	double *A = new double[5];
	int M;
	double B;

	if (rank % 3 == 0)
		for (int i = 0; i < 5; ++i)
			pt >> A[i];

	if ((rank % 3) - 1 == 0)
		pt >> M >> B;

	MPI_Win win;
	MPI_Win_create(A, 5 * sizeof(double), sizeof(double), MPI_INFO_NULL, MPI_COMM_WORLD, &win);
	MPI_Win_fence(0, win);

	if ((rank % 3) - 1 == 0)
		MPI_Accumulate(&B, 1, MPI_DOUBLE, rank - 1, M, 1, MPI_DOUBLE, MPI_MIN, win);

	MPI_Win_fence(0, win);

	if ((rank % 3) - 2 == 0)
		MPI_Get(A, 5, MPI_DOUBLE, rank - 2, 0, 5, MPI_DOUBLE, win);

	MPI_Win_fence(0, win);

	if ((rank % 3) - 2 == 0)
		for (int i = 0; i < 5; ++i)
			pt << A[i];

	delete[] A;
  
}
