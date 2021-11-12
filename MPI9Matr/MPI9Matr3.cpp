#include "pt4.h"
#include "mpi.h"
#include <cmath>

int k;              // количество процессов
int r;              // ранг текущего процесса

int m, p, q;        // размеры исходных матриц
int na, nb;         // размеры полос матриц

int *a_, *b_, *c_;  // массивы для хранения исходных матриц в главном процессе
int *a, *b, *c;     // массивы для хранения полос матриц в каждом процессе

void MatrInit() {
	pt >> na >> p >> nb >> q;
	int sz_a = na * p,
		sz_b = nb * q,
		sz_c = na * q;
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

int LineCalc(int ai, int bj) {
	int res = 0;
	int z = nb * (r + 1);
	if (z <= p) {
		for (int i = 0; i < nb; ++i) {
			res += a[p * ai + nb * r + i] * b[q * i + bj];
		}
	}
	else {
		int count = nb - (z - p);
		for (int i = 0; i < count; ++i) {
			res += a[p * ai + nb * r + i] * b[q * i + bj];
		}
	}
	return res;
}

void Matr1Calc() {
	int sz_a = na * p,
		sz_b = nb * q,
		sz_c = na * q;
	for (int i = 0; i < na; ++i) {
		for (int j = 0; j < q; ++j) {
			c[q * i + j] = LineCalc(i, j);
		}
	}
	MPI_Status st;
	MPI_Sendrecv_replace(b, sz_b, MPI_INT, r == 0 ? k - 1 : (r - 1) % k, 0, (r + 1) % k, 0, MPI_COMM_WORLD, &st);
}

void MatrPrint() {
	int sz_b = nb * q,
		sz_c = na * q;
	for (int i = 0; i < sz_c; ++i) {
		pt << c[i];
	}
	for (int i = 0; i < sz_b; ++i) {
		pt << b[i];
	}
}

void MatrClear() {
	delete[] a;
	delete[] b;
	delete[] c;
}

void Solve()
{
    Task("MPI9Matr3");
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
	Matr1Calc();
	MatrPrint();
	MatrClear();
}
