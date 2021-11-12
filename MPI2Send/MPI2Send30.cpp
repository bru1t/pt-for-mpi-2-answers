#include "pt4.h"
#include "mpi.h"

void Solve()
{

	Task("MPI2Send30");
	int flag;
	MPI_Initialized(&flag);
	if (flag == 0)
		return;
	int rank, size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int n;
	pt >> n;

	if (n == 1) {

		int r;
		pt >> r;
		int x;
		MPI_Request request;
		MPI_Status status;
		MPI_Ssend_init(&x, 1, MPI_INT, r, 0, MPI_COMM_WORLD, &request);

		for (int i = 0; i < size; ++i) {
			pt >> x;
			MPI_Start(&request);
			double start_time = MPI_Wtime();
			MPI_Wait(&request, &status);
			double end_time = MPI_Wtime();
			Show((end_time - start_time) * 1000);
		}

	}

	if (n == 2) {

		int x;
		MPI_Request request;
		MPI_Status status;

		MPI_Recv_init(&x, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &request);

		for (int i = 0; i < size; ++i) {

			MPI_Start(&request);
			double start_time = MPI_Wtime();
			MPI_Wait(&request, &status);
			double end_time = MPI_Wtime();
			Show((end_time - start_time) * 1000);
			pt << x;

		}

	}
	
}
