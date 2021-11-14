#include "pt4.h"
#include "mpi.h"
void Solve()
{
    Task("MPI6File25");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    char name[20];
    if (rank == 0)
        pt >> name;
    MPI_Bcast(name, 20, MPI_CHAR, 0, MPI_COMM_WORLD);

    int* n = new int[3 * (rank + 1)];
    for (int i = 0; i < 3 * (rank + 1); i++)
        pt >> n[i];

    int count = 0, r = 0;
    for (int i = 0; i <= size; i++)
        count += i;
    for (int i = 0; i < rank + 1; i++)
        r += i;
    

    MPI_File f;
    MPI_File_open(MPI_COMM_WORLD, name, MPI_MODE_CREATE | MPI_MODE_RDWR, MPI_INFO_NULL, &f);

    MPI_Datatype subtype;
    MPI_Type_vector(3, rank + 1, count, MPI_INT, &subtype);
    MPI_File_set_view(f, r * 4, MPI_INT, subtype, (char*)"native", MPI_INFO_NULL);
    MPI_File_write_all(f, n, 3 * (rank + 1), MPI_INT, MPI_STATUS_IGNORE);

    MPI_File_close(&f);
}
