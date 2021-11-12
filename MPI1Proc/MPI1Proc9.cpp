#include "pt4.h"
#include "mpi.h"

void Solve()
{

    Task("MPI1Proc9");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int n;
    pt >> n;

    if (rank % 2 && rank) {

        double rez = 0;
        double j;

        for (int i = 0; i < n; i++) {
            pt >> j;
            rez += j;
        }

        pt << rez / n;

    }
    else if (!(rank % 2) && rank) {

        double rez = 0;
        double j;

        for (int i = 0; i < n; i++) {
            pt >> j;
            rez += j;
        }

        pt << rez;

    }
    else {

        double rez = 1;
        double j;

        for (int i = 0; i < n; i++) {
            pt >> j;
            rez *= j;
        }

        pt << rez;

    }

}
