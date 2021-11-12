#include "mpi.h"
#include "pt4.h"

void Solve()
{
  
    Task("MPI7Win19");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0) return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    float *A = new float[size];
    float end_num = 0;
    int update_size = size - rank - 1;

	if (rank == 0)
		for (int i = 0 ; i < size - 1; i++)
			pt >> A[i];

	MPI_Win win;
	MPI_Win_create(A, (size - 1) * sizeof(float), sizeof(float), MPI_INFO_NULL, MPI_COMM_WORLD, &win);
	MPI_Win_fence(0, win);

	if (rank != 0)
		MPI_Get(&end_num, 1, MPI_FLOAT, 0, update_size, 1, MPI_FLOAT, win);

	MPI_Win_fence(0, win);

	if (rank != 0)
		pt << end_num;

}
