/*
Copyright (c) 2015-2016 Advanced Micro Devices, Inc. All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include "include/hcblas.h"
#include "include/hcblaslib.h"
#include <cblas.h>
#include <cstdlib>
#include <hc_am.hpp>
#include <iostream>

unsigned int global_seed = 100;

int main(int argc, char **argv) {
  /* HCBLAS implementation */
  hc::accelerator accl;
  hc::accelerator_view av = accl.get_default_view();
  Hcblaslibrary hc(&av);
  if (argc < 3) {
    std::cout << "No sufficient commandline arguments specified"
              << "argc :" << argc << std::endl;
    return -1;
  }
  int N = atoi(argv[1]);
  int Imple_type = atoi(argv[2]);
  const float alpha = 1;
  int incX = 1;
  int incY = 1;
  __int64_t xOffset = 0;
  __int64_t yOffset = 0;
  __int64_t X_batchOffset = N;
  __int64_t Y_batchOffset = N;
  int batchSize = 128;
  hcblasStatus status;
  /* CBLAS implementation */
  bool ispassed = 1;
  __int64_t lenx = 1 + (N - 1) * abs(incX);
  __int64_t leny = 1 + (N - 1) * abs(incY);
  std::vector<hc::accelerator> acc = hc::accelerator::get_all();
  hc::accelerator_view accl_view = (acc[1].get_default_view());

  /* Implementation type I - Inputs and Outputs are HCC float array containers
   */
  if (Imple_type == 1) {
    float *X = (float *)calloc(lenx, sizeof(float));
    float *Y = (float *)calloc(leny, sizeof(float));
    float *Ycblas = (float *)calloc(N, sizeof(float));
    float *devX = hc::am_alloc(sizeof(float) * lenx, acc[1], 0);
    float *devY = hc::am_alloc(sizeof(float) * leny, acc[1], 0);
    for (int i = 0; i < lenx; i++) {
      X[i] = rand_r(&global_seed) % 10;
    }
#ifdef PROFILE
    for (int iter = 0; iter < 10; iter++) {
#endif
      for (int i = 0; i < leny; i++) {
        Y[i] = rand_r(&global_seed) % 15;
        Ycblas[i] = Y[i];
      }
      accl_view.copy(X, devX, lenx * sizeof(float));
      accl_view.copy(Y, devY, leny * sizeof(float));
      status = hc.hcblas_saxpy(accl_view, N, alpha, devX, incX, devY, incY,
                               xOffset, yOffset);
      accl_view.copy(devY, Y, leny * sizeof(float));
      cblas_saxpy(N, alpha, X, incX, Ycblas, incY);
      for (int i = 0; i < leny; i++) {
        if (Y[i] != Ycblas[i]) {
          ispassed = 0;
          std::cout << " HCSAXPY[" << i << "] " << Y[i]
                    << " does not match with CBLASSAXPY[" << i << "] "
                    << Ycblas[i] << std::endl;
          break;
        } else {
          continue;
        }
      }
      if (!ispassed) std::cout << "TEST FAILED" << std::endl;
      if (status) std::cout << "TEST FAILED" << std::endl;
#ifdef PROFILE
    }
#endif
    free(X);
    free(Y);
    free(Ycblas);
    hc::am_free(devX);
    hc::am_free(devY);
  }

  /* Implementation type II - Inputs and Outputs are HC++ float array containers
     with batch processing */

  else {
    float *Xbatch = (float *)calloc(lenx * batchSize, sizeof(float));
    float *Ybatch = (float *)calloc(leny * batchSize, sizeof(float));
    float *Ycblasbatch = (float *)calloc(N * batchSize, sizeof(float));
    float *devXbatch =
        hc::am_alloc(sizeof(float) * lenx * batchSize, acc[1], 0);
    float *devYbatch =
        hc::am_alloc(sizeof(float) * leny * batchSize, acc[1], 0);
    for (int i = 0; i < lenx * batchSize; i++) {
      Xbatch[i] = rand_r(&global_seed) % 10;
    }
#ifdef PROFILE
    for (int iter = 0; iter < 10; iter++) {
#endif
      for (int i = 0; i < leny * batchSize; i++) {
        Ybatch[i] = rand_r(&global_seed) % 15;
        Ycblasbatch[i] = Ybatch[i];
      }
      accl_view.copy(Xbatch, devXbatch, lenx * batchSize * sizeof(float));
      accl_view.copy(Ybatch, devYbatch, leny * batchSize * sizeof(float));
      status = hc.hcblas_saxpy(accl_view, N, alpha, devXbatch, incX,
                               X_batchOffset, devYbatch, incY, Y_batchOffset,
                               xOffset, yOffset, batchSize);
      accl_view.copy(devYbatch, Ybatch, leny * batchSize * sizeof(float));
      for (int i = 0; i < batchSize; i++)
        cblas_saxpy(N, alpha, Xbatch + i * N, incX, Ycblasbatch + i * N, incY);
      for (int i = 0; i < leny * batchSize; i++) {
        if (Ybatch[i] != Ycblasbatch[i]) {
          ispassed = 0;
          std::cout << " HCSAXPY[" << i << "] " << Ybatch[i]
                    << " does not match with CBLASSAXPY[" << i << "] "
                    << Ycblasbatch[i] << std::endl;
          break;
        } else {
          continue;
        }
      }
      if (!ispassed) std::cout << "TEST FAILED" << std::endl;
      if (status) std::cout << "TEST FAILED" << std::endl;
#ifdef PROFILE
    }
#endif
    free(Xbatch);
    free(Ybatch);
    free(Ycblasbatch);
    hc::am_free(devXbatch);
    hc::am_free(devYbatch);
  }
  return 0;
}
