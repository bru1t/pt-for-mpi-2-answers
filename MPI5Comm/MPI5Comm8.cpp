#include "pt4.h"
#include "mpi.h"
void Solve()
{
    Task("MPI5Comm8");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int n;
	pt >> n;
	
	MPI_Comm comm;
	int color = n;
	MPI_Comm_split(MPI_COMM_WORLD, color, rank, &comm);

	if (n == 1) {
		double a;
		pt >> a;
		int nsize, nrank;
		MPI_Comm_size(comm, &nsize);
		MPI_Comm_rank(comm, &nrank);

		double* rec = new double[nsize];
		MPI_Gather(&a, 1, MPI_DOUBLE, rec, 1, MPI_DOUBLE, nsize - 1, comm);
		if (nrank == (nsize - 1))
			for (int i = 0; i < nsize; ++i)
				pt << rec[i];
	}
}
