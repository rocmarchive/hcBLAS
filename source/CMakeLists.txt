# Object libraries require CMAKE 2.8.8 version 
CMAKE_MINIMUM_REQUIRED (VERSION 2.8.8) 
MESSAGE(STATUS "CMAKE VERSION ${CMAKE_VERSION}")

#Setting a Project name
SET(PROJECT_NAME "libampblas")
PROJECT (${PROJECT_NAME})
MESSAGE(STATUS "PROJECT NAME    : ${PROJECT_NAME}")

#Setting a variable for source files
SET (SRCS
    ampblaslib.h 
    saxpy.cpp sgemm.cpp cgemm.cpp sgemv.cpp sger.cpp
    )

IF(WIN32)
  #include the additional libraries 
  #settings->c/c++->general->additional directories 
  SET(SRCS ${SRCS} targetver.h stdafx.cpp stdafx.h)
  ADD_DEFINITIONS(-D__LITTLE_ENDIAN__)
  ADD_DEFINITIONS(-DTARGET_OS_WIN32=1)
    
  IF(MSVC)
    ADD_DEFINITIONS(/W3)
    ADD_EXECUTABLE(${PROJECT_NAME} ${SRCS})
    INSTALL(TARGETS ${PROJECT_NAME} DESTINATION bin)
  ELSE(MSVC)
    MESSAGE(FATAL_ERROR "TARGET IDE ISNT VISUAL STUDIO 11/12.... EXITING")
  ENDIF(MSVC)
ELSE(WIN32)

  # MCW CLAMP Specific. Version >= 0.3 is Must
  SET (OPENCL_INC "$ENV{AMDAPPSDKROOT}/include")
  SET (OPENCL_LIB "$ENV{AMDAPPSDKROOT}/lib/x86_64")
  SET (CLBLAS_INC "$ENV{CLBLASROOT}/include")
  SET (CLBLAS_LIB "$ENV{CLBLASROOT}/lib64")
  SET (BOLT_SRC_INC "$ENV{MCWCPPAMPROOT}/src/Bolt/include/")
  SET (BOOST_SRC_INC "$ENV{MCWCPPAMPROOT}/gmac_exp_build_cache/Bolt/superbuild/external/boost/src/Boost/")
  SET (BOLT_VER_INC "$ENV{MCWCPPAMPROOT}/gmac_exp_build_cache/Bolt/superbuild/Bolt-build/include/")
  SET (PREFIX "$ENV{MCWCPPAMPROOT}/gmac_exp_build_cache")
  SET (CLANG_AMP "${PREFIX}/compiler/bin/clang++")
  SET (CLAMP_CONFIG "${PREFIX}/build/Release/bin/clamp-config")

  execute_process(COMMAND ${CLAMP_CONFIG} --build --cxxflags
    OUTPUT_VARIABLE CLAMP_CXXFLAGS)
  string(STRIP ${CLAMP_CXXFLAGS} CLAMP_CXXFLAGS)
  set (CLAMP_CXXFLAGS "${CLAMP_CXXFLAGS} -I${OPENCL_INC}")
  execute_process(COMMAND ${CLAMP_CONFIG} --build --ldflags --shared 
    OUTPUT_VARIABLE CLAMP_LDFLAGS)
  string(STRIP ${CLAMP_LDFLAGS} CLAMP_LDFLAGS)
  set (CLAMP_CXXFLAGS "${CLAMP_CXXFLAGS} -I${OPENCL_INC} -I${CLBLAS_INC} -I${BOLT_SRC_INC} -I${BOLT_VER_INC} -I${BOOST_SRC_INC} -Wall")
  set (CLAMP_LDFLAGS "${CLAMP_LDFLAGS} -L${OPENCL_LIB} -L${CLBLAS_LIB}")

  FOREACH(src_file ${SRCS})
    SET_PROPERTY(SOURCE ${src_file} APPEND_STRING PROPERTY COMPILE_FLAGS " ${CLAMP_CXXFLAGS}")
  ENDFOREACH()

  ADD_EXECUTABLE("${PROJECT_NAME}.so" ${SRCS})
  SET_PROPERTY(TARGET "${PROJECT_NAME}.so" APPEND_STRING PROPERTY LINK_FLAGS " ${CLAMP_LDFLAGS}")
  INSTALL(TARGETS "${PROJECT_NAME}.so" DESTINATION bin)

ENDIF(WIN32)
