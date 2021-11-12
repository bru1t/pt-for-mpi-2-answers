#include "pt4.h"
#include "mpi.h"
#include <vector>

void Solve()
{

    Task("MPI3Coll5");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> n(rank + 2);
    std::vector<int> rc(size);
    std::vector<int> di(size);
    int sum = 0;

    for (int i = 0; i < size; ++i) {
        di[i] = sum;
        sum += i + 2;
        rc[i] = i + 2;
    }

    std::vector<int> rb(sum);

    for (int i = 0; i < rank + 2; ++i)
        pt >> n[i];

    MPI_Gatherv(&n[0], rank + 2, MPI_INT, &rb[0], &rc[0], &di[0], MPI_INT, 0, MPI_COMM_WORLD);

    if (!rank)
        for (auto& i : rb)
            pt << i;
            
}
