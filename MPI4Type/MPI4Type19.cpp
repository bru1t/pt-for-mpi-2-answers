#include "pt4.h"
#include "mpi.h"
#include <vector>

void Solve()
{
    Task("MPI4Type19");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank != 0) {

		int n, p;
		pt >> n >> p;
		int sz = n * 2 + n - 2;
		std::vector<int> g(sz);

		for (int i = 0; i < sz; ++i)
			pt >> g[i];

		MPI_Send(&g[0], sz, MPI_INT, 0, p, MPI_COMM_WORLD);

	}
	else {

		int n;
		pt >> n;
		int sz = n * 2 + n - 2;
		int lensize = n * 3;
		std::vector<int> displs(n, 0);

		for (int i = 1; i < n; ++i)
			displs[i] = i * lensize + (n - i - 1);

		std::vector<int> blocklens(n, 1);
		blocklens[0] = n;
		blocklens[n - 1] = n;
		MPI_Datatype nt;
		MPI_Type_indexed(n, &blocklens[0], &displs[0], MPI_INT, &nt);
		MPI_Type_commit(&nt);
		std::vector<int> res(n * n * (size - 1), 0);

		for (int i = 1; i < size; ++i) {
			MPI_Status st;
			MPI_Probe(i, MPI_ANY_TAG, MPI_COMM_WORLD, &st);
			int p = st.MPI_TAG;
			MPI_Recv(&res[p], 1, nt, i, p, MPI_COMM_WORLD, &st);
		}

		for (auto &i : res)
			pt << i;

	}
  
}
