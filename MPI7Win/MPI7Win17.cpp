#include "mpi.h"
#include "pt4.h"

void Solve()
{

    Task("MPI7Win17");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    float B,
		  *arr = new float[size];
	MPI_Win win;

	for (int i = 0 ; i < size ; i++)
		pt >> arr[i];
	pt >> B;

	MPI_Win_create(arr, size * sizeof(float), sizeof(float), MPI_INFO_NULL, MPI_COMM_WORLD, &win);
	MPI_Win_fence(0, win);

	for (int i = 0; i < size; i++)
        if (rank == 0)
            MPI_Accumulate(&B, 1, MPI_FLOAT, size - 1, i, 1, MPI_FLOAT, MPI_MIN, win);
        else
            MPI_Accumulate(&B, 1, MPI_FLOAT, rank - 1, i, 1, MPI_FLOAT, MPI_MIN, win);

    MPI_Win_fence(0, win);

    if (rank != 0)
        for (int i = 0; i < size; i++)
            MPI_Accumulate(&arr[0], 1, MPI_FLOAT, i, rank, 1, MPI_FLOAT, MPI_SUM, win);

    MPI_Win_fence(0, win);

    for (int i = 0; i < size; i++)
        pt << arr[i];

}
