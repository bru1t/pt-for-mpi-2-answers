#include "mpi.h"
#include "pt4.h"

void Solve()
{

    Task("MPI1Proc10");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int n;
    pt >> n;

    if (rank == 0) {

    	if (size % 2 != 0) {

    		int elem, min, arr[n];

    		pt >> elem;
    		arr[0] = elem;
    		min = elem;

    		for (int i = 1; i < n; i++) {

      		pt >> elem;
          arr[i] = elem;

          if (elem < min)
            min = elem;

			}

			pt << min;

		}
		else {

			float elem, min, arr[n];

			pt >> elem;

      arr[0] = elem;
    	min = elem;

    	for (int i = 1; i < n; i++) {
				pt >> elem;
      	arr[i] = elem;

      	if (elem < min)
      		min = elem;

			}

			pt << min;

		}
	}
	else {

		if (rank % 2 != 0) {

    		float elem, max, arr[n];

    		pt >> elem;
    		arr[0] = elem;
    		max = elem;

    		for (int i = 1; i < n; i++) {
					pt >> elem;
					arr[i] = elem;

					if (elem > max)
						max = elem;

			}

			pt << max;

		}
		else {

			int elem, min, arr[n];

			pt >> elem;

    	arr[0] = elem;
    	min = elem;

    	for (int i = 1; i < n; i++) {
				pt >> elem;
				arr[i] = elem;

				if (elem < min)
					min = elem;

			}

			pt << min;

		}

	}

}
