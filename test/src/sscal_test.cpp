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
  __int64_t xOffset = 0;
  hcblasStatus status;
  int batchSize = 128;
  __int64_t X_batchOffset = N;
  __int64_t lenx = 1 + (N - 1) * abs(incX);
  std::vector<hc::accelerator> accs = hc::accelerator::get_all();
  hc::accelerator_view accl_view = (accs[1].get_default_view());
  bool ispassed = 1;

  /* Implementation type I - Inputs and Outputs are HCC device pointers */

  if (Imple_type == 1) {
    float *X = (float *)calloc(lenx, sizeof(float));  // host input
    float *Xcblas = (float *)calloc(lenx, sizeof(float));
    float *devX = hc::am_alloc(sizeof(float) * lenx, accs[1], 0);
    for (int i = 0; i < lenx; i++) {
      X[i] = rand_r(&global_seed) % 10;
      Xcblas[i] = X[i];
    }
    accl_view.copy(X, devX, lenx * sizeof(float));
    status = hc.hcblas_sscal(accl_view, N, alpha, devX, incX, xOffset);
    accl_view.copy(devX, X, lenx * sizeof(float));
    cblas_sscal(N, alpha, Xcblas, incX);
    for (int i = 0; i < lenx; i++) {
      if (X[i] != Xcblas[i]) {
        ispassed = 0;
        std::cout << " HCSSCAL[" << i << "] " << X[i]
                  << " does not match with CBLASSSCAL[" << i << "] "
                  << Xcblas[i] << std::endl;
        break;
      } else {
        continue;
      }
    }
    if (!ispassed) std::cout << "TEST FAILED" << std::endl;
    if (status) std::cout << "TEST FAILED" << std::endl;
    hc::am_free(devX);
    free(X);
    free(Xcblas);
  }

  /* Implementation type II - Inputs and Outputs are HCC device pointers with
     batch processing */

  else {
    float *Xbatch =
        (float *)calloc(lenx * batchSize, sizeof(float));  // host input
    float *devXbatch =
        hc::am_alloc(sizeof(float) * lenx * batchSize, accs[1], 0);
    float *Xcblasbatch = (float *)calloc(lenx * batchSize, sizeof(float));
    for (int i = 0; i < lenx * batchSize; i++) {
      Xbatch[i] = rand_r(&global_seed) % 10;
      Xcblasbatch[i] = Xbatch[i];
    }
    accl_view.copy(Xbatch, devXbatch, lenx * batchSize * sizeof(float));
    status = hc.hcblas_sscal(accl_view, N, alpha, devXbatch, incX, xOffset,
                             X_batchOffset, batchSize);
    accl_view.copy(devXbatch, Xbatch, lenx * batchSize * sizeof(float));
    for (int i = 0; i < batchSize; i++)
      cblas_sscal(N, alpha, Xcblasbatch + i * N, incX);
    for (int i = 0; i < lenx * batchSize; i++) {
      if (Xbatch[i] != Xcblasbatch[i]) {
        ispassed = 0;
        std::cout << " HCSSCAL[" << i << "] " << Xbatch[i]
                  << " does not match with CBLASSSCAL[" << i << "] "
                  << Xcblasbatch[i] << std::endl;
        break;
      } else {
        continue;
      }
    }
    if (!ispassed) std::cout << "TEST FAILED" << std::endl;
    if (status) std::cout << "TEST FAILED" << std::endl;
    hc::am_free(devXbatch);
    free(Xbatch);
    free(Xcblasbatch);
  }
  return 0;
}
