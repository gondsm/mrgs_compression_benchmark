DEFINITIONFILES = wrappers/lz4_wrapper.cpp wrappers/lz4_wrapper_slowest.cpp wrappers/lz4_wrapper_twopass.cpp wrappers/zlib_wrapper.cpp wrappers/zlib_wrapper_fastest.cpp wrappers/zlib_wrapper_slowest.cpp wrappers/lzma_wrapper.cpp wrappers/lzma_wrapper_fastest.cpp wrappers/lzma_wrapper_slowest.cpp wrappers/quicklz_wrapper.cpp wrappers/snappy_wrapper.cpp compression_test.cpp
LIBS = lz4/lz4.o zlib/libz.so lzma/C/liblzma.so quicklz/libquicklz.so lz4/lz4hc.o snappy/snappy.o snappy/snappy-c.o snappy/snappy-sinksource.o
LZMA_CODE = lzma/C/Alloc.c lzma/C/Bra.c lzma/C/LzFind.c lzma/C/LzmaDec.c lzma/C/LzmaEnc.c lzma/C/LzmaLib.c

test: $(DEFINITIONFILES) $(LIBS) compression_test.cpp compression_test.h test.cpp
	g++ $(DEFINITIONFILES) -I./ test.cpp -o test $(LIBS) -lgomp -lrt -lm

lzma/C/liblzma.so:
	gcc -shared -fPIC -O2 -D_7ZIP_ST -o lzma/C/liblzma.so $(LZMA_CODE)
	
quicklz/libquicklz.so:
	gcc -shared -fPIC -O2 quicklz/quicklz.c -o quicklz/libquicklz.so
	
	
clean:
	rm test lzma/C/liblzma.so quicklz/libquicklz.o
