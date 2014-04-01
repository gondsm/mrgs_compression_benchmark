# mrgs_compression_benchmark

## Short description

This tool was created with the purpose of testing various compression techniques. Each technique we want to test is wrapped in a function which tracks and reports its performance.

## Long description

This tool was designed to be extended. You can add any compression tool you'd like to test, given that it has a C++ API, by adding its wrapper prototype in compression\_test.h and defining the function in a file you'd have to add to the makefile. Alternatively, you can define your new function in compression\_test.cpp, and it should compile as it is.

### Motivation

This work was done in the context of my M.Sc. project, which involves exploring the optimization of inter-robot communication during SLAM mission. In order to determine if compression was a viable solution, and which technique worked the best, I've created this tool.

### pull_dependencies.sh

If you've just cloned this repository, I'd start by looking here. This shell script downloads and extracts all the tools I've tested myself using this tool.

### Building and running the tool

Running ```make``` on the project's root directory should build everything you need to run a simple test. If you want to add other tools, you'll have to add them to the makefile, so that everything gets linked together.

To run the default test, which benchmarks LZ4 (http://code.google.com/p/lz4/), zlib (http://www.zlib.net/), LZMA (http://7-zip.org/) and QuickLZ (http://quicklz.com/), all you should have to do is run ```./test``` on your terminal.
