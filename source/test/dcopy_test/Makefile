TARGET = dcopy
LIBS = -lblas -L/usr/lib/libblas/ -lampblas -L ${AMPBLAS_LIBRARY_PATH}
CC = ${MCWCPPAMPBUILD}/compiler/bin/clang++
CFLAGS = `${MCWCPPAMPBUILD}/build/Release/bin/clamp-config --build --cxxflags --ldflags`

.PHONY: default all clean 

default: $(TARGET)
all: default

.PRECIOUS: $(TARGET) 


$(TARGET):dcopy_test.cpp
	$(CC) $(CFLAGS) $(INCLUDE) $(LIBS) $< -o $@
	

clean:
	-rm -f $(TARGET)
	-rm -f dcopy_test.o
