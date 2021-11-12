#include "pt4.h"
#include "mpi.h"
#include <vector>

using namespace std;

void Solve() {
    Task("MPI5Comm14");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        int N, K = size;
        pt >> N;
        for (int i = 1; i < size; ++i)
            MPI_Send(&N, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        
        MPI_Comm comm;
        int dims[] = { N, K / N }, periods[] = { 0, 0 };
        MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 0, &comm);

        int coords[2];
        MPI_Cart_coords(comm, rank, 2, coords);
        pt << coords[0] << coords[1];
    }
    else {
        int N, K = size;
        MPI_Status s;
        MPI_Recv(&N, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &s);

        MPI_Comm comm;
        int dims[] = { N, K / N }, periods[] = { 0, 0 };
        MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 0, &comm);

        if (rank < N) {
            int coords[2];
            int result = MPI_Cart_coords(comm, rank, 2, coords);
            if (!result)
                pt << coords[0] << coords[1];
            Show(result);
        }
    }
    
}
