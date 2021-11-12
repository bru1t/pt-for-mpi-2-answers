#include "pt4.h"
#include "mpi.h"
#include <cmath>

int k;              // ���������� ���������
int r;              // ���� �������� ��������

int m, p, q;        // ������� �������� ������
int m0, p0, q0;     // ������� ������ ������
int k0;             // ������� ���������� �������, ������ sqrt(k)

int *a_, *b_, *c_;  // ������� ��� �������� �������� ������ � ������� ��������
int *a, *b, *c;     // ������� ��� �������� ������ ������ � ������ ��������

MPI_Datatype MPI_BLOCK_A; // ��� ������ ��� ����� ������� A
MPI_Datatype MPI_BLOCK_B; // ��� ������ ��� ����� ������� B
MPI_Datatype MPI_BLOCK_C; // ��� ������ ��� ����� ������� C

MPI_Comm MPI_COMM_GRID = MPI_COMM_NULL;
                // ������������, ��������� � ��������� ���������� ��������

void Matr3CreateCommGrid(MPI_Comm& comm) { 
    int dims[] = { k0, k0 }, periods[] = { 1, 1 }; 
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 0, &comm); 
}

void Solve() {
    Task("MPI9Matr22");
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
    int a_sz = m0 * p0;
    a = new int[a_sz];
    for (int i = 0; i < a_sz; ++i)
        pt >> a[i];

    Matr3CreateCommGrid(MPI_COMM_GRID);
    int coords[2];
    MPI_Cart_coords(MPI_COMM_GRID, rank, 2, coords);
    pt << coords[0] << coords[1];
    int I = coords[0], J = coords[1];


    enum DIRECTIONS { LEFT, RIGHT };
    int neighbours_ranks[2];
    MPI_Cart_shift(MPI_COMM_GRID, 1, I, &neighbours_ranks[LEFT], &neighbours_ranks[RIGHT]);
    
    //Show(neighbours_ranks[LEFT]);
    //Show(neighbours_ranks[RIGHT]);

    MPI_Sendrecv_replace(a, a_sz, MPI_INT,
        neighbours_ranks[LEFT], 0, neighbours_ranks[RIGHT], 0,
        MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    for (int i = 0; i < a_sz; ++i)
        pt << a[i];
}