#include "pt4.h"
#include "mpi.h"

void Solve()
{
    Task("MPI6File29");
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

    MPI_File f;
    MPI_File_open(MPI_COMM_WORLD, name, MPI_MODE_RDONLY, MPI_INFO_NULL, &f);

    MPI_Offset offset;
    MPI_File_get_size(f, &offset);

    int all_cnt = offset / sizeof(int);
    int nsize = all_cnt / size;
    int n = round(sqrt(nsize));

    Show(nsize);
    Show(n);
    Show(rank / 3);

    MPI_Datatype t;
    MPI_Type_vector(n, n, 3 * n, MPI_INT, &t);

    char native[7] = "native";

    MPI_File_set_view(f, ((rank % 3) * n + (rank / 3) * 3 * nsize) * sizeof(int), MPI_INT, t, native, MPI_INFO_NULL);

    int* arr = new int[nsize];
    MPI_File_read_all(f, arr , nsize , MPI_INT, MPI_STATUS_IGNORE);

    MPI_File_close(&f);

    for (int i = 0; i < nsize; i++)
    {
        pt << arr[i];
    }

    delete[] arr;

}
