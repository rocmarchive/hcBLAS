cmake_minimum_required(VERSION 2.8)

# HIP_PATH
IF(NOT DEFINED $ENV{HIP_PATH})
  SET(HIP_PATH "/opt/rocm/hip")
ELSE()
  SET(HIP_PATH $ENV{HIP_PATH})
ENDIF()

SET(HIP_SHARED_OBJ "DEFAULT" CACHE STRING "HIP SO")
SET(INSTALL_OPT $ENV{INSTALL_OPT})

SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake  "${HIP_PATH}/cmake")
EXECUTE_PROCESS(COMMAND ${HIP_PATH}/bin/hipconfig -P OUTPUT_VARIABLE PLATFORM)

IF (${PLATFORM} MATCHES "hcc")
  IF(DEFINED INSTALL_OPT)
    SET( lib_so_link ${CMAKE_INSTALL_PREFIX}/../lib/libhcblas.so)
    SET( hcc_so_target ${CMAKE_INSTALL_PREFIX}/lib/libhcblas.so)
    execute_process(COMMAND ln -s ${hcc_so_target} ${lib_so_link})
  
    IF(${HIP_SHARED_OBJ} MATCHES "ON")
      SET( lib_so_link ${CMAKE_INSTALL_PREFIX}/../lib/libhipblas.so)
      SET( hip_so_target ${CMAKE_INSTALL_PREFIX}/lib/libhipblas_hcc.so)
      execute_process(COMMAND ln -s ${hip_so_target} ${lib_so_link})
    ENDIF()
  ENDIF()
ENDIF()

