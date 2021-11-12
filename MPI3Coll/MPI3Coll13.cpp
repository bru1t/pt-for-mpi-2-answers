#include "pt4.h"
#include "mpi.h"

void Solve()
{

    Task("MPI3Coll13");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int _size = 0;
    for (size_t i = 0; i < size; i++)
        _size += i + 2;

    int* arr = (int*)malloc((rank + 2) * sizeof(int));
    int* arr_recv = (int*)malloc(_size * sizeof(int));
    int* rcounts = (int*)malloc(size * sizeof(int));
    int* displs = (int*)malloc(size * sizeof(int));

    for (size_t i = 0; i < size; i++) {
        *(rcounts + i) = i + 2;
        if (i == 0) {
            *(displs + i) = 0;
            continue;
        }
        *(displs + i) = *(displs + i - 1) + *(rcounts + i - 1);
    }

    for (int i = 0; i < rank + 2; i++)
        pt >> *(arr + i);

    MPI_Allgatherv(arr, rank + 2, MPI_INT, arr_recv, rcounts, displs, MPI_INT, MPI_COMM_WORLD);

    for (int i = 0; i < _size; i++)
        pt << *(arr_recv + i);
        
}
