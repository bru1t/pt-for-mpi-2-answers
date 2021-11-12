#include "mpi.h"
#include "pt4.h"
#include <vector>

void Solve()
{
    Task("MPI5Comm15");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 

	// INSERT DATA
	const int DIM = 2;
    int K = size,
		N = K >> 1,
		dims[DIM] = {DIM, N},
		add_dims[] = {0, 1},
		periods[DIM] = {true, true};
	float A = 0, end_num = -1;
    std::vector<int> count(size, 1), disp(size, 0);
	
    MPI_Comm comm, add_comm;
    MPI_Cart_create(MPI_COMM_WORLD, DIM, dims, periods, 0, &comm);
    MPI_Cart_sub(comm, add_dims, &add_comm);
    MPI_Comm_size(add_comm, &size); 
    MPI_Comm_rank(add_comm, &rank);
    
    // MAIN
    if (rank == 0)
        pt >> A;
        
    MPI_Scatterv(&A, &count[0], &disp[0], MPI_FLOAT, &end_num, count[0], MPI_FLOAT, 0, add_comm);
    
    pt << end_num;

}
