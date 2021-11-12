#include "mpi.h"
#include "pt4.h"
#include <vector>

void Solve()
{

    Task("MPI4Type22");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int updated_size = (size - 1) * (size - 1), d_size;
	std::vector<float> result_vector(updated_size), main_v(size - 1, 0);
	std::vector<int> disp(size, 0), count(size, size - 1), rdisp(size, 0), rcount(size, size - 1);

	MPI_Datatype datatype;
	MPI_Type_size(MPI_FLOAT, &d_size);
	MPI_Type_create_resized(MPI_FLOAT, 0, (size - 1) * d_size, &datatype);
	MPI_Type_commit(&datatype);
	std::vector<MPI_Datatype> types(size, MPI_FLOAT), rtypes(size, datatype);
  
	if (rank != 0)
		for (int i = 0; i < size - 1; i++)
			pt >> main_v[i];

	for (int i = 2; i < size; i++)
		rdisp[i] = (i - 1) * d_size;

	MPI_Alltoallw(&main_v[0], &count[0], &disp[0], &types[0], &result_vector[0], &rcount[0], &rdisp[0], &rtypes[0], MPI_COMM_WORLD);

	if (rank == 0)
		for (auto &elem : result_vector)
			pt << elem;

}
