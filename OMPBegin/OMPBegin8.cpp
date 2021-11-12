#include "pt4.h"
#include "omp.h"

double nonparallel_f(double x, int n) {
	double res = 0.;
	for (int i = 1; i <= n; ++i) {
		double it = 0.;
		int d = 2 * n;
		for (int j = i; j <= d; ++j) {
			it += (j + sin(x + j)) / (2 * i * j - 1);
		}
		res += 1 / it;
	}
	return res;
}

double parallel_f(double x, int n) {
	double res = 0.;
	#pragma omp parallel num_threads(4) reduction(+:res)
	{
		int count = 0;
		int num = omp_get_thread_num();
		int c_proc = omp_get_num_procs();
		int c_th = omp_get_num_threads();
		if (num == 0) {
			Show("\nnum_proc:");
			Show(c_proc);
			Show("\nnum_threads");
			Show(c_th);
		}
		double tim = omp_get_wtime();
		for (int i = 1; i <= n; i+=8) {
			double it = 0.;
			int d = 2 * n;
			int t_f = i + num,
				t_s = i + 7 - num;
			for (int j = t_f; j <= d; ++j) {
				it += (j + sin(x + j)) / (2 * t_f * j - 1);
				++count;
			}
			res += 1 / it;
			it = 0.;
			for (int j = t_s; j <= d; ++j) {
				it += (j + sin(x + j)) / (2 * t_s * j - 1);
				++count;
			}
			res += 1 / it;
		}
		double en = omp_get_wtime() - tim;
		Show("\nthread_num:");
		Show(num);
		Show("Count:");
		Show(count);
		Show("Thread time:");
		Show(en);
	}
	return res;
}

void Solve()
{
    Task("OMPBegin8");
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
