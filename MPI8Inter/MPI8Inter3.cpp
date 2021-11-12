#include "mpi.h"
#include "pt4.h"

void Solve()
{
  
    Task("MPI8Inter3");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    float X, Y, in_X, in_Y;
    MPI_Comm comm, check_comm, add_comm_1, add_comm_2;
    MPI_Comm_dup(MPI_COMM_WORLD, &check_comm);

	int check_rank_num = rank % 3, R, updated_size = size - rank, add_rank_1, add_rank_2;

	// MAIN

	// ================================================================ BLOCK IN
	if (check_rank_num - 2 == 0) {

		pt >> Y;

		MPI_Comm_split(MPI_COMM_WORLD, 2, rank, &comm);
		MPI_Comm_rank(comm, &R);

	}

	if (check_rank_num - 1 == 0) {

		pt >> X >> Y;

		MPI_Comm_split(MPI_COMM_WORLD, 1, updated_size, &comm);
		MPI_Comm_rank(comm, &R);

	}

	if (check_rank_num == 0) {

		pt >> X;

		MPI_Comm_split(MPI_COMM_WORLD, 0, rank, &comm);
		MPI_Comm_rank(comm, &R);

	}

	// ================================================================ BLOCK FUNC
	if (check_rank_num - 2 == 0) {

		MPI_Intercomm_create(comm, 0, MPI_COMM_WORLD, size - 2, 2, &add_comm_2);
		MPI_Comm_rank(add_comm_2, &add_rank_2);

	}

	if (check_rank_num - 1 == 0) {

		MPI_Intercomm_create(comm, 0, check_comm, 0, 1, &add_comm_1);
		MPI_Comm_rank(add_comm_1, &add_rank_1);

		MPI_Intercomm_create(comm, 0, MPI_COMM_WORLD, 2, 2, &add_comm_2);
		MPI_Comm_rank(add_comm_2, &add_rank_2);

	}

	if (check_rank_num == 0) {

		MPI_Intercomm_create(comm, 0, check_comm, size - 2, 1, &add_comm_1);
		MPI_Comm_rank(add_comm_1, &add_rank_1);

	}

	// ================================================================ BLOCK OUT

	pt << R;

	if (check_rank_num - 2 == 0) {

		MPI_Send(&Y, 1, MPI_FLOAT, updated_size, 0, check_comm);
		MPI_Recv(&in_Y, 1, MPI_FLOAT, MPI_ANY_SOURCE, 0, check_comm, MPI_STATUS_IGNORE);

		pt << in_Y;

	}

	if (check_rank_num - 1 == 0) {

		MPI_Recv(&in_X, 1, MPI_FLOAT, MPI_ANY_SOURCE, 0, check_comm, MPI_STATUS_IGNORE);
		MPI_Recv(&in_Y, 1, MPI_FLOAT, MPI_ANY_SOURCE, 0, check_comm, MPI_STATUS_IGNORE);

		MPI_Send(&X, 1, MPI_FLOAT, updated_size - 2, 0, check_comm);
		MPI_Send(&Y, 1, MPI_FLOAT, updated_size, 0, check_comm);

		pt << in_X << in_Y;

	}

	if (check_rank_num == 0) {

		MPI_Send(&X, 1, MPI_FLOAT, updated_size - 2, 0, check_comm);
		MPI_Recv(&in_X, 1, MPI_FLOAT, MPI_ANY_SOURCE, 0, check_comm, MPI_STATUS_IGNORE);

		pt << in_X;

	}

}
