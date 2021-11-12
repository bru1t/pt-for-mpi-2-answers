#include "pt4.h"
#include "mpi.h"
#include <vector>

struct max_iv {
	float a;
	int b;
};

void Solve()
{

	Task("MPI3Coll21");
	int flag;
	MPI_Initialized(&flag);
	if (flag == 0)
		return;
	int rank, size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int sz = size + 5;
	std::vector<max_iv> n(sz);

	for (int i = 0; i < sz; ++i) {
		int z;
		pt >> z;
		n[i] = { float(z), rank };
	}

	std::vector<max_iv> re(sz);
	MPI_Reduce(&n[0], &re[0], sz, MPI_FLOAT_INT, MPI_MAXLOC, 0, MPI_COMM_WORLD);

	if (rank == 0) {
		for (auto& i : re)
			pt << int(i.a);
		for (auto& i : re)
			pt << i.b;
	}
	
}
