#include "pt4.h"
#include "mpi.h"

void Solve()
{

    Task("MPI4Type1");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Datatype newtype;
    MPI_Type_contiguous(3, MPI_INT, &newtype);
    MPI_Type_commit(&newtype);

    int* numbers = new int[(size - 1) * 3];

    if (rank == 0)
		    for (int i = 0; i < (size - 1) * 3; ++i)
			    pt >> numbers[i];

    MPI_Bcast(numbers, size - 1, newtype, 0, MPI_COMM_WORLD);

    if (rank != 0)
        for (int i = 0; i < (size - 1) * 3; ++i)
			    pt << numbers[i];
          
}
