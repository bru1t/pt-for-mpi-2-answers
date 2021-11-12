#include "pt4.h"
#include "mpi.h"

void Solve()
{

	Task("MPI2Send15");
	int flag;
	MPI_Initialized(&flag);
	if (flag == 0)
		return;
	int rank, size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	double a, a_rec;
	int n;
	pt >> a;
	pt >> n;

	if (n == rank) {

		pt << a;
		pt << n;

	}
	else {

		MPI_Send(&a, 1, MPI_DOUBLE, n, 0, MPI_COMM_WORLD);
		MPI_Status status;
		MPI_Probe(MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
		MPI_Recv(&a_rec, 1, MPI_DOUBLE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
		pt << a_rec;
		pt << status.MPI_SOURCE;

	}
	
}
