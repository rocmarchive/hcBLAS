cmake_minimum_required(VERSION 2.8)

# HIP_PATH
IF(NOT DEFINED $ENV{HIP_PATH})
  SET(HIP_PATH "/opt/rocm/hip")
ELSE()
  SET(HIP_PATH $ENV{HIP_PATH})
ENDIF()

SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake  "${HIP_PATH}/cmake")
EXECUTE_PROCESS(COMMAND ${HIP_PATH}/bin/hipconfig -P OUTPUT_VARIABLE PLATFORM)
add_subdirectory(lib/src)
# Build hcblas and hipblas from source on a AMD platform

  # Get the current working branch
  execute_process(
    COMMAND git rev-parse --abbrev-ref HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_BRANCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )

  # Get the latest abbreviated commit hash of the working branch
  execute_process(
    COMMAND git log -1 --format=%h
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_COMMIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )

  # Set default install and package paths.
  set(CMAKE_INSTALL_PREFIX "/opt/rocm/hcblas" CACHE STRING "Install location.")
  set(CPACK_PACKAGING_INSTALL_PREFIX "/opt/rocm/hcblas" CACHE STRING "Package install location.")

IF (${PLATFORM} MATCHES "hcc")
  set(CPACK_PACKAGE_NAME "hcblas")
  SET(CPACK_DEBIAN_PACKAGE_DESCRIPTION "HcBLAS: a BLAS Library based on Heterogeneous C++ construct")
ELSEIF (${PLATFORM} MATCHES "nvcc")
  set(CPACK_PACKAGE_NAME "hipblas")
  SET(CPACK_DEBIAN_PACKAGE_DESCRIPTION "HipBLAS: a BLAS Library interface that links against cublas library")
ENDIF() # PLATFORM 
  set(CPACK_PACKAGE_VENDOR "MulticoreWare, Inc")
  set(CPACK_GENERATOR "DEB;TGZ" CACHE STRING "Packages generated.")
  set(CPACK_SOURCE_GENERATOR "TGZ")
  set(CPACK_BINARY_DEB "ON")
  set(CPACK_BINARY_STGZ "OFF")
  set(CPACK_SOURCE_TGZ "OFF")
  set(CPACK_SOURCE_TZ "OFF")
  set(CPACK_SOURCE_TBZ2 "OFF")
  set(CPACK_BINARY_TZ "OFF")
  SET(CPACK_PACKAGE_FILE_NAME ${CPACK_PACKAGE_NAME}-${GIT_BRANCH}-${GIT_COMMIT_HASH}-${CMAKE_SYSTEM_NAME})
  SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Neelakandan <neelakandan@multicorewareinc.com>") #required

  # Debian specific flags
  set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA
  "${CMAKE_SOURCE_DIR}/packaging/debian/postinst;${CMAKE_SOURCE_DIR}/packaging/debian/prerm")

  # RPM specific flags
  set(CPACK_RPM_PRE_INSTALL_SCRIPT_FILE "${CMAKE_SOURCE_DIR}/packaging/rpm/post")
  set(CPACK_RPM_POST_UNINSTALL_SCRIPT_FILE "${CMAKE_SOURCE_DIR}/packaging/rpm/postun")

  INCLUDE(CPack)
