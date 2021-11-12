#include "mpi.h"
#include "pt4.h"

void Solve()
{
  
    Task("MPI8Inter1");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int X, R, Y, lead = 0, half_size = size / 2;
    int* ranks_pack_1 = new int[size / 2];
    int* ranks_pack_2 = new int[size / 2];

    pt >> X;

    MPI_Comm comm, peer, inter;
    MPI_Group old_group, new_group_1, new_group_2;
    MPI_Status status;
    MPI_Comm_dup(MPI_COMM_WORLD, &peer);
    MPI_Comm_group(MPI_COMM_WORLD, &old_group);

    for (int i = 0; i < half_size; i++)
        ranks_pack_1[i] = 2 * i;

    MPI_Group_incl(old_group, half_size, ranks_pack_1, &new_group_1);

    for (int i = 0; i < half_size; i++)
        ranks_pack_2[i] = ranks_pack_1[i] + 1;

    MPI_Group_incl(old_group, half_size, ranks_pack_2, &new_group_2);

    rank % 2 == 0 ? MPI_Comm_create(MPI_COMM_WORLD, new_group_1, &comm) : MPI_Comm_create(MPI_COMM_WORLD, new_group_2, &comm);

    MPI_Comm_rank(comm, &R);
    pt << R;

    if (rank % 2 == 0)
        lead = 1;

    MPI_Intercomm_create(comm, 0, peer, lead, 100, &inter);
    MPI_Send(&X, 1, MPI_INT, R, 0, inter);
    MPI_Recv(&Y, 1, MPI_INT, R, 0, inter, &status);

    pt << Y;

}
