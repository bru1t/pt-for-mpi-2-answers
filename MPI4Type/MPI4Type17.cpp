#include "pt4.h"
#include "mpi.h"
#include <vector>
#include <cmath>

void Solve()
{
    Task("MPI4Type17");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank != 0) {
		int n;
		pt >> n;
		int sz = n * n;
		std::vector<int> vec(sz);

		for (int i = 0; i < sz; ++i)
			pt >> vec[i];

		MPI_Send(&vec[0], sz, MPI_INT, 0, 0, MPI_COMM_WORLD);

	}
	else {

		int n;
		pt >> n;
		MPI_Status st;
		int size_vec = n * n * (size - 1);
		std::vector<int> res(size_vec);
		MPI_Datatype nt;
		MPI_Type_vector(n, n, n * 3, MPI_INT, &nt);
		MPI_Type_commit(&nt);

		for (int i = 1; i < size; ++i) {
			int temp = (i - 1) / 3;
			int temp_mod = (i - 1) % 3;
			MPI_Recv(&res[temp * (n * n * 3) + temp_mod * n], 1, nt, i, MPI_ANY_TAG, MPI_COMM_WORLD, &st);
		}

		for (auto &i : res)
			pt << i;

	}
  
}
