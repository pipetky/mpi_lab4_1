/*Минимальный элемент среди элементов с четными индексами
 * с применением коллективных функций.*/

#include <stdio.h>
#include "mpi.h"
#include <random>
#include <math.h>
#include "iostream"
#include <time.h>
#include <unistd.h>
#define LEN 30
using namespace std;

int random_int() {
    random_device rd{};
    default_random_engine generator(rd());
    uniform_int_distribution<int> distribution(1, 100);
    return distribution(generator);
}

int main(int argc, char **argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int *vector = new int[LEN];
    MPI_Status status;
    int buf;
    if(rank == 0) {

        cout << endl << "vector: ";
        for (int i = 0; i < LEN; i++) {
            vector[i] = random_int();
            cout << vector[i] << " ";
        }


    }
    MPI_Bcast(vector, LEN, MPI_INT, 0, MPI_COMM_WORLD);
    int min = vector[rank*2];
    for (int i = rank * 2; i < LEN; i +=size*2){
        if (vector[i] < min){
            min = vector[i];
        }
    }
    if (rank == 0){
        for (int i = 1; i < size; i++) {
            MPI_Recv(&buf, 1, MPI_INT, MPI_ANY_SOURCE, 00,
                     MPI_COMM_WORLD, &status);
            cout << endl << "received minimum: " << buf;
            if (buf < min){
                min = buf;
            }

        }
        cout << endl << "minimum is " << min << endl;
    } else {
        MPI_Send(&min, 1, MPI_INT, 0, 00,
                 MPI_COMM_WORLD);
    }


    MPI_Finalize();
}