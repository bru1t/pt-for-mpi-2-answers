#include "pt4.h"
#include "mpi.h"
#include <vector>

void Solve()
{

	Task("MPI3Coll6");
	int flag;
	MPI_Initialized(&flag);
	if (flag == 0)
		return;
	int rank, size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	std::vector<double> n(size);

	if (rank == 0)
		for (int i = 0; i < size; ++i)
			pt >> n[i];

	double t;
	MPI_Scatter(&n[0], 1, MPI_DOUBLE, &t, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	pt << t;

}
