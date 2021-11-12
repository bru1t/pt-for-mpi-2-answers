#include "mpi.h"
#include "pt4.h"

void Solve()
{
    Task("MPI5Comm6");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	// INSERT DATA
	
	int K, N;
	MPI_Comm comm;
	
	if (rank == 0) {
		pt >> K;
		N = 1;
	}
	else
		pt >> N;
		
	int thread_check = N;
	MPI_Comm_split(MPI_COMM_WORLD, thread_check, rank, &comm);

	// MAIN
	
	if (N == 1) {
		
		float* numbers = new float[1];
		int* counts = new int[1];
		int* disp = new int[1];
		float end_num;

		if (rank == 0) {
			
			numbers = new float[K + 1];
			
			for (int i = 1; i < K + 1; ++i)
				pt >> numbers[i];
				
			counts = new int[K + 1];
			std::fill(counts, counts + K + 1, 1);
			counts[0] = 0;
			disp = new int[K + 1];
			
			for (int i = 0; i < K + 1; ++i)
				disp[i] = i * 1;
				
			disp[0] = 0;
			
		}

		MPI_Scatterv(numbers, counts, disp, MPI_FLOAT, &end_num, 1, MPI_FLOAT, 0, comm);
		
		if (rank != 0)
			pt << end_num;
			
	}

}
