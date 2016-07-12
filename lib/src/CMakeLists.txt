
CMAKE_MINIMUM_REQUIRED (VERSION 2.8.8) 
MESSAGE(STATUS "CMAKE VERSION ${CMAKE_VERSION}")

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Find HCC compiler
FIND_PACKAGE(HC++ 1.0 REQUIRED)

ADD_SUBDIRECTORY(blas)

#Setting a Project name
SET(PROJECT_NAME "libhcblas")
PROJECT (${PROJECT_NAME})
MESSAGE(STATUS "PROJECT NAME    : ${PROJECT_NAME}")

find_package(PythonInterp REQUIRED)

################################################################################
# AutoGemm Begin
################################################################################
# AutoGemm scripts and out files
set(AUTOGEMM_SCRIPTS
  ${CMAKE_SOURCE_DIR}/lib/src/blas/autogemm/AutoGemm.py
  ${CMAKE_SOURCE_DIR}/lib/src/blas/autogemm/AutoGemmParameters.py
  ${CMAKE_SOURCE_DIR}/lib/src/blas/autogemm/Common.py
  ${CMAKE_SOURCE_DIR}/lib/src/blas/autogemm/Includes.py
  ${CMAKE_SOURCE_DIR}/lib/src/blas/autogemm/hcblasKernel.py
  ${CMAKE_SOURCE_DIR}/lib/src/blas/autogemm/KernelParameters.py
  ${CMAKE_SOURCE_DIR}/lib/src/blas/autogemm/KernelSelection.py
  ${CMAKE_SOURCE_DIR}/lib/src/blas/autogemm/KernelsToPreCompile.py
)
set(AUTOGEMM_HEADERS
  ${CMAKE_BINARY_DIR}/include/AutoGemmIncludes/AutoGemmKernelSelection.h
  ${CMAKE_BINARY_DIR}/include/AutoGemmIncludes/AutoGemmKernelSelectionSpecific.h
  ${CMAKE_BINARY_DIR}/include/AutoGemmIncludes/AutoGemmKernelSources.h
)

set(AUTOGEMM_SRC
  ${CMAKE_BINARY_DIR}/include/AutoGemmIncludes/AutoGemmKernelSelection.cpp
  ${CMAKE_BINARY_DIR}/include/AutoGemmIncludes/AutoGemmKernelSelectionSpecific.cpp
  ${CMAKE_BINARY_DIR}/include/AutoGemmIncludes/AutoGemmKernelSources.cpp
)

# AutoGemm options for pre-compiling kernels
option( PRECOMPILE_GEMM_PRECISION_SGEMM "AutoGemm: pre-compile sgemm kernels" OFF)
option( PRECOMPILE_GEMM_PRECISION_DGEMM "AutoGemm: pre-compile dgemm kernels" OFF)
option( PRECOMPILE_GEMM_PRECISION_CGEMM "AutoGemm: pre-compile cgemm kernels" OFF)
option( PRECOMPILE_GEMM_PRECISION_ZGEMM "AutoGemm: pre-compile zgemm kernels" OFF)

option( PRECOMPILE_GEMM_TRANS_NN "AutoGemm: pre-compile NN transpose cases" OFF)
option( PRECOMPILE_GEMM_TRANS_NT "AutoGemm: pre-compile NT transpose cases" OFF)
option( PRECOMPILE_GEMM_TRANS_NC "AutoGemm: pre-compile NC transpose cases" OFF)
option( PRECOMPILE_GEMM_TRANS_TN "AutoGemm: pre-compile TN transpose cases" OFF)
option( PRECOMPILE_GEMM_TRANS_TT "AutoGemm: pre-compile TT transpose cases" OFF)
option( PRECOMPILE_GEMM_TRANS_TC "AutoGemm: pre-compile TC transpose cases" OFF)
option( PRECOMPILE_GEMM_TRANS_CN "AutoGemm: pre-compile CN transpose cases" OFF)
option( PRECOMPILE_GEMM_TRANS_CT "AutoGemm: pre-compile CT transpose cases" OFF)
option( PRECOMPILE_GEMM_TRANS_CC "AutoGemm: pre-compile CC transpose cases" OFF)
    
option( INSTALL_SRC "Install source tree" OFF )
    
#preCompile precision selected? 
#Enable Autogemm by default TODO: ADD USER OPTION TO TURN ON AUTOGEMM
    
set( PRECOMPILE_GEMM_PRECISION_SGEMM ON)
set( PRECOMPILE_GEMM_PRECISION_DGEMM ON)
set( PRECOMPILE_GEMM_PRECISION_CGEMM ON)
set( PRECOMPILE_GEMM_PRECISION_ZGEMM ON)
set( PRECOMPILE_GEMM_PRECISION_SELECTED ON)

# PreCompile transpose selected?
set( PRECOMPILE_GEMM_TRANS_NN ON)
set( PRECOMPILE_GEMM_TRANS_NT ON)
set( PRECOMPILE_GEMM_TRANS_NC ON)
set( PRECOMPILE_GEMM_TRANS_TN ON)
set( PRECOMPILE_GEMM_TRANS_TT ON)
set( PRECOMPILE_GEMM_TRANS_TC ON)
set( PRECOMPILE_GEMM_TRANS_CN ON)
set( PRECOMPILE_GEMM_TRANS_CT ON)
set( PRECOMPILE_GEMM_TRANS_CC ON)
set( PRECOMPILE_GEMM_TRANS_SELECTED ON)

# PreCompile is valid and active?
set( PRECOMPILE_GEMM_ACTIVE OFF)
if ( PRECOMPILE_GEMM_PRECISION_SELECTED
    AND PRECOMPILE_GEMM_TRANS_SELECTED)
  # valid selection
  set( PRECOMPILE_GEMM_ACTIVE ON)
  MESSAGE( STATUS "AutoGemm-PreCompile: selected kernels will be pre-compiled." )
elseif(NOT PRECOMPILE_GEMM_PRECISION_SELECTED
    AND NOT PRECOMPILE_GEMM_TRANS_SELECTED)
  MESSAGE( STATUS "AutoGemm-PreCompile: no kernels to be pre-compiled." )
else()
  MESSAGE( SEND_ERROR "AutoGemm-PreCompile: To pre-compile gemm kernels, select at lease one option from each of PRECOMPILE_GEMM_PRECISION_* and PRECOMPILE_GEMM_TRANS_*; otherwise, unselect all PRECOMPILE_GEMM_* options to not pre-compile any gemm kernels." )
endif()

# build commandline argument for AutoGemm
set( AGPC_ARGS --output ${CMAKE_BINARY_DIR}/include)

if ( PRECOMPILE_GEMM_ACTIVE )
  # precisions
  set(AGPC_ARGS ${AGPC_ARGS} --precisions )
  if (PRECOMPILE_GEMM_PRECISION_SGEMM)
    set(AGPC_ARGS ${AGPC_ARGS} s )
  endif()
  if (PRECOMPILE_GEMM_PRECISION_DGEMM)
    set(AGPC_ARGS ${AGPC_ARGS} d )
  endif()
  if (PRECOMPILE_GEMM_PRECISION_CGEMM)
    set(AGPC_ARGS ${AGPC_ARGS} c )
  endif()
  if (PRECOMPILE_GEMM_PRECISION_ZGEMM)
    set(AGPC_ARGS ${AGPC_ARGS} z )
  endif()

  # orders
  set(AGPC_ARGS ${AGPC_ARGS} --orders ColMajor )

  # transposes
  set(AGPC_ARGS ${AGPC_ARGS} --transposes )
  if (PRECOMPILE_GEMM_TRANS_NN)
    set(AGPC_ARGS ${AGPC_ARGS} NN )
  endif()
  if (PRECOMPILE_GEMM_TRANS_NT)
    set(AGPC_ARGS ${AGPC_ARGS} NT )
  endif()
  if (PRECOMPILE_GEMM_TRANS_NC)
    set(AGPC_ARGS ${AGPC_ARGS} NC )
  endif()
  if (PRECOMPILE_GEMM_TRANS_TN)
    set(AGPC_ARGS ${AGPC_ARGS} TN )
  endif()
  if (PRECOMPILE_GEMM_TRANS_TT)
    set(AGPC_ARGS ${AGPC_ARGS} TT )
  endif()
  if (PRECOMPILE_GEMM_TRANS_TC)
    set(AGPC_ARGS ${AGPC_ARGS} TC )
  endif()
  if (PRECOMPILE_GEMM_TRANS_CN)
    set(AGPC_ARGS ${AGPC_ARGS} CN )
  endif()
  if (PRECOMPILE_GEMM_TRANS_CT)
    set(AGPC_ARGS ${AGPC_ARGS} CT )
  endif()
  if (PRECOMPILE_GEMM_TRANS_CC)
    set(AGPC_ARGS ${AGPC_ARGS} CC )
  endif()

  # betas
  set(AGPC_ARGS ${AGPC_ARGS} --betas 0 1 )

################################################################################
# add target for generating pre-compile WhichKernels header
################################################################################
set( AUTOGEMM_PRECOMPILE_HEADER_SRC ${CMAKE_SOURCE_DIR}/lib/src/blas/autogemm/KernelsToPreCompile.py )
set( AUTOGEMM_PRECOMPILE_HEADER_OUT ${CMAKE_BINARY_DIR}/include/AutoGemmIncludes/AutoGemmKernelsToPreCompile.h )
add_custom_command(
  OUTPUT ${AUTOGEMM_PRECOMPILE_HEADER_OUT}
  COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/lib/src/blas/autogemm/KernelsToPreCompile.py ${AGPC_ARGS}
  DEPENDS ${AUTOGEMM_PRECOMPILE_HEADER_SRC}
)

endif()#endif precompile active

################################################################################
# add target for main AutoGemm headers / source
################################################################################
add_custom_command(
  OUTPUT ${AUTOGEMM_HEADERS} ${AUTOGEMM_SRC}
  COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/lib/src/blas/autogemm/AutoGemm.py --output-path ${CMAKE_BINARY_DIR}/include
  DEPENDS ${AUTOGEMM_SCRIPTS}
)

################################################################################
# Autogemm End
################################################################################

#Setting a variable for source files
SET (SRCS ${BLASSRC} hcblas.cpp  
  ${AUTOGEMM_PRECOMPILE_HEADER_OUT}
  ${AUTOGEMM_HEADERS}
  ${AUTOGEMM_SRC}
  ${AUTOGEMM_SCRIPTS}
) 

  # Choice to take compilation flags from source or package
  if(EXISTS ${MCWHCCBUILD})
    execute_process(COMMAND ${HCC_CONFIG} --build --cxxflags
                            OUTPUT_VARIABLE HCC_CXXFLAGS)
    execute_process(COMMAND ${HCC_CONFIG} --build --ldflags --shared
                            OUTPUT_VARIABLE HCC_LDFLAGS)
  else(EXISTS ${MCWHCCBUILD})
    execute_process(COMMAND ${HCC_CONFIG} --install --cxxflags
                            OUTPUT_VARIABLE HCC_CXXFLAGS)
    execute_process(COMMAND ${HCC_CONFIG} --install --ldflags --shared
                            OUTPUT_VARIABLE HCC_LDFLAGS)
  endif(EXISTS ${MCWHCCBUILD})
 
  string(STRIP "${HCC_CXXFLAGS}" HCC_CXXFLAGS)
  string(STRIP "${HCC_LDFLAGS}" HCC_LDFLAGS)
  set (HCC_CXXFLAGS "${HCC_CXXFLAGS} -I${CMAKE_CURRENT_SOURCE_DIR}/../include -I${CMAKE_BINARY_DIR}/include")
  set (HCC_LDFLAGS "${HCC_LDFLAGS}")

  FOREACH(src_file ${SRCS})
    SET_PROPERTY(SOURCE ${src_file} APPEND_STRING PROPERTY COMPILE_FLAGS " ${HCC_CXXFLAGS} ")
  ENDFOREACH()

  ADD_EXECUTABLE("${PROJECT_NAME}.so" ${SRCS} )
  
  SET_PROPERTY(TARGET "${PROJECT_NAME}.so" APPEND_STRING PROPERTY LINK_FLAGS " ${HCC_LDFLAGS} ")
  TARGET_LINK_LIBRARIES("${PROJECT_NAME}.so" hc_am)
  INSTALL(TARGETS "${PROJECT_NAME}.so" 
    RUNTIME DESTINATION lib
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
  )

  INSTALL(FILES "${CMAKE_CURRENT_SOURCE_DIR}/../include/hcblaslib.h" "${CMAKE_CURRENT_SOURCE_DIR}/../include/hcblas.h" DESTINATION include)
