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
	if (r == 0) {
		pt >> m >> q;
	}
	pt >> na >> nb;
	int sz_c = na * k * nb;
	c = new int[sz_c];
	if (r == 0) {
		int sz_c_ = na * k * nb * k;
		c_ = new int[sz_c_];
	}
	for (int i = 0; i < sz_c; ++i) {
		pt >> c[i];
	}
}

void Matr2CreateTypeBand(int p, int n, int q, MPI_Datatype &t) {
	MPI_Type_vector(p, n, q, MPI_INT, &t);
	MPI_Type_commit(&t);
}

void MatrPrint() {
	if (r == 0) {
		for (int i = 0; i < m; ++i) {
			for (int j = 0; j < q; ++j) {
				pt << c_[i * (nb * k) + j];
			}
		}
	}
}

void Matr2GatherData() {
	Matr2CreateTypeBand(na * k, nb, nb * k, MPI_BAND_C);
	if (r != 0) {
		MPI_Send(c, na * nb * k, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}
	else {
		MPI_Status st;
		MPI_Sendrecv(c, na * nb * k, MPI_INT, 0, 0, c_, 1, MPI_BAND_C, 0, 0, MPI_COMM_WORLD, &st);
		for (int i = 1; i < k; ++i) {
			MPI_Recv(&c_[i * nb], 1, MPI_BAND_C, i, 0, MPI_COMM_WORLD, &st);
		}
	}
	MatrPrint();
}


void MatrClear() {
	delete[] c;
	if (r == 0) {
		delete[] c_;
	}
}

void Solve()
{
    Task("MPI9Matr16");
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
	Matr2GatherData();
	MatrClear();
}
