#include "pt4.h"
#include "mpi.h"
#include <vector>

struct newtype {
	double a;
	int b;
	int c;
};

void Solve()
{

    Task("MPI4Type6");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Datatype nt;
    std::vector<MPI_Datatype> types = { MPI_DOUBLE, MPI_INT };
    std::vector<int> displs = { 0, sizeof(MPI_DOUBLE) };
    std::vector<int> blocklens = {1, 2};
    MPI_Type_create_struct(2, &blocklens[0], &displs[0], &types[0], &nt);
    MPI_Type_commit(&nt);
    newtype n{ 0.0, 0, 0 };
    std::vector<newtype> t(size);
		
    if (rank != 0) {
		    double t1;
		    int t2, t3;
		    pt >> t1 >> t2 >> t3;
		    n = { t1, t2, t3 };
    }

    MPI_Gather(&n, 1, nt, &t[0], 1, nt, 0, MPI_COMM_WORLD);

    if (rank == 0)
        for (int i = 1; i < size; ++i)
            pt << t[i].a << t[i].b << t[i].c;

}
