#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include "mpi.h"

#define stepY 0.00005
#define stepX 0.0003

struct Coord {
    double x;
    double y;
    Coord() : x(0), y(0) {}
    Coord(double _x, double _y) : x(_x), y(_y) {}
};

int computingMandelbrotSet(Coord &topCoord, Coord &bottomCoord, std::string &out, int rank, int numberProc);

int main() {
    std::string drobStr;
    double workTime;
    double result;

    int er = 0;
    if (er = MPI_Init(NULL, NULL)) {
        std::cout << "Starting error" << std::endl;
        MPI_Abort(MPI_COMM_WORLD, er);
    }
    int numberProc;
    int rank;
    MPI_Comm_size(MPI_COMM_WORLD, &numberProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    workTime = MPI_Wtime();
    Coord topCoord(-2, 2); // Область a
    Coord bottomCoord(2, -2); // Область b
    computingMandelbrotSet(topCoord, bottomCoord, drobStr, rank, numberProc);

    workTime = MPI_Wtime() - workTime;
    MPI_Reduce(&workTime, &result, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    std::cout << numberProc << ": ";
    std::cout << result << std::endl;
    MPI_Finalize();
    return 0;
}

int computingMandelbrotSet(Coord &topCoord, Coord &bottomCoord, std::string &out, int rank, int numberProc) {
    double realCoord, imageCoord;
    double realTemp, imageTemp, realTemp2, arg;
    int iterations;
    for (imageCoord = topCoord.y - stepY * rank; imageCoord >= bottomCoord.y;
        imageCoord -= (stepY * numberProc) ) {
        for (realCoord = topCoord.x; realCoord <= bottomCoord.x;
            realCoord += stepX) {
            iterations = 0;
            realTemp = realCoord;
            imageTemp = imageCoord;
            arg = (realCoord * realCoord) + (imageCoord * imageCoord);

            while (arg < 4 && iterations < 40) {
                realTemp2 = (realTemp * realTemp) - (imageTemp * imageTemp) + realCoord;
                imageTemp = (2 * realTemp * imageTemp) + imageCoord;
                realTemp = realTemp2;
                arg = (realTemp * realTemp) + (imageTemp * imageTemp);
                iterations++;
            }
            switch (iterations % 4) {
                case 0:
                    out += ".";
                    break;
                case 1:
                    out += "o";
                    break;
                case 2:
                    out += "0";
                    break;
                case 3:
                    out += "@";
                    break;
            }
        }
        out += "\n";
    }
    return 0;
}
