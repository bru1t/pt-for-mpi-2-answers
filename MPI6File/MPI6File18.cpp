#include "mpi.h"
#include "pt4.h"

void Solve()
{
    Task("MPI6File18");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0) return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int FILE_NAME_SIZE = 12;
    int N;
    char name[FILE_NAME_SIZE];

    if (rank == 0)
        pt >> name;

    MPI_Bcast(name, FILE_NAME_SIZE, MPI_CHAR, 0, MPI_COMM_WORLD);
    MPI_File file;
    MPI_File_open(MPI_COMM_WORLD, name, MPI_MODE_CREATE | MPI_MODE_RDWR, MPI_INFO_NULL, &file);

    pt >> N;
    
    MPI_File_set_view(file, rank * sizeof(int) * 5, MPI_INT, MPI_INT, (char*)"native", MPI_INFO_NULL);
    MPI_File_write_at_all(file, N - 1, &rank, 1, MPI_INT, MPI_STATUS_IGNORE);

    MPI_File_close(&file);

}
