#include "pt4.h"
#include "mpi.h"
#include <vector>

using namespace std;

void Solve() {

    Task("MPI6File10");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int R = rank;
    char file_name[30];
    if (rank == 0)
        pt >> file_name;
    MPI_Bcast(file_name, 30, MPI_CHAR, 0, MPI_COMM_WORLD);

    MPI_File f;
    MPI_File_open(MPI_COMM_WORLD, file_name, MPI_MODE_CREATE | MPI_MODE_RDWR, MPI_INFO_NULL, &f);

    vector<int> data(R + 1);
    for (int i = 0; i < R; ++i)
        pt >> data[i];

    MPI_File_seek(f, (R * (R - 1)) / 2 * sizeof(int), MPI_SEEK_SET);
    MPI_File_write_all(f, &data[0], R, MPI_INT, MPI_STATUS_IGNORE);
    MPI_File_close(&f);
    
}
