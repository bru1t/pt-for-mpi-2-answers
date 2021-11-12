#include "pt4.h"
#include "mpi.h"

void Solve()
{

    Task("MPI4Type9");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int nsize = sizeof(int)* size + sizeof(double) * size;
    char* buff = new char[nsize];
    int position = 1;
    int* nums1 = new int[size];
    double* nums2 = new double[size];

    if (rank == 0) {
		    for (int i = 0; i < size; ++i)
            pt >> nums1[i];
        for (int i = 0; i < size; ++i)
            pt >> nums2[i];

        position = 0;
        MPI_Pack(nums1, size, MPI_INT, buff, nsize, &position, MPI_COMM_WORLD);
        MPI_Pack(nums2, size, MPI_DOUBLE, buff, nsize, &position, MPI_COMM_WORLD);
    }

    MPI_Bcast(buff, nsize, MPI_PACKED, 0, MPI_COMM_WORLD);

    if (rank != 0) {
		    position = 0;
		    MPI_Unpack(buff, nsize, &position, nums1, size, MPI_INT, MPI_COMM_WORLD);
		    MPI_Unpack(buff, nsize, &position, nums2, size, MPI_DOUBLE, MPI_COMM_WORLD);

		    for (int i = 0; i < size; ++i)
            pt << nums1[i];
        for (int i = 0; i < size; ++i)
            pt << nums2[i];
            
    }

}
