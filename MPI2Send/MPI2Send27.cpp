#include "pt4.h"
#include "mpi.h"

void Solve()
{

    Task("MPI2Send27");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int n;
    pt >> n;
    if (n == -1) {

		    int num = 0;

		    for (int i = 0; i < size - 1; ++i) {
			    if (rank == num)
				    ++num;

			    double t;
			    MPI_Recv(&t, 1, MPI_DOUBLE, MPI_ANY_SOURCE, num, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
			    pt << t;
			    ++num;

		    }

    }
    else {

		    double a;
		    pt >> a;
		    int num = 0;

		    MPI_Request req;
		    MPI_Issend(&a, 1, MPI_DOUBLE, n, rank, MPI_COMM_WORLD, &req);
		    MPI_Status status;
		    int check = 0;

		    while (check == 0) {
			    MPI_Test(&req, &check, &status);
			    ++num;
		    }

		    Show(num);
        
    }

}
