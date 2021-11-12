#include "pt4.h"
#include "mpi.h"

void Solve()
{

	Task("MPI2Send28");
	int flag;
	MPI_Initialized(&flag);
	if (flag == 0)
		return;
	int rank, size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int n;
	pt >> n;

	if (n == -1) {

		for (int i = size - 1; i >= 0; --i) {

			if (i == rank) continue;

			double a;
			MPI_Request req;
			int iter = 0, flag = 0;
			MPI_Status status;

			MPI_Irecv(&a, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &req);

			while (flag == 0) {
				MPI_Test(&req, &flag, &status);
				++iter;
			}

			Show(iter);
			pt << a;

		}

	}
	else {

		double a;
		pt >> a;
		MPI_Ssend(&a, 1, MPI_DOUBLE, n, 0, MPI_COMM_WORLD);

	}

}
