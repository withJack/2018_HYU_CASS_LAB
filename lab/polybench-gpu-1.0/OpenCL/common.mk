LIB=-lOpenCL -lm
all:
	gcc -O3 ${CFILES} ${LIB} -o ${EXECUTABLE}

clean:
	rm -f *~ *.exe
