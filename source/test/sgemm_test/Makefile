LD_LIBRARY_PATH = ~/ampblas/Build/linux/
CBLASPATH = ~/CBLAS
TARGET = sgemm
LIBS = -lblas -L/usr/lib/libblas/ -lampblas -L $(LD_LIBRARY_PATH)
INCLUDE = -I/opt/AMDAPP/include   -I$(CBLASPATH)/include
CC = ${MCWCPPAMPROOT}build/compiler/bin/clang++
CFLAGS = `${MCWCPPAMPROOT}build/build/Release/bin/clamp-config --build --cxxflags --ldflags`

.PHONY: default all clean 

default: $(TARGET)
all: default

HEADERS = $(LD_LIBRARY_PATH)../../source/ampblaslib.h 


.PRECIOUS: $(TARGET) 


$(TARGET):sgemm_test.cpp $(HEADERS)
	$(CC) $(CFLAGS) $(INCLUDE) $(LIBS) $< -o $@
	
export LD_LIBRARY_PATH

clean:
	-rm -f $(TARGET)
