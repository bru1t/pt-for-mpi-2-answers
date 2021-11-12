#include "pt4.h"
#include "mpi.h"

template <typename T>
void send(int t, MPI_Datatype d) {
    int N;
    pt >> N;
    T* arr = (T*)malloc(N * sizeof(T));
    for (int i = 0; i < N; i++)
        pt >> *(arr + i);
    MPI_Send(arr, N, d, 0, t, MPI_COMM_WORLD);
    free(arr);
}

template <typename T>
void recv(MPI_Datatype d, MPI_Status s) {
    int c;
    MPI_Get_count(&s, d, &c);
    T* arr = (T*)malloc(c * sizeof(T));
    MPI_Recv(arr, c, d, s.MPI_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &s);
    for (int i = 0; i < c; i++)
        pt << *(arr + i);
    free(arr);
}

void Solve()
{

    Task("MPI2Send23");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank != 0) {

        int t;
        pt >> t;
        if (t)
            send<double>(t, MPI_DOUBLE);
        else
            send<int>(t, MPI_INT);
    }
    else {

        for (size_t i = 1; i < size; i++)  {
            MPI_Status s;
            MPI_Probe(i, MPI_ANY_TAG, MPI_COMM_WORLD, &s);
            if (s.MPI_TAG)
                recv<double>(MPI_DOUBLE, s);
            else
                recv<int>(MPI_INT, s);
        }

    }
    
}
