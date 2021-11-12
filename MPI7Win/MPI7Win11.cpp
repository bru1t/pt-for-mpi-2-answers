#include "mpi.h"
#include "pt4.h"

void Solve()
{

    Task("MPI7Win11");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int HALF_SIZE = size / 2;
    int *A = new int[HALF_SIZE],
		*B = new int[HALF_SIZE];
	MPI_Win win;

	if (rank % 2)
		for (int i = 0 ; i < HALF_SIZE; i++)
			pt >> B[i];
	else
		for (int i = 0 ; i < HALF_SIZE; i++)
			pt >> A[i];
      
	MPI_Win_create(B, HALF_SIZE * sizeof(int), sizeof(int), MPI_INFO_NULL, MPI_COMM_WORLD, &win);
	MPI_Win_fence(0, win);

	if (rank % 2 == 0)
		for (int i = 0; i < HALF_SIZE; i++)
			MPI_Accumulate(&A[i], 1, MPI_INT, i * 2 + 1, rank / 2, 1, MPI_INT, MPI_SUM, win);

	MPI_Win_fence(0, win);

	if (rank % 2)
		for (int i = 0 ; i < HALF_SIZE; i++)
			pt << B[i];

}
