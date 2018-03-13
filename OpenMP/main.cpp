#include <stdio.h>
#include <iostream>
#include <omp.h>

int main() {
	int i;	// shared
	omp_set_num_threads(4);	//thread numbers

#pragma omp parallel //project propertis -> C/C++ -> Language -> Open MP Support YES
	{		
		int i; // local for thread
		printf("Hello World\n");
/*
#pragma omp parallel //all thread have number 0
		schedule(static) 1 blok iteracji oraz ... przydzial porowno
		 chodzi o zachowanie lokalnosci 
#pragma omp for schedule(static, 1) // next instructions is for, then i (before #pragma omp parallel) is private variable
		6 to jeden dostanie i watek jeden robi prace
		1 to all cos robi
#pragma omp for schedule(dynamic, 5) // dynamicznie przydzielane zadania (czas przydzielania sie wydluzy)
		przy konkretnych obliczeniach moze sie przydac == niedetermistyczny
*/
#pragma omp for schedule(guided) // na poczatku duzo przydziela jak duzo iteracji

		for (i = 0; i < 6; i++)
			printf("Iter:%d wykonal watek %d\n", i, omp_get_thread_num());

		printf("GoodBye World\n");
	}

	system("pause");
}
