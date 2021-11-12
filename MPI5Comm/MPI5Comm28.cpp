#include "pt4.h"
#include "mpi.h"
#include <vector>

void Solve()
{
    Task("MPI5Comm28");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	int a;
	pt >> a;
	int n = (size - 1) / 2;
	int count_odd = size / 2;
	MPI_Comm nc;
	std::vector<int> indexes(size, count_odd);
	for (int i = 1; i < size; ++i) {
		int t = i % 2 ? 2 : 1;
		indexes[i] = indexes[i - 1] + t;
	}
	int sz = indexes[size - 1];
	std::vector<int> edges(sz);
	for (int i = 0; i < count_odd; ++i) {
		edges[i] = 2 * i + 1;
	}
	int ind_v = 1;
	int odd = 0;
	for (int i = count_odd; i < sz; ++i) {
		if (odd == 0) {
			edges[i] = 0;
			++odd;
		}
		else if (odd == 1) {
			++ind_v;
			edges[i] = ind_v;
			++odd;
		}
		else {
			edges[i] = ind_v - 1;
			++ind_v;
			odd = 0;
		}	
	}
	MPI_Graph_create(MPI_COMM_WORLD, size, &indexes[0], &edges[0], 0, &nc);
	int neighbors_count;
	MPI_Graph_neighbors_count(nc, rank, &neighbors_count);
	std::vector<int> neighbors(neighbors_count);
	MPI_Graph_neighbors(nc, rank, neighbors_count, &neighbors[0]);
	for (auto &i : neighbors) {
		int res;
		MPI_Send(&a, 1, MPI_INT, i, 0, nc);
		MPI_Status st;
		MPI_Recv(&res, 1, MPI_INT, i, 0, nc, &st);
		pt << res;
	}
}
