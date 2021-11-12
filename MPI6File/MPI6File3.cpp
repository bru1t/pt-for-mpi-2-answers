#include "pt4.h"
#include "mpi.h"
#include <vector>

using namespace std;

void Solve() {

    Task("MPI6File3");
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

    MPI_Offset file_size;
    MPI_File_get_size(f, &file_size);

    if (rank > 0) {
        int amount = file_size / sizeof(double) / K;

        vector<double> result(amount);
        Show((rank - 1) * amount);
        Show(amount);
        MPI_File_read_at(f, (rank - 1) * amount * sizeof(double), &result[0], amount, MPI_DOUBLE, MPI_STATUS_IGNORE);

        for (auto tmp : result)
            pt << tmp;

    }
    
}
