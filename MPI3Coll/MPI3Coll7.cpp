#include "pt4.h"
#include "mpi.h"
#include <vector>

void Solve()
{

	Task("MPI3Coll7");
	int flag;
	MPI_Initialized(&flag);
	if (flag == 0)
		return;
	int rank, size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	std::vector<double> n(3 * size);
	std::vector<double> t(3 * size);

	if (rank == 0)
		for (int i = 0; i < 3 * size; ++i)
			pt >> n[i];

	for (int i = 0; i < 3; ++i) {
		MPI_Scatter(&n[i], 3, MPI_DOUBLE, &t[i], 3, MPI_DOUBLE, 0, MPI_COMM_WORLD);
		pt << t[i];
	}
	
}
