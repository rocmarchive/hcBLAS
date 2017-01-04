cmake_minimum_required(VERSION 2.8)
# HIP_PATH
IF(NOT DEFINED $ENV{HIP_PATH})
  SET(HIP_PATH "/opt/rocm/hip")
ELSE()
  SET(HIP_PATH $ENV{HIP_PATH})
ENDIF()

SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake  "${HIP_PATH}/cmake")
EXECUTE_PROCESS(COMMAND ${HIP_PATH}/bin/hipconfig -P OUTPUT_VARIABLE PLATFORM)
#SET(HIP_SHARED_OBJ $ENV{HIP_SHARED_OBJ})
SET(HIP_SHARED_OBJ "DEFAULT" CACHE STRING "HIP SO")

IF(${HIP_SHARED_OBJ} MATCHES "ON" OR ${PLATFORM} MATCHES "nvcc")
  add_subdirectory(unit-hip)
ENDIF()

IF(${PLATFORM} MATCHES "hcc")
add_subdirectory(src)
add_subdirectory(unit)
add_subdirectory(unit-api)
add_subdirectory(src/statistical_timer)
ENDIF()
