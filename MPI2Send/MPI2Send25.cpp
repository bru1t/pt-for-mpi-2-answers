#include "pt4.h"
#include "mpi.h"

template<typename T1, typename T2> void sendrecv(MPI_Datatype d1, int cnt1, int rank2, MPI_Datatype d2, int cnt2) {
	T1* nums = new T1[cnt1];
	for (int i = 0; i < cnt1; ++i)
		pt >> nums[i];
	T2* recBuf = new T2[cnt2];
	MPI_Sendrecv(nums, cnt1, d1, rank2, 0, recBuf, cnt2, d2, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	for (int i = 0; i < cnt2; ++i)
		pt << recBuf[i];
}

void Solve()
{

	Task("MPI2Send25");
	int flag;
	MPI_Initialized(&flag);
	if (flag == 0)
		return;
	int rank, size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int N = size / 2;

	if (rank < N)
		sendrecv<int, double>(MPI_INT, rank + 1, N + rank, MPI_DOUBLE, 2 * N - (N + rank));
	else
		sendrecv<double, int>(MPI_DOUBLE, 2 * N - rank, rank - N, MPI_INT, rank - N + 1);
		
}
