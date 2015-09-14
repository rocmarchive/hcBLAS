#include "hcblaslib.h"
#include <amp.h>
#include <amp_math.h>
#define TILE_SIZE 256
using namespace concurrency;
using namespace concurrency::fast_math;
float sdot_HC(Concurrency::accelerator_view &accl_view, long n,
               Concurrency::array<float, 1> &xView, long incx, long xOffset,
               Concurrency::array<float, 1> &yView, long incy, long yOffset, float out)
{
  out = 0.0;
  // runtime sizes
  unsigned int tile_count = (n+TILE_SIZE-1) / TILE_SIZE;
  // simultaneous live threads
  const unsigned int thread_count = tile_count * TILE_SIZE;
  // global buffer (return type)
  concurrency::array<float,1> global_buffer(tile_count);
  concurrency::array_view<float,1> global_buffer_view(global_buffer);
  // configuration
  concurrency::extent<1> extent(thread_count);
  concurrency::parallel_for_each(
    extent.tile<TILE_SIZE>(),
    [=, &xView, &yView] (concurrency::tiled_index<TILE_SIZE> tid) restrict(amp)
  {
    // shared tile buffer
    tile_static float local_buffer[TILE_SIZE];
    // indexes
    int idx = tid.global[0];
    // this threads's shared memory pointer
    float& smem = local_buffer[ tid.local[0] ];
    // initialize local buffer
    smem = 0.0f;
    // fold data into local buffer
    while (idx < n) {
      // reduction of smem and X[idx] with results stored in smem
      smem += xView[xOffset + concurrency::index<1>(idx)] * yView[yOffset + concurrency::index<1>(idx)] ;
      // next chunk
      idx += thread_count;
    }
    // synchronize
    tid.barrier.wait_with_tile_static_memory_fence();
    // reduce all values in this tile
    unsigned int local = tid.local[0];
    float *mem = &smem;
    // unrolled for performance
    if (local < 128) { mem[0] = mem[0] + mem[128]; } tid.barrier.wait_with_tile_static_memory_fence();
    if (local <  64) { mem[0] = mem[0] + mem[ 64]; } tid.barrier.wait_with_tile_static_memory_fence();
    if (local <  32) { mem[0] = mem[0] + mem[ 32]; } tid.barrier.wait_with_tile_static_memory_fence();
    if (local <  16) { mem[0] = mem[0] + mem[ 16]; } tid.barrier.wait_with_tile_static_memory_fence();
    if (local <   8) { mem[0] = mem[0] + mem[  8]; } tid.barrier.wait_with_tile_static_memory_fence();
    if (local <   4) { mem[0] = mem[0] + mem[  4]; } tid.barrier.wait_with_tile_static_memory_fence();
    if (local <   2) { mem[0] = mem[0] + mem[  2]; } tid.barrier.wait_with_tile_static_memory_fence();
    if (local <   1) { mem[0] = mem[0] + mem[  1]; } tid.barrier.wait_with_tile_static_memory_fence();
    // only 1 thread per tile does the inter tile communication*/
    if (tid.local[0] == 0) {
  // write to global buffer in this tiles
      global_buffer_view[ tid.tile[0] ] = smem;
    }
  });
  // 2nd pass reduction
    for(int i = 0; i< tile_count; i++)
    {
        out = (isnan(out) || isinf(out)) ? 0 : out;
    	out += global_buffer_view[ i ] ;
    }
    return out;
}

float sdot_HC(Concurrency::accelerator_view &accl_view, long n,
               Concurrency::array<float, 1> &xView, long incx, long xOffset,
               Concurrency::array<float, 1> &yView, long incy, long yOffset, float out, 
               const long X_batchOffset, const long Y_batchOffset, const int batchSize)

{
  out = 0.0;
  // runtime sizes
  unsigned int tile_count = (n+TILE_SIZE-1) / TILE_SIZE;
  // simultaneous live threads
  const unsigned int thread_count = tile_count * TILE_SIZE;
  // global buffer (return type)
  concurrency::array<float,1> global_buffer(batchSize * tile_count);
  concurrency::array_view<float,1> global_buffer_view(global_buffer);
  // configuration
  concurrency::extent<2> extent(batchSize, thread_count);
  concurrency::parallel_for_each(
    extent.tile<1, TILE_SIZE>(),
    [=, &xView, &yView] (concurrency::tiled_index<1, TILE_SIZE> tid) restrict(amp)
  {
    // shared tile buffer
    tile_static float local_buffer[TILE_SIZE];
    // indexes
    int elt = tid.tile[0];
    int idx = tid.global[1];
    // this threads's shared memory pointer
    float& smem = local_buffer[ tid.local[1] ];
    // initialize local buffer
    smem = 0.0f;
    // fold data into local buffer
    while (idx < n) {
      // reduction of smem and X[idx] with results stored in smem
      smem += xView[xOffset + X_batchOffset * elt + concurrency::index<1>(idx)] * yView[yOffset + Y_batchOffset * elt + concurrency::index<1>(idx)] ;
      // next chunk
      idx += thread_count;
    }
    // synchronize
    tid.barrier.wait_with_tile_static_memory_fence();
    // reduce all values in this tile
    unsigned int local = tid.local[1];
    float *mem = &smem;
    // unrolled for performance
    if (local < 128) { mem[0] = mem[0] + mem[128]; } tid.barrier.wait_with_tile_static_memory_fence();
    if (local <  64) { mem[0] = mem[0] + mem[ 64]; } tid.barrier.wait_with_tile_static_memory_fence();
    if (local <  32) { mem[0] = mem[0] + mem[ 32]; } tid.barrier.wait_with_tile_static_memory_fence();
    if (local <  16) { mem[0] = mem[0] + mem[ 16]; } tid.barrier.wait_with_tile_static_memory_fence();
    if (local <   8) { mem[0] = mem[0] + mem[  8]; } tid.barrier.wait_with_tile_static_memory_fence();
    if (local <   4) { mem[0] = mem[0] + mem[  4]; } tid.barrier.wait_with_tile_static_memory_fence();
    if (local <   2) { mem[0] = mem[0] + mem[  2]; } tid.barrier.wait_with_tile_static_memory_fence();
    if (local <   1) { mem[0] = mem[0] + mem[  1]; } tid.barrier.wait_with_tile_static_memory_fence();
    // only 1 thread per tile does the inter tile communication*/
    if (tid.local[1] == 0) {
  // write to global buffer in this tiles
      global_buffer_view[ elt * tile_count + tid.tile[1] ] = smem;
    }
  });
  // 2nd pass reduction
  for(int i = 0; i< tile_count * batchSize; i++)
    {
        out = (isnan(out) || isinf(out)) ? 0 : out;
        out += global_buffer_view[ i ] ;
    }
    return out;
}


// SDOT call Type I - SSCAL Inputs and Outputs are host float pointers 
hcblasStatus Hcblaslibrary :: hcblas_sdot(const int N, float *X, const int incX, const long xOffset, 
                                             float *Y, const int incY, const long yOffset, float *dot)
{

    if (Y == NULL || X == NULL || N <= 0 ) {
        return HCBLAS_INVALID;
    }
    int lenX = 1 + (N - 1) * abs(incX);
    int lenY = 1 + (N - 1) * abs(incY);
    Concurrency::array<float, 1> xView(lenX, X);
    Concurrency::array<float, 1> yView(lenY, Y);
    std::vector<float> HostX(lenX);
    std::vector<float> HostY(lenY);
    for( int i = 0; i < lenX; i++)
	HostX[i] = X[i];
    for( int i = 0; i < lenY; i++)
        HostY[i] = Y[i];
    Concurrency::copy(begin(HostX), end(HostX), xView);
    Concurrency::copy(begin(HostY), end(HostY), yView);
    std::vector<Concurrency::accelerator>acc = Concurrency::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view());
    *dot = sdot_HC(accl_view, N, xView, incX, xOffset, yView, incY, yOffset, *dot);
    return HCBLAS_SUCCESS;
}

// SDOT Call Type II: Inputs and outputs are C++ HC float array containers
hcblasStatus Hcblaslibrary :: hcblas_sdot(Concurrency::accelerator_view &accl_view, const int N,
                                             Concurrency::array<float> &X, const int incX, const long xOffset,
                                             Concurrency::array<float> &Y, const int incY, const long yOffset, float &dot)

{
    /*Check the conditions*/
    if (N <= 0 ){
        return HCBLAS_INVALID;
    }
    dot = sdot_HC(accl_view, N, X, incX, xOffset, Y, incY, yOffset, dot);
    return HCBLAS_SUCCESS;

}

// SDOT TYpe III - Overloaded function with arguments related to batch processing 
 hcblasStatus Hcblaslibrary :: hcblas_sdot(Concurrency::accelerator_view &accl_view, const int N, 
                                              Concurrency::array<float> &X, const int incX, const long xOffset,
                                              Concurrency::array<float> &Y, const int incY, const long yOffset, float &dot, 
                                              const long X_batchOffset, const long Y_batchOffset, const int batchSize)
{
    /*Check the conditions*/
    if (  N <= 0 ){
        return HCBLAS_INVALID;
    }
    dot = sdot_HC(accl_view, N, X, incX, xOffset, Y, incY, yOffset, dot, X_batchOffset, Y_batchOffset, batchSize);

    return HCBLAS_SUCCESS;

}


