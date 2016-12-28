#pragma once

#include <iostream>
//using namespace std;

namespace Kaguya
{

/*
a[0]  a[1]  a[2]
0     3     6
1     4     7
2     5     8
*/
enum MatrixMajor
{
    COL_STORAGE,
    ROW_STORAGE
};
template <typename T>
void Aligned_2DArray(T** &twod_array, size_t wdt = 3, size_t hgt = 3,
                     MatrixMajor matrix_major = COL_STORAGE)
{

    // size of the matrix
    size_t major, secondary;

    // if column major storage
    // Major direction length is width
    if (matrix_major == COL_STORAGE)
    {
        /*
        0 3 6
        1 4 7
        2 5 8
        */
        major = wdt;
        secondary = hgt;
    }
    else
    {
        /*
        0 1 2
        3 4 5
        6 7 8
        */
        major = hgt;
        secondary = wdt;
    }
    // To access data in 2D array, use A[y][x]
    // To access data in 1D array, use A[0][i]
    // Row Matrix
    T* oned_array = new T[major * secondary];

    twod_array = new T*[major];

    for (size_t i = 0; i < major; i++)
    {
        twod_array[i] = &oned_array[i * secondary];
    }
}
template <typename T>
void delete_2DArray(T** &twod_array)
{
    delete[] twod_array[0];
    delete[] twod_array;
    twod_array = nullptr;
}

}
