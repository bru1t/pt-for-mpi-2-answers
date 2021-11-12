#include "pt4.h"
#include "omp.h"
#include <cmath>

using namespace std;

double f_nonparallel(double x, int n) {
	
	double res = 0.;
	for (int i = 1; i <= n; i++) {
		
		double it_res = 0.;
		int d = i + n;
		
		for (int j = 1; j <= d; j++)
			it_res += (j + pow(x + j, 1. / 3)) / (2 * i * j - 1);
			
		res += 1 / it_res;
		
	}
	
	return res;
	
}

double f_parallel(double x, int n) {
	
	double res = 0;
	#pragma omp parallel num_threads(2) reduction(+:res)
	{
		
		int num = omp_get_thread_num();
		int num_th = omp_get_num_threads();
		int num_proc = omp_get_num_procs();
		int count = 0;
		
		if (num == 0) {
			Show("\nnum_procs: ");
			Show(num_proc);
			Show("\nnum_threads: ");
			Show(num_th);
		}
		
		double tim = omp_get_wtime();
		
		for (int i = num; i <= n; i+=2) {
			
			double it_res = 0.;
			int d = i + n;
			
			for (int j = 1; j <= d; j++) {
				it_res += (j + pow(x + j, 1. / 3)) / (2 * i * j - 1);
				count++;
			}
			
			res += 1 / it_res;
			
		}
		
		Show("\nthread_num:");
		Show(num);
		Show("Count:");
		Show(count);
		Show("Thread time:");
		Show(omp_get_wtime() - tim);
		
	}
	
	return res;
	
}

void Solve()
{
    Task("OMPBegin2");
	double x, np_t, par_t;
	int n;
	
	pt >> x >> n;
	double t = omp_get_wtime(), res = f_nonparallel(x, n);
	np_t = omp_get_wtime() - t;
	
	Show("Non-parallel time:");
	Show(np_t);
	
	pt << res;
	pt >> x >> n;
	
	t = omp_get_wtime();
	res = f_nonparallel(x, n);
	par_t = omp_get_wtime() - t;
	
	Show("\nTotal parallel time:");
	Show(par_t);
	Show("\nRate:");
	Show(np_t / par_t);
	
	pt << res;
	
}
