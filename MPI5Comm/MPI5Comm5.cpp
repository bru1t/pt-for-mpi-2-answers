#include "pt4.h"
#include "mpi.h"
void Solve()
{
    Task("MPI5Comm5");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm nc;
	int color = rank % 2;
	MPI_Comm_split(MPI_COMM_WORLD, color, rank, &nc);
	double n;
	double re;
	pt >> n;
	MPI_Op op = rank % 2 ? MPI_MIN : MPI_MAX;
	MPI_Reduce(&n, &re, 1, MPI_DOUBLE, op, 0, nc);
	if (rank == 0 || rank == 1) {
		pt << re;
	}
}
