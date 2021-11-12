#include "pt4.h"
#include "omp.h"

double nonparallel_f(double x, int n) {
	double res = 0.;
	for (int i = 1; i <= n; ++i) {
		double it = 0.;
		for (int j = 1; j <= i; ++j) {
			it += (j + pow(x + j, 0.2)) / (2 * i * j - 1);
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
		if (num == 0) {
			Show("\nnum_procs: ");
			Show(omp_get_num_procs());
			Show("\nnum_threads: ");
			Show(omp_get_num_threads());
		}
		double tim = omp_get_wtime();
		#pragma omp for nowait schedule (dynamic, 1)
		for (int i = 1; i <= n; ++i) {
			double it = 0.;
			for (int j = 1; j <= i; ++j) {
				it += (j + pow(x + j, 0.2)) / (2 * i * j - 1);
				++count;
			}
			res += 1 / it;
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
    Task("OMPBegin21");
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
