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

void Matr2CreateTypeBand(int p, int n, int q, MPI_Datatype &t) {
	MPI_Type_vector(p, n, q, MPI_INT, &t);
	MPI_Type_commit(&t);
}

void CreateTempMatr(int *&ta, int *&tb) {
	int a_re = m * p,
		a_sz = na * p * k,
		b_re = p * q,
		b_q = nb * k,
		b_sz = p * b_q;
	ta = new int[a_sz];
	tb = new int[b_sz];
	Matr2CreateTypeBand(p, nb, b_q, MPI_BAND_B);
	if (r == 0) {
		for (int i = 0; i < a_sz; ++i) {
			if (i < a_re) {
				ta[i] = a_[i];
			}
			else {
				ta[i] = 0;
			}
		}
		int d = 0;
		for (int i = 0; i < b_sz; ++i) {
			if (i % b_q < q) {
				tb[i] = b_[d++];
			}
			else {
				tb[i] = 0;
			}
		}
	}
}

void Matr2ScatterData(int &asz, int &bsz, int &csz) {
	int temp[3];
	int *ta, *tb;
	if (r == 0) {
		na = (int)ceil((double)m / k);
		nb = (int)ceil((double)q / k);
		temp[0] = na;
		temp[1] = p;
		temp[2] = nb;
	}
	MPI_Bcast(temp, 3, MPI_INT, 0, MPI_COMM_WORLD);
	na = temp[0];
	p = temp[1];
	nb = temp[2];
	asz = na * p;
	bsz = p * nb;
	csz = na * k * nb;
	a = new int[asz];
	b = new int[bsz];
	c = new int[csz];
	for (int i = 0; i < csz; ++i) {
		c[i] = 0;
	}
	CreateTempMatr(ta, tb);
	if (r == 0) {
		int b_q = nb * k,
			b_sz = p * b_q;
		for (int i = 0; i < b_sz; ++i) {
			if (i != 0 && i % b_q == 0) {
				Show('\n');
			}
			Show(tb[i]);
		}
	}
	MPI_Scatter(ta, na * p, MPI_INT, a, na * p, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Status st;
	if (r == 0) {
		MPI_Sendrecv(tb, 1, MPI_BAND_B, 0, 0, b, bsz, MPI_INT, 0, 0, MPI_COMM_WORLD, &st);
		for (int i = 1; i < k; ++i) {
			MPI_Send(&tb[nb * i], 1, MPI_BAND_B, i, 0, MPI_COMM_WORLD);
		}
	}
	else {
		MPI_Recv(b, bsz, MPI_INT, 0, 0, MPI_COMM_WORLD, &st);
	}
	delete[] ta;
	delete[] tb;
}

void PrintResult(int asz, int bsz, int csz) {
	pt << na << p << nb;
	for (int i = 0; i < asz; ++i) {
		pt << a[i];
	}
	for (int i = 0; i < bsz; ++i) {
		pt << b[i];
	}
	for (int i = 0; i < csz; ++i) {
		pt << c[i];
	}
}

void MatrInit() {
	if (r == 0) {
		pt >> m >> p >> q;
		int a_sz = m * p,
			b_sz = p * q;
		a_ = new int[a_sz];
		b_ = new int[b_sz];
		for (int i = 0; i < a_sz; ++i) {
			pt >> a_[i];
		}
		for (int i = 0; i < b_sz; ++i) {
			pt >> b_[i];
		}
	}
}

void MatrClear() {
	delete[] a;
	delete[] b;
	delete[] c;
	if (r == 0) {
		delete[] a_;
		delete[] b_;
	}
}

void Solve()
{
    Task("MPI9Matr12");
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
	int asz, bsz, csz;
	Matr2ScatterData(asz, bsz, csz);
	PrintResult(asz, bsz, csz);
	MatrClear();
}
