#include "mpi.h"
#include "pt4.h"
#include <iostream>
#include <cmath>

void Solve()
{
    Task("MPI5Comm1");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
    
	
	
	// INSERT DATA
	MPI_Status status;
	MPI_Group group;
	MPI_Comm_group(MPI_COMM_WORLD, &group);
	
	// MAIN
	int N;
	if (size % 2 == 0)
		N = size/2;
	else 
		N = std::floor(size/2) + 1;
		
	int *ranks = new int [N];
	int j = 0;
	
	for (int i = 0 ; i <= size ; i +=2){
		ranks[j] = i;
		j++;
	}
	
	const int NUMS_AMOUNT = 10;
	int add_rank = -1, add_size = -1;
	MPI_Group add_group;
	MPI_Comm add_comm;
	MPI_Group_incl(group, N, ranks, &add_group);
	MPI_Comm_create(MPI_COMM_WORLD, add_group, &add_comm);
	
	if (MPI_COMM_NULL != add_comm) {
		MPI_Comm_rank(add_comm, &add_rank);
		MPI_Comm_size(add_comm, &add_size);
	}
	
	int nums[NUMS_AMOUNT];
	int num_in, num_out;
	
	if (rank == 0) {
		
		for (int i = 0; i < N; i++)
			pt >> nums[i];
			
		pt << nums[0];
		
		for (int i = 1; i <= N; i++) {
			num_out = nums[i];
			MPI_Send(&num_out, 1, MPI_INT, i, 0, add_comm);
		}
		
	}
	if (rank % 2 == 0 && rank != 0) {
		MPI_Recv(&num_in, 1, MPI_INT, 0, 0, add_comm, &status);
		pt << num_in;
	}

}
