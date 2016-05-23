/*
 * Quick Sort OpenMP
Многопоточная программа, реализующая алгоритм быстрой сортировки.
Входными данными для программы является неотсортированный числовой массив a,
заполненный случайными значениями. Результат выполнения программы –
отсортированный массив a. Необходимо реализовать рекурсивный алгоритм быстрой
сортировки. При каждом вызове функции быстрой сортировки необходимо порождать
поток. Порождение потоков необходимо остановить при достижении количества потоков,
равного заданному числу, которое варьируется от 1 до числа процессорных ядер на
вычислительном узле. В качестве опорного элемента выбирать первый элемент
последовательности. Результат работы программы – отсортированный массив a.
Необходимо оценить ускорение параллельной программы.
 */
#include <iostream>
#include <fstream>

#include <cstring>
#include <cstdlib>
#include <ctime>

#include <omp.h>

int N = 10;         // Massive size


void partition (int *mas, int *left, int *right)
{
    int pivot = 0;
    int elem1;
    int elem2;

    while (1) {
        elem1 = *(mas + (*left));
        elem2 = *(mas + (*right));

        while (elem1 < *(mas + pivot)) {
            ++(*left);
            elem1 = *(mas + (*left));
        }
 
        while (elem2 > *(mas + pivot)) {
            --(*right);
            elem2 = *(mas + (*right));
        }
 
        if ((*left) < (*right)) {
            std::swap (*(mas + (*left)), *(mas + (*right)));
            if ((*left) == pivot) {
                pivot = (*right);
                if (elem1 == elem2) {
                    ++(*left);
                }
            } else if ((*right) == pivot) {
                pivot = (*left);
                if (elem1 == elem2) {
                    --(*right);
                }
            }
        } else {
            break;
        }
    }
}


void quick_sort (int *mas, int size)
{
    int i = 0, j = size - 1;

    partition (mas, &i, &j);

    if (i >= 1) {
        quick_sort (mas, i);
    }
    if (size - (j + 1) > 1) {
        quick_sort (mas + i + 1, size - (j + 1));
    }
}


void quick_sort_mt (int *mas, int size)
{
    int i = 0, j = size - 1;

    partition (mas, &i, &j);

    #pragma omp parallel sections num_threads (2)
    {
        #pragma omp section
        {
            if (i >= 1) {
                quick_sort_mt (mas, i);
            }
        }
        
        #pragma omp section
        {
            if (size - (j + 1) > 1) {
                quick_sort_mt (mas + i + 1, size - (j + 1));
            }
        }
    }
}


int main (int argc, char *argv[])
{
    omp_set_nested (1);
    omp_set_max_active_levels (omp_get_num_threads ());

    if (argc >= 2) {
        N = atoi (argv[1]);
    }

    std::ofstream fout;
 
    fout.open ("res_02.txt");
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
    fout << "Simple quick sort:\n";

    memcpy (mas2, mas1, sizeof (int) * N);

    clock_t start, end;

    start = clock ();
    quick_sort (mas1, N);
    end = clock ();

    std::cout << "Simple quick sort:\n";
    std::cout << (end - start) / 1000000.00 << std::endl;

    for (int i = 0; i < N; ++i) {
        fout << mas1[i] << " ";
    }
    fout << std::endl;

    fout << "Multithreading quick sort:\n";

    start = clock ();
    quick_sort_mt (mas2, N);
    end = clock ();

    std::cout << "Multithreading quick sort:\n";
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
