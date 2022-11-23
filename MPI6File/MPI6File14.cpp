#include "pt4.h"
#include "mpi.h"

void Solve()
{
    Task("MPI6File14");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    char name[12];
    if (rank == 0)
        pt >> name;
    MPI_Bcast(name, 12, MPI_CHAR, 0, MPI_COMM_WORLD);

    int val = 0;
    pt >> val;
    int color = val != 0 ? 0 : MPI_UNDEFINED;
    MPI_Comm newcomm;
    MPI_Comm_split(MPI_COMM_WORLD, color, rank, &newcomm);

    if (newcomm == MPI_COMM_NULL)
    {
        return;
    }
    
    MPI_File f;
    MPI_File_open(newcomm, name, MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &f);

    MPI_Comm_size(newcomm, &size);
    MPI_Comm_rank(newcomm, &rank);

    int* arr = new int[size];
    for (int i = 0; i < size; i++)
    {
        arr[i] = val;
    }

    MPI_Status s;
    MPI_File_write_ordered(f, arr, size, MPI_INT, &s);

    MPI_File_close(&f);

    delete [] arr;
    
}
