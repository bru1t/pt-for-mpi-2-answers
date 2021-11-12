#include "mpi.h"
#include "pt4.h"

void Solve()
{
    Task("MPI4Type15");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int update_size = size - 1;
	MPI_Status status;

	if (rank == 0) {

		MPI_Datatype datatype;
		MPI_Type_vector(update_size, 1, update_size, MPI_FLOAT, &datatype);
		MPI_Type_commit(&datatype);
		float* A = new float[update_size * update_size];

		for (int i = 0; i < update_size * update_size; i++)
			pt >> A[i];
		for (int i = 0; i < update_size; i++)
			MPI_Send(A + i, 1, datatype, i + 1, 0, MPI_COMM_WORLD);

	}
	else {

		float* recv_data = new float[update_size];
		MPI_Recv(recv_data, update_size, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, &status);

		for (int i = 0; i < update_size; i++)
			pt << recv_data[i];

	}

}
