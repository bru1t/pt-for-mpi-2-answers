#include "mpi.h"
#include "pt4.h"
#include <vector>

void Solve()
{
    Task("MPI5Comm30");
	int flag;
	MPI_Initialized(&flag);
	if (flag == 0)
		return;
	int rank, size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	// INSERT DATA
	int temp,
		count,
		A,
		neighbor_count;
	std::vector<int> data(size), 
					 side,
					 answers;
	MPI_Comm comm;
	MPI_Status status;
	
	data[0] = (size - 1) / 3;
	
	for (int i = 1; i < size; i++)
		(i % 3 == 0) ? (data[i] = data[i - 1] + 3) : (data[i] = data[i - 1] + 2);

	for (int i = 3; i < size; i += 3)
		side.push_back(i);
		
	for (int i = 1; i < size; i++) {
		
		if (i % 3 == 0) {
			side.push_back(0);
			side.push_back(i - 2);
			side.push_back(i - 1);
		}
		else {
			if (i % 3 == 1) {
				side.push_back(i + 1);
				side.push_back(i + 2);
			}
			else {
				side.push_back(i - 1);
				side.push_back(i + 1);
			}
		}
		
	}

	// MAIN
	
	pt >> A;
	
	MPI_Graph_create(MPI_COMM_WORLD, size, &data[0], &side[0], 0, &comm);
	MPI_Graph_neighbors_count(comm, rank, &neighbor_count);
	std::vector<int> neighbors(neighbor_count);
	MPI_Graph_neighbors(comm, rank, neighbor_count, &neighbors[0]);
	
	for (int i = 0; i < neighbor_count; i++) {
		MPI_Sendrecv(&A, 1, MPI_INT, neighbors[i], 0, &temp, 1, MPI_INT, neighbors[i], 0, comm, &status);
		answers.push_back(temp);
	}
	
	for (auto elem : answers)
		pt << elem;

}
