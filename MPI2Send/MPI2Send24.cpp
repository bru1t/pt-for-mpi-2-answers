#include "pt4.h"
#include "mpi.h"
#include <vector>

template<typename T>
void sendrecv(int rank, int size, MPI_Datatype datatype, int step) {

	MPI_Status status;
	int next = (rank + step) % size,
		N = size / 2;
	std::vector<T> vector(N);

	if (next < 0)
		next += size;

	for (int i = 0; i < N; ++i)
		pt >> vector[i];

	MPI_Sendrecv_replace(&vector[0], N, datatype, next, 0, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);

	for (auto &index : vector)
		pt << index;

}

void Solve()
{

    Task("MPI2Send24");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    (rank % 2 != 0) ? sendrecv<int>(rank, size, MPI_INT, -2) : sendrecv<float>(rank, size, MPI_FLOAT, 2);

}
