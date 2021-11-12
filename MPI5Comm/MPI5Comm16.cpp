#include "pt4.h"
#include "mpi.h"
void Solve()
{
    Task("MPI5Comm16");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	double a;
	if (rank == 0 || rank == 1) {
		pt >> a;
	}
	MPI_Comm nc, nc_sub;
	int n = size / 2;
	int dims[] = { n, 2 },
		periods[] = { 0, 0 },
		remain_dims[] = { 1, 0 };
	MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 0, &nc);
	MPI_Cart_sub(nc, remain_dims, &nc_sub);
	MPI_Bcast(&a, 1, MPI_DOUBLE, 0, nc_sub);
	pt << a;
}
