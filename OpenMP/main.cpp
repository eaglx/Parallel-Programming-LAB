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
	double pi, sum = 0.0, tablica[100];
	int i;
	step = 1.0 / (double)num_steps;
	
	
	for (int j = 0; j < 100; j++)
        tablica[j] = 0;
    
    omp_set_num_threads(2);	//ile watkow
    
    //main loop
    for (int j = 0; j < 30; j++) {
        start = clock();
        sum = 0.0;
#pragma omp parallel // i is private for thread
	    {
		    int id = omp_get_thread_num();	//raz wywolana
		    tablica[id + j] = 0;
#pragma omp for
		    //powstaja kopie zm. sum
		    for (i = 0; i < num_steps; i++)
		    {
			    double x = (i + .5)*step;
			    //lokalny sum
			    tablica[id + j] += 4.0 / (1. +x*x);
		    }
#pragma omp atomic
		
		    sum += tablica[id + j];	//tablica -> zamek -> odczyt z pamieci -> zmienic wartosz -> zmiana wartosci -> zciagam blokade	
	    }

	    pi = sum*step;
	    stop = clock();

	    printf("Wartosc liczby PI wynosi %15.12f\n", pi);
	    printf("Czas przetwarzania wynosi %f sekund\n", ((double)(stop - start) / 1000.0));
	}
	system("pause");

	return 0;
}
