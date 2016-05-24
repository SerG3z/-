/*
 * MPI-программа стационарного распределения тепла.
 * Разработать параллельную MPI-программу моделирования стационарного распределения тепла в тонкой стальной пластине.
 * Пластина с трёх сторон окружена водяным паром (температура 100°С), а с четвёртой стороны граничит со льдом (температура 0°С).
 * верху и снизу пластины находится теплоизоляционный слой.
 * При реализации параллельной программы необходимо использовать оптимальный способ декомпозиции расчётной области.
 * Размеры пластины: a = 1 м, b = 2 м. Размер расчётной сетки – 10000 на 10000. Необходимо оценить ускорение параллельной программы.

 * По http://www.slideshare.net/apaznikov/5-mpi стр.23
 * Поскольку точки распределены не равномерно (h = 1/2, alpha = 1/2)
 * Формула вычисления нового значения ячейки
 * new[x * i + j] = (old[x * i + j - 1] / 4  + old[x * i + j + 1] / 4 +
                    old[x * (i - 1) + j] + old[x * (i + 1) + j]) / (2 * 1 + 0.25);

 * Использование: programm <чиcло узлов по x> <чиcло узлов по y>
 * ОГРАНИЧЕНИИ ПРОГРАММЫ число узлов должно превышать РЕАЛЬНОМОГО ЧИСЛА УЗЛОВ в системе
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <mpi.h>

#define EPS 0.01

/* Размеры расчетной сетки */
const int total_x = 1000, total_y = 1000;

struct decart_top_s {
    int max_x, max_y;
    int x, y;
    MPI_Comm comm;
    int field_x, field_y;
};

enum heat_comm_type {
    HEAT_COMM_TOP = 100,
    HEAT_COMM_BOT,
    HEAT_COMM_RIGHT,
    HEAT_COMM_LEFT
};

/*
 * Определение принадлежности узла с данными координатами топологии.
 */
int is_in_topology(struct decart_top_s *top, int x, int y) {
    if (x >= top->max_x || y >= top->max_y || x < 0 || y < 0)
        return 0;
    return 1;
}

/*
 * Заполняем свою часть поля средними значениями.
 */
void field_init(struct decart_top_s *top, double *field, double mean)
{
    int i, j;
    int x = top->field_x;
    int y = top->field_y;

    for (i = 1; i < (y - 1); i++) {
        for (j = 1; j < (x - 1); j++) {
            field[x * i + j] = mean;
        }
    }
}

/*
 * Сделедующие 4 функции служат для связи между соседними узлами
 * или задание краевых значений, если узел не имеет соседа в этом направлении.
 * Функции возвращяют число запросов, окончания которых надо дождатся.
 * 0 если нет соседа, 2 если есть
 *
 * sendrecv_top -   принимем от нижнего, посылаем врехнему узлу, если нет соседа заполняем 0.0
 * sendrecv_bot -   принимем от верхнего, посылаем нижнему узлу, если нет соседа заполняем 100.0
 * sendrecv_right - принимем от правого, посылаем левому узлу, если нет соседа заполняем 100.0
 * sendrecv_left -  принимем от левого, посылаем правому узлу, если нет соседа заполняем 100.0
 */

int sendrecv_top(struct decart_top_s *top, double *in, double *out, int size,
                 MPI_Request *requests) {
    int dest;
    int coords[2];
    int res;
    int req_shift = 0;
    
    coords[0] = top->x;
    coords[1] = top->y + 1;
    
    res = is_in_topology(top, coords[0], coords[1]);
    
    if (res) {
        MPI_Cart_rank(top->comm, coords, &dest);
        MPI_Isend(out, size, MPI_DOUBLE, dest, HEAT_COMM_BOT, top->comm, &requests[0]);
        MPI_Irecv(in, size, MPI_DOUBLE, dest, HEAT_COMM_TOP, top->comm, &requests[1]);
        req_shift = 2;
    } else {
        int i;
        for (i = 0; i < size; i++) {
            in[i] = 0.0;
        }
    }
    return req_shift;
}

int sendrecv_bot(struct decart_top_s *top, double *in, double *out, int size,
                 MPI_Request *requests) {
    int dest;
    int coords[2];
    int res;
    int req_shift = 0;
    
    coords[0] = top->x;
    coords[1] = top->y - 1;
    
    res = is_in_topology(top, coords[0], coords[1]);
    
    if (res) {
        MPI_Cart_rank(top->comm, coords, &dest);
        MPI_Isend(out, size, MPI_DOUBLE, dest, HEAT_COMM_TOP, top->comm, &requests[0]);
        MPI_Irecv(in, size, MPI_DOUBLE, dest, HEAT_COMM_BOT, top->comm, &requests[1]);
        req_shift = 2;
    } else {
        int i;
        for (i = 0; i < size; i++) {
            in[i] = 100.0;
        }
    }
    return req_shift;
}

int sendrecv_right(struct decart_top_s *top, double *in, double *out, int size,
                 MPI_Request *requests) {
    int dest;
    int coords[2];
    int res;
    int req_shift = 0;
    
    coords[0] = top->x + 1;
    coords[1] = top->y;
    
    res = is_in_topology(top, coords[0], coords[1]);
    
    if (res) {
        MPI_Cart_rank(top->comm, coords, &dest);
        MPI_Isend(out, size, MPI_DOUBLE, dest, HEAT_COMM_LEFT, top->comm, &requests[0]);
        MPI_Irecv(in, size, MPI_DOUBLE, dest, HEAT_COMM_RIGHT, top->comm, &requests[1]);
        req_shift = 2;
    } else {
        int i;
    for (i = 0; i < size; i++) {
        in[i] = 100.0;
    }
    }
    return req_shift;
}

int sendrecv_left(struct decart_top_s *top, double *in, double *out, int size,
                 MPI_Request *requests) {
    int dest;
    int coords[2];
    int res;
    int req_shift = 0;
    
    coords[0] = top->x - 1;
    coords[1] = top->y;
    
    res = is_in_topology(top, coords[0], coords[1]);
    
    if (res) {
        MPI_Cart_rank(top->comm, coords, &dest);
        MPI_Isend(out, size, MPI_DOUBLE, dest, HEAT_COMM_RIGHT, top->comm, &requests[0]);
        MPI_Irecv(in, size, MPI_DOUBLE, dest, HEAT_COMM_LEFT, top->comm, &requests[1]);
        req_shift = 2;
    } else {
        int i;
        for (i = 0; i < size; i++) {
            in[i] = 100.0;
        }
    }
    return req_shift;
}

/*---------------------------------------------------------------------------*/

/*
 * Следующие функции служат для копирования левых/правых столбцов, 
 * в массив где эти элементы расположенны непрервыно
 *
 */
void copy_from_horiz_left(double *field, double *row, int x, int y) {
    int i;
    for (i = 1; i < (y - 1); i++) {
        field[x * i] = row[i - 1];
    }
}

void copy_from_horiz_right(double *field, double *row, int x, int y) {
    int i;
    for (i = 1; i < (y - 1); i++) {
        field[x * i + (x - 1)] = row[i - 1];
    }
}

void copy_to_horiz_left(double *field, double *row, int x, int y) {
    int i;
    for (i = 1; i < (y - 1); i++) {
        row[i - 1] = field[x * i + 1];
    }
}

void copy_to_horiz_right(double *field, double *row, int x, int y) {
    int i;
    for (i = 1; i < (y - 1); i++) {
        row[i - 1] = field[x * i + (x - 2)];
    }
}

/*----------------------------------------------------------------------------------------*/
/*
 * Вычисляем новые значения распределения тепла
 * Функция возвращяет модуль максимума между старым и новым значением температуры
 */
double heat_iteartion(double *new, double *old, int x, int y) {
    double diff = 0.0;
    int i, j;
    
    for (i = 1; i < (y - 1); i++) {
        for (j = 1; j < (x - 1); j++) {

            new[x * i + j] = (old[x * i + j - 1] / 4  + old[x * i + j + 1] / 4 +
            old[x * (i - 1) + j] + old[x * (i + 1) + j]) / (2 * 1.25);

/*
            // Для равномерной сетки
            new[x * i + j] = (old[x * i + j - 1] + old[x * i + j + 1] +
            old[x * (i - 1) + j] + old[x * (i + 1) + j]) / 4.0;
*/
            if (fabs(new[x * i + j] - old[x * i + j]) > diff)
                diff = fabs(new[x * i + j] - old[x * i + j]);
        }
    }
    return diff;
}

int main(int argc, char *argv[]) {
    int dims[2], pereiods[2] = {0, 0};
    MPI_Comm comm_cart;
    int i, j;
    MPI_Request requests[8];
    MPI_Status wait_statuses[8];
    MPI_Status field_status;
    int nodes_x, nodes_y;
    int mycoords[2];
    int proc_cnt, myrank;
    int size_x, size_y, rsize_x, rsize_y; // size of process' parts
    int max_block_x, max_block_y;
    double *field, *tmp_field, *tmp;
    struct decart_top_s top;
    double mean;
    double time, time_max;
    double *buf_left_out, *buf_right_out, *buf_left_in, *buf_right_in;
    double *final_arr;
    int iterations = 0;
    int recv_size;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_cnt);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    if (argc < 3) {
        fprintf(stderr, "Usage: programm <nodex_x> <nodes_y>\n");
        return -1;
    }
    sscanf(argv[1], "%d", &nodes_x);
    sscanf(argv[2], "%d", &nodes_y);
    if (nodes_x <=0 || nodes_y <= 0) {
        fprintf(stderr, "Wrong commnad line arguments!\n");
        return -1;
    }
    dims[0] = nodes_x;
    dims[1] = nodes_y;
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, pereiods, 0, &comm_cart);
    MPI_Comm_rank(comm_cart, &myrank);
    MPI_Cart_coords(comm_cart, myrank, 2, mycoords);

    /* Размеры блоков */
    size_x = total_x / nodes_x;
    max_block_x = size_x + (total_x % nodes_x) + 2;
    if (mycoords[0] == nodes_x)
        size_x += total_x % nodes_x;
    size_y = total_y / nodes_y;
    max_block_y = size_y + (total_y % nodes_y) + 2;
    if (mycoords[1] == nodes_y)
        size_y += total_y % nodes_y;
    rsize_x = size_x + 2;
    rsize_y = size_y + 2;

    /* Выделяем память под поле*/
    field = (double*)malloc(max_block_x * max_block_y * sizeof(double));
    tmp_field = (double*)malloc(max_block_x * max_block_y * sizeof(double));
    /* Буферы содержат правые и левые столбцы*/
    buf_left_out = (double*)malloc(rsize_y * sizeof(double));
    buf_right_out = (double*)malloc(rsize_y * sizeof(double));
    buf_left_in = (double*)malloc(rsize_y * sizeof(double));
    buf_right_in = (double*)malloc(rsize_y * sizeof(double));


    /* Нулевой процесс выделяет память под все поле*/
    recv_size = max_block_x * max_block_y * nodes_x * nodes_y;
    if (myrank == 0) {
        final_arr = (double*)malloc(recv_size * sizeof(double));
    }

    /* Инциализации структуры хрянящей данные о топологии*/
    top.x = mycoords[0];
    top.y = mycoords[1];
    top.max_x = nodes_x;
    top.max_y = nodes_y;
    top.comm = comm_cart;
    top.field_x = rsize_x;
    top.field_y = rsize_y;

    /* Вычисляем средню температуру, основываясь на температуре краев*/
    mean = 100.0*4 / (2*2 + 2 * 1);
    field_init(&top, field, mean);

    time = MPI_Wtime();
    for (;;) {
        int req_shift = 0;
        double diff, diff_max;

        iterations++;
        /* Копирем правые/левые столбцы в буфер для передачи*/
        copy_to_horiz_right(field, buf_right_out, rsize_x, rsize_y);
        copy_to_horiz_left(field, buf_left_out, rsize_x, rsize_y);
        /* Обмен крайними строкаи/столбцами */
        req_shift += sendrecv_left(&top, buf_left_in, buf_left_out, rsize_y - 2,
                                   requests + req_shift);
        req_shift += sendrecv_right(&top, buf_right_in, buf_right_out, rsize_y - 2,
                                    requests + req_shift);
        req_shift += sendrecv_top(&top, field + 1, field + rsize_x + 1,
                                  rsize_x - 2, requests + req_shift);
        req_shift += sendrecv_bot(&top, field + (rsize_x * (rsize_y - 1)) + 1,
                                  field + (rsize_x * (rsize_y - 2)) + 1, rsize_x - 2,
                                  requests + req_shift);
        /* Ждем окончания передачи*/
        MPI_Waitall(req_shift, requests, wait_statuses);
        /* Копируем полученные крайние столбцы */
        copy_from_horiz_right(field, buf_right_in, rsize_x, rsize_y);
        copy_from_horiz_left(field, buf_left_in, rsize_x, rsize_y);

        /* Производим расчет темературы для своего прямоугольника */
        diff = heat_iteartion(tmp_field, field, rsize_x, rsize_y);
        /* Меняем старое и новое поля местами */
        tmp = tmp_field;
        tmp_field = field;
        field = tmp;

        /* Если разнича между новым и прошлым значением, хоть в одной ячейке меньше EPS, завершаем программу*/
        MPI_Allreduce(&diff, &diff_max, 1, MPI_DOUBLE, MPI_MAX, comm_cart);
        if (diff_max <= EPS)
            break;
    }

    time = MPI_Wtime() - time;
    MPI_Reduce(&time, &time_max, 1, MPI_DOUBLE, MPI_MAX, 0, comm_cart);
    if (myrank == 0) {
        printf("MAX time = %lf\n", time_max);
        printf("iterations = %d\n", iterations);
    }

    /* Получаем все поле на первом узле, без определенного порядка*/
    MPI_Gather(tmp_field, max_block_x * max_block_y, MPI_DOUBLE, final_arr, max_block_x * max_block_y, MPI_DOUBLE, 0, comm_cart);
    MPI_Finalize();
    return 0;
}

