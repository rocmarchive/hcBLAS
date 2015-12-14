# http://www.cmake.org/Wiki/CMake:How_To_Find_Libraries

# - Try to find HC++ Compiler
# Once done this will define
#  HC++_FOUND - System has HC++
#  HC++_BIN_DIR - The HC++ binaries directories
#  CLAMP_CXXFLAGS - The HC++ compilation flags
#  CLAMP_LDFLAGS - The HC++ linker flags

# The following are available when in installation mode
#  HC++_INCLUDE_DIRS - The HC++ include directories
#  HC++_LIBRARIES - The libraries needed to use HC++

if( MSVC OR APPLE)
  message(FATAL_ERROR "Unsupported platform.")
endif()

set(MCWHCCBUILD $ENV{MCWHCCBUILD})
# Package built from sources
# Compiler and configure file are two key factors to advance
if(EXISTS /opt/clamp/bin/clang++)
  find_path(HC++_BIN_DIR clang++
           HINTS /opt/clamp/bin)
  find_path(HC++_CONFIGURE_DIR clamp-config
           HINTS /opt/clamp/bin)
  include(FindPackageHandleStandardArgs)
  # handle the QUIETLY and REQUIRED arguments and set HC++_FOUND to TRUE
  # if all listed variables are TRUE
  find_package_handle_standard_args(HC++  DEFAULT_MSG
                                    HC++_BIN_DIR HC++_CONFIGURE_DIR)
  mark_as_advanced(HC++_BIN_DIR HC++_CONFIGURE_DIR)
  if (HC++_FOUND)
    message(STATUS "HC++ Compiler found in ${HC++_BIN_DIR}/..")
    set(CMAKE_C_COMPILER ${HC++_BIN_DIR}/clang)
    set(CMAKE_CXX_COMPILER ${HC++_BIN_DIR}/clang++)
  elseif()
    message(FATAL_ERROR "HC++ Compiler not found.")
  endif()

  # Build mode
  set (CLANG_AMP "${HC++_BIN_DIR}/clang++")
  set (CLAMP_CONFIG "${HC++_CONFIGURE_DIR}/clamp-config")
  execute_process(COMMAND ${CLAMP_CONFIG} --bolt --cxxflags
                  OUTPUT_VARIABLE CLAMP_CXXFLAGS)
  string(STRIP "${CLAMP_CXXFLAGS}" CLAMP_CXXFLAGS)
  set (CLAMP_CXXFLAGS "${CLAMP_CXXFLAGS}")
  execute_process(COMMAND ${CLAMP_CONFIG} --bolt --ldflags --shared
                  OUTPUT_VARIABLE CLAMP_LDFLAGS)
  string(STRIP "${CLAMP_LDFLAGS}" CLAMP_LDFLAGS)
  set (CLAMP_CXXFLAGS "${CLAMP_CXXFLAGS} -Wall -Wno-deprecated-register -Wno-deprecated-declarations")
  set (CLAMP_LDFLAGS "${CLAMP_LDFLAGS}")

elseif(EXISTS ${MCWHCCBUILD})
  find_path(HC++_BIN_DIR clang++
           HINTS ${MCWHCCBUILD}/compiler/bin)
  find_path(HC++_CONFIGURE_DIR clamp-config
           HINTS ${MCWHCCBUILD}/build/Release/bin)
  include(FindPackageHandleStandardArgs)
  # handle the QUIETLY and REQUIRED arguments and set HC++_FOUND to TRUE
  # if all listed variables are TRUE
  find_package_handle_standard_args(HC++  DEFAULT_MSG
                                    HC++_BIN_DIR HC++_CONFIGURE_DIR)
  mark_as_advanced(HC++_BIN_DIR HC++_CONFIGURE_DIR)
  if (HC++_FOUND)
    message(STATUS "HC++ Compiler found in ${HC++_BIN_DIR}/..")
    set(CMAKE_C_COMPILER ${HC++_BIN_DIR}/clang)
    set(CMAKE_CXX_COMPILER ${HC++_BIN_DIR}/clang++)
  elseif()
    message(FATAL_ERROR "HC++ Compiler not found.")
  endif()

  # Build mode
  set (CLANG_AMP "${HC++_BIN_DIR}/clang++")
  set (CLAMP_CONFIG "${HC++_CONFIGURE_DIR}/clamp-config")
  execute_process(COMMAND ${CLAMP_CONFIG} --build --bolt --cxxflags
                  OUTPUT_VARIABLE CLAMP_CXXFLAGS)
  string(STRIP "${CLAMP_CXXFLAGS}" CLAMP_CXXFLAGS)
  set (CLAMP_CXXFLAGS "${CLAMP_CXXFLAGS}")
  execute_process(COMMAND ${CLAMP_CONFIG} --build --bolt --ldflags --shared
                  OUTPUT_VARIABLE CLAMP_LDFLAGS)
  string(STRIP "${CLAMP_LDFLAGS}" CLAMP_LDFLAGS)
  set (CLAMP_CXXFLAGS "${CLAMP_CXXFLAGS} -Wall -Wno-deprecated-register -Wno-deprecated-declarations")
  set (CLAMP_LDFLAGS "${CLAMP_LDFLAGS}")

else()
  # Package from installation
  find_path(HC++_INCLUDE_DIR hc.hpp
            HINTS /opt /opt/kalmar/include)
  find_library(HC++_LIBRARY NAMES mcwamp
              HINTS /opt /opt/kalmar/lib)
  find_path(HC++_BIN_DIR clang++
            HINTS /opt/kalmar /opt/kalmar/bin)
  set(HC++_LIBRARIES ${HC++_LIBRARY} )
  set(HC++_INCLUDE_DIRS ${HC++_INCLUDE_DIR} )

  include(FindPackageHandleStandardArgs)
  # handle the QUIETLY and REQUIRED arguments and set HC++_FOUND to TRUE
  # if all listed variables are TRUE
  find_package_handle_standard_args(HC++  DEFAULT_MSG
                                    HC++_BIN_DIR HC++_LIBRARY HC++_INCLUDE_DIR )
  mark_as_advanced(HC++_BIN_DIR HC++_INCLUDE_DIR HC++_LIBRARY)
  if (HC++_FOUND)
    message(STATUS "HC++ Compiler found in ${HC++_BIN_DIR}../")
    set(CMAKE_C_COMPILER ${HC++_BIN_DIR}/clang)
    set(CMAKE_CXX_COMPILER ${HC++_BIN_DIR}/clang++)
  elseif()
    message(FATAL_ERROR "HC++ Compiler not found.")
  endif()

  # Installation mode
  set (CLANG_AMP "${HC++_BIN_DIR}/clang++")
  set (CLAMP_CONFIG "${HC++_BIN_DIR}/clamp-config")
  execute_process(COMMAND ${CLAMP_CONFIG} --install --bolt --cxxflags
                  OUTPUT_VARIABLE CLAMP_CXXFLAGS)
  string(STRIP "${CLAMP_CXXFLAGS}" CLAMP_CXXFLAGS)
  set (CLAMP_CXXFLAGS "${CLAMP_CXXFLAGS}")
  execute_process(COMMAND ${CLAMP_CONFIG} --install --bolt --ldflags --shared
                  OUTPUT_VARIABLE CLAMP_LDFLAGS)
  string(STRIP "${CLAMP_LDFLAGS}" CLAMP_LDFLAGS)
  set (CLAMP_CXXFLAGS "-hc ${CLAMP_CXXFLAGS} -Wall -Wno-deprecated-register -Wno-deprecated-declarations")
  set (CLAMP_LDFLAGS "${CLAMP_LDFLAGS}")

endif()


