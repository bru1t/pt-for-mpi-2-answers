#include "pt4.h"
#include "omp.h"

double nonparallel_f(double x, int n) {
	double res = 0.;
	for (int i = 1; i <= n; ++i) {
		double it = 0.;
		int d = i + n;
		for (int j = 1; j <= d; ++j) {
			it += (j + log(1 + x + j)) / (2 * i * j - 1);
		}
		res += 1 / it;
	}
	return res;
}

double parallel_f(double x, int n) {
	double res = 0.;
	int count = 0;
	double tim = omp_get_wtime();
	int k = n * (-2  + sqrt(10)) / 2;
	#pragma omp parallel sections reduction(+:res)
	{
		#pragma omp section
		{
			Show("\nnum_proc:");
			Show(omp_get_num_procs());
			Show("\nnum_threads");
			Show(omp_get_num_threads());
			double tim = omp_get_wtime();
			for (int i = 1; i <= k; ++i) {
				double it = 0.;
				int d = i + n;
				for (int j = 1; j <= d; ++j) {
					it += (j + log(1 + x + j)) / (2 * i * j - 1);
					++count;
				}
				res += 1 / it;
			}
			double en = omp_get_wtime() - tim;
			Show("\nthread_num:");
			Show(omp_get_thread_num());
			Show("Count:");
			Show(count);
			Show("Thread time:");
			Show(en);
		}
		#pragma omp section
		{
			double tim = omp_get_wtime();
			for (int i = k + 1; i <= n; ++i) {
				double it = 0.;
				int d = i + n;
				for (int j = 1; j <= d; ++j) {
					it += (j + log(1 + x + j)) / (2 * i * j - 1);
					++count;
				}
				res += 1 / it;
			}
			double en = omp_get_wtime() - tim;
			Show("\nthread_num:");
			Show(omp_get_thread_num());
			Show("Count:");
			Show(count);
			Show("Thread time:");
			Show(en);
		}
	}
	return res;
}

void Solve()
{
    Task("OMPBegin10");
	double x;
	int n;
	pt >> x >> n;
	double t, np, pl;
	t = omp_get_wtime();
	double res = nonparallel_f(x, n);
	np = omp_get_wtime() - t;
	Show("Non-parallel time:");
	Show(np);
	pt << res;
	pt >> x >> n;
	t = omp_get_wtime();
	res = parallel_f(x, n);
	pl = omp_get_wtime() - t;
	Show("\nTotal parallel time:");
	Show(pl);
	Show("\nRate");
	Show(np / pl);
	pt << res;
}
