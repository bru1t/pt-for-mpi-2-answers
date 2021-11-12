#include "pt4.h"
#include "mpi.h"
#include <vector>

using namespace std;

void Solve() {
  
    Task("MPI8Inter4");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int C, X, Y;
    pt >> C >> X >> Y;

    MPI_Comm tmp_comm, comm_1, comm_2, comm_3;
    MPI_Comm_split(MPI_COMM_WORLD, C, rank, &tmp_comm);

    int R;
    MPI_Comm_rank(tmp_comm, &R);
    pt << R;

    if(C == 0) {
        MPI_Intercomm_create(tmp_comm, 0, MPI_COMM_WORLD, 1,
            1, &comm_1);
        MPI_Intercomm_create(tmp_comm, 0, MPI_COMM_WORLD, 2,
            2, &comm_2);
    }
    else if (C == 1) {
        MPI_Intercomm_create(tmp_comm, 0, MPI_COMM_WORLD, 0,
            1, &comm_1);
        MPI_Intercomm_create(tmp_comm, 0, MPI_COMM_WORLD, 2,
            12, &comm_2);
    }
    else if (C == 2) {
        MPI_Intercomm_create(tmp_comm, 0, MPI_COMM_WORLD, 0,
            2, &comm_1);
        MPI_Intercomm_create(tmp_comm, 0, MPI_COMM_WORLD, 1,
            12, &comm_2);
    }

    if (C != 1) {
        MPI_Send(&Y, 1, MPI_INT, R, 0, comm_1);
        MPI_Send(&X, 1, MPI_INT, R, 0, comm_2);
    }
    else {
        MPI_Send(&X, 1, MPI_INT, R, 0, comm_1);
        MPI_Send(&Y, 1, MPI_INT, R, 0, comm_2);
    }

    int X_rec, Y_rec;
    MPI_Recv(&Y_rec, 1, MPI_INT, R, 0, comm_1, MPI_STATUSES_IGNORE);
    MPI_Recv(&X_rec, 1, MPI_INT, R, 0, comm_2, MPI_STATUSES_IGNORE);

    if (C != 1)
        pt << X_rec << Y_rec;
    else
        pt << Y_rec << X_rec;

}
