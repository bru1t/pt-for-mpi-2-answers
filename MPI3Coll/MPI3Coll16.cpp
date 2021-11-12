#include "mpi.h"
#include "pt4.h"

void Solve()
{

	Task("MPI3Coll16");
	int flag;
	MPI_Initialized(&flag);
	if (flag == 0)
		return;
	int rank, size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int arr_size = size * (size + 1) / 2,
		s1 = 0,
		s2 = 0;
	std::vector<int> arr(arr_size);
	std::vector<int> count(size);
	std::vector<int> disp(size);
	std::vector<int> rcount(size, rank + 1);
	std::vector<int> rdisp(size);

	for (int i = 0; i < arr_size; i++)
		pt >> arr[i];

	for (int i = 0; i < size; i++) {
		count[i] = i + 1;
		disp[i] = s1;
		rcount[i] = size * (i + 1);
		rdisp[i] = s2;
		s1 += i + 1;
		s2 += rank + 1;
	}

	int arr_size_re = size * (rank + 1);
	std::vector<int> re(arr_size_re);

	MPI_Alltoallv(&arr[0], &count[0], &disp[0], MPI_INT, &re[0], &rcount[0], &rdisp[0], MPI_INT, MPI_COMM_WORLD);

	for (auto& i : re)
		pt << i;
		
}
