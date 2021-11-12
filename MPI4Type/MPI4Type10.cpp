#include "pt4.h"
#include "mpi.h"
#include <vector>

void Solve()
{

    Task("MPI4Type10");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int one_m = 0;
	int temp;

	MPI_Pack_size(2, MPI_INT, MPI_COMM_WORLD, &temp);
	one_m += temp;
	MPI_Pack_size(1, MPI_DOUBLE, MPI_COMM_WORLD, &temp);
	one_m += temp;
	int buff_size = one_m * (size - 1);

	std::vector<char> buff (buff_size);
	std::vector<char> rbuf(one_m);
	std::vector<int> scount(size, one_m);

	scount[0] = 0;
	std::vector<int> displs(size, 0);

	for (int i = 2; i < size; ++i)
		displs[i] = displs[i - 1] + one_m;

	int position;

	if (rank == 0) {
		position = 0;

		for (int i = 0; i < size - 1; ++i) {

			int a, c;
			double b;
			pt >> a >> b >> c;
			MPI_Pack(&a, 1, MPI_INT, &buff[0], buff_size, &position, MPI_COMM_WORLD);
			MPI_Pack(&b, 1, MPI_DOUBLE, &buff[0], buff_size, &position, MPI_COMM_WORLD);
			MPI_Pack(&c, 1, MPI_INT, &buff[0], buff_size, &position, MPI_COMM_WORLD);

		}

	}

	MPI_Scatterv(&buff[0], &scount[0], &displs[0], MPI_PACKED, &rbuf[0], one_m, MPI_PACKED, 0, MPI_COMM_WORLD);

	if (rank != 0) {

		position = 0;
		int a, c;
		double b;
		MPI_Unpack(&rbuf[0], buff_size, &position, &a, 1, MPI_INT, MPI_COMM_WORLD);
		MPI_Unpack(&rbuf[0], buff_size, &position, &b, 1, MPI_DOUBLE, MPI_COMM_WORLD);
		MPI_Unpack(&rbuf[0], buff_size, &position, &c, 1, MPI_INT, MPI_COMM_WORLD);
		pt << a << b << c;

	}
  
}
