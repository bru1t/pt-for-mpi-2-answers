#include "mpi.h"
#include "pt4.h"

void Solve()
{

    Task("MPI2Send8");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int send_num, source_index;
    MPI_Status status;

    if (rank == 0) {
    	MPI_Recv(&send_num, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
    	MPI_Recv(&source_index, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
    	pt << send_num;
    	pt << source_index;
		}
	else {

		int check_num;
		pt >> check_num;

		if (check_num != 0) {
			source_index = rank;
			send_num = check_num;
			MPI_Send(&send_num, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
			MPI_Send(&source_index, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
		}

	}

}
