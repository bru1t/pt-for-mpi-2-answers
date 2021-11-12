#include "pt4.h"
#include "mpi.h"
#include <vector>

using namespace std;

void Solve() {
    Task("MPI8Inter2");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int K = size, C;
    double X, new_X;
    pt >> C >> X;

    MPI_Comm tmp_comm, new_comm;

    int sign = (C == 0 ? +1 : -1);
    MPI_Comm_split(MPI_COMM_WORLD, C, C * K + sign * rank, &tmp_comm);

    int R;
    MPI_Comm_rank(tmp_comm, &R);

    if (C == 0)
        MPI_Intercomm_create(tmp_comm, 0, MPI_COMM_WORLD, K - 1, 1, &new_comm);
    else
        MPI_Intercomm_create(tmp_comm, 0, MPI_COMM_WORLD, 0, 1, &new_comm);

    int new_comm_rank;
    MPI_Comm_rank(new_comm, &new_comm_rank);

    MPI_Send(&X, 1, MPI_DOUBLE, R, 0, new_comm);
    MPI_Recv(&new_X, 1, MPI_DOUBLE, R, 0, new_comm, MPI_STATUS_IGNORE);
    
    Show(new_comm_rank);
    pt << R << new_X;
}
