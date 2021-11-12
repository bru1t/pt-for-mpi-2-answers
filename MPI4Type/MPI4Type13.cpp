#include "mpi.h"
#include "pt4.h"

void Solve()
{
  
    Task("MPI4Type13");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int arr_size = sizeof(float) + sizeof(int) * (size - 1), pos;
	float elem;
	char* buff = new char[arr_size];
	int* numbers = new int[size - 1];
	std::fill(numbers, numbers + size - 1, 0);

	if (rank != 0) {

		pt >> elem;

		for (int i = 0; i < rank; i++)
			pt >> numbers[i];

		pos = 0;
		MPI_Pack(&elem, 1, MPI_FLOAT, buff, arr_size, &pos, MPI_COMM_WORLD);
		MPI_Pack(numbers, size - 1, MPI_INT, buff, arr_size, &pos, MPI_COMM_WORLD);

	}

	int all_elem_size = arr_size * (size - 1);
	char* rec = new char[all_elem_size];
	MPI_Gather(buff, arr_size, MPI_PACKED, rec, arr_size, MPI_PACKED, 0, MPI_COMM_WORLD);

	if (rank == 0) {

		pos = arr_size;
		for (int i = 1; i < size; i++) {

			MPI_Unpack(rec, arr_size, &pos, &elem, 1, MPI_FLOAT, MPI_COMM_WORLD);
			MPI_Unpack(rec, arr_size, &pos, numbers, size - 1, MPI_INT, MPI_COMM_WORLD);

			pt << elem;
			for (int j = 0; j < i; j++)
				pt << numbers[j];
		}

	}

}
