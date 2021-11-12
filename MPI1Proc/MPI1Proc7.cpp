#include "pt4.h"
#include "mpi.h"

void Solve()
{

    Task("MPI1Proc7");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (!(rank % 2)) {

        int n;
        double num;
        double sum = 0;

        pt >> n;

        for (int i = 0; i < n; ++i) {
            pt >> num;
            sum += num;
        }

        pt << sum;

    }

}
