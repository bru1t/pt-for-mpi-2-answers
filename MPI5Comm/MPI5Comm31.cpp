#include "mpi.h"
#include "pt4.h"

void Solve()
{
    Task("MPI5Comm31");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    // INSERT DATA
    int a, 
		index = 0,
		size1 = 2 * size / 3,
		size_d = 0,
		cur_v = 0,
		help = 0;
    MPI_Comm comm;
	MPI_Status status;

	// MAIN
	
	pt >> a;
	int res = a;
	
	std::vector<int> sources(size1), degrees(size1);
		
	for (int i = 0; i < size; ++i) {
		
		if (i % 3 == 2)
			continue;
			
		sources[index] = i;
		index++;
		
	}
	
	for (int i = 0; i < size1; ++i) {
		
		if (sources[i] % 3 == 1) {
			degrees[i] = 1;
			size_d++;
		}
		else {
			degrees[i] = 3;
			size_d += 3;
		}
		
	}
	
	std::vector<int> destinations(size_d);
	
	for (int i = 0; i < size_d; ++i) {
		if (help < 3) {
			destinations[i] = (sources[cur_v] + help + 1) % size;
			help++;
		}
		else {
			help = 0;
			cur_v++;
			destinations[i] = sources[cur_v] + 1;
			cur_v++;
		}
	}
	
	int in, out, j;
	int wow = (rank == 0) ? size1 : 0;
	MPI_Dist_graph_create(MPI_COMM_WORLD, wow, &sources[0], &degrees[0], &destinations[0], MPI_UNWEIGHTED, MPI_INFO_NULL, 0, &comm);
	MPI_Dist_graph_neighbors_count(comm, &in, &out, &j);
	
	int out_s = (out == 0) ? 1 : out;
	std::vector<int> in_n(in), in_w(in);
	std::vector<int> out_n(out_s), out_w(out_s);
	
	MPI_Dist_graph_neighbors(comm, in, &in_n[0], &in_w[0], out, &out_n[0], &out_w[0]);
	
	if (out != 0) {
		for (auto &i : out_n) {
			MPI_Send(&a, 1, MPI_INT, i, 0, comm);
		}
	}
	
	for (auto &i : in_n) {
		int temp;
		MPI_Recv(&temp, 1, MPI_INT, i, 0, comm, &status);
		res += temp;
	}
	
	pt << res;

}
