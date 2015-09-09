TARGET = sscal
LIBS = -lblas -L/usr/lib/libblas/ -lhcblas -L ${HCBLAS_LIBRARY_PATH}
CC = ${MCWCPPAMPBUILD}/compiler/bin/clang++
CFLAGS = `${MCWCPPAMPBUILD}/build/Release/bin/clamp-config --build --cxxflags --ldflags`

.PHONY: default all clean 

default: $(TARGET)
all: default

.PRECIOUS: $(TARGET) 


$(TARGET):sscal_test.cpp
	$(CC) $(CFLAGS) $(INCLUDE) $(LIBS) $< -o $@
	

clean:
	-rm -f $(TARGET)
	-rm -f sscal_test.o
