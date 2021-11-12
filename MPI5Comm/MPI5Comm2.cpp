#include "pt4.h"
#include "mpi.h"
#include <vector>

void Solve()
{
    Task("MPI5Comm2");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	int g = size / 2;
	int g_n = size - g;
	std::vector<double> n(2);
	std::vector<double> res(2 * g);
	if (rank % 2) {
		pt >> n[0] >> n[1];
	}
	std::vector<int> ranks(g_n);
	for (int i = 0; i < g_n; ++i) {
		ranks[i] = 2 * i;
	}
	MPI_Group og, ng;
	MPI_Comm_group(MPI_COMM_WORLD, &og);
	MPI_Group_excl(og, g_n, &ranks[0], &ng);
	MPI_Comm nc;
	MPI_Comm_create(MPI_COMM_WORLD, ng, &nc);
	if (rank % 2) {
		MPI_Allgather(&n[0], 2, MPI_DOUBLE, &res[0], 2, MPI_DOUBLE, nc);
		for (auto &i : res) {
			pt << i;
		}
	}
}
