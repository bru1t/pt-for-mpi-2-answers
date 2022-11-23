#include "pt4.h"
#include "mpi.h"

void Solve()
{
    Task("MPI6File5");
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

    if (rank != 0)
    {

        int val = 0;

        while (val != rank)
        {
            MPI_File_read(f, &val, 1, MPI_INT, MPI_STATUSES_IGNORE);
            pt << val;
        }

        MPI_Offset offset;
        MPI_File_get_position(f, &offset);
        MPI_File_seek(f, offset * (-1), MPI_SEEK_END);

        for (int i = 0; i < offset / sizeof(int); i++)
        {
            MPI_File_read(f, &val, 1, MPI_INT, MPI_STATUSES_IGNORE);
            pt << val;
        }
        
    }

    MPI_File_close(&f);

}
