# Object libraries require CMAKE 2.8.8 version 
CMAKE_MINIMUM_REQUIRED (VERSION 2.8.8) 
MESSAGE(STATUS "CMAKE VERSION ${CMAKE_VERSION}")

#Setting a Project name
SET(PROJECT_NAME "libhcblas")
PROJECT (${PROJECT_NAME})
MESSAGE(STATUS "PROJECT NAME    : ${PROJECT_NAME}")
#Setting a variable for source files
SET (SRCS
    sgemm_array_wrapper.cpp sgemm_array_view_wrapper.cpp sgemm_array_kernels.cpp sgemm_array_view_kernels.cpp
    sgemm_array_kernels_batch.cpp sgemm_array_view_kernels_batch.cpp sgemm_array_kernels_rMajor.cpp
    sgemm_array_view_kernels_rMajor.cpp sgemm_array_view_kernels_rMajor_batch.cpp sgemm_array_kernels_rMajor_batch.cpp 
    cgemm_array_wrapper.cpp cgemm_array_kernels.cpp cgemm_array_kernels_batch.cpp cgemm_array_kernels_rMajor.cpp 
    cgemm_array_kernels_rMajor_batch.cpp  cgemm_array_view_wrapper.cpp cgemm_array_view_kernels.cpp 
    cgemm_array_view_kernels_batch.cpp cgemm_array_view_kernels_rMajor.cpp cgemm_array_view_kernels_rMajor_batch.cpp
    saxpy_array.cpp  saxpy_array_view.cpp sgemv_array.cpp sgemv_array_view.cpp sger_array.cpp sger_array_view.cpp
    sscal_array.cpp sscal_array_view.cpp dscal_array.cpp dscal_array_view.cpp 
    scopy_array.cpp scopy_array_view.cpp dcopy_array.cpp dcopy_array_view.cpp
    sdot_array.cpp sdot_array_view.cpp ddot_array.cpp ddot_array_view.cpp
    sasum_array.cpp sasum_array_view.cpp dasum_array.cpp dasum_array_view.cpp
    )

IF(WIN32)
  #include the additional libraries 
  #settings->c/c++->general->additional directories 
  SET(SRCS ${SRCS})
  ADD_DEFINITIONS(-D__LITTLE_ENDIAN__)
  ADD_DEFINITIONS(-DTARGET_OS_WIN32=1)
    
  IF(MSVC)
    ADD_DEFINITIONS(/W3)
    ADD_EXECUTABLE(${PROJECT_NAME} ${SRCS})
    INSTALL(TARGETS ${PROJECT_NAME} DESTINATION lib)
  ELSE(MSVC)
    MESSAGE(FATAL_ERROR "TARGET IDE ISNT VISUAL STUDIO 11/12.... EXITING")
  ENDIF(MSVC)
ELSE(WIN32)
  # MCW CLAMP Specific. Version >= 0.3 is Must
  SET (CLANG_PATH "/opt/hcc/bin/")
  SET (PREIX "${CLANG_PATH}")
  SET (CLANG_AMP "${CLANG_PATH}/clang++")
  SET (CLAMP_CONFIG "${CLANG_PATH}/clamp-config")
  execute_process(COMMAND ${CLAMP_CONFIG} --cxxflags
                            OUTPUT_VARIABLE CLAMP_CXXFLAGS)
  string(STRIP "${CLAMP_CXXFLAGS}" CLAMP_CXXFLAGS)
  set (CLAMP_CXXFLAGS "${CLAMP_CXXFLAGS}")
  execute_process(COMMAND ${CLAMP_CONFIG} --ldflags --shared
                            OUTPUT_VARIABLE CLAMP_LDFLAGS)
  string(STRIP "${CLAMP_LDFLAGS}" CLAMP_LDFLAGS)
  set (CLAMP_CXXFLAGS "${CLAMP_CXXFLAGS} -Wall -Wno-deprecated-register -Wdeprecated-declarations")
  set (CLAMP_LDFLAGS "${CLAMP_LDFLAGS}")

  FOREACH(src_file ${SRCS})
    SET_PROPERTY(SOURCE ${src_file} APPEND_STRING PROPERTY COMPILE_FLAGS " ${CLAMP_CXXFLAGS}")
  ENDFOREACH()

  ADD_EXECUTABLE("${PROJECT_NAME}.so" ${SRCS})
  SET_PROPERTY(TARGET "${PROJECT_NAME}.so" APPEND_STRING PROPERTY LINK_FLAGS " ${CLAMP_LDFLAGS}")
  INSTALL(TARGETS "${PROJECT_NAME}.so" DESTINATION "/usr/lib/")
  INSTALL(FILES hcblas.h DESTINATION include)

ENDIF(WIN32)
