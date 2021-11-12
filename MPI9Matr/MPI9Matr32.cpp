#include "pt4.h"
#include "mpi.h"
#include <cmath>
#include <vector>

using namespace std;

int k;              // ���������� ���������
int r;              // ���� �������� ��������

int m, p, q;        // ������� �������� ������
int m0, p0, q0;     // ������� ������ ������
int k0;             // ������� ���������� �������, ������ sqrt(k)

int *a_, *b_, *c_;  // ������� ��� �������� �������� ������ � ������� ��������
int *a, *b, *c, *t; // ������� ��� �������� ������ ������ � ������ ��������

MPI_Datatype MPI_BLOCK_A; // ��� ������ ��� ����� ������� A
MPI_Datatype MPI_BLOCK_B; // ��� ������ ��� ����� ������� B
MPI_Datatype MPI_BLOCK_C; // ��� ������ ��� ����� ������� C

MPI_Comm MPI_COMM_GRID = MPI_COMM_NULL;
                // ������������, ��������� � ��������� ���������� ��������
MPI_Comm MPI_COMM_ROW = MPI_COMM_NULL;
                // ������������, ��������� �� �������� ��������� �������
MPI_Comm MPI_COMM_COL = MPI_COMM_NULL;
                // ������������, ��������� �� ��������� ��������� �������

void Matr4CreateTypeBlock(int m0, int p0, int p, MPI_Datatype& t) {
	MPI_Type_vector(m0, p0, p, MPI_INT, &t);
	MPI_Type_commit(&t);
}

void Solve() {
    Task("MPI9Matr32");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    k = size;
    r = rank;
    k0 = (int)floor(sqrt((double)k) + 0.1);

    pt >> m >> p;
    m0 = m / k0;
    p0 = p / k0;
    int a_sz = m0 * p0;
    a = new int[a_sz];
	if (rank == 0) {
		int tmp = m * p;
		a_ = new int[tmp];
		for (int i = 0; i < tmp; ++i)
			pt >> a_[i];
		Matr4CreateTypeBlock(m0, p0, p, MPI_BLOCK_A);
		tmp = 0;
		for (int i = 1; i < k; ++i) {
			MPI_Send(&a_[tmp + p0 * (i % k0)], 1, MPI_BLOCK_A, i, 0, MPI_COMM_WORLD);
			if ((i + 1) % k0 == 0)
				tmp += p * m0;
		}
		MPI_Sendrecv(a_, 1, MPI_BLOCK_A, 0, 0, a, a_sz, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		for (int i = 0; i < a_sz; ++i)
			pt << a[i];
	}
	else {
		MPI_Recv(a, a_sz, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		for (int i = 0; i < a_sz; ++i)
			pt << a[i];
	}
}
