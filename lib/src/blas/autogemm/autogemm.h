#ifndef AUTOGEMM_KERNEL_H
#define AUTOGEMM_KERNEL_H

#include <iostream>
#include <string.h>
#include <sstream>
#include <cstdio>
#include <map>
#include <fstream>
#include <unistd.h>
#include <dlfcn.h>

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

static std::map<char, std::string> dataTypes = {{'s', "float"}, {'d', "double"}, {'c', "float"}, {'z', "double"}};

int hcblasAutogemmCall(hc::accelerator_view &accl_view, hcblasOrder order,
                       hcblasTranspose typeA, hcblasTranspose typeB, const int M,
                       const uint N, const uint K, const float &alpha, float *A,
                       const uint lda, float *B, const uint ldb, const float &beta,
                       float *C, const uint ldc, const uint aOffset, const uint bOffset,
                       const uint cOffset);

class AutogemmKernel {


   private:

       typedef struct {
         uint macrotileNumRows;
         uint macrotileNumCols;
       }kernTypes;

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

       std::string fileName;
       std::string kernelLib;

       kernTypes *rowKernel;
       kernTypes *colKernel;
       kernTypes *cornerKernel;
       kernTypes *tileKernel;

       bool isRowKernel(kernTypes* kernelInst);
       bool isColKernel(kernTypes* kernelInst);
       void setFileName();
       std::string getKernelName(kernTypes* kernelInst);
       std::string getFileName();
       std::string getKernelLib();

   public:

       void initKernParam(AutogemmKernel* gemmKernel, hcblasOrder order,
                          hcblasTranspose typeA, hcblasTranspose typeB,
                          const float &beta);
       int validateKernParam(AutogemmKernel* gemmKernel);
       int selectMicrotileLogic(AutogemmKernel* gemmKernel, hcblasOrder order,
                                hcblasTranspose typeA, hcblasTranspose typeB,
                                uint M, uint N, uint K, float beta);
       void writeKernel(AutogemmKernel* gemmKernel, uint M, uint N, uint K);
       int makeGemmKernel(AutogemmKernel* gemmKernel, kernTypes* kernelType, std::string& kStr);
       int compileKernel(AutogemmKernel* gemmKernel);
       int invokeKernel(AutogemmKernel* gemmKernel, hc::accelerator_view &accl_view,
                        const uint M, const uint N, const uint K, const float &alpha,
                        float *A, const uint lda, float *B, const uint ldb, const float &beta,
                        float *C, const uint ldc, const uint aOffset, const uint bOffset,
                        const uint cOffset);
};

#endif // AUTOGEMM_KERNEL_H
