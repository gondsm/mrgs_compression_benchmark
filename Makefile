INCLUDEDIRS = -Ilz4
DEFINITIONFILES = lz4_wrapper.cpp
LIBS = lz4/liblz4.so

test: $(DEFINITIONFILES) $(LIBS) compression_test.cpp compression_test.h
	g++ $(DEFINITIONFILES) compression_test.cpp -o test $(LIBS)
clean:
	rm *.o *~ test
