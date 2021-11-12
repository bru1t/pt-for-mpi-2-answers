#include "pt4.h"
#include "omp.h"

double nonparallel_f(double x, int n) {
	double res = 0.;
	int d = 2 * n;
	for (int i = 1; i <= n; ++i) {
		double it = 0.;
		for (int j = i; j <= d; ++j) {
			it += (j + pow(x + j, 0.25)) / (2 * i * j - 1);
		}
		res += 1 / it;
	}
	return res;
}

void help_f(int n, int &k0, int &k1, int &k2) {
	int k = 3 * n * n / 2;
	int d = 2 * n;
	int k_t = k / 4;
	int c = 0;
	int i = 1;
	while (c <= k_t) {
		c += d - i + 1;
		++i;
	}
	k0 = i;
	c = 0;
	while (c <= k_t) {
		c += d - i + 1;
		++i;
	}
	k1 = i;
	c = 0;
	while (c <= k_t) {
		c += d - i + 1;
		++i;
	}
	k2 = i;
}

double parallel_f(double x, int n) {
	double res = 0.;
	int count = 0;
	double tim = omp_get_wtime();
	int k0, k1, k2;
	help_f(n, k0, k1, k2);
	int d = 2 * n;
	#pragma omp parallel sections num_threads(4) reduction(+:res)
	{
		#pragma omp section
		{
			Show("\nnum_proc:");
			Show(omp_get_num_procs());
			Show("\nnum_threads");
			Show(omp_get_num_threads());
			double tim = omp_get_wtime();
			double res0 = 0.;
			for (int i = 1; i <= k0; ++i) {
				double it = 0.;
				for (int j = i; j <= d; ++j) {
					it += (j + pow(x + j, 0.25)) / (2 * i * j - 1);
					++count;
				}
				res0 += 1 / it;
			}
			res += res0;
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
			double res1 = 0.;
			double tim = omp_get_wtime();
			for (int i = k0 + 1; i <= k1; ++i) {
				double it = 0.;
				for (int j = i; j <= d; ++j) {
					it += (j + pow(x + j, 0.25)) / (2 * i * j - 1);
					++count;
				}
				res1 += 1 / it;
			}
			res += res1;
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
			double res2 = 0.;
			double tim = omp_get_wtime();
			for (int i = k1 + 1; i <= k2; ++i) {
				double it = 0.;
				for (int j = i; j <= d; ++j) {
					it += (j + pow(x + j, 0.25)) / (2 * i * j - 1);
					++count;
				}
				res2 += 1 / it;
			}
			res += res2;
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
			double res3 = 0.;
			double tim = omp_get_wtime();
			for (int i = k2 + 1; i <= n; ++i) {
				double it = 0.;
				for (int j = i; j <= d; ++j) {
					it += (j + pow(x + j, 0.25)) / (2 * i * j - 1);
					++count;
				}
				res3 += 1 / it;
			}
			res += res3;
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
    Task("OMPBegin16");
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
