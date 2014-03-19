DEFINITIONFILES = lz4_wrapper.cpp zlib_wrapper.cpp compression_test.cpp
LIBS = lz4/lz4.o zlib/libz.so

test: $(DEFINITIONFILES) $(LIBS) compression_test.cpp compression_test.h test.cpp
	g++ $(DEFINITIONFILES) test.cpp -o test $(LIBS) -lgomp -lrt -lm
clean:
	rm *.o *~ test results.csv
