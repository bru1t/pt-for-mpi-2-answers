#include "pt4.h"
#include "mpi.h"

void Solve()
{

    Task("MPI2Send29");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int n;
    pt >> n;

    if (n == -1) {

        double s = 0;
        double* x = new double[size - 1];
        double b;
        int ind;

        MPI_Request* Q = new MPI_Request[size - 1];
        MPI_Request request;
        MPI_Status status;

        for (int i = 0; i < size - 1; ++i) {

            MPI_Irecv(&x[i], 1, MPI_DOUBLE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &request);
            Q[i] = request;
            
        }

        for (int i = 0; i < size - 1; ++i) {

            MPI_Waitany(size - 1, Q, &ind, &status);
            s += x[ind];
            Show(x[ind]);
            ShowLine(status.MPI_SOURCE);

        }

        pt << s;

    }
    else {

        double a;
        pt >> a;
        MPI_Ssend(&a, 1, MPI_DOUBLE, n, 0, MPI_COMM_WORLD);

    }

}
