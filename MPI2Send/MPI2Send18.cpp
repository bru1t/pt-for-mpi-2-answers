#include "mpi.h"
#include "pt4.h"

void Solve()
{

    Task("MPI2Send18");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int N, ARR_SIZE = 50;
    MPI_Status status;
    double arr1[ARR_SIZE], arr2[ARR_SIZE];
    memset(arr1, 0, sizeof(arr1));
    memset(arr2, 0, sizeof(arr2));

    pt >> N;

    if (rank % 2 == 0) {

    	for (int i = 0; i < N; i++)
    		pt >> arr1[i];

    	MPI_Sendrecv(arr1, ARR_SIZE, MPI_INT, rank + 1, 0, arr2, ARR_SIZE, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);

    	for (int i = 0; i < ARR_SIZE; i++)
    		if (arr2[i] > 0)
    			pt << arr2[i];

    }
    else {

		    for (int i = 0; i < N; i++)
			    pt >> arr1[i];

		    MPI_Sendrecv(arr1, ARR_SIZE, MPI_INT, rank - 1, 0, arr2, ARR_SIZE, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);

		    for (int i = 0; i < ARR_SIZE; i++)
    		    if (arr2[i] > 0)
    			    pt << arr2[i];

    }

}
