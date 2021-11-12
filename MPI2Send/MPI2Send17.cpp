#include "pt4.h"
#include "mpi.h"

void Solve()
{

    Task("MPI2Send17");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int r = 0;
    for (int i = 0; i < size - 1; ++i) {

		    int in;
		    pt >> in;

		    if (r == rank)
			    ++r;

		    MPI_Send(&in, 1, MPI_INT, r, 0, MPI_COMM_WORLD);

		    int out;
		    MPI_Status status;

		    MPI_Recv(&out, 1, MPI_INT, r, 0, MPI_COMM_WORLD, &status);

		    pt << out;
		    ++r;
        
    }

}
