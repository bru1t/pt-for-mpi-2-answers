#include "pt4.h"
#include "omp.h"
#include <cmath>

void find_k(int n, int& k1, int& k2, int& k3) {
	
	int k = 0,
		c = 0,
		d = 0;
	
	for (int i = 1; i <= n; i++)
		k += i;
		
	int k0 = k / 4;
	
	for (int i = 1; i <= n; i++) {
		
		c += i;
		if (c > k0) {
			if (d == 0)
				k1 = i;
			else 
				if (d == 1)
					k2 = i;
			else {
				k3 = i;
				return;
			}
			
			++d;
			c = 0;
		}
		
	}
	
}

double f_parallel(double x, int n) {
	
	double res = 0;
	int k1, k2, k3;
	find_k(n, k1, k2, k3);
	
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
				
				for (int j = 1; j <= i; j++) {
					res_it += (j + pow((x + j), 0.25)) / (2 * i * j - 1);
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
			
			for (int i = k1 + 1; i <= k2; i++) {
				
				res_it = 0;
				
				for (int j = 1; j <= i; j++) {
					res_it += (j + pow((x + j), 0.25)) / (2 * i * j - 1);
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
		
		#pragma omp section
		{
			double time = omp_get_wtime(),
				   res_it = 0,
				   res2 = 0;
				   
			int count = 0;
			
			for (int i = k2 + 1; i <= k3; i++) {
				
				res_it = 0;
				
				for (int j = 1; j <= i; j++) {
					res_it += (j + pow((x + j), 0.25)) / (2 * i * j - 1);
					count++;
				}
				
				res2 += 1 / res_it;
				
			}
			
			res += res2;
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
				   t = 0,
				   res3 = 0;
				   
			int count = 0;
			
			for (int i = k3 + 1; i <= n; i++) {
				t = 0;
				for (int j = 1; j <= i; j++) {
					t += (j + pow((x + j), 0.25)) / (2 * i * j - 1);
					count++;
				}
				res += 1 / t;
			}
			
			res += res3;
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
		
		for (int j = 1; j <= i; j++)
			res_it += (j + pow((x + j), 0.25)) / (2 * i * j - 1);
			
		res += 1 / res_it;
		
	}
	
	return res;
	
}

void Solve()
{
    Task("OMPBegin13");

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
