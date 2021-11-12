#include "pt4.h"
#include "mpi.h"

void Solve()
{

    Task("MPI2Send21");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank > 0) {
        int l;
        pt >> l;

        for (int i = 0; i < l; ++i) {
            double a;
            pt >> a;
            int n;
            pt >> n;
            MPI_Send(&a, 1, MPI_DOUBLE, 0, n, MPI_COMM_WORLD);
        }
        
    }
    else {

        for (int i = 0; i <  2 * size; ++i) {
            MPI_Status status;
            double s;
            MPI_Recv(&s, 1, MPI_DOUBLE, MPI_ANY_SOURCE, i + 1, MPI_COMM_WORLD, &status);
            pt << s;
        }

    }

}
