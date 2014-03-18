DEFINITIONFILES = lz4_wrapper.cpp
LIBS = lz4/lz4.o

test: $(DEFINITIONFILES) $(LIBS) compression_test.cpp compression_test.h
	g++ $(DEFINITIONFILES) compression_test.cpp -o test $(LIBS) -lgomp -lrt -lm
clean:
	rm *.o *~ test
