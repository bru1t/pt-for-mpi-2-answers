#include "pt4.h"
#include "mpi.h"
#include <vector>

using namespace std;


void Solve() {
    Task("MPI5Comm19");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	MPI_Comm new_comm, sub_comm;
	int K = size / 4;
	int dims[] = { 2, 2, K }, periods[] = { 0, 0, 0 }, remain_dims[] = { 0, 0, 1 };
	MPI_Cart_create(MPI_COMM_WORLD, 3, dims, periods, 1, &new_comm);
	MPI_Cart_sub(new_comm, remain_dims, &sub_comm);
	
	
	vector<int> ans(K);
	int a;
	pt >> a;
	MPI_Gather(&a, 1, MPI_INT, &ans[0], 1, MPI_INT, 0, sub_comm);

	int sub_rank;
	MPI_Comm_rank(sub_comm, &sub_rank);
	
	if (sub_rank == 0) {
		for (auto i : ans)
			pt << i;
	}
}
