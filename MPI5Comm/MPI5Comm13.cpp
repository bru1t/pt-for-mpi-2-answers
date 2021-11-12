#include "mpi.h"
#include "pt4.h"

void Solve()
{
    Task("MPI5Comm13");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    // INSERT DATA
    const int DIM = 2;
    int N;
    float X, Y;
    MPI_Status status;
    
    // MAIN
    
    if (rank == 0) {
    	pt >> N;
    	for (int i = 1; i < size; i++)
    		MPI_Send(&N, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
	}
	else {
		MPI_Recv(&N, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
}
		
		int comm_size;
		MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
		
		int dims[DIM] = {N, size / N};
		MPI_Dims_create(comm_size, DIM, dims);
		
		int periods[DIM] = {true, true};
		int reorder = true;
		
		MPI_Comm comm;
		MPI_Cart_create(MPI_COMM_WORLD, DIM, dims, periods, reorder, &comm);
		
		int new_rank;
		MPI_Comm_rank(comm, &new_rank);
		
		int my_cords[DIM];
		MPI_Cart_coords(comm, new_rank, DIM, my_cords);
		
		pt << my_cords[0];
		pt << my_cords[1];

}
