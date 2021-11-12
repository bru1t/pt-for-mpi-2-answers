#include "mpi.h"
#include "pt4.h"

void Solve()
{
    Task("MPI8Inter8");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int C, R1, R2, X, Y, check_size, comm_size, comm_rank;
    MPI_Comm comm, inter_comm, check_comm;

    pt >> C;
    pt >> X;

    Show(C);
    Show(X);

    MPI_Comm_split(MPI_COMM_WORLD, rank % 2, 0, &comm);
    MPI_Comm_rank(comm, &R1);

    pt << R1;

    MPI_Intercomm_create(comm, 0, MPI_COMM_WORLD, (rank + 1) % 2, 0, &inter_comm);
    MPI_Comm_split(inter_comm, C, 0, &check_comm);
    MPI_Comm_size(check_comm, &check_size);
    MPI_Comm_rank(check_comm, &R2);

    pt << R2;

    MPI_Intercomm_merge(check_comm, rank % 2, &comm);
    MPI_Comm_size(comm, &comm_size);
    MPI_Comm_rank(comm, &comm_rank);

    if (rank % 2 == 0)
    	for (int i = check_size; i < comm_size; i++)
            MPI_Send(&X, 1, MPI_INT, i, 0, comm);
    else
        for (int i = 0; i < comm_size - check_size; i++)
            MPI_Send(&X, 1, MPI_INT, i, 0, comm);


    if (rank % 2 == 0) {
    	for (int i = check_size; i < comm_size; i++) {
            MPI_Recv(&Y, 1, MPI_INT, i, 0, comm, MPI_STATUS_IGNORE);
            pt << Y;
        }
    }
    else {
        for (int i = 0; i < comm_size - check_size; i++) {
            MPI_Recv(&Y, 1, MPI_INT, i, 0, comm, MPI_STATUS_IGNORE);
            pt << Y;
        }
    }

}
