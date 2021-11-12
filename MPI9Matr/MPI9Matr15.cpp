#include "pt4.h"
#include "mpi.h"
#include <cmath>

int k;              // количество процессов
int r;              // ранг текущего процесса

int m, p, q;        // размеры исходных матриц
int na, nb;         // размеры полос матриц

int *a_, *b_, *c_;  // массивы для хранения исходных матриц в главном процессе
int *a, *b, *c;     // массивы для хранения полос матриц в каждом процессе

MPI_Datatype MPI_BAND_B; // тип данных для полосы матрицы B
MPI_Datatype MPI_BAND_C; // тип данных для полосы матрицы C

void MatrInit() {
	pt >> na >> p >> nb;
	int sz_a = na * p,
		sz_b = nb * p,
		sz_c = na * k * nb;
	a = new int[sz_a];
	b = new int[sz_b];
	c = new int[sz_c];
	for (int i = 0; i < sz_a; ++i) {
		pt >> a[i];
	}
	for (int i = 0; i < sz_b; ++i) {
		pt >> b[i];
	}
	for (int i = 0; i < sz_c; ++i) {
		pt >> c[i];
	}
}

int LineCalc(int ci, int cj) {
	int res = 0;
	for (int i = 0; i < p; ++i) {
		res += a[ci * p + i] * b[cj + i * nb];
	}
	return res;
}

void Matr2Calc(int step) {
	int sz_a = na * p,
		sz_b = nb * p,
		te = na * nb,
		sz_c = te * k;
	int start_pos_r = na * nb * r;
	int h = r + step >= k ? -1 * na * nb * (k - step) : na * nb * step;
	for (int i = 0; i < na; ++i) {
		for (int j = 0; j < nb; ++j) {
			c[start_pos_r + h + i * nb + j] += LineCalc(i, j);
		}
	}
	MPI_Status st;
	MPI_Sendrecv_replace(a, sz_a, MPI_INT, r == 0 ? k - 1 : (r - 1) % k, 0, (r + 1) % k, 0, MPI_COMM_WORLD, &st);
}

void MatrPrint() {
	int sz_a = na * p,
		sz_c = na * k * nb;
	for (int i = 0; i < sz_c; ++i) {
		pt << c[i];
	}
	for (int i = 0; i < sz_a; ++i) {
		pt << a[i];
	}
}

void MatrClear() {
	delete[] a;
	delete[] b;
	delete[] c;
}

void Solve()
{
    Task("MPI9Matr15");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    k = size;
    r = rank;
	MatrInit();
	int l;
	pt >> l;
	for (int i = 0; i < l; ++i) {
		Matr2Calc(i);
	}
	MatrPrint();
	MatrClear();
}
