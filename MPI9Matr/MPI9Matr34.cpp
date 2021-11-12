#include "pt4.h"
#include "mpi.h"
#include <cmath>

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

void Matr4CreateCommGrid(MPI_Comm& comm) {
	int dims[2] = { k0, k0 };
	int periods[2] = { 0, 0 };
	MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 0, &comm);
}

void Matr4CreateCommCol(MPI_Comm grid, MPI_Comm& col) {
	int remain_dims[2] = { 1, 0 };
	MPI_Cart_sub(grid, remain_dims, &col);
}

void Solve()
{
    Task("MPI9Matr34");
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

	pt >> p0 >> q0;
	int b_size = p0 * q0;
	b_ = new int[b_size];
	for (int i = 0; i < b_size; ++i)
		pt >> b_[i];

	Matr4CreateCommGrid(MPI_COMM_GRID);
	Matr4CreateCommCol(MPI_COMM_GRID, MPI_COMM_COL);

	int grid_rank, col_rank;
	MPI_Comm_rank(MPI_COMM_GRID, &grid_rank);
	MPI_Comm_rank(MPI_COMM_COL, &col_rank);
	int coords[2];
	MPI_Cart_coords(MPI_COMM_GRID, grid_rank, 2, coords);
	pt << coords[0] << coords[1] << col_rank;

	int dest = col_rank == 0 ? (k0 - 1) : (col_rank - 1);
	int source = (col_rank + 1) % k0;
	MPI_Sendrecv_replace(b_, b_size, MPI_INT, dest, 0, source, 0, MPI_COMM_COL, MPI_STATUSES_IGNORE);

	for (int i = 0; i < b_size; ++i)
		pt << b_[i];
}
