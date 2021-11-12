#include "mpi.h"
#include "pt4.h"

void Solve()
{

    Task("MPI2Send20");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Status status;

    if (rank == 0) {
		    for (int i = 1; i < size; ++i) {
			    float num;
			    MPI_Recv(&num, 1, MPI_FLOAT, MPI_ANY_SOURCE, i, MPI_COMM_WORLD, &status);
			    pt << num;
		    }
    }
    else {
		    int N;
		    float A;

		    pt >> A;
		    pt >> N;

		    MPI_Send(&A, 1, MPI_FLOAT, 0, N, MPI_COMM_WORLD);
    }

}
