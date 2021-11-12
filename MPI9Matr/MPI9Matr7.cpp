#include "pt4.h"
#include "mpi.h"
#include <cmath>

int k;              // количество процессов
int r;              // ранг текущего процесса

int m, p, q;        // размеры исходных матриц
int na, nb;         // размеры полос матриц

int *a_, *b_, *c_;  // массивы для хранения исходных матриц в главном процессе
int *a, *b, *c;     // массивы для хранения полос матриц в каждом процессе

//Tested
void MatrInit() {
	if (r == 0) {
		pt >> m >> p >> q;
		int sz_a_ = m * p,
			sz_b_ = p * q,
			sz_c_ = m * q;
		a_ = new int[sz_a_];
		b_ = new int[sz_b_];
		for (int i = 0; i < sz_a_; ++i) {
			pt >> a_[i];
		}
		for (int i = 0; i < sz_b_; ++i) {
			pt >> b_[i];
		}
		c_ = new int[sz_c_];
	}
}

//Tested
void Matr1ScatterData() {
	int temp[3];
	if (r == 0) {
		temp[0] = p;
		temp[1] = q;
		temp[2] = m;
	}
	MPI_Bcast(temp, 3, MPI_INT, 0, MPI_COMM_WORLD);
	p = temp[0];
	q = temp[1];
	m = temp[2];
	na = ceil((double)m / k);
	nb = ceil((double)p / k);
	int sz_a = na * p,
		sz_b = nb * q,
		sz_c = na * q;
	a = new int[sz_a];
	b = new int[sz_b];
	c = new int[sz_c];
	for (int i = 0; i < sz_a; ++i) {
		a[i] = 0;
	}
	for (int i = 0; i < sz_b; ++i) {
		b[i] = 0;
	}
	for (int i = 0; i < sz_c; ++i) {
		c[i] = 0;
	}
	int *rcount_a = new int[k];
	int *rcount_b = new int[k];
	int *displs_a = new int[k];
	int *displs_b = new int[k];
	for (int i = 0; i < k; ++i) {
		int dela = (i + 1) * na - m;
		int delb = (i + 1) * nb - p;
		rcount_a[i] = dela <= 0 ? sz_a : dela <= na ? (na - dela) * p : 0;
		rcount_b[i] = delb <= 0 ? sz_b : delb <= nb ? (nb - delb) * q : 0;
	}
	displs_a[0] = 0;
	displs_b[0] = 0;
	for (int i = 1; i < k; ++i) {
		displs_a[i] = displs_a[i - 1] + rcount_a[i - 1];
		displs_b[i] = displs_b[i - 1] + rcount_b[i - 1];
	}
	MPI_Scatterv(a_, rcount_a, displs_a, MPI_INT, a, rcount_a[r], MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatterv(b_, rcount_b, displs_b, MPI_INT, b, rcount_b[r], MPI_INT, 0, MPI_COMM_WORLD);
	delete[] rcount_a;
	delete[] rcount_b;
	delete[] displs_a;
	delete[] displs_b;
}

//Tested
int LineCalc(int ai, int bj, int step) {
	int res = 0;
	int st = (step + r) * nb;
	if (st >= nb * k) {
		st -= nb * k;
		st = (st / nb) * nb;
	}
	for (int i = 0; i < nb; ++i) {
		if (st + i >= p) {
			break;
		}
		res += a[p * ai + st + i] * b[q * i + bj];
	}
	return res;
}

//Tested
void Matr1Calc(int step) {
	int sz_a = na * p,
		sz_b = nb * q,
		sz_c = na * q;
	for (int i = 0; i < na; ++i) {
		for (int j = 0; j < q; ++j) {
			c[q * i + j] += LineCalc(i, j, step);
		}
	}
	if (step == (k - 1)) {
		return;
	}
	MPI_Status st;
	MPI_Sendrecv_replace(b, sz_b, MPI_INT, r == 0 ? k - 1 : r - 1, 0, (r + 1) % k, 0, MPI_COMM_WORLD, &st);
}

//Tested
void Matr1GatherData() {
	int sz_c = na * q;
	int *r_count = new int[k],
		*r_displs = new int[k];
	r_displs[0] = 0;
	for (int i = 0; i < k; ++i) {
		int delc = (i + 1) * na - m;
		r_count[i] = delc <= 0 ? sz_c : delc <= na ? (na - delc) * q : 0;
	}
	for (int i = 1; i < k; ++i) {
		r_displs[i] = r_displs[i - 1] + r_count[i - 1];
	}
	MPI_Gatherv(c, r_count[r], MPI_INT, c_, r_count, r_displs, MPI_INT, 0, MPI_COMM_WORLD);
}

//Tested
void MatrPrintC() {
	int sz_c = na * q;
	for (int i = 0; i < sz_c; ++i) {
		pt << c[i];
	}
}

//Tested
void MatrPrintC_() {
	if (r == 0) {
		int sz_c_ = m * q;
		for (int i = 0; i < sz_c_; ++i) {
			pt << c_[i];
		}
	}
}

//Tested
void MatrClear() {
	if (r == 0) {
		delete[] a_;
		delete[] b_;
		delete[] c_;
	}
	delete[] a;
	delete[] b;
	delete[] c;
}

void Solve()
{
    Task("MPI9Matr7");
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
	Matr1ScatterData();
	for (int i = 0; i < k; ++i) {
		Matr1Calc(i);
		MatrPrintC();
	}
	Matr1GatherData();
	MatrPrintC_();
	MatrClear();
}
