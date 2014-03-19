#!/bin/bash
# Pull LZ4:
svn checkout http://lz4.googlecode.com/svn/trunk/ lz4

# Pull zlib
wget http://zlib.net/zlib-1.2.8.tar.gz
tar xf zlib-1.2.8.tar.gz
rm zlib-1.2.8.tar.gz
mv zlib-1.2.8/ zlib/
