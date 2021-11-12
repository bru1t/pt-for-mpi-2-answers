#include "mpi.h"
#include "pt4.h"

void Solve()
{
  
    Task("MPI6File1");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int FILE_NAME_SIZE = 20, LOCAL_INDEXES_AMOUNT = 20;
    int elem_amount, local_indexes[LOCAL_INDEXES_AMOUNT], buffer_arr[LOCAL_INDEXES_AMOUNT];
    char file_name[FILE_NAME_SIZE];

    if (rank == 0)
    	pt >> file_name;
    else {

    	pt >> elem_amount;

    	for (int i = 0; i < elem_amount; i++)
    		pt >> local_indexes[i];

	}

	MPI_Bcast(file_name, FILE_NAME_SIZE, MPI_CHAR, 0, MPI_COMM_WORLD);
    MPI_File file;
    MPI_File_open(MPI_COMM_WORLD, file_name, MPI_MODE_CREATE | MPI_MODE_RDWR, MPI_INFO_NULL, &file);

	if (rank != 0)
		for (int i = 0; i < elem_amount; i++) {

			long long int file_size;
			MPI_File_read_at(file, (local_indexes[i] - 1) * sizeof(int), &buffer_arr[i], 1, MPI_INT, MPI_STATUS_IGNORE);
			MPI_File_get_size(file, &file_size);

			if ((local_indexes[i] - 1) * sizeof(int) < file_size)
				pt << buffer_arr[i];

		}

	MPI_File_close(&file);

}
