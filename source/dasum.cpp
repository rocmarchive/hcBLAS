#include "hcblaslib.h"
#include <amp.h>
#include <amp_math.h>
#define TILE_SIZE 256
using namespace concurrency;

double dasum_HC(Concurrency::accelerator_view &accl_view,
               long n, Concurrency::array<double, 1> &xView, long incx, long xOffset, double Y)
{
  Y = 0.0;
  // runtime sizes
  unsigned int tile_count = (n+TILE_SIZE-1) / TILE_SIZE;
  // simultaneous live threads
  const unsigned int thread_count = tile_count * TILE_SIZE;
  // global buffer (return type)
  concurrency::array<double,1> global_buffer(tile_count);
  concurrency::array_view<double,1> global_buffer_view(global_buffer);
  // configuration
  concurrency::extent<1> extent(thread_count);
  concurrency::parallel_for_each(
    extent.tile<TILE_SIZE>(),
    [=, &xView] (concurrency::tiled_index<TILE_SIZE> tid) restrict(amp)
  {
    // shared tile buffer
    tile_static double local_buffer[TILE_SIZE];
    // indexes
    int idx = tid.global[0];
    // this threads's shared memory pointer
    double& smem = local_buffer[ tid.local[0] ];
    // initialize local buffer
    smem = 0.0f;
    // fold data into local buffer
    while (idx < n) {
      // reduction of smem and X[idx] with results stored in smem
      smem += Concurrency::fast_math::fabs(xView[xOffset + concurrency::index<1>(idx)]);
      // next chunk
      idx += thread_count;
    }
    // synchronize
    tid.barrier.wait_with_tile_static_memory_fence();
    // reduce all values in this tile
    unsigned int local = tid.local[0];
    double *mem = &smem;
    // unrolled for performance
    if (local < 128) { mem[0] = mem[0] + mem[128]; } tid.barrier.wait_with_tile_static_memory_fence();
    if (local <  64) { mem[0] = mem[0] + mem[ 64]; } tid.barrier.wait_with_tile_static_memory_fence();
    if (local <  32) { mem[0] = mem[0] + mem[ 32]; } tid.barrier.wait_with_tile_static_memory_fence();
    if (local <  16) { mem[0] = mem[0] + mem[ 16]; } tid.barrier.wait_with_tile_static_memory_fence();
    if (local <   8) { mem[0] = mem[0] + mem[  8]; } tid.barrier.wait_with_tile_static_memory_fence();
    if (local <   4) { mem[0] = mem[0] + mem[  4]; } tid.barrier.wait_with_tile_static_memory_fence();
    if (local <   2) { mem[0] = mem[0] + mem[  2]; } tid.barrier.wait_with_tile_static_memory_fence();
    if (local <   1) { mem[0] = mem[0] + mem[  1]; } tid.barrier.wait_with_tile_static_memory_fence();
    // only 1 thread per tile does the inter tile communication
    if (tid.local[0] == 0) {
      // write to global buffer in this tiles
      global_buffer_view[ tid.tile[0] ] = smem;
    }
  } );
  // 2nd pass reduction
   for(int i = 0; i< tile_count; i++)
    {
        Y += global_buffer_view[ i ] ;
    }
  return Y;
}

double dasum_HC(Concurrency::accelerator_view &accl_view,
               long n, Concurrency::array<double, 1> &xView, long incx, long xOffset, double Y,
	       long X_batchOffset, int batchSize)
{
  Y = 0.0;
  // runtime sizes
  unsigned int tile_count = (n+TILE_SIZE-1) / TILE_SIZE;
  // simultaneous live threads
  const unsigned int thread_count = tile_count * TILE_SIZE;
  // global buffer (return type)
  concurrency::array<double,1> global_buffer(batchSize * tile_count);
  concurrency::array_view<double,1> global_buffer_view(global_buffer);
  // configuration
  concurrency::extent<2> extent(batchSize, thread_count);
  concurrency::parallel_for_each(
    extent.tile<1, TILE_SIZE>(),
    [=, &xView] (concurrency::tiled_index<1, TILE_SIZE> tid) restrict(amp)
  {
    // shared tile buffer
    tile_static double local_buffer[TILE_SIZE];
    // indexes
    int elt = tid.tile[0];    
    int idx = tid.global[1];
    // this threads's shared memory pointer
    double& smem = local_buffer[ tid.local[1] ];
    // initialize local buffer
    smem = 0.0f;
    // fold data into local buffer
    while (idx < n) {
      // reduction of smem and X[idx] with results stored in smem
      smem += Concurrency::fast_math::fabs(xView[xOffset + X_batchOffset * elt + concurrency::index<1>(idx)]);
      // next chunk
      idx += thread_count;
    }
    // synchronize
    tid.barrier.wait_with_tile_static_memory_fence();
    // reduce all values in this tile
    unsigned int local = tid.local[1];
    double *mem = &smem;
    // unrolled for performance
    if (local < 128) { mem[0] = mem[0] + mem[128]; } tid.barrier.wait_with_tile_static_memory_fence();
    if (local <  64) { mem[0] = mem[0] + mem[ 64]; } tid.barrier.wait_with_tile_static_memory_fence();
    if (local <  32) { mem[0] = mem[0] + mem[ 32]; } tid.barrier.wait_with_tile_static_memory_fence();
    if (local <  16) { mem[0] = mem[0] + mem[ 16]; } tid.barrier.wait_with_tile_static_memory_fence();
    if (local <   8) { mem[0] = mem[0] + mem[  8]; } tid.barrier.wait_with_tile_static_memory_fence();
    if (local <   4) { mem[0] = mem[0] + mem[  4]; } tid.barrier.wait_with_tile_static_memory_fence();
    if (local <   2) { mem[0] = mem[0] + mem[  2]; } tid.barrier.wait_with_tile_static_memory_fence();
    if (local <   1) { mem[0] = mem[0] + mem[  1]; } tid.barrier.wait_with_tile_static_memory_fence();
    // only 1 thread per tile does the inter tile communication
    if (tid.local[1] == 0) {
      // write to global buffer in this tiles
      global_buffer_view[ tile_count * elt + tid.tile[1] ] = smem;
    }
  } );
  // 2nd pass reduction
   for(int i = 0; i< tile_count * batchSize ; i++)
    {
        Y += global_buffer_view[ i ] ;
    }
  return Y;
}

// DASUM call Type I - Inputs and Outputs are host float pointers 
hcblasStatus Hcblaslibrary :: hcblas_dasum(const int N, double *X, const int incX, const long xOffset, double *Y)
{

    if ( X == NULL || N <= 0 ) {
        return HCBLAS_INVALID;
    }

    int lenX = 1 + (N - 1) * abs(incX);
    Concurrency::array<double, 1> xView(lenX, X);
    std::vector<double> HostX(lenX);
    for( int i = 0; i < lenX; i++)
	HostX[i] = X[i];
    Concurrency::copy(begin(HostX), end(HostX), xView);
    std::vector<Concurrency::accelerator>acc = Concurrency::accelerator::get_all();
    accelerator_view accl_view = (acc[1].create_view());
    *Y = dasum_HC(accl_view, N, xView, incX, xOffset, *Y);
    return HCBLAS_SUCCESS;

}

// DASUM Call Type II: Inputs and outputs are C++ HC float array containers
hcblasStatus Hcblaslibrary :: hcblas_dasum(Concurrency::accelerator_view &accl_view, const int N,
                                              Concurrency::array<double> &X, const int incX,
                                              const long xOffset, double &Y)
{
    /*Check the conditions*/
    if (  N <= 0 ){
        return HCBLAS_INVALID;
    }
    Y = dasum_HC(accl_view, N, X, incX, xOffset, Y);
    return HCBLAS_SUCCESS;

}

// SASUM TYpe III - Overloaded function with arguments related to batch processing 
hcblasStatus Hcblaslibrary :: hcblas_dasum(Concurrency::accelerator_view &accl_view, const int N,
                                              Concurrency::array<double> &X, const int incX,
                                              const long xOffset, double &Y, const long X_batchOffset, const int batchSize)
{
    /*Check the conditions*/
    if (  N <= 0 ){
        return HCBLAS_INVALID;
    }
    Y = dasum_HC(accl_view, N, X, incX, xOffset, Y, X_batchOffset, batchSize);
    return HCBLAS_SUCCESS;

}

