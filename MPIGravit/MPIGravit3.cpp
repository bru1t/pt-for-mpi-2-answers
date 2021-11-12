#include "pt4.h"
#include "mpi.h"
#define gravity 10 // гравитационная постоянная
#define dt 0.1     // шаг по времени
#define N 800      // количество частиц
#define fmax 1     // максимальное значение силы

int k;             // количество процессов
int r;             // ранг текущего процесса
int Niter;         // число итераций

struct Particle
{
    double x, y, vx, vy;
};

struct Force
{
    double x, y;
};

Particle p[N];
Force f[N];
double m[N];

void Init()
{
    for (int i = 0; i < N; i++)
    {
        p[i].x = 20 * (i / 20 - 20) + 10;
        p[i].y = 20 * (i % 20 - 10) + 10;
        p[i].vx = p[i].y / 15;
        p[i].vy = -p[i].x / 50;

        m[i] = 100 + i % 100;

        f[i].x = 0;
        f[i].y = 0;
    }
}

// Реализация непараллельного алгоритма

void CalcForces2()
{
    for (int i = 0; i < N - 1; i++)
    for (int j = i + 1; j < N; j++)
    {
        double dx = p[j].x - p[i].x, dy = p[j].y - p[i].y,
        r_2 = 1 / (dx * dx + dy * dy),
        r_1 = sqrt(r_2),
        fabs = gravity * m[i] * m[j] * r_2;
        if (fabs > fmax) fabs = fmax;
        f[i].x += dx = fabs * dx * r_1;
        f[i].y += dy = fabs * dy * r_1;
        f[j].x -= dx;
        f[j].y -= dy;
    }
}

void MoveParticlesAndFreeForces()
{
    for (int i = 0; i < N; i++)
    {
        double dvx = f[i].x * dt / m[i],
            dvy = f[i].y * dt / m[i];
        p[i].x += (p[i].vx + dvx / 2) * dt;
        p[i].y += (p[i].vy + dvy / 2) * dt;
        p[i].vx += dvx;
        p[i].vy += dvy;
        f[i].x = 0;
        f[i].y = 0;
    }
}

void NonParallelCalc(int n)
{
    Init();
    for (int i = 0; i < n; i++)
    {
        CalcForces2();
        MoveParticlesAndFreeForces();
    }
}

// Конец реализации непараллельного алгоритма

// Реализация параллельного алгоритма,
// основанного на методе конвейера с разбиением на полосы
constexpr int countProc = 8;
constexpr int blockSize = N / countProc;

Particle p_conveyor[blockSize];
Force f_conveyor[blockSize];
double m_conveyor[blockSize];
int p_c = 0;
MPI_Datatype MPI_PARTICLE;
MPI_Datatype MPI_FORCE;

void InitConveyor() {
	for (int i = 0; i < blockSize; i++)
	{
		int curr_i = (r + i * countProc);
		p_conveyor[i].x = 20 * (curr_i / 20 - 20) + 10;
		p_conveyor[i].y = 20 * (curr_i % 20 - 10) + 10;
		p_conveyor[i].vx = p_conveyor[i].y / 15;
		p_conveyor[i].vy = -p_conveyor[i].x / 50;

		m_conveyor[i] = 100 + curr_i % 100;

		f_conveyor[i].x = 0;
		f_conveyor[i].y = 0;
	}
}

void CreateParticleType() {
	MPI_Type_contiguous(4, MPI_DOUBLE, &MPI_PARTICLE);
	MPI_Type_commit(&MPI_PARTICLE);
}

void CreateForceType() {
	MPI_Type_contiguous(2, MPI_DOUBLE, &MPI_FORCE);
	MPI_Type_commit(&MPI_FORCE);
}

int TrueNumber(int ind, int iter = 0) {
	return (countProc + r - iter) % countProc + countProc * ind;
}

Particle p_temp[blockSize];
double m_temp[blockSize];
Force f_temp[blockSize];

void ExchangeData() {
	MPI_Status st;
	MPI_Sendrecv_replace(p_temp, blockSize, MPI_PARTICLE, (r + 1) % countProc, 0, (r - 1 + countProc) % countProc, 0, MPI_COMM_WORLD, &st);
	MPI_Sendrecv_replace(f_temp, blockSize, MPI_FORCE, (r + 1) % countProc, 0, (r - 1 + countProc) % countProc, 0, MPI_COMM_WORLD, &st);
	MPI_Sendrecv_replace(m_temp, blockSize, MPI_DOUBLE, (r + 1) % countProc, 0, (r - 1 + countProc) % countProc, 0, MPI_COMM_WORLD, &st);
}

void CalcForcesConveyor() {
	for (int i = 0; i < blockSize; ++i) {
		p_temp[i] = p_conveyor[i];
		m_temp[i] = m_conveyor[i];
		f_temp[i] = f_conveyor[i];
	}
	for (int i = 0; i < blockSize - 1; i++) {
		for (int j = i + 1; j < blockSize; j++) {
			double dx = p_conveyor[j].x - p_conveyor[i].x, dy = p_conveyor[j].y - p_conveyor[i].y,
				r_2 = 1 / (dx * dx + dy * dy),
				r_1 = sqrt(r_2),
				fabs = gravity * m_conveyor[i] * m_conveyor[j] * r_2;
			if (fabs > fmax) {
				fabs = fmax;
			}
			f_conveyor[i].x += dx = fabs * dx * r_1;
			f_conveyor[i].y += dy = fabs * dy * r_1;
			f_conveyor[j].x -= dx;
			f_conveyor[j].y -= dy;
		}
	}
	ExchangeData();
	for (int i = 1; i < countProc; ++i) {
		for (int h = 0; h < blockSize; ++h) {
			for (int j = 0; j < blockSize; ++j) {
				if (TrueNumber(h) > TrueNumber(j, i)) {
					++p_c;
					double dx = p_conveyor[h].x - p_temp[j].x, dy = p_conveyor[h].y - p_temp[j].y,
						r_2 = 1 / (dx * dx + dy * dy),
						r_1 = sqrt(r_2),
						fabs = gravity * m_conveyor[h] * m_temp[j] * r_2;
					if (fabs > fmax) {
						fabs = fmax;
					}
					f_temp[j].x += dx = fabs * dx * r_1;
					f_temp[j].y += dy = fabs * dy * r_1;
					f_conveyor[h].x -= dx;
					f_conveyor[h].y -= dy;
				}
				else {
					break;
				}
			}
		}
		ExchangeData();
	}
}

void MovePracticlesAndFreeForcesConveyor() {
	for (int i = 0; i < blockSize; i++)
	{
		double dvx = (f_conveyor[i].x + f_temp[i].x) * dt / m_conveyor[i],
			dvy = (f_conveyor[i].y + f_temp[i].y) * dt / m_conveyor[i];
		p_conveyor[i].x += (p_conveyor[i].vx + dvx / 2) * dt;
		p_conveyor[i].y += (p_conveyor[i].vy + dvy / 2) * dt;
		p_conveyor[i].vx += dvx;
		p_conveyor[i].vy += dvy;
		f_conveyor[i].x = 0;
		f_conveyor[i].y = 0;
	}
}

void ParallelCalc(int n) {
	InitConveyor();
	for (int i = 0; i < n; ++i) {
		CalcForcesConveyor();
		MovePracticlesAndFreeForcesConveyor();
	}
}

void ParallelPrint() {
	if (r == 0) {
		pt << p_conveyor[0].x << p_conveyor[0].y;
	}
	else if (r == 7) {
		pt << p_conveyor[99].x << p_conveyor[99].y;
	}
}

// Конец реализации параллельного алгоритма

void Solve()
{
    Task("MPIGravit3");
    int flag;
    MPI_Initialized(&flag);
    if (flag == 0)
        return;
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    k = size;
    r = rank;
    if (r == 0)
    {
        pt >> Niter;
    // Тестирование непараллельного алгоритма:

        ShowLine("НЕПАРАЛЛЕЛЬНЫЙ АЛГОРИТМ");
        NonParallelCalc(1);

        ShowLine("После одной итерации");
        SetPrecision(12);
        Show("    Координаты точки 0:   ", p[0].x, 17);
        ShowLine(p[0].y, 17);
        Show("    Координаты точки 799: ", p[799].x, 17);
        ShowLine(p[799].y, 17);

        double t = MPI_Wtime();
        NonParallelCalc(Niter);
        t = MPI_Wtime() - t;

        ShowLine("После требуемого числа итераций");
        Show("    Координаты точки 0:   ", p[0].x, 17);
        ShowLine(p[0].y, 17);
        Show("    Координаты точки 799: ", p[799].x, 17);
        ShowLine(p[799].y, 17);

        SetPrecision(2);
        ShowLine("Время работы непараллельного алгоритма: ", t * 1000);
    }
    MPI_Bcast(&Niter, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Тестирование параллельного алгоритма:
	CreateParticleType();
	CreateForceType();
	ParallelCalc(1);
	ParallelPrint();
	double t = MPI_Wtime();
	ParallelCalc(Niter);
	t = MPI_Wtime() - t;
	ParallelPrint();
	Show("Time = ", t * 1000);
	Show("PairsCount = ", p_c);
}
