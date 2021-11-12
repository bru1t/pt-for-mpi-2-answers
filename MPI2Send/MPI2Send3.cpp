#include "mpi.h"
#include "pt4.h"

void Solve()
{

	Task("MPI2Send3");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int ELEM_AMOUNT = 4;
    int data_list[ELEM_AMOUNT];
    MPI_Status status;

		if (rank != 0) {
			for (int i = 0; i < ELEM_AMOUNT; i++)
				pt >> data_list[i];

			MPI_Send(&data_list, ELEM_AMOUNT, MPI_INT, 0, 0, MPI_COMM_WORLD);

		}
		else {

			for (int i = 1; i < size; i++) {

				MPI_Recv(&data_list, ELEM_AMOUNT, MPI_INT, i, 0, MPI_COMM_WORLD, &status);

				for (int j = 0; j < ELEM_AMOUNT; j++)
					pt << data_list[j];

			}

		}

}
