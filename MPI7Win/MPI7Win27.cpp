#include "mpi.h"
#include "pt4.h"
#include <math.h>

void Solve()
{
    Task("MPI7Win27");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int LINE_SIZE = 2;
    float indexes[LINE_SIZE] = {0, 0}, indexes_dist[LINE_SIZE] = {1, 1}, all_indexes[10][LINE_SIZE];
    MPI_Win win;

	if (rank != 0)
		pt >> indexes[0] >> indexes[1];

	MPI_Win_create(indexes, LINE_SIZE * sizeof(float), sizeof(float), MPI_INFO_NULL, MPI_COMM_WORLD, &win);
	MPI_Win_fence(0, win);
  
	if (rank == 0)
		for (int i = 1; i < size; i++) {
			MPI_Get(&all_indexes[i][0], 1, MPI_FLOAT, i, 0, 1, MPI_FLOAT, win);
			MPI_Get(&all_indexes[i][1], 1, MPI_FLOAT, i, 1, 1, MPI_FLOAT, win);
		}

	MPI_Win_fence(0, win);

	if (rank == 0) {
		for (int i = 1; i < size; i++)
			if (sqrt(pow(all_indexes[i][0], 2.0) + pow(all_indexes[i][1], 2.0)) > sqrt(pow(indexes_dist[0], 2.0) + pow(indexes_dist[1], 2.0))) {
				indexes_dist[0] = all_indexes[i][0];
				indexes_dist[1] = all_indexes[i][1];
			}

		indexes[0] = indexes_dist[0];
		indexes[1] = indexes_dist[1];

	}

	MPI_Win_fence(0, win);

	if (rank)
		MPI_Get(indexes_dist, LINE_SIZE, MPI_FLOAT, 0, 0, 2, MPI_FLOAT, win);

	MPI_Win_fence(0, win);

	if (rank)
		pt << indexes_dist[0] << indexes_dist[1];

}
