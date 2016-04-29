# Object libraries require CMAKE 2.8.8 version 
CMAKE_MINIMUM_REQUIRED (VERSION 2.8.8) 
MESSAGE(STATUS "CMAKE VERSION ${CMAKE_VERSION}")

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/../../../cmake ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
# Find HCC compiler
FIND_PACKAGE(HC++ 1.0 REQUIRED)

file(GLOB SRCS *.cpp)

# Choice to take compilation flags from source or package
if(EXISTS ${MCWHCCBUILD})
  execute_process(COMMAND ${HCC_CONFIG} --build --cxxflags
                         OUTPUT_VARIABLE HCC_CXXFLAGS)
  execute_process(COMMAND ${HCC_CONFIG} --build --ldflags  
                            OUTPUT_VARIABLE HCC_LDFLAGS)
else(EXISTS ${MCWHCCBUILD})
  execute_process(COMMAND ${HCC_CONFIG} --install --cxxflags
                            OUTPUT_VARIABLE HCC_CXXFLAGS)
  execute_process(COMMAND ${HCC_CONFIG}  --install --ldflags
                            OUTPUT_VARIABLE HCC_LDFLAGS)
endif(EXISTS ${MCWHCCBUILD})

SET(HCBLAS_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../../lib/include/")
SET(HCBLAS_LIBRARY_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../../build/lib/src")

string(STRIP "${HCC_CXXFLAGS}" HCC_CXXFLAGS)
string(STRIP "${HCC_LDFLAGS}" HCC_LDFLAGS)
set (HCC_CXXFLAGS "${HCC_CXXFLAGS} -I${HCBLAS_INCLUDE_PATH}")
set (HCC_LDFLAGS "${HCC_LDFLAGS} -L${HCBLAS_LIBRARY_PATH}")

SET_PROPERTY(SOURCE ${SRCS} APPEND_STRING PROPERTY COMPILE_FLAGS " ${HCC_CXXFLAGS} -DGTEST_HAS_TR1_TUPLE=0 ")
ADD_EXECUTABLE(bin/unittest ${SRCS})
SET_PROPERTY(TARGET bin/unittest APPEND_STRING PROPERTY LINK_FLAGS " ${HCC_LDFLAGS} ")

TARGET_LINK_LIBRARIES(bin/unittest hcblas hc_am blas)
add_test(NAME unittest COMMAND ./bin/unittest)
