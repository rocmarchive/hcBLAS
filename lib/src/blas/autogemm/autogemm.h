#ifndef AUTOGEMM_KERNEL_H
#define AUTOGEMM_KERNEL_H

#include <iostream>
#include <string.h>
#include <sstream>
#include <cstdio>
#include <map>

#include "hcblaslib.h"

#define uint unsigned int
#define SUCCESS 0
#define CRIT_ERR -1
#define endLine "\n"

template <typename T>
std::string toString(T val) {

    std::stringstream txt;
    txt << val;
    return txt.str();

}

std::map<char, std::string> dataTypes = {{'s', "float"}, {'d', "double"}, {'c', "float"}, {'z', "double"}};

class AutogemmKernel {

   private:

       /* Tile Parameters */
       uint tileNumRows;
       uint tileNumCols;
       uint microtileNumRows;
       uint microtileNumCols;
       uint macrotileNumRows;
       uint macrotileNumCols;
       uint unroll;

       /* Non tile Parameters */
       char precision;
       bool isColMajor;
       bool isTransA;
       bool isTransB;
       bool isBeta_nonZero;

       const char* nameFormatTile = "MX%03d_NX%03d_KX%02d";
       const char* nameFormatRow  = "ML%03d_NX%03d_KX%02d";
       const char* nameFormatCol = "MX%03d_NL%03d_KX%02d";
       const char* nameFormatCorner = "ML%03d_NL%03d_KX%02d";

       std::string kernelName;
       std::string fileName;
       std::string kernelLib;

       bool isRowKernel();
       bool isColKernel();
       std::string getKernelName();
       std::string getFileName();
       std::string getKernelLib();

   public:

       void initKernParam(AutogemmKernel* gemmKernel);
       int validateKernParam(AutogemmKernel* gemmKernel);
       int makeGemmKernel(AutogemmKernel* gemmKernel);
       int compileKernel(AutogemmKernel* gemmKernel);
       int invokeKernel(AutogemmKernel* gemmKernel, hc::accelerator_view &accl_view,
                        hcblasOrder order, hcblasTranspose typeA, hcblasTranspose typeB, 
                        const int M, const int N, const int K, const float &alpha,
                        float *A, const long lda, float *B, const long ldb, const float &beta,
                        float *C, const long ldc, const long aOffset, const long bOffset, const long cOffset);
};

#endif // AUTOGEMM_KERNEL_H
