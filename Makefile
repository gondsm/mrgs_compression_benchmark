DEFINITIONFILES = lz4_wrapper.cpp zlib_wrapper.cpp lzma_wrapper.cpp compression_test.cpp
LIBS = lz4/lz4.o zlib/libz.so lzma/C/liblzma.so
LZMA_CODE = lzma/C/Alloc.c lzma/C/Bra.c lzma/C/LzFind.c lzma/C/LzmaDec.c lzma/C/LzmaEnc.c lzma/C/LzmaLib.c

test: $(DEFINITIONFILES) $(LIBS) compression_test.cpp compression_test.h test.cpp
	g++ $(DEFINITIONFILES) test.cpp -o test $(LIBS) -lgomp -lrt -lm
	
lzma/C/liblzma.so:
	gcc -shared -fPIC -O2 -D_7ZIP_ST -o lzma/C/liblzma.so $(LZMA_CODE)
	
clean:
	rm test results.csv lzma/C/liblzma.so
