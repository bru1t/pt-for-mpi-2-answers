#include "pt4.h"
#include "mpi.h"
#include <vector>

void Solve()
{

	Task("MPI3Coll10");
	int flag;
	MPI_Initialized(&flag);
	if (flag == 0)
		return;
	int rank, size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	std::vector<double> n(size + 2);
	std::vector<int> scounts(size, 3);
	std::vector<int> displs(size);

	if (rank == 0) {
		for (int i = 0; i < size + 2; ++i)
			pt >> n[i];
		for (int i = 0; i < size; ++i)
			displs[i] = i;
	}

	std::vector<double> t(3);
	MPI_Scatterv(&n[0], &scounts[0], &displs[0], MPI_DOUBLE, &t[0], 3, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	for (auto& i : t)
		pt << i;
		
}
