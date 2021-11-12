#include "mpi.h"
#include "pt4.h"

void Solve()
{
    Task("MPI7Win2");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int AMOUNT = 40;
    int i_sum = 0, line = 0;
    float *out = new float[rank], *in = new float[AMOUNT];
    MPI_Win win;

	for (int i = 0 ; i < rank ; i++)
		pt >> out[i];

	for  (int i = 0 ; i < rank ; i++)
		i_sum += i;

	MPI_Win_create(in, AMOUNT * sizeof(float), sizeof(float), MPI_INFO_NULL, MPI_COMM_WORLD, &win);
	MPI_Win_fence(0, win);

	if (rank != 0)
        MPI_Put(out, rank, MPI_FLOAT, 0, i_sum, rank, MPI_FLOAT, win);

    MPI_Win_fence(0, win);
    MPI_Win_free(&win);

	if (rank == 0) {

		for (int i = 0 ; i < size ; i++)
			line+=i;

		for (int i = 0 ; i < line ; i++)
			pt << in[i];

	}

}
