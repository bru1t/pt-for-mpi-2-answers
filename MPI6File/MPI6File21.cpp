#include "mpi.h"
#include "pt4.h"

void Solve()
{
    Task("MPI6File21");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int FILE_NAME_SIZE = 20, COUNT = 3;
    int elem_arr[COUNT];
    char file_name[FILE_NAME_SIZE];
    MPI_File file;
    MPI_Datatype datatype;

    if (rank == 0)
        pt >> file_name;

    MPI_Bcast(file_name, FILE_NAME_SIZE, MPI_CHAR, 0, MPI_COMM_WORLD);
    MPI_File_open(MPI_COMM_WORLD, file_name, MPI_MODE_CREATE | MPI_MODE_RDWR, MPI_INFO_NULL, &file);
    MPI_Type_vector(COUNT, 1, size, MPI_INT, &datatype);
    MPI_File_set_view(file, sizeof(int) * rank, MPI_INT, datatype, (char*)"native", MPI_INFO_NULL);
    
    MPI_File_read_all(file, elem_arr, COUNT, MPI_INT, MPI_STATUS_IGNORE);

    for (int i = 0; i < COUNT; i++)
        pt << elem_arr[i];

    MPI_File_close(&file);

}
