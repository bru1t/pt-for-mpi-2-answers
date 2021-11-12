#include "pt4.h"
#include "mpi.h"

template<typename T>
void send(int t, int dest, MPI_Datatype d) {
	T z;
	pt >> z;
	MPI_Send(&z, 1, d, dest, t, MPI_COMM_WORLD);
}

template<typename T>
void recv(MPI_Datatype d, MPI_Status& status) {
	T z;
	MPI_Recv(&z, 1, d, 0, status.MPI_TAG, MPI_COMM_WORLD, &status);
	pt << z;
}

void Solve()
{
    Task("MPI2Send22");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
		    for (int i = 1; i < size; ++i) {
			    int T;
			    pt >> T;
			    if (T)
				    send<double>(T, i, MPI_DOUBLE);
			    else
				    send<int>(T, i, MPI_INT);
		}

	}
	else {

		MPI_Status status;
		MPI_Probe(0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

		if (status.MPI_TAG)
			recv<double>(MPI_DOUBLE, status);
		else
			recv<int>(MPI_INT, status);

	}
	
}
