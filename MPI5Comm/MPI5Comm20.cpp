#include "mpi.h"
#include "pt4.h"

void Solve()
{
    Task("MPI5Comm20");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	// INSERT DATA
	int num;
	pt >> num;

	// MAIN
	
	int* dims = new int[3];
	dims[0] = 2;
	dims[1] = 2;
	dims[2] = size / 4;

	int* periods = new int[3];
	periods[0] = 0;
	periods[1] = 0;
	periods[2] = 0;

	int remain_dims[3] = {1, 1, 0}, n_rank, nums[4];
	MPI_Comm d_comm, n_comm;
	MPI_Cart_create(MPI_COMM_WORLD, 3, dims, periods, false, &d_comm);
	MPI_Cart_sub(d_comm, remain_dims, &n_comm);
	MPI_Gather(&num, 1, MPI_INT, nums, 1, MPI_INT, 0, n_comm);
	
	MPI_Comm_rank(n_comm, &n_rank);
	if (n_rank == 0)
		for (int i = 0; i < 4; ++i)
			pt << nums[i];

}
