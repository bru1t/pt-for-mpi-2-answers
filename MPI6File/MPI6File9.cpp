#include "mpi.h"
#include "pt4.h"

void Solve()
{
    Task("MPI6File9");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int FILE_NAME_SIZE = 20, LOCAL_INDEXES_AMOUNT = 20;
    char name[FILE_NAME_SIZE];

    if (rank == 0)
        pt >> name;

    MPI_Bcast(name, FILE_NAME_SIZE, MPI_CHAR, 0, MPI_COMM_WORLD);
    MPI_File file;
    long long int file_size;
    MPI_File_open(MPI_COMM_WORLD, name, MPI_MODE_CREATE | MPI_MODE_RDWR, MPI_INFO_NULL, &file);
    MPI_File_get_size(file, &file_size);

    int* arr = new int[rank + 1];
    MPI_File_seek(file, rank * sizeof(int), MPI_SEEK_SET);
    MPI_File_read_all(file, arr, rank + 1, MPI_INT, MPI_STATUS_IGNORE);

    for (int i = 0; i < rank + 1; i++)
        if (rank + i < file_size / sizeof(int))
            pt << arr[i];

    MPI_File_close(&file);

}
