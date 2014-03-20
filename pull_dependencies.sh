#!/bin/bash
# Pull LZ4:
svn checkout http://lz4.googlecode.com/svn/trunk/ lz4

# Pull zlib
wget http://zlib.net/zlib-1.2.8.tar.gz
tar xf zlib-1.2.8.tar.gz
rm zlib-1.2.8.tar.gz
mv zlib-1.2.8/ zlib/

# Pull lzma
#http://downloads.sourceforge.net/sevenzip/lzma920.tar.bz2
# You'll have to get the tarball for 7-zip manually from the link above.
# leave it on the repository's root, next to the makefile, compression_test.h
# et al. We'll take care of the rest.
mkdir lzma
mv lzma920.tar.bz2 lzma/
cd lzma
tar xf lzma920.tar.bz2
rm lzma920.tar.bz2
cd ..

# Pull quicklz
mkdir quicklz
cd quicklz
wget http://www.quicklz.com/quicklz.h
wget http://www.quicklz.com/quicklz.c
