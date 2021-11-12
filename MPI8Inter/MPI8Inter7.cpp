#include "pt4.h"
#include "mpi.h"

void Solve()
{

    Task("MPI8Inter7");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Comm comm, inter, inte, peer;
    MPI_Status s;
    MPI_Comm_dup(MPI_COMM_WORLD, &peer);
    int c, color, x, lead = 0;
    pt >> c;

    if (rank < size / 2)
        color = 0;
    else
        color = 1;

    MPI_Comm_split(MPI_COMM_WORLD, color, rank, &comm);

    int r;
    MPI_Comm_rank(comm, &r);
    pt << r;
    if (rank < size / 2)
        lead = size / 2;
    MPI_Intercomm_create(comm, 0, peer, lead, 100, &inter);
    int size_2;
    MPI_Comm_remote_size(inter, &size_2);

    MPI_Comm_split(inter, c, size - rank - 1, &inte);
    int r2, s2, rs2;
    MPI_Comm_rank(inte, &r2);
    MPI_Comm_size(inte, &s2);
    MPI_Comm_remote_size(inte, &rs2);
    int* y = new int[rs2];

    if (s2 > 1 && c == 1) {
        pt << r2;
        pt >> x;
        MPI_Send(&x, 1, MPI_INT, 0, 0, inte);
        MPI_Recv(&x, 1, MPI_INT, 0, 0, inte, &s);
        pt << x;
    }
    if (s2 == 1 && c == 1) {
        for (int i = 0; i < rs2; i++) {
            pt >> y[i];
            MPI_Recv(&x, 1, MPI_INT, i, 0, inte, &s);
            pt << x;
            MPI_Send(&y[i], 1, MPI_INT, i, 0, inte);
        }

    }

}
