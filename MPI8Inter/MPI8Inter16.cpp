#include "mpi.h"
#include "pt4.h"

void Solve()
{

    Task("MPI8Inter16");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int K = size,
		fix_rank;
    float arr_max_elem = 0;
    std::vector<float> main_array(K);
    MPI_Status status;
    MPI_Comm comm;
    MPI_Comm_get_parent(&comm);

    for (int i = 0; i < K; i++)
        pt >> main_array[i];
        
	if (comm == MPI_COMM_NULL) {
		fix_rank = rank;
		MPI_Comm_spawn("ptprj.exe", NULL, K, MPI_INFO_NULL, 0, MPI_COMM_WORLD, &comm, MPI_ERRCODES_IGNORE);
	}
	else
		fix_rank = MPI_ROOT;

    MPI_Reduce_scatter_block(&main_array[0], &arr_max_elem, 1, MPI_FLOAT, MPI_MAX, comm);

    if (fix_rank == MPI_ROOT) {
        Show(arr_max_elem);
        MPI_Send(&arr_max_elem, 1, MPI_FLOAT, rank, 0, comm);
    }
    else {
        MPI_Recv(&arr_max_elem, 1, MPI_FLOAT, rank, 0, comm, &status);
        pt << arr_max_elem;
    }

}
