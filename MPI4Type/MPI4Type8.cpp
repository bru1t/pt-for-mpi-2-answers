#include "pt4.h"
#include "mpi.h"
#include <vector>

struct newtype {
	int a;
	int b;
	double c;
};

void Solve()
{

    Task("MPI4Type8");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Datatype nt;
    std::vector<MPI_Datatype> types = { MPI_INT, MPI_DOUBLE };
    std::vector<int> displs_type = { 0, 2 * sizeof(MPI_INT) };
    std::vector<int> blocklens = { 2, 1 };

    MPI_Type_create_struct(2, &blocklens[0], &displs_type[0], &types[0], &nt);
    MPI_Type_commit(&nt);

    std::vector<int> rcount(size, 0);
    std::vector<int> displs(size, 0);

    int sz = (size * (size - 1)) / 2;
    std::vector<newtype> buf(sz);
    std::vector<newtype> sbuf(size);

    for (int i = 1; i < size; ++i) {
        rcount[i] = i;
        displs[i] = displs[i - 1] + rcount[i - 1];
    }

    if (rank != 0) {
        for (int i = 0; i < rank; ++i) {
            int a, b;
            double c;
            pt >> a >> b >> c;
            sbuf[i] = { a, b, c };
        }
    }

    MPI_Gatherv(&sbuf[0], rank, nt, &buf[0], &rcount[0], &displs[0], nt, 0, MPI_COMM_WORLD);

    if (rank == 0)
        for (auto &i : buf)
            pt << i.a << i.b << i.c;

}
