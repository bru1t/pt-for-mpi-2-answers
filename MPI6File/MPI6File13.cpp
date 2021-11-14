#include "pt4.h"
#include "mpi.h"
void Solve()
{
    Task("MPI6File13");
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
    int n, p, color;
    pt >> n;

    MPI_Comm comm;
    if (n == 1)
        color = 0;
    else
        color = MPI_UNDEFINED;
    MPI_Comm_split(MPI_COMM_WORLD, color, rank, &comm);
    if (comm == MPI_COMM_NULL)
        return;
    MPI_Comm_rank(comm, &rank);
    
    int* m = new int[rank + 1];

    MPI_File f;
    MPI_Status s;
    MPI_Offset off;
    MPI_File_open(comm, name, MPI_MODE_CREATE | MPI_MODE_RDWR, MPI_INFO_NULL, &f);

    MPI_File_read_ordered(f, m, rank + 1, MPI_INT, &s);
    for (int i = 0; i < rank + 1; i++)
        if (m[i] >= 0)
            pt << m[i];
    
    MPI_Get_count(&s, MPI_INT, &n);
    pt << n;

    MPI_File_get_position_shared(f, &off);
    p = off;
    pt << p;

    MPI_File_close(&f);
}
