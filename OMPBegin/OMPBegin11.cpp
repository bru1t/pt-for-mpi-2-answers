#include "pt4.h"
#include "omp.h"
#include <cmath>

void find_k(int n, int& k1) {
	
	int k = 0,
		c = 0,
		d = 0;
	
	for (int i = 1; i <= n; i++)
		k += i;
		
	int k0 = k / 4;
	
	for (int i = 1; i <= n; i++) {
		
		c += i;
		if (c > k0) {
			k1 = i;
			++d;
			c = 0;	
		}
		
	}
	
}

double f_parallel(double x, int n) {
	
	double res = 0;
	int k1;
	find_k(n, k1);
	
	#pragma omp parallel sections reduction(+:res)
	{
		#pragma omp section
		{
			Show("\nnum_proc:");
			Show(omp_get_num_procs());
			Show("\nnum_threads");
			Show(omp_get_num_threads());

			double time = omp_get_wtime(),
				   res0 = 0,
				   res_it;
			int count = 0;
			
			for (int i = 1; i <= k1; ++i) {
				
				res_it = 0;
				
				for (int j = i; j <= n; j++) {
					res_it += (j + log(1 + x + j)) / (2 * i * j - 1);
					count++;
				}
				
				res0 += 1 / res_it;
			}
			
			res += res0;
			Show("\nthread_num:");
			Show(omp_get_thread_num());
			Show("Count:");
			Show(count);
			Show("Thread time:");
			Show(omp_get_wtime() - time);
			
		}

		#pragma omp section
		{
			
			double time = omp_get_wtime(),
				   res_it = 0,
				   res1 = 0;
				   
			int count = 0;
			
			for (int i = k1 + 1; i <= n; i++) {
				
				res_it = 0;
				
				for (int j = i; j <= n; j++) {
					res_it += (j + log(1 + x + j)) / (2 * i * j - 1);
					count++;
				}
				
				res += 1 / res_it;
				
			}
			
			res += res1;
			Show("\nthread_num:");
			Show(omp_get_thread_num());
			Show("Count:");
			Show(count);
			Show("Thread time:");
			Show(omp_get_wtime() - time);
			
		}

	}
	return res;
	
}

double f_nonparallel(double x, int n) {
	
	double res = 0;
	
	for (int i = 1; i <= n; i++) {
		
		double res_it = 0;
		
		for (int j = i; j <= n; j++)
			res_it += (j + log(1 + x + j)) / (2 * i * j - 1);
			
		res += 1 / res_it;
		
	}
	
	return res;
	
}

void Solve()
{
    Task("OMPBegin11");
    
    double X;
	int N;
	pt >> X >> N;
	
	double start_time = omp_get_wtime();
	double res = f_nonparallel(X, N);
	Show("Non-parallel time:");
	
	double np_time = omp_get_wtime() - start_time;
	Show(np_time);
	
	pt << res;
	pt >> X >> N;
	
	start_time = omp_get_wtime();
	res = f_parallel(X, N);
	double p_time = omp_get_wtime() - start_time;
	Show("\nTotal parallel time:");
	Show(p_time);
	
	pt << res;
	Show("\nRate:");
	Show(np_time / p_time);

}
