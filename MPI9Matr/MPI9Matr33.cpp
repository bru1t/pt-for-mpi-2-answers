#include "pt4.h"
#include "mpi.h"
#include <cmath>

int k;              // количество процессов
int r;              // ранг текущего процесса

int m, p, q;        // размеры исходных матриц
int m0, p0, q0;     // размеры блоков матриц
int k0;             // порядок декартовой решетки, равный sqrt(k)

int *a_, *b_, *c_;  // массивы для хранения исходных матриц в главном процессе
int *a, *b, *c, *t; // массивы для хранения блоков матриц в каждом процессе

MPI_Datatype MPI_BLOCK_A; // тип данных для блока матрицы A
MPI_Datatype MPI_BLOCK_B; // тип данных для блока матрицы B
MPI_Datatype MPI_BLOCK_C; // тип данных для блока матрицы C

MPI_Comm MPI_COMM_GRID = MPI_COMM_NULL;
                // коммуникатор, связанный с двумерной декартовой решеткой
MPI_Comm MPI_COMM_ROW = MPI_COMM_NULL;
                // коммуникатор, связанный со строками двумерной решетки
MPI_Comm MPI_COMM_COL = MPI_COMM_NULL;
                // коммуникатор, связанный со столбцами двумерной решетки

void Matr4CreateCommGrid(MPI_Comm &comm) {
	int k0 = (int)floor(sqrt((double)k) + 0.1);
	int dims[2] = { k0, k0 };
	int periods[2] = { 1, 1 };
	MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 0, &comm);
}

void Matr4CreateCommRow(MPI_Comm grid, MPI_Comm &row) {
	int remain_dims[2] = { 0, 1 };
	MPI_Cart_sub(grid, remain_dims, &row);
}

void Solve()
{
    Task("MPI9Matr33");
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
	pt >> m0 >> p0;
	int t_sz = m0 * p0;
	a = new int[t_sz];
	t = new int[t_sz];
	for (int i = 0; i < t_sz; ++i) {
		pt >> a[i];
	}
	Matr4CreateCommGrid(MPI_COMM_GRID);
	Matr4CreateCommRow(MPI_COMM_GRID, MPI_COMM_ROW);
	int r_rank, g_rank;
	MPI_Comm_rank(MPI_COMM_ROW, &r_rank);
	MPI_Comm_rank(MPI_COMM_GRID, &g_rank);
	int ij[2];
	MPI_Cart_coords(MPI_COMM_GRID, g_rank, 2, ij);
	pt << ij[0] << ij[1] << r_rank;
	int mag = rank / k0;
	if (mag == r_rank) {
		for (int i = 0; i < t_sz; ++i) {
			t[i] = a[i];
		}
	}
	MPI_Bcast(t, t_sz, MPI_INT, mag, MPI_COMM_ROW);
	for (int i = 0; i < t_sz; ++i) {
		pt << t[i];
	}
	delete[] a;
	delete[] b;
}
