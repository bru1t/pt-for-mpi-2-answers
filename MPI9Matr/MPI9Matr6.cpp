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
	if (r == 0) {
		pt >> m;
	}
	MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);
	pt >> na >> q;
	int sz_c = na * q;
	c = new int[sz_c];
	for (int i = 0; i < sz_c; ++i) {
		pt >> c[i];
	}
	if (r == 0) {
		int sz_c_ = m * q;
		c_ = new int[sz_c_];
	}
}

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

void MatrClear() {
	if (r == 0) {
		delete[] c_;
	}
	delete[] c;
}

//Ignore MPI9Matr6 condition
//Solve only for test part of MPI9Matr7
void Solve()
{
    Task("MPI9Matr6");
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
	Matr1GatherData();
	if (r == 0) {
		int sz_c_ = m * q;
		for (int i = 0; i < sz_c_; ++i) {
			pt << c_[i];
		}
	}
	MatrClear();
}
