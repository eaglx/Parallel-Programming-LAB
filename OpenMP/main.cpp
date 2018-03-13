#include <iostream>
#include <stdio.h>
#include <time.h>
#include <omp.h>

long long num_steps = 10000000;
double step;

int main(int argc, char* argv[])
{
	clock_t start, stop;
	volatile double x; //kompilator nie zaklada ze zmienna moze sie zmieniac poza obszarem gdzie wytepuje
	double pi, sum = 0.0;
	int i;
	step = 1. / (double)num_steps;
	start = clock();

#pragma omp parallel // i is private for thread
	{
		//double suml = 0;
#pragma omp for reduction(+:sum)
		//powstaja kopie zm. sum
		for (i = 0; i < num_steps; i++)
		{
			double x = (i + .5)*step;
			//lokalny sum
			sum += 4.0 / (1. + x*x);
		}
#pragma omp atomic
		//scalenie
        //globalna sum
	}

	pi = sum*step;
	stop = clock();

	printf("Wartosc liczby PI wynosi %15.12f\n", pi);
	printf("Czas przetwarzania wynosi %f sekund\n", ((double)(stop - start) / 1000.0));
	
	system("pause");

	return 0;
}
