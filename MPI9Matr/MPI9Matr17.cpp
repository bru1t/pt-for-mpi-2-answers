#include "pt4.h"
#include "mpi.h"
#include <cmath>

int k;              // ���������� ���������
int r;              // ���� �������� ��������

int m, p, q;        // ������� �������� ������
int na, nb;         // ������� ����� ������

int *a_, *b_, *c_;  // ������� ��� �������� �������� ������ � ������� ��������
int *a, *b, *c;     // ������� ��� �������� ����� ������ � ������ ��������

MPI_Datatype MPI_BAND_B; // ��� ������ ��� ������ ������� B
MPI_Datatype MPI_BAND_C; // ��� ������ ��� ������ ������� C

void Solve()
{
    Task("MPI9Matr17");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    k = size;
    r = rank;

}
