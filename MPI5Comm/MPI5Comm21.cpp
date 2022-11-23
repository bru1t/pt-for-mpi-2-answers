#include "pt4.h"
#include "mpi.h"

void Solve()
{
    Task("MPI5Comm21");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   
    MPI_Comm dcomm;
    int dims[] = { 2, 2, size / 4 }, periods[] = { 0, 0, 0 };
    MPI_Cart_create(MPI_COMM_WORLD, 3, dims, periods, 0, &dcomm);

    MPI_Comm ncomm;
    int remain_dims[3] = { 0, 1, 0 };
    MPI_Cart_sub(dcomm, remain_dims, &ncomm);

    double x = 0.0;
    pt >> x;
    double res = 0.0;
    
    MPI_Reduce(&x, &res, 1, MPI_DOUBLE, MPI_PROD, 0, ncomm);

    int nrank;
    MPI_Comm_rank(ncomm, &nrank);
    
    if (nrank == 0)
    {
        pt << res;
    }

}
