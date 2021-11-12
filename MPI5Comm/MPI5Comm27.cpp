#include "pt4.h"
#include "mpi.h"
void Solve()
{
    Task("MPI5Comm27");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm nc;
	int k = size / 4;
	int dims[] = { 2, 2, k },
		periods[] = { 0, 0, 1 };
	MPI_Cart_create(MPI_COMM_WORLD, 3, dims, periods, 0, &nc);
	double a;
	pt >> a;
	int rank_source, rank_dest;
	MPI_Cart_shift(nc, 2, 1, &rank_source, &rank_dest);
	MPI_Status st;
	MPI_Sendrecv_replace(&a, 1, MPI_DOUBLE, rank_dest, 0, rank_source, 0, nc, &st);
	pt << a;
}
