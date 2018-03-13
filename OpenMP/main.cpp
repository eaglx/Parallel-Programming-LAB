#include <iostream>
#include <stdio.h>
#include <time.h>
#include <omp.h>

long long num_steps = 10000000;
double step;

int main(int argc, char* argv[])
{
	clock_t start, stop;
	double x, pi, sum = 0.0;
	int i;
	step = 1. / (double)num_steps;
	start = clock();

#pragma omp parallel //for // then i is private for thread
	{
		//region
		double suml = 0;
#pragma omp for
		for (i = 0; i < num_steps; i++)
		{
			double x = (i + .5)*step; // x must be private (add double)


			//#pragma omp atomic //Niepodzielnoosc uaktualnienia
			//sum += 4.0 / (1. + x*x);

			suml += 4.0 / (1. + x*x);
		}
#pragma omp atomic
		sum += suml;
	}

	pi = sum*step;
	stop = clock();

	printf("Wartosc liczby PI wynosi %15.12f\n", pi);
	printf("Czas przetwarzania wynosi %f sekund\n", ((double)(stop - start) / 1000.0));
	
	system("pause");

	return 0;
}
