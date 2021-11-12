#include "pt4.h"
#include "mpi.h"
#include <vector>

void Solve()
{

	Task("MPI3Coll26");
	int flag;
	MPI_Initialized(&flag);
	if (flag == 0)
		return;
	int rank, size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int sz = size * (size + 3) / 2;
	std::vector<int> n(sz);
	int* rcv = new int[size];

	for (int i = 0, j = 2; i < size; i++, j++)
		rcv[i] = j;

	for (int i = 0; i < sz; ++i)
		pt >> n[i];

	int sz_re = rank + 2;
	std::vector<int> re(sz_re);

	MPI_Reduce_scatter(&n[0], &re[0], rcv, MPI_INT, MPI_MIN, MPI_COMM_WORLD);

	for (auto& i : re)
		pt << i;
		
}
