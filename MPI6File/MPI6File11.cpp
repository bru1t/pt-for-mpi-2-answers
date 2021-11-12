#include "pt4.h"
#include "mpi.h"
#include <vector>

using namespace std;

void Solve() {

    Task("MPI6File11");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    char file_name[30];
    if (rank == 0)
        pt >> file_name;
    MPI_Bcast(file_name, 30, MPI_CHAR, 0, MPI_COMM_WORLD);

    int number;
    pt >> number;

    MPI_Comm new_comm;
    MPI_Comm_split(MPI_COMM_WORLD, number > 0, rank, &new_comm);

    MPI_File f;
    MPI_File_open(new_comm, file_name, MPI_MODE_RDONLY, MPI_INFO_NULL, &f);

    double get;
    
    MPI_File_read_at_all(f, (number - 1) * sizeof(double), &get, 1, MPI_DOUBLE, MPI_STATUS_IGNORE);

    if (number > 0)
        pt << get;

}
