#include "pt4.h"
#include "mpi.h"

void Solve()
{

	Task("MPI2Send6");
	int flag;
	MPI_Initialized(&flag);
	if (flag == 0)
		return;
	int rank, size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == 0) {
		
		double x;
		int buffsize = sizeof(double) + MPI_BSEND_OVERHEAD;
		double* buff = (double*)malloc(buffsize);
		MPI_Buffer_attach(buff, buffsize);

		for (int i = size - 1; i > 0; i--) {
			pt >> x;
			MPI_Bsend(&x, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
		}

		MPI_Buffer_detach(buff, &buffsize);

	}
	else {

		double x;
		MPI_Recv(&x, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		pt << x;

	}

}
