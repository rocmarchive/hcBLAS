# Object libraries require CMAKE 2.8.8 version 
CMAKE_MINIMUM_REQUIRED (VERSION 2.8.8) 
MESSAGE(STATUS "CMAKE VERSION ${CMAKE_VERSION}")

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
# Find HCC compiler
FIND_PACKAGE(HC++ 1.0 REQUIRED)

file(GLOB SRCS *.cpp)
SET(HIP_SHARED_OBJ "DEFAULT" CACHE STRING "HIP SO")

IF(${PLATFORM} MATCHES "hcc")
  execute_process(COMMAND ${HCC_CONFIG} --install --cxxflags
                            OUTPUT_VARIABLE HCC_CXXFLAGS)
  execute_process(COMMAND ${HCC_CONFIG}  --install --ldflags
                            OUTPUT_VARIABLE HCC_LDFLAGS)

  SET(HIPBLAS_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../lib/include/")
  string(STRIP "${HCC_CXXFLAGS}" HCC_CXXFLAGS)
  set (HCC_CXXFLAGS "${HCC_CXXFLAGS} -I${HIPBLAS_INCLUDE_PATH}")

  SET(HIPBLAS_LIBRARY_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../build/lib/src")
  string(STRIP "${HCC_LDFLAGS}" HCC_LDFLAGS)
  set (HCC_LDFLAGS "${HCC_LDFLAGS} -L${HIPBLAS_LIBRARY_PATH} -L${HIP_PATH}/lib -Wl,-rpath,${HIP_PATH}/lib")

  INCLUDE_DIRECTORIES(${HIP_PATH}/include)
  
  IF(${HIP_SHARED_OBJ} MATCHES "ON")
    SET_PROPERTY(SOURCE ${SRCS} APPEND_STRING PROPERTY COMPILE_FLAGS " ${HCC_CXXFLAGS} -DGTEST_HAS_TR1_TUPLE=0 -D__HIP_PLATFORM_HCC__ -DHIP_SHARED_OBJ=1")
  ELSE()
    SET_PROPERTY(SOURCE ${SRCS} APPEND_STRING PROPERTY COMPILE_FLAGS " ${HCC_CXXFLAGS} -DGTEST_HAS_TR1_TUPLE=0 -D__HIP_PLATFORM_HCC__")
  ENDIF()
  
  SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin/")
  ADD_EXECUTABLE(unit-hip-test ${SRCS})
  SET(LINK "-lhc_am -lblas -lhip_hcc")
  SET_PROPERTY(TARGET unit-hip-test APPEND_STRING PROPERTY LINK_FLAGS " ${HCC_LDFLAGS} ${LINK}")

  IF(${HIP_SHARED_OBJ} MATCHES "ON")
    TARGET_LINK_LIBRARIES(unit-hip-test hipblas_hcc)
    MESSAGE(STATUS "HIPBLAS UNIT TEST USING libhipblas.so")
    MESSAGE(STATUS "**************************************")
  ELSE()  
    TARGET_LINK_LIBRARIES(unit-hip-test hcblas)
    MESSAGE(STATUS "HIPBLAS UNIT TEST USING header containing inline static hipblas APIs")
    MESSAGE(STATUS "*****************************************************************")
  ENDIF()

  add_test(NAME unit-hip-test COMMAND ./bin/unit-hip-test)

ELSEIF(${PLATFORM} MATCHES "nvcc")
 
  SET(HIPBLAS_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../lib/include/")
  set (HIP_CXXFLAGS "${HIP_CXXFLAGS} -I${HIPBLAS_INCLUDE_PATH} -I/usr/local/cuda/include/ -I${HIP_PATH}/include")
  
  IF(${HIP_SHARED_OBJ} MATCHES "ON")
    SET(HIPBLAS_LIBRARY_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../build/lib/src")
    set (HIP_LDFLAGS "${HIP_LDFLAGS} -L${HIPBLAS_LIBRARY_PATH} -L/usr/local/cuda/lib64")
  ELSE()
    set (HIP_LDFLAGS "${HIP_LDFLAGS} -L/usr/local/cuda/lib64")
  ENDIF() 
  
  INCLUDE_DIRECTORIES(${HIP_PATH}/include)

  IF(${HIP_SHARED_OBJ} MATCHES "ON")
    SET_PROPERTY(SOURCE ${SRCS} APPEND_STRING PROPERTY COMPILE_FLAGS " ${HIP_CXXFLAGS} -DGTEST_HAS_TR1_TUPLE=0 -D__HIP_PLATFORM_NVCC__ -DHIP_SHARED_OBJ=1")
  ELSE()
    SET_PROPERTY(SOURCE ${SRCS} APPEND_STRING PROPERTY COMPILE_FLAGS " ${HIP_CXXFLAGS} -DGTEST_HAS_TR1_TUPLE=0 -D__HIP_PLATFORM_NVCC__")
  ENDIF()
 
  SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin/")
  ADD_EXECUTABLE(unit-hip-test ${SRCS})
  SET(LINK "-pthread")
  
  SET_PROPERTY(TARGET unit-hip-test APPEND_STRING PROPERTY LINK_FLAGS " ${HIP_LDFLAGS} ${LINK}")
   
  IF(${HIP_SHARED_OBJ} MATCHES "ON")
    TARGET_LINK_LIBRARIES(unit-hip-test hipblas_nvcc blas cudart)
    MESSAGE(STATUS "HIPBLAS UNIT TEST USING libhipblas.so")
    MESSAGE(STATUS "**************************************")
  ELSE()
    TARGET_LINK_LIBRARIES(unit-hip-test cublas blas cudart)
    MESSAGE(STATUS "HIPBLAS UNIT TEST USING header containing inline static hipblas APIs")
    MESSAGE(STATUS "*****************************************************************")
  ENDIF()

  add_test(NAME unit-hip-test COMMAND ./bin/unit-hip-test)  

ENDIF()
