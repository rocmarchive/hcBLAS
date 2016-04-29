cmake_minimum_required(VERSION 2.8)
add_subdirectory(lib/src)

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

set(CPACK_SET_DESTDIR TRUE)
set(CPACK_INSTALL_PREFIX "/opt/rocm/hcblas/")
set(CPACK_PACKAGE_NAME "hcblas")
set(CPACK_PACKAGE_VENDOR "MulticoreWare, Inc")
set(CPACK_GENERATOR "DEB;TGZ")
set(CPACK_SOURCE_GENERATOR "TGZ")
set(CPACK_BINARY_DEB "ON")
set(CPACK_BINARY_STGZ "OFF")
set(CPACK_SOURCE_TGZ "OFF")
set(CPACK_SOURCE_TZ "OFF")
set(CPACK_SOURCE_TBZ2 "OFF")
set(CPACK_BINARY_TZ "OFF")
SET(CPACK_PACKAGE_NAME "hcblas")
SET(CPACK_PACKAGE_FILE_NAME ${CPACK_PACKAGE_NAME}-${GIT_BRANCH}-${GIT_COMMIT_HASH}-${CMAKE_SYSTEM_NAME})
SET(CPACK_DEBIAN_PACKAGE_DESCRIPTION "HcBLAS: a BLAS Library based on Heterogeneous C++ construct")
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Neelakandan <neelakandan@multicorewareinc.com>") #required
 
INCLUDE(CPack)
