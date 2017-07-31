cmake_minimum_required(VERSION 2.8)
# HIP_PATH
IF(NOT DEFINED $ENV{HIP_PATH})
  SET(HIP_PATH "/opt/rocm/hip")
ELSE()
  SET(HIP_PATH $ENV{HIP_PATH})
ENDIF()

SET(HIP_SUPPORT $ENV{HIP_SUPPORT})

SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake  "${HIP_PATH}/cmake")
EXECUTE_PROCESS(COMMAND ${HIP_PATH}/bin/hipconfig -P OUTPUT_VARIABLE PLATFORM)

SET(TEST_BASIC "DEFAULT" CACHE STRING "BASIC TESTING")

IF(${HIP_SUPPORT} MATCHES "on")
  add_subdirectory(unit-hip)
ENDIF()

 IF(${TEST_BASIC} MATCHES "ON")
    ADD_DEFINITIONS(-DTEST_BASIC=1)
  ELSEIF(${TEST_BASIC} MATCHES "OFF")
    ADD_DEFINITIONS(-DTEST_BASIC=0)
  ELSE()
    ADD_DEFINITIONS(-DTEST_BASIC=0)
  ENDIF()


IF(${PLATFORM} MATCHES "hcc")
  add_subdirectory(src)
  add_subdirectory(unit)
  add_subdirectory(unit-api)
  add_subdirectory(src/statistical_timer)
ENDIF()
