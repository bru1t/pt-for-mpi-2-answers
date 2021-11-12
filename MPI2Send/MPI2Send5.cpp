#include "pt4.h"
#include "mpi.h"

void Solve()
{

	Task("MPI2Send5");
	int flag;
	MPI_Initialized(&flag);
	if (flag == 0)
		return;
	int rank, size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == 0) {
		for (int i = 1; i < size; ++i) {
			double z;
			pt >> z;
			MPI_Send(&z, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
		}
	}
	else {
		MPI_Status status;
		double h;
		MPI_Recv(&h, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
		pt << h;
	}
	
}
