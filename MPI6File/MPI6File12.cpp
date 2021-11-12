#include "mpi.h"
#include "pt4.h"

void Solve()
{
    Task("MPI6File12");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int FILE_NAME_SIZE = 20;
    int n, check;
    char name[FILE_NAME_SIZE];
    MPI_Comm comm;

    if (rank == 0)
        pt >> name;

    MPI_Bcast(name, 20, MPI_CHAR, 0, MPI_COMM_WORLD);

    pt >> n;

    n > 0 ? check = 0 : check = MPI_UNDEFINED;

    MPI_Comm_split(MPI_COMM_WORLD, check, rank, &comm);
    if (comm == MPI_COMM_NULL) return;
    MPI_Comm_rank(comm, &rank);
    double fix_rank = rank;
    
    MPI_File file;
    MPI_File_open(comm, name, MPI_MODE_CREATE | MPI_MODE_RDWR, MPI_INFO_NULL, &file);
    MPI_File_write_at_all(file, (n - 1) * sizeof(double), &fix_rank, 1, MPI_DOUBLE, MPI_STATUS_IGNORE);

    MPI_File_close(&file);

}
