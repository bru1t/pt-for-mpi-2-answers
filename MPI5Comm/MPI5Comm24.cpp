#include "pt4.h"
#include "mpi.h"
void Solve()
{
    Task("MPI5Comm24");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	int m, n;
	if (rank == 0) {
		pt >> m >> n;
	}
	MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
	int board = m * n - 1;
	int x, y, res;
	MPI_Comm nc;
	int dims[] = { m, n },
		periods[] = { 0, 1 };
	MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 0, &nc);
	if (rank <= board) {
		pt >> x >> y;
		int coord[] = { x, y };
		MPI_Comm_set_errhandler(nc, MPI_ERRORS_RETURN);
		if (MPI_Cart_rank(nc, coord, &res)) {
			pt << -1;
		}
		else {
			pt << res;
		}
	}
}
