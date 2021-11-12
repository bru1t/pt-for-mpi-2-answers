#include "pt4.h"
#include "mpi.h"

void Solve()
{

    Task("MPI2Send10");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	  if (rank == 0) {
      
		    int k;
		    pt >> k;
		    int z, rez = 0;
		    MPI_Status status;

		    for (int i = 0; i < k; ++i) {
            MPI_Probe(MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
            MPI_Recv(&z, 1, MPI_DOUBLE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
            rez += z;
        }

    pt << rez;

	}
	else {

		int n;

		pt >> n;

		if (n > 0)
			MPI_Send(&n, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

	}

}
