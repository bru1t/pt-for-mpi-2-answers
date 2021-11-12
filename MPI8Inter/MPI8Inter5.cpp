#include "mpi.h"
#include "pt4.h"

void Solve()
{

    Task("MPI8Inter5");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int X, R1, R2, safe_line[1][3], intercomm_int, end_inter_size;
    MPI_Comm comm, half_part_comm, end_inter_comm, queue_comm;

    pt >> X;

    MPI_Group group;
    MPI_Comm_group(MPI_COMM_WORLD, &group);
    MPI_Group half_group;

    int half_size = size / 2;
    
    if (rank < half_size) {

        safe_line[0][0] = 0;
        safe_line[0][1] = half_size - 1;
        safe_line[0][2] = 1;

        MPI_Group_range_incl(group, 1, safe_line, &half_group);

    }
    else {

        safe_line[0][0] = half_size;
        safe_line[0][1] = size - 1;
        safe_line[0][2] = 1;

        MPI_Group_range_incl(group, 1, safe_line, &half_group);

    }

    MPI_Comm_create(MPI_COMM_WORLD, half_group, &half_part_comm);
    MPI_Comm_rank(half_part_comm, &R1);

    pt << R1;

	intercomm_int = rank < half_size ? size / 2 : 0;
    MPI_Intercomm_create(half_part_comm, 0, MPI_COMM_WORLD, intercomm_int, 1, &end_inter_comm);

    MPI_Comm_size(end_inter_comm, &end_inter_size);
    MPI_Comm_group(end_inter_comm, &group);
    MPI_Group queue_group;

    if (rank < half_size) {

        safe_line[0][0] = 0;
        safe_line[0][1] = end_inter_size - 1;
        safe_line[0][2] = 2;

    }
    else {

        safe_line[0][0] = end_inter_size - 1;
        safe_line[0][1] = 0;
        safe_line[0][2] = 2;

    }

    MPI_Group_range_incl(group, 1, safe_line, &queue_group);
    MPI_Comm_create(end_inter_comm, queue_group, &queue_comm);

    if (X != 0) {

        MPI_Comm_rank(queue_comm, &R2);
        MPI_Send(&X, 1, MPI_INT, R2 , 0, queue_comm);
        MPI_Recv(&X, 1, MPI_INT, R2, 0, queue_comm, MPI_STATUSES_IGNORE);

        pt << R2 << X;

    }

}
