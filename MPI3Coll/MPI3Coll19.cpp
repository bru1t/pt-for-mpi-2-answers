#include "pt4.h"
#include "mpi.h"
#include <vector>

void Solve()
{

	Task("MPI3Coll19");
	int flag;
	MPI_Initialized(&flag);
	if (flag == 0)
		return;
	int rank, size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int sz = size + 5;
	std::vector<int> n(sz);

	for (int i = 0; i < sz; ++i)
		pt >> n[i];

	std::vector<int> rez(sz);
	MPI_Reduce(&n[0], &rez[0], sz, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	if (rank == 0) {
		for (auto& i : rez)
			pt << i;
	}
	
}
