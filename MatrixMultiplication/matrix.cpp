#include <stdio.h>
#include <time.h>
#include <windows.h>
#include "omp.h"

#define USE_MULTIPLE_THREADS true
#define MAXTHREADS 128
int NumThreads;
double start;

static const int ROWS = 1000 ;     // liczba wierszy macierzy
static const int COLUMNS = 1000 ;  // lizba kolumn macierzy

float matrix_a[ROWS][COLUMNS] ;    // lewy operand 
float matrix_b[ROWS][COLUMNS] ;    // prawy operand
float matrix_r[ROWS][COLUMNS] ;    // wynik

FILE *result_file ;

void initialize_matrices()
{
    // zdefiniowanie zawarosci poczatkowej macierzy
//#pragma omp parallel for 
    for (int i = 0 ; i < ROWS ; i++) {
            for (int j = 0 ; j < COLUMNS ; j++) {
                    matrix_a[i][j] = (float) rand() / RAND_MAX ;
                    matrix_b[i][j] = (float) rand() / RAND_MAX ;
                    matrix_r[i][j] = 0.0 ;
            }
    }
}


void print_result()
{
    // wydruk wyniku
    for (int i = 0 ; i < ROWS ; i++) {
            for (int j = 0 ; j < COLUMNS ; j++) {
                    fprintf(result_file, "%6.4f ", matrix_r[i][j]) ;
            }
            fprintf(result_file, "\n") ;
    }
}

void multiply_matrices_IJK()
{
        // mnozenie macierzy 
#pragma omp parallel for 
    for (int i = 0 ; i < ROWS ; i++) {
            for (int j = 0 ; j < COLUMNS ; j++) {
                    float sum = 0.0 ;
                    for (int k = 0 ; k < COLUMNS ; k++) {
                            sum = sum + matrix_a[i][k] * matrix_b[k][j] ;
                    }
                    matrix_r[i][j] = sum ;
            }
     }
}

void multiply_matrices_IKJ()
{
        // mnozenie macierzy 
#pragma omp parallel for 
    for (int i = 0 ; i < ROWS ; i++) 
	    for (int k = 0 ; k < COLUMNS ; k++) 
            for (int j = 0 ; j < COLUMNS ; j++)
                    matrix_r[i][j] += matrix_a[i][k] * matrix_b[k][j] ;
                      
}

void multiply_matrices_IJK_IKJ()
{
    for (int i = 0; i < ROWS; i += R)
        for (int j = 0; j < COLUMNS; j += R)
            for (int k = 0; k < COLUMNS; k += R)
            {
                #pragma omp parallel for
                for (int ii = i; ii < i + R; ii++)
                    for (int kk = k; kk < k + R; kk++)
                        for (int jj = j; jj < j + R; jj++)
                            matrix_r[ii][jj] += matrix_a[ii][kk] * matrix_b[kk][jj];
            }
}


void print_elapsed_time()
{
    double elapsed ;
    double resolution ;

    // wyznaczenie i zapisanie czasu przetwarzania
    elapsed = (double) clock() / CLK_TCK ;
    resolution = 1.0 / CLK_TCK ;
	printf("Czas: %8.4f sec \n",
            elapsed-start) ;

    fprintf(result_file, 
            "Czas wykonania programu: %8.4f sec (%6.4f sec rozdzielczosc pomiaru)\n",
            elapsed-start, resolution) ;
}

int main(int argc, char* argv[])
{
//	 start = (double) clock() / CLK_TCK ;
    if ((result_file = fopen("classic.txt", "a")) == NULL) {
        fprintf(stderr, "nie mozna otworzyc pliku wyniku \n") ;
        perror("classic") ;
        return( EXIT_FAILURE ) ;
    }
      

	//Determine the number of threads to use
	if (USE_MULTIPLE_THREADS) {
		SYSTEM_INFO SysInfo;
		GetSystemInfo(&SysInfo);
		NumThreads = SysInfo.dwNumberOfProcessors;
		if(NumThreads > MAXTHREADS)
			NumThreads = MAXTHREADS;
	}
	else
		NumThreads = 1;
    fprintf(result_file, "Klasyczny algorytm mnozenia macierzy, liczba watkow %d \n",NumThreads) ;
	printf("liczba watkow  = %d\n\n", NumThreads);
	
    initialize_matrices();
    printf( "IKJ ");
    start = (double) clock() / CLK_TCK ;
    multiply_matrices_IKJ();
    print_elapsed_time() ;

    initialize_matrices();
    printf( "IJK ");
    start = (double) clock() / CLK_TCK ;
    multiply_matrices_IJK();
    print_elapsed_time() ;

    initialize_matrices();
    printf( "IJK_IKJ ");
    start = (double) clock() / CLK_TCK ;
    multiply_matrices_IJK_IKJ()
    print_elapsed_time() ;

    fclose(result_file) ;

    return( 0 ) ;
}
