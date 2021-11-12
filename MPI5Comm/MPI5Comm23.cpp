#include "mpi.h"
#include "pt4.h"

void Solve()
{

    Task("MPI5Comm23");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    // INSERT DATA
    const int DIM = 2;
    int M, N, X, Y, end_rank;
    MPI_Comm comm;
    
    // MAIN
    
    if (rank == 0) {
    	pt >> M;
    	pt >> N;
	}
    
    MPI_Bcast(&M, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    int dims[DIM] = {M, N},
		periods[DIM] = {true, false};
    MPI_Cart_create(MPI_COMM_WORLD, DIM, dims, periods, 0, &comm);

	if (rank < M * N) {
		pt >> X;
		pt >> Y;
	}
	
	int coords[DIM] = {X, Y};
	
	if (comm != MPI_COMM_NULL)
		if (!MPI_Cart_rank(comm, coords, &end_rank))
			pt << end_rank;
		else
			pt << -1;

}
