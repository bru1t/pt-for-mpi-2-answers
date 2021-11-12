#include "mpi.h"
#include "pt4.h"

void Solve()
{
	
    Task("MPI5Comm12");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	// INSERT DATA
	const int COMM_SIZE = 2;
	int N;
	float a1, a2;
    MPI_Comm comm[COMM_SIZE];
    
    // MAIN
    
    pt >> N;
    pt >> a1;

	(N == 1) ? MPI_Comm_split(MPI_COMM_WORLD, 0, rank, &comm[0]) : MPI_Comm_split(MPI_COMM_WORLD, 1, rank, &comm[1]);

    MPI_Allreduce(&a1, &a2, 1, MPI_FLOAT, MPI_MIN, comm[0]);
    MPI_Allreduce(&a1, &a2, 1, MPI_FLOAT, MPI_MAX, comm[1]);

    pt << a2;

}
