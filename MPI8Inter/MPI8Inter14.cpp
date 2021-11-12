#include "pt4.h"
#include "mpi.h"

void Solve()
{

    Task("MPI8Inter14");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Comm comm, peer, inter;
    MPI_Comm_dup(MPI_COMM_WORLD, &peer);
    int c;
    pt >> c;
    if (c == 0)
        c = MPI_UNDEFINED;

    if(c == 1)
        MPI_Comm_split(MPI_COMM_WORLD, c, rank, &comm);
    else
        MPI_Comm_split(MPI_COMM_WORLD, c, size - rank, &comm);
    if (comm == MPI_COMM_NULL)
    {
        pt << -1;
        return;
    }

    int ra, lead = 0;
    MPI_Comm_rank(comm, &ra);
    pt << ra;
    if (c == 1)
        lead = size - 1;
    MPI_Intercomm_create(comm, 0, peer, lead, 100, &inter);
    int rs;
    MPI_Comm_remote_size(inter, &rs);
    int* x = new int[rs];
    int* y = new int[rs];
    for (int i = 0; i < rs; i++)
            pt >> x[i];
    MPI_Alltoall(x, 1, MPI_INT, y, 1, MPI_INT, inter);
    for (int i = 0; i < rs; i++)
        pt << y[i];
        
}
