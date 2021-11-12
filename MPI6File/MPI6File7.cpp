#include "pt4.h"
#include "mpi.h"
#include <vector>

using namespace std;

void Solve() {

    Task("MPI6File7");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int K = size - 1;
    char file_name[30];
    if (rank == 0)
        pt >> file_name;
    MPI_Bcast(file_name, 30, MPI_CHAR, 0, MPI_COMM_WORLD);

    MPI_File f;
    MPI_File_open(MPI_COMM_WORLD, file_name, MPI_MODE_RDONLY, MPI_INFO_NULL, &f);

    if (rank > 0) {
        double S = 0;

        while (S < rank) {
            double tmp;
            MPI_File_read(f, &tmp, 1, MPI_DOUBLE, MPI_STATUS_IGNORE);
            S += tmp;
        }

        MPI_Offset pos;
        MPI_File_get_position(f, &pos);
        int N = pos / sizeof(double);
        pt << S << N;

        MPI_File_seek(f, -pos, MPI_SEEK_END);

        for (int i = 0; i < N; ++i) {
            double tmp;
            MPI_File_read(f, &tmp, 1, MPI_DOUBLE, MPI_STATUS_IGNORE);
            pt << tmp;
        }

    }
    
}
