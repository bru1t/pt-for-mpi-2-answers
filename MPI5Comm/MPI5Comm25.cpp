#include "mpi.h"
#include "pt4.h"

void Solve()
{
    Task("MPI5Comm25");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	// INSERT DATA
	int dims[] = {size}, periods[] = {0}, rank_dest, comm_rank;
    float f1, f2;
	MPI_Status status;
	MPI_Comm comm;
	MPI_Cart_create(MPI_COMM_WORLD, 1, dims, periods, 0, &comm);
	MPI_Comm_rank(comm, &comm_rank);
	MPI_Cart_shift(comm, 0, -1, &comm_rank, &rank_dest);
	
	// MAIN
	if(rank != 0) {
		pt >> f1;
		MPI_Send(&f1, 1, MPI_FLOAT, rank_dest, 0, comm);
	}
	
	if(rank != size - 1) {
		MPI_Recv(&f2, 1, MPI_FLOAT, MPI_ANY_SOURCE, 0, comm, &status);
		pt << f2;
	}

}
