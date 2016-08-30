#include <unistd.h>
#include "autogemm.h"

using namespace std;

/* isRowKernel():  Return true if
  *                     - macroTileNumRows = 1
  *                     - guards around gA -> lA
  *                     - guards around gC[gRow,:] = rC[row,:]
 */
 bool AutogemmKernel::isRowKernel(kernTypes* kernelType) {

  if ( this->tileNumRows * this->microtileNumRows == kernelType->macrotileNumRows)
      return false;  //normal tile kernel
  else if ( kernelType->macrotileNumRows == 1)
      return true; // single row kernel
  else {
        cout << "ERROR: tileNumRows= " << this->tileNumRows <<" microTileNumRows=" \
                  <<this->microtileNumRows << "and macroTileNumRows=" << kernelType->macrotileNumRows \
                  <<" doesn't make sense\n" << endl;
        return false; // ERROR
  }
}

/* isColKernel():  Return true if
  *                    - macroTileNumCols = 1
  *                    - guards around gB -> lB
  *                    - guards around gC[:,gCol] = rC[:,Col]
 */
 bool AutogemmKernel::isColKernel(kernTypes* kernelType) {

  if ( this->tileNumCols * this->microtileNumCols == kernelType->macrotileNumCols)
      return false;  //normal tile kernel
  else if ( kernelType->macrotileNumCols == 1)
      return true; // single Col kernel
  else {
        cout << "ERROR: tileNumCols= " << this->tileNumCols <<" microtileNumCols=" \
                  <<this->microtileNumCols << "and macrotileNumCols=" << kernelType->macrotileNumCols \
                  <<" doesn't make sense\n" << endl;
        return false; // ERROR
  }
}

/* getKernelName():  Return the generated Kernel name based on
 *                                - TransA, TransB
 *                                - Beta value
 *                                - microtileSize * macrotileSize
 *                                - unroll
 */
 std::string  AutogemmKernel::getKernelName(kernTypes* kernType) {

    assert(kernType != NULL);
    char* kName = new char[512];
    std::string kernStr;

    /* NonTile parameters construct */
    kernStr = kernStr + this->precision + "gemm_"  + \
                    (this->isColMajor ? "Col_" : "Row_") + \
                    (this->isTransA ? 'T' : 'N') + \
                    (this->isTransB ? 'T' : 'N') + "_B" + \
                    toString(this->isBeta_nonZero) + "_" ;

    /* Tile Parameters construct */
    // Using defaultTile format TODO : Add Row, Col, Corner formats
    if ( !(this->isRowKernel(kernType)) && !(this->isColKernel(kernType))) {
      sprintf(kName, this->nameFormatTile, kernType->macrotileNumRows , kernType->macrotileNumCols, this->unroll);
    }
    if (isRowKernel(kernType) && !(this->isColKernel(kernType))) {
      sprintf(kName, this->nameFormatRow, kernType->macrotileNumRows , kernType->macrotileNumCols, this->unroll);
    }
    if (!(isRowKernel(kernType)) && isColKernel(kernType)) {
      sprintf(kName, this->nameFormatCol, kernType->macrotileNumRows , kernType->macrotileNumCols, this->unroll);
    }
    if (isRowKernel(kernType) && isColKernel(kernType)) {
      sprintf(kName, this->nameFormatCorner, kernType->macrotileNumRows , kernType->macrotileNumCols, this->unroll);
    }

    std::string str(kName);
    kernStr = kernStr + str;
    return kernStr;

 }

/* setFileName():  Set the filename onto which the kernel functions are about to write
 *                 using the kernel parameters
 *
 */
void AutogemmKernel::setFileName() {

    std::string fName;
 
    /* Append the kernel function and extension */
    char* kName = new char[512];

    /* NonTile parameters construct */
    fName = fName + this->precision + "gemm_"  + \
                    (this->isColMajor ? "Col_" : "Row_") + \
		    (this->isTransA ? 'T' : 'N') + \
		    (this->isTransB ? 'T' : 'N') + "_B" + \
		    toString(this->isBeta_nonZero) + "_" ;

    /* Tile Parameters construct */
    sprintf(kName, "MX%03d_NX%03d", this->macrotileNumRows , this->macrotileNumCols);

    std::string str(kName);
    fName = fName + str + "_src.cpp";
    this->fileName = fName;
}

/* getFileName(): Returns the file name
 *
 */
std::string AutogemmKernel::getFileName() {

  return this->fileName;

}

/*
 * getKernelLib():  Return Kernellib name 
 */
std::string AutogemmKernel::getKernelLib() {
 
  return this->kernelLib;

}

/* initKernParam():   Initialize the tile and non-tile parameters of
 *                                  AutogemmKernel object
 *  TODO : Generalize the init logic
 */
void AutogemmKernel::initKernParam(AutogemmKernel* gemmKernel, hcblasOrder order, 
                                   hcblasTranspose typeA, hcblasTranspose typeB,
                                   const float &beta) {

   // TODO: Generalize the precision assignment 
   gemmKernel->precision = 's';

   gemmKernel->isColMajor = (order == ColMajor) ? 1 : 0  ;
   gemmKernel->isTransA = (typeA == NoTrans) ? 0 : 1;
   gemmKernel->isTransB = (typeB == NoTrans) ? 0 : 1;
   gemmKernel->isBeta_nonZero = (beta == 0) ? 0 : 1;

   // Default Kernel tile elements
   // These elements will be changed in the kernelSelection function
   gemmKernel->tileNumRows = 16;
   gemmKernel->tileNumCols = 16;
   gemmKernel->microtileNumRows = 6;
   gemmKernel->microtileNumCols = 6;
   gemmKernel->unroll = 16;

   gemmKernel->macrotileNumRows = gemmKernel->tileNumRows * gemmKernel->microtileNumRows;
   gemmKernel->macrotileNumCols = gemmKernel->tileNumCols * gemmKernel->microtileNumCols;

   // Always the generate "libblasKernel.so" file
   gemmKernel->kernelLib = "libblaskernel.so";
}

/* ValidateKernParam():   Return CRIT_ERR if following
 *                                          conditions are not met
 *                                            - unroll : {1, 8, 16}
 *                                            - precision : {'s', 'd', 'c', 'z'}
 *                                            - microtilesize : <= 6
 *                                            - tilesize : {8, 16}
 */
int AutogemmKernel::validateKernParam(AutogemmKernel* gemmKernel) {

  char prec = gemmKernel->precision;
  if (prec != 's' && prec != 'd' && prec != 'c' && prec != 'z')  {
    cout << "Parameter validation failed: Invalid precision specified" <<endl;
    return CRIT_ERR;
  }

  if (gemmKernel->tileNumRows != 8  && gemmKernel->tileNumRows != 16) {
    cout <<"Parameter validation failed: Invalid tile size specified" <<endl;
    return CRIT_ERR;
  }

   if (gemmKernel->tileNumCols != 8  && gemmKernel->tileNumCols != 16) {
    cout <<"Parameter validation failed: Invalid tile size specified" <<endl;
    return CRIT_ERR;
  }

  if (gemmKernel->microtileNumCols > 6 || gemmKernel->microtileNumRows > 6) {
    cout << "Parameter validation failed: Microtilesize doesnt make sense" << endl;
    return CRIT_ERR;
  }

  uint unroll = gemmKernel->unroll;
  if (unroll != 1 && unroll != 8 && unroll != 16) {
    cout << "Parameters validation failed: Invalid Unroll limit" <<endl;
    return CRIT_ERR;
  }
  return SUCCESS;

}

/*
 * writeKernel():  This function
 *                  1) Determines number of kernels required for a given size
 *                  2) Calls makegemmKernel to generate it
 *
 */
void AutogemmKernel::writeKernel(AutogemmKernel* gemmKernel, uint M, uint N, uint K) {


  // Temporarily access the microtile elements
  uint macrotileNumRows = gemmKernel->macrotileNumRows;
  uint macrotileNumCols = gemmKernel->macrotileNumCols;

  bool needTileKernel = M/macrotileNumRows > 0 && N/macrotileNumCols > 0;
  bool needRowKernel = M%macrotileNumRows > 0 && N/macrotileNumCols > 0;
  bool needColKernel = N%macrotileNumCols > 0 && M/macrotileNumRows > 0;
  bool needCornerKernel = M%macrotileNumRows > 0 && N%macrotileNumCols > 0;

  std::string kStr;

  if (needTileKernel) {
    if (gemmKernel->tileKernel == NULL)
      gemmKernel->tileKernel = (kernTypes *)malloc(sizeof(kernTypes));
    gemmKernel->tileKernel->macrotileNumRows = gemmKernel->macrotileNumRows;
    gemmKernel->tileKernel->macrotileNumCols = gemmKernel->macrotileNumCols;
    gemmKernel->makeGemmKernel(gemmKernel, gemmKernel->tileKernel, kStr);
  }

  if (needRowKernel) {

    // Set macrotile value for Row Kernel generation
    if (gemmKernel->rowKernel == NULL)
      gemmKernel->rowKernel = (kernTypes *)malloc(sizeof(kernTypes));
    gemmKernel->rowKernel->macrotileNumRows = 1;
    gemmKernel->rowKernel->macrotileNumCols = gemmKernel->macrotileNumCols;

    // Generate the kernel with the same string
    gemmKernel->makeGemmKernel(gemmKernel, gemmKernel->rowKernel, kStr);
  }

  if (needColKernel) {

    // Set macrotile value for Row Kernel generation
    if (gemmKernel->colKernel == NULL)
      gemmKernel->colKernel = (kernTypes *)malloc(sizeof(kernTypes));
    gemmKernel->colKernel->macrotileNumRows = gemmKernel->macrotileNumRows;
    gemmKernel->colKernel->macrotileNumCols = 1;

    // Generate the kernel with the same string
    gemmKernel->makeGemmKernel(gemmKernel, gemmKernel->colKernel, kStr);
  }

  if (needCornerKernel) {

    // Set macrotile value for Corner Kernel generation
    if (gemmKernel->cornerKernel == NULL)
      gemmKernel->cornerKernel = (kernTypes *)malloc(sizeof(kernTypes));
    gemmKernel->cornerKernel->macrotileNumRows = 1;
    gemmKernel->cornerKernel->macrotileNumCols = 1;

    // Generate the kernel with the same string
    gemmKernel->makeGemmKernel(gemmKernel, gemmKernel->cornerKernel, kStr);
  }

  system("mkdir -p /home/sujitha/Documents/hcblas/lib/src/blas/autogemm/sources");
  ofstream kFile;
  kFile.open(gemmKernel->getFileName());
  kFile << kStr;
  kFile.close();

}
/* compileKernel():  This function 
 *                    1) picks either COMPILER default path or user path  
 *                      and compile the generated kernel at runtime
 *
 *                    2) Load the symbol of compiled kernel and invoke it                  
 *                    
*/
int AutogemmKernel::compileKernel(AutogemmKernel* gemmKernel) {

    // Check if the default compiler path exists
    std::string execCmd; 
    std::string hcblasIncPath;
    std::string autogemmSourcePath;
    char fname[256] = "/opt/rocm/hcc/bin/clang++";
    char cwd[1024];

    if (getcwd(cwd, sizeof(cwd)) == NULL) {
       perror("getcwd() error");
       return CRIT_ERR;
    }

    hcblasIncPath = hcblasIncPath + cwd + "/./../../lib/include/";
    autogemmSourcePath = autogemmSourcePath + cwd + "/./../../lib/src/blas/autogemm/sources/";

    if ( access ( getenv ("MCWHCCBUILD"), F_OK ) != -1) {
      // TODO: This path shall be removed. User shall build from default path
      // compiler doesn't exist in default path
      // check if user has specified compiler build path
      // build_mode = true;
      char* compilerPath = getenv ("MCWHCCBUILD");
      string Path(compilerPath);
      execCmd = Path + "/compiler/bin/clang++ `" + Path + "/bin/hcc-config --build --cxxflags --ldflags --shared` -lhc_am -I" + hcblasIncPath + " " + autogemmSourcePath + gemmKernel->getFileName() + " -o " + autogemmSourcePath + gemmKernel->getKernelLib() ;
    }
    else if( access( fname, F_OK ) != -1 ) {
      // compiler exists
      // install_mode = true;
      string Path = "/opt/rocm/hcc/bin/";
      execCmd = Path + "/clang++ `" + Path + "/hcc-config --install --cxxflags --ldflags --shared` -I" + hcblasIncPath + " " + autogemmSourcePath + gemmKernel->getFileName() + " -o " + autogemmSourcePath + gemmKernel->getKernelLib();
    }
    else {
      // No compiler found
      std::cout << "HCC compiler not found" << std::endl;
      return CRIT_ERR;
    }

   system(execCmd.c_str());

   return SUCCESS;

}

/* invokeKernel():  This function does the following
 *                     - Open the kernel symbol and return a valid handle
 *                     - Load the symbol with the handle and assign it to func pointer
 *                     - invoke the kernel by calling the func pointer with arguments
 *                     - Close the symbol
 */
int AutogemmKernel::invokeKernel(AutogemmKernel* gemmKernel, hc::accelerator_view &accl_view,
                                 const uint M, const uint N, const uint K, const float &alpha,
                                 float *A, const uint lda, float *B, const uint ldb, const float &beta,
                                 float *C, const uint ldc, const uint aOffset, const uint bOffset, 
                                 const uint cOffset) {

  // loads the module specified by FilePath into the executing process's address space 
  void* kernelHandle = dlopen("/home/sujitha/Documents/hcblas/lib/src/blas/autogemm/sources/libblaskernel.so", RTLD_NOW);
  if(!kernelHandle) {
    std::cout << "Failed to load Kernel: " << gemmKernel->getKernelLib().c_str() << std::endl;
    return CRIT_ERR;
  }

  // Define the function pointer to access the autogemm kernel
  typedef hcblasStatus (hcblas_sgemm_fn_ptr)(hc::accelerator_view accl_view, float const * A, float const * B, \
                                     float * C, float const alpha, float const beta, uint const M,\
                                     uint const N, uint const K, uint const lda, uint const ldb, \
                                     uint const ldc, uint const aOffset, uint const bOffset,\
                                     uint const cOffset);
  hcblas_sgemm_fn_ptr* hcblas_sgemm_call = NULL;

  std::string funcName = gemmKernel->getKernelName(gemmKernel->tileKernel);
  // obtain the address of a symbol defined within an object made accessible through a dlopen() cal
  hcblas_sgemm_call = (hcblas_sgemm_fn_ptr*) dlsym(kernelHandle, funcName.c_str());

  // Catch the symbol load error with dlerror for user 
  if (!hcblas_sgemm_call) {
     cout << "Loading the Autogemm Call " << gemmKernel->getKernelName(gemmKernel->tileKernel) << "Failed" << endl;
  }
  char *err = dlerror();
  if (err) {
    std::cout << "Failed to locate " << gemmKernel->getKernelName(gemmKernel->tileKernel) << " kernel : " << err << endl;
    return CRIT_ERR;
  }
  free(err);
 
  // If no errors, invoke the function with arguments
  hcblas_sgemm_call(accl_view, A, B, C, alpha, beta, M, N, K, lda, ldb, ldc, aOffset, bOffset, cOffset);
  return SUCCESS;
}

/* hcblasAutogemmCall():  Top level function called by the sgemm wrapper to
 *                        invoke the Autogemm routines.
 *                           Does the following -
 *                            1) Create a autogemm object instantiate with default parameters
 *                            2) Initialize the Kernel Parameters
 *                            3) Validate the setted kernel parameters
 *                            4) Select the Autogemm kernel for the given size
 *                            5) Compile and generate the shared library
 *                            6) Load the function symbol from the shared library and
 *                                invoke the Autogemm kernel
 */
//TODO : Add templates to this function
int hcblasAutogemmCall(hc::accelerator_view &accl_view, hcblasOrder order,
                       hcblasTranspose typeA, hcblasTranspose typeB, const int M,
                       const uint N, const uint K, const float &alpha, float *A,
                       const uint lda, float *B, const uint ldb, const float &beta,
                       float *C, const uint ldc, const uint aOffset, const uint bOffset,
                       const uint cOffset) 
{

    // Instantiate the autogemm kernel
    AutogemmKernel* gemmKernel = new AutogemmKernel();

    // Initialize and validate the kernel parameters
    // TODO: Move the init function into constructor
    // TODO: Remove the validate function call here
    gemmKernel->initKernParam(gemmKernel, order, typeA, typeB, beta);
    gemmKernel->validateKernParam(gemmKernel);

    gemmKernel->selectMicrotileLogic(gemmKernel, order, typeA, typeB, M, N, K, beta);
    gemmKernel->writeKernel(gemmKernel, M, N, K);

    // Compile and invoke the Autogemm Kernel 
    gemmKernel->compileKernel(gemmKernel);
    gemmKernel->invokeKernel(gemmKernel, accl_view, M, N, K, alpha, A, lda, B, ldb, beta,
                             C, ldc, aOffset, bOffset, cOffset);
    return 0;
}

int main() {

  return 0;

}
