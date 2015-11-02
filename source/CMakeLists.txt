# Object libraries require CMAKE 2.8.8 version 
CMAKE_MINIMUM_REQUIRED (VERSION 2.8.8) 
MESSAGE(STATUS "CMAKE VERSION ${CMAKE_VERSION}")

#Setting a Project name
SET(PROJECT_NAME "libhcblas")
PROJECT (${PROJECT_NAME})
MESSAGE(STATUS "PROJECT NAME    : ${PROJECT_NAME}")

#Setting a variable for source files
SET (SRCS
    sgemm_wrapper.cpp sgemm_kernels.cpp sgemm_kernels_batch.cpp sgemm_kernels_rMajor.cpp 
    sgemm_kernels_rMajor_batch.cpp cgemm_wrapper.cpp cgemm_kernels.cpp cgemm_kernels_batch.cpp 
    cgemm_kernels_rMajor.cpp cgemm_kernels_rMajor_batch.cpp saxpy.cpp sgemv.cpp sger.cpp
    sscal.cpp dscal.cpp scopy.cpp dcopy.cpp sdot.cpp ddot.cpp sasum.cpp dasum.cpp
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
  SET (CLANG_PATH "/opt/clamp/bin/")

  IF(EXISTS "${CLANG_PATH}/clang++")
   SET (PREIX "${CLANG_PATH}")
   SET (CLANG_AMP "${CLANG_PATH}/clang++")
   SET (CLAMP_CONFIG "${CLANG_PATH}/clamp-config")
  ELSE(EXISTS "${CLANG_PATH}/clang++")
   SET (PREFIX "$ENV{MCWCPPAMPBUILD}")
   SET (CLANG_AMP "${PREFIX}/compiler/bin/clang++")
   SET (CLAMP_CONFIG "${PREFIX}/build/Release/bin/clamp-config")
  ENDIF(EXISTS "${CLANG_PATH}/clang++")

  IF(EXISTS "${CLANG_PATH}/clang++")
    execute_process(COMMAND ${CLAMP_CONFIG} --cxxflags
                            OUTPUT_VARIABLE CLAMP_CXXFLAGS)
  ELSE(EXISTS "${CLANG_PATH}/clang++")
    execute_process(COMMAND ${CLAMP_CONFIG} --build --cxxflags
                            OUTPUT_VARIABLE CLAMP_CXXFLAGS)
  ENDIF(EXISTS "${CLANG_PATH}/clang++")
  
  string(STRIP "${CLAMP_CXXFLAGS}" CLAMP_CXXFLAGS)
  set (CLAMP_CXXFLAGS "${CLAMP_CXXFLAGS}")

  IF(EXISTS "${CLANG_PATH}/clang++")
    execute_process(COMMAND ${CLAMP_CONFIG} --ldflags --shared
                            OUTPUT_VARIABLE CLAMP_LDFLAGS)
  ELSE(EXISTS "${CLANG_PATH}/clang++")
    execute_process(COMMAND ${CLAMP_CONFIG} --build --ldflags --shared
                            OUTPUT_VARIABLE CLAMP_LDFLAGS)
  ENDIF(EXISTS "${CLANG_PATH}/clang++")

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
