#include "mpi.h"
#include "pt4.h"

void Solve()
{

    Task("MPI2Send14");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int n1, n2;
    MPI_Status status;

    if (rank == 0) {

        int cache;

        pt >> n1;
		    pt >> n2;

		    MPI_Ssend(&n2, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
		    MPI_Recv(&n2, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD, &status);
		    MPI_Recv(&cache, 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD, &status);
		    MPI_Ssend(&n1, 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD);

		    pt << cache;
		    pt << n2;

    }
    else {

        int cache;
		    int next_index;

				rank + 1 == size ? next_index = 0 : next_index = rank + 1;

        MPI_Recv(&cache, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &status);
        pt >> n1;
        pt >> n2;
        MPI_Ssend(&n1, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD);
        MPI_Ssend(&n2, 1, MPI_INT, next_index, 0, MPI_COMM_WORLD);
        MPI_Recv(&n2, 1, MPI_INT, next_index, 0, MPI_COMM_WORLD, &status);

        pt << cache;
        pt << n2;

    }

}
