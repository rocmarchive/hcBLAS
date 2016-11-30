#include "hipblas.h"

#include <hc.hpp> 
#include <hip/hcc_detail/hcc_acc.h>

// TODO - review use of this handle:
hipblasHandle_t dummyGlobal;

hipblasStatus_t hipblasCreate(hipblasHandle_t* handle) {
  int deviceId;
  hipError_t err;
  hipblasStatus_t retval = HIPBLAS_STATUS_SUCCESS;

  err = hipGetDevice(&deviceId);
  if (err == hipSuccess) {
    hc::accelerator acc;
    err = hipHccGetAccelerator(deviceId, &acc);
    if (err == hipSuccess) {
      retval = hipHCBLASStatusToHIPStatus(hcblasCreate(&*handle, &acc));
      dummyGlobal = *handle;
    } else {
      retval = HIPBLAS_STATUS_INVALID_VALUE;
    }
  }
  return retval;
}
