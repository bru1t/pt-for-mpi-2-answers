#include "pt4.h"
#include "mpi.h"
#include <vector>

void Solve()
{

	Task("MPI3Coll15");
	int flag;
	MPI_Initialized(&flag);
	if (flag == 0)
		return;
	int rank, size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int sz = 3 * size;
	std::vector<int> n(sz);

	for (int i = 0; i < sz; ++i)
		pt >> n[i];

	std::vector<int> re(sz);

	MPI_Alltoall(&n[0], 3, MPI_INT, &re[0], 3, MPI_INT, MPI_COMM_WORLD);

	for (auto& i : re)
		pt << i;
		
}
