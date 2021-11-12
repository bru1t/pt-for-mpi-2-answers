#include "pt4.h"
#include "mpi.h"
void Solve()
{
    Task("MPI5Comm10");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	int n;
	pt >> n;
	MPI_Comm nc;
	MPI_Comm_split(MPI_COMM_WORLD, n, rank, &nc);
	int a;
	pt >> a;
	int n_sz;
	MPI_Comm_size(nc, &n_sz);
	std::vector<int> res(n_sz);
	MPI_Allgather(&a, 1, MPI_INT, &res[0], 1, MPI_INT, nc);
	for (auto &i : res) {
		pt << i;
	}
}
