#include "pt4.h"
#include "mpi.h"
#include <vector>

void Solve()
{

	Task("MPI2Send9");
	int flag;
	MPI_Initialized(&flag);
	if (flag == 0)
		return;
	int rank, size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == 0) {

		int sz;
		MPI_Status status;
		MPI_Probe(MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
		MPI_Get_count(&status, MPI_DOUBLE, &sz);
		std::vector<double> ans(sz);
		MPI_Recv(&ans[0], sz, MPI_DOUBLE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
		for (int i = 0; i < sz; ++i)
			pt << ans[i];
		pt << status.MPI_SOURCE;

	}
	else {

		int N;
		pt >> N;
		if (N) {
			std::vector<double> arr(N);
			for (int i = 0; i < N; ++i)
				pt >> arr[i];
			MPI_Send(&arr[0], N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);

		}

	}

}
