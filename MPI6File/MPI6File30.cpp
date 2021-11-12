#include "mpi.h"
#include "pt4.h"

void Solve()
{
    Task("MPI6File30");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0) return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int FILE_NAME_SIZE = 20;
    char file_name[FILE_NAME_SIZE];
    int N_size,
		I_index,
		J_index;
    MPI_File file;
    MPI_Datatype datatype;

    if (rank == 0)
        pt >> file_name >> N_size;

    MPI_Bcast(file_name, FILE_NAME_SIZE, MPI_CHAR, 0, MPI_COMM_WORLD);
    MPI_Bcast(&N_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
    int N_sq_size = N_size * N_size;

    pt >> I_index >> J_index;

    int* elem_arr = new int[N_sq_size];

    for (int i = 0; i < N_sq_size; i++)
        elem_arr[i] = rank;

    MPI_File_open(MPI_COMM_WORLD, file_name, MPI_MODE_CREATE | MPI_MODE_RDWR, MPI_INFO_NULL, &file);

    int K_index = (I_index - 1) * 3 + J_index - 1,
		sq_matrix_size = K_index / 3 * 3 * N_sq_size  + K_index % 3 * N_size;

    MPI_Type_vector(N_size, N_size, 3 * N_size, MPI_INT, &datatype);
    MPI_File_set_view(file, sq_matrix_size * sizeof(int), MPI_INT, datatype, (char*)"native", MPI_INFO_NULL);
    MPI_File_write_all(file, elem_arr, N_sq_size, MPI_INT, MPI_STATUS_IGNORE);

    MPI_File_close(&file);

}
