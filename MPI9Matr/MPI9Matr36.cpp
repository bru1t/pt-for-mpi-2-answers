#include "pt4.h"
#include "mpi.h"
#include <cmath>
#include <vector>

using namespace std;

int m0, p0, q0, k0;

int a_sz, b_sz, c_sz, t_sz;

vector<int> a, b, c, t;

void Matr4CreateCommRow(MPI_Comm& grid, MPI_Comm& row) {
    int dims[] = { k0, k0 }, periods[] = { 1, 1 };
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 0, &grid);

    int remain_dims[2] = { false, true };
    MPI_Cart_sub(grid, remain_dims, &row);
}

void Matr4Calc1() {
    MPI_Comm MPI_COMM_GRID, MPI_COMM_ROW;
    Matr4CreateCommRow(MPI_COMM_GRID, MPI_COMM_ROW);

    int grid_rank;
    MPI_Comm_rank(MPI_COMM_GRID, &grid_rank);

    int coord[2];
    MPI_Cart_coords(MPI_COMM_GRID, grid_rank, 2, coord);

    t = a;
    MPI_Bcast(&t[0], t_sz, MPI_INT, coord[0], MPI_COMM_ROW);

    for (int i = 0; i < m0; ++i) 
        for (int j = 0; j < q0; ++j) 
            for (int k = 0; k < p0; ++k) 
                c[i * q0 + j] += t[i * p0 + k] * b[k * q0 + j];
}

void Solve() {
    Task("MPI9Matr36");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    k0 = (int)floor(sqrt((double)size) + 0.1);

    pt >> m0 >> p0 >> q0;

    a_sz = m0 * p0;
    b_sz = p0 * q0;
    c_sz = m0 * q0;
    t_sz = a_sz;

    a.resize(a_sz);
    b.resize(b_sz);
    c.resize(c_sz);
    t.resize(t_sz);

    for (int i = 0; i < a_sz; ++i)
        pt >> a[i];
    for (int i = 0; i < b_sz; ++i)
        pt >> b[i];
    for (int i = 0; i < c_sz; ++i)
        pt >> c[i];
    for (int i = 0; i < t_sz; ++i)
        pt >> t[i];

    Matr4Calc1();

    for (auto tmp : t)
        pt << tmp;
    for (auto tmp : c)
        pt << tmp;
}