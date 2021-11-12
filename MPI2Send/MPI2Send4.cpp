#include "pt4.h"
#include "mpi.h"
#include <vector>

void Solve()
{

    Task("MPI2Send4");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank > 0) {

        int n;
        pt >> n;
        std::vector<int> l(n);
        for (int i = 0; i < n; ++i)
            pt >> l[i];
        int bfs = n * sizeof(int) + MPI_BSEND_OVERHEAD;
        int *bf = (int*)malloc(bfs);
        MPI_Buffer_attach(bf, bfs);
        MPI_Bsend(&l[0], n, MPI_INT, 0, rank, MPI_COMM_WORLD);
        MPI_Buffer_detach(bf, &bfs);

    }
    else {
        for (int i = 1; i < size; ++i) {

            int n;
            MPI_Status status;
            MPI_Probe(i, i, MPI_COMM_WORLD, &status);
            MPI_Get_count(&status, MPI_INT, &n);
            std::vector<int> li(n);
            MPI_Recv(&li[0], n, MPI_INT, i, i, MPI_COMM_WORLD, &status);
            int s = li.size();
            for (int j = 0; j < s; ++j)
                pt << li[j];

        }

    }
    
}
