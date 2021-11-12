#include "pt4.h"
#include "mpi.h"
#include <cmath>

int k;              // количество процессов
int r;              // ранг текущего процесса

int m, p, q;        // размеры исходных матриц
int m0, p0, q0;     // размеры блоков матриц
int k0;             // порядок декартовой решетки, равный sqrt(k)

int *a_, *b_, *c_;  // массивы для хранения исходных матриц в главном процессе
int *a, *b, *c;     // массивы для хранения блоков матриц в каждом процессе

MPI_Datatype MPI_BLOCK_A; // тип данных для блока матрицы A
MPI_Datatype MPI_BLOCK_B; // тип данных для блока матрицы B
MPI_Datatype MPI_BLOCK_C; // тип данных для блока матрицы C

MPI_Comm MPI_COMM_GRID = MPI_COMM_NULL;
                // коммуникатор, связанный с двумерной декартовой решеткой

void Matr3CreateTypeBlock(int m0, int p0, int p, MPI_Datatype &t) {
	MPI_Type_vector(m0, p0, p, MPI_INT, &t);
	MPI_Type_commit(&t);
}


void Solve()
{
    Task("MPI9Matr21");
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
	int m0 = m / k0;
	int p0 = p / k0;
	int te_sz = m0 * p0;
	a = new int[te_sz];
	if (rank == 0) {
		int t = m * p;
		a_ = new int[t];
		for (int i = 0; i < t; ++i) {
			pt >> a_[i];
		}
		Matr3CreateTypeBlock(m0, p0, p, MPI_BLOCK_A);
		int level = 0;
		for (int i = 1; i < k; ++i) {
			MPI_Send(&a_[level + p0 * (i % k0)], 1, MPI_BLOCK_A, i, 0, MPI_COMM_WORLD);
			if ((i + 1) % k0 == 0) {
				level += p * m0;
			}
		}
		MPI_Status st;
		MPI_Sendrecv(a_, 1, MPI_BLOCK_A, 0, 0, a, te_sz, MPI_INT, 0, 0, MPI_COMM_WORLD, &st);
		for (int i = 0; i < te_sz; ++i) {
			pt << a[i];
		}
		delete[] a_;
	}
	else {
		MPI_Status st;
		MPI_Recv(a, te_sz, MPI_INT, 0, 0, MPI_COMM_WORLD, &st);
		for (int i = 0; i < te_sz; ++i) {
			pt << a[i];
		}
	}
	delete[] a;
}
