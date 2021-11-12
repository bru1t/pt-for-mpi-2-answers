#include "pt4.h"
#include "mpi.h"
#include <cmath>

int k;              // количество процессов
int r;              // ранг текущего процесса

int m, p, q;        // размеры исходных матриц

int *a_, *b_, *c_;  // массивы для хранения исходных матриц в главном процессе

int calculate(int i, int j) {
	int res = 0;
	for (int h = 0; h < p; ++h) {
		res += a_[i * p + h] * b_[h * q + j];
	}
	return res;
}

void Solve()
{
    Task("MPI9Matr1");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    k = size;
    r = rank;
	if (rank == 0) {
		pt >> m >> p >> q;
		int a_size = m * p;
		int b_size = p * q;
		int c_size = m * q;
		a_ = new int[a_size];
		b_ = new int[b_size];
		c_ = new int[c_size];
		for (int i = 0; i < a_size; ++i) {
			pt >> a_[i];
		}
		for (int i = 0; i < b_size; ++i) {
			pt >> b_[i];
		}
		for (int i = 0; i < m; ++i) {
			for (int j = 0; j < q; ++j) {
				c_[i * q + j] = calculate(i, j);
			}
		}
		for (int i = 0; i < c_size; ++i) {
			pt << c_[i];
		}
		delete[] a_;
		delete[] b_;
		delete[] c_;
	}
}
