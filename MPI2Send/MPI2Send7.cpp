#include "mpi.h"
#include "pt4.h"

void Solve()
{

    Task("MPI2Send7");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int N, rec_rank = 1;
    float send_num;
    MPI_Status status;

    if (rank == 0) {
    	pt >> N;
    	MPI_Send(&N, 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD);

    	for (int i = 0; i < N; i++) {
    		pt >> send_num;

    		if (rec_rank < size - 1) {
    			MPI_Send(&send_num, 1, MPI_FLOAT, rec_rank, 0, MPI_COMM_WORLD);
    			rec_rank++;
			}
			else {
				MPI_Send(&send_num, 1, MPI_FLOAT, size - 1, 0, MPI_COMM_WORLD);
			}
		}

	}
	else {

		if (rank != size - 1) {
			MPI_Recv(&send_num, 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, &status);
			pt << send_num;
		}

		if (rank == size - 1) {
			MPI_Recv(&N, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
			for (int i = 0; i < N - (size - 2); i++) {
				MPI_Recv(&send_num, 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, &status);
				pt << send_num;
			}
		}

	}

}
