#include "pt4.h"
#include "mpi.h"
#include <vector>

void Solve()
{

	Task("MPI2Send31");
	int flag;
	MPI_Initialized(&flag);
	if (flag == 0)
		return;
	int rank, size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int N;
	pt >> N;
	if (N == 2) {

		int C;
		pt >> C;
		std::vector<int> S(size, 0);
		std::vector<MPI_Request> Q(C);
		std::vector<int> temp(size);

		for (int i = 0; i < C; ++i)
			MPI_Recv_init(&temp[0], size, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &Q[i]);

		MPI_Startall(C, &Q[0]);

		for (int i = 0; i < C; ++i) {

			int index;
			MPI_Status status;
			double begin = MPI_Wtime();
			MPI_Waitany(C, &Q[0], &index, &status);

			for (int i = 0; i < size; ++i)
				S[i] += temp[i];

			Show((MPI_Wtime() - begin) * 1000);
			Show(index);
			ShowLine(status.MPI_SOURCE);

		}

		for (auto& i : S)
			pt << i;

	}
	else if (N == 1) {

		int R;
		pt >> R;
		std::vector<int> z(size);

		for (int i = 0; i < size; ++i)
			pt >> z[i];

		MPI_Ssend(&z[0], size, MPI_INT, R, rank, MPI_COMM_WORLD);

	}
	
}
