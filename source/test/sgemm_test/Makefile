TARGET = sgemm
LIBS = -lblas -L/usr/lib/libblas/ -lampblas -L ${AMPBLAS_LIBRARY_PATH}
CC = ${MCWCPPAMPBUILD}/compiler/bin/clang++
CFLAGS = `${MCWCPPAMPBUILD}/build/Release/bin/clamp-config --build --cxxflags --ldflags`

.PHONY: default all clean 

default: $(TARGET)
all: default

#HEADERS = ../../source/ampblaslib.h 


.PRECIOUS: $(TARGET) 


$(TARGET):sgemm_test.cpp $(HEADERS)
	$(CC) $(CFLAGS) $(INCLUDE) $(LIBS) $< -o $@
	

clean:
	-rm -f $(TARGET)
