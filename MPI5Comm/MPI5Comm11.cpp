#include "pt4.h"
#include "mpi.h"
#include <vector>

using namespace std;

void Solve() {
    Task("MPI5Comm11");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int N;
    pt >> N;
    MPI_Comm new_comm;
    MPI_Comm_split(MPI_COMM_WORLD, N ? 1 : MPI_UNDEFINED, rank, &new_comm);

    if (N) {
        double A, sum;
        pt >> A;

        MPI_Allreduce(&A, &sum, 1, MPI_DOUBLE, MPI_SUM, new_comm);

        pt << sum;
    }
}
