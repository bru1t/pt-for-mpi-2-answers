#include "mpi.h"
#include "pt4.h"

void Solve()
{

    Task("MPI7Win16");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	double *A = new double[size];
	double *A_T = new double [size];
	double B;
	double *B_arr = new double[size];

	for (int i = 0 ; i < size ; ++i)
		pt >> A[i];
	pt >> B;

	for (int i = 0 ; i < size ; ++i)
		B_arr[i] = B;

	MPI_Win win;
	MPI_Win_create(A, size * sizeof(double), sizeof(double), MPI_INFO_NULL, MPI_COMM_WORLD, &win);
	MPI_Win_fence(0, win);

	if (rank + 1 < size)
		MPI_Accumulate(B_arr, size, MPI_DOUBLE, rank + 1, 0, size, MPI_DOUBLE, MPI_MAX, win);
	else
		MPI_Accumulate(B_arr, size, MPI_DOUBLE, 0, 0, size, MPI_DOUBLE, MPI_MAX, win);

	MPI_Win_fence(0, win);

		for (int i = 0; i < size; ++i)
			MPI_Get(&A_T[i], 1, MPI_DOUBLE, i, rank, 1, MPI_DOUBLE, win);

	MPI_Win_fence(0, win);
  
	for (int i = 0 ; i < size ; ++i)
		pt << A_T[i];

}
