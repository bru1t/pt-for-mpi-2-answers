#include "pt4.h"
#include "mpi.h"
#include <vector>

struct newtype {
	int a;
	int b;
	int c;
};

void Solve()
{

    Task("MPI4Type2");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Datatype nt;
    MPI_Type_contiguous(3, MPI_INT, &nt);
    MPI_Type_commit(&nt);
    std::vector<newtype> n(size);
    newtype t;

    if (rank == 0) {
		    for (int i = 1; i < size; ++i) {
			    int t1, t2, t3;
			    pt >> t1 >> t2 >> t3;
			    n[i] = { t1, t2, t3 };
		    }
    }
		
    MPI_Scatter(&n[0], 1, nt, &t, 1, nt, 0, MPI_COMM_WORLD);

    if (rank != 0)
        pt << t.a << t.b << t.c;

}
