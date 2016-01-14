#include "hcblas.h"
#include "gtest/gtest.h"


TEST(hcblasCreateTest, return_Check_hcblasCreate) {
 // Case I: Input to the API is null handle
 hcblasHandle_t *handle = NULL;
 // Passing a Null handle to the API
 hcblasStatus_t status = hcblasCreate(handle); 
 // Assert if the handle is still NULL after allocation
 EXPECT_TRUE(handle != NULL);
 // If allocation succeeds we must expect a success status
 if (handle != NULL)
   EXPECT_EQ(status, HCBLAS_STATUS_SUCCESS); 
 else
   EXPECT_EQ(status, HCBLAS_STATUS_ALLOC_FAILED);

 // Case II: Input to API is not a null handle
 status = hcblasCreate(handle);
 // the status shouldn't be HCBLAS_STATUS_SUCCESS
 EXPECT_TRUE(status != HCBLAS_STATUS_SUCCESS); 
}
