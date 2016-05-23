/*Многопоточная программа, реализующая алгоритм сортировки Шелла.
На вход алгоритма поступает неотсортированный массив a длины n, заполненный
случайными числовыми значениями. Результат программы – отсортированный массив a.
Длины di промежутков следует выбирать следующим образом: d1 = n / 2, d2 = n / 4,
di = di – 1 / 2, dk = 1. В ходе выполнения алгоритма сортировку каждого подмассива (для
текущей длины d промежутка) необходимо выполнять в отдельном потоке. Число потоков
не должно превышать количества процессорных ядер в системе. Выходные данные
программы – отсортированный массив a.
Необходимо оценить ускорение параллельной программы*/
#include <iostream>
#include <fstream>

#include <cstring>
#include <cstdlib>
#include <ctime>

#include <omp.h>


int N = 10;

int NUM_THREADS;

void shell_sort (int *mas, int size)
{
    for (int d = size / 2; d > 0; d /= 2) {
        for (int i = 0; i + d < size; i += d) {
            int k = i;

            for (int j = k; j >= 0 && j + d < size; j -= d) {
                if (mas[j] - mas[j + d] > 0) {
                    std::swap (mas[j], mas[j + d]);
                    if (j - d < 0) {
                        k += d;
                        j = k;
                    }
                } else {
                    k += d;
                    j = k;
                }
            }
        }
    }
}


void shell_sort_mt (int *mas, int size)
{
    for (int d = size / 2; d > 0; d /= 2) {
        #pragma omp parallel num_threads (NUM_THREADS)
        {   
            #pragma omp for schedule (static, 1) nowait
            for (int i = d; i < size; i += d) {
                int k = i - d;
 
                for (int j = k; j >= 0 && j + d < size; j -= d) {
                    if (mas[j] - mas[j + d] > 0) {
                        std::swap (mas[j], mas[j + d]);
                        if (j - d < 0) {
                            k += d;
                            j = k;
                        }
                    } else {
                        k += d;
                        j = k;
                    }
                }
            }
        }
    }
}



int main (int argc, char *argv[])
{
    if (argc >= 2) {
        N = atoi (argv[1]);
    }

    NUM_THREADS = omp_get_num_threads ();

    std::ofstream fout;
 
    fout.open ("res_03.txt");
    fout << "Massive:\n";
   
    int *mas1, *mas2;

    mas1 = new int[N];
    mas2 = new int[N];

    srand (time (NULL));
    
    for (int i = 0; i < N; ++i) {
        mas1[i] = rand () % 100;
        fout << mas1[i] << " ";
    }
    fout << std::endl;
    fout << "Simple shell sort:\n";

    memcpy (mas2, mas1, sizeof (int) * N);

    clock_t start, end;

    start = clock ();
    shell_sort (mas1, N);
    end = clock ();

    std::cout << "Simple shell sort:\n";
    std::cout << (end - start) / 1000000.00 << std::endl;

    for (int i = 0; i < N; ++i) {
        fout << mas1[i] << " ";
    }
    fout << std::endl;

    fout << "Multithreading shell sort:\n";

    start = clock ();
    shell_sort_mt (mas2, N);
    end = clock ();

    std::cout << "Multithreading shell sort:\n";
    std::cout << (end - start) / 1000000.00 << std::endl;

    for (int i = 0; i < N; ++i) {
        fout << mas2[i] << " ";
    }
    fout << std::endl;

    fout.close ();

    delete[] mas1;
    delete[] mas2;

    return 0;
}
