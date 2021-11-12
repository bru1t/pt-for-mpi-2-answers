#include "pt4.h"
#include "mpi.h"

void Solve()
{

    Task("MPI8Inter6");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double x, y;
    pt >> x;

    MPI_Comm oldcomm, peer, inter, inter2;
    MPI_Comm_dup(MPI_COMM_WORLD, &peer);
    MPI_Group oldgroup, newgroup1, newgroup2;
    MPI_Comm_group(MPI_COMM_WORLD, &oldgroup);
    MPI_Status s;

    int ranks[1][3], r, r2, lead = 0;
    ranks[0][0] = 0;
    ranks[0][1] = size / 2 - 1;
    ranks[0][2] = 1;

    MPI_Group_range_incl(oldgroup, 1, ranks, &newgroup1);
    ranks[0][0]  = size / 2;
    ranks[0][1]  = size - 1;
    MPI_Group_range_incl(oldgroup, 1, ranks, &newgroup2);

    if(rank < size / 2)
        MPI_Comm_create(MPI_COMM_WORLD, newgroup1, &oldcomm);
    else
        MPI_Comm_create(MPI_COMM_WORLD, newgroup2, &oldcomm);
    MPI_Comm_rank(oldcomm, &r);
    pt << r;

    if (rank < size / 2)
        lead = size / 2;
    MPI_Intercomm_create(oldcomm, 0, peer, lead, 100, &inter);

    int color;
    if (rank % 2 == 0)
        color = 0;
    else
        color = 1;

    if(rank < size / 2)
        MPI_Comm_split(inter, color, rank, &inter2);
    else
        MPI_Comm_split(inter, color, size - rank, &inter2);

    MPI_Comm_rank(inter2, &r2);
    pt << r2;

    MPI_Send(&x, 1, MPI_DOUBLE, r2, 0, inter2);
    MPI_Recv(&y, 1, MPI_DOUBLE, r2, 0, inter2, &s);
    pt << y;

}
