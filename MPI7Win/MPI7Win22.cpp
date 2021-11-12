#include "mpi.h"
#include "pt4.h"

void Solve()
{

    Task("MPI7Win22");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	double *A = new double[size - 1];
	double *B = new double[rank];
	int N[8];

	if (rank == 0) {
		for(int i = 0; i < size - 1; ++i){
			pt >> A[i];
			ShowLine(A[i]);
		}
		for (int i = 0 ; i < 8; ++i){
			pt >> N[i];
			ShowLine(N[i]);
		}
	} else {
		for (int i = 0 ; i < rank ; ++i)
			pt >> B[i];
	}

	MPI_Win win;
	MPI_Win_create(B, rank * sizeof(double), sizeof(double), MPI_INFO_NULL, MPI_COMM_WORLD, &win);
	MPI_Win_fence(0, win);

	if (rank == 0)
		for (int i = 0; i < 8; ++i)
			MPI_Accumulate(A, N[i], MPI_DOUBLE, N[i], 0, N[i], MPI_DOUBLE, MPI_SUM, win);

	MPI_Win_fence(0, win);

	if (rank != 0)
		for (int i = 0; i < rank; ++i)
			pt << B[i];

	delete[] A;
	delete[] B;
  
}
