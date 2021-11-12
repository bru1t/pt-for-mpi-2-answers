#include "pt4.h"
#include "mpi.h"
#include <vector>

void Solve()
{
	
	Task("MPI3Coll17");
	int flag;
	MPI_Initialized(&flag);
	if (flag == 0)
		return;
	int rank, size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int sz = size + 1;
	std::vector<double> n(sz);
	std::vector<int> scount(size, 2);
	std::vector<int> sdispls(size);
	std::vector<int> rcount(size, 2);
	std::vector<int> rdispls(size);

	for (int i = 0; i < sz; ++i) {
		pt >> n[i];
		if (i == sz - 1) continue;
		sdispls[i] = i;
		rdispls[i] = 2 * i;
	}

	int sz_re = size * 2;
	std::vector<double> re(sz_re);

	MPI_Alltoallv(&n[0], &scount[0], &sdispls[0], MPI_DOUBLE, &re[0], &rcount[0], &rdispls[0], MPI_DOUBLE, MPI_COMM_WORLD);

	for (auto& i : re)
		pt << i;

}
