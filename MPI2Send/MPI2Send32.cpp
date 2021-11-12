#include "pt4.h"
#include "mpi.h"

void Solve()
{

    Task("MPI2Send32");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int N, C;
    double A;
    pt >> N;
    MPI_Request r;

    if (N == 1) {

        pt >> A;
        pt >> C;
        MPI_Request* Q = new MPI_Request[C];

        for (int i = 0; i < C; i++) {
            int ran;
            pt >> ran;
            MPI_Ssend_init(&A, 1, MPI_DOUBLE, ran, 0, MPI_COMM_WORLD, &Q[i]);
        }

        MPI_Startall(C, &Q[0]);
        int c = 0;

        for (int i = 0; i < C; i++) {

            int f = 0, ind = 0;
            while (!f) {

                MPI_Testany(C, &Q[0], &ind, &f, MPI_STATUSES_IGNORE);
                ++c;
                if (f) {
                    Show(ind);
                    ShowLine(c);
                }

            }

        }

    }
    else if (N == 2) {

        MPI_Recv(&A, 1, MPI_DOUBLE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
        pt << A;

    }
    
}
