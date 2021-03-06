/*********************************************************************
*
* Software License Agreement (BSD License)
*
*  Copyright (c) 2014, ISR University of Coimbra.
*  All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*   * Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*   * Redistributions in binary form must reproduce the above
*     copyright notice, this list of conditions and the following
*     disclaimer in the documentation and/or other materials provided
*     with the distribution.
*   * Neither the name of the ISR University of Coimbra nor the names of its
*     contributors may be used to endorse or promote products derived
*     from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
*  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
*  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
*  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
*  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
*  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
*  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
*  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
*  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*  POSSIBILITY OF SUCH DAMAGE.
*
* Author: Gonçalo S. Martins, 2014
*********************************************************************/

// Header guards
#ifndef COMPRESSION_TEST_HEADER_
#define COMPRESSION_TEST_HEADER_

// Includes
#include <string>
#include <fstream>
#include <vector>

/** Structures:
 *  These structures form a standardized way of interacting with the algorithms
 *  we intend to test.
 */
// This structure defines the way datasets are provided to the algorithms.
struct Dataset {
  // Dataset's name (its filename)
  std::string name;
  // The data we want to (de)compress, in row-major order
  std::vector<unsigned char> bytes;
};

struct Results {
  // Dataset/technique name, dataset size
  std::string dataset_name;
  unsigned int dataset_size;
  std::string technique_name;
  // Number of iterations
  unsigned int num_iterations;
  // Compression stats
  double avg_time_compression;
  double std_deviation_compression;
  // Decompression stats
  double avg_time_decompression;
  double std_deviation_decompression;
  // Compression ratio achieved
  double compression_ratio;
  // Were there any errors?
  // 0: No errors
  // 1: Error related to the technique
  // 2: Data mismatch error
  char error;
};

typedef struct Dataset Dataset;
typedef struct Results Results;

/** Functions
 *  These declarations are defined in their respective cpps, and are here for
 *  reference.
 *  This implementation is able to be extended by creating more ***Test-type
 *  functions.
 */
// This function loads a dataset from a given filename.
// The file must be a text file in which values are separated by spaces
// and lines by a line break.
// The file is read line-by-line and each "word" is attributed to a "cell".
// The dataset must already be allocated.
bool LoadDataset(std::string filename, Dataset * dataset);

// This function loads a PGM grayscale image into a dataset structure.
bool LoadDatasetPGM(std::string filename, Dataset* dataset);

// This function fills in the average and standard deviation of the compression
// and decompression times. Saves us from repeating a lot of code.
// results must, of course, already be allocated. If all goes well, after
// calling this, results will have its times filled in.
void FillInTimes(std::vector<double> compress_times, 
                 std::vector<double> decompress_times, 
                 Results* results);

// This function writes the test results to stdout and, maybe, into a log
// file for easier viewing.
// results is a Results struct, title is the title you want for this technique.
void WriteResults(std::vector<Results>& results, std::string filename, bool stdout);

/** Test Functions
 *  These functions are wrappers around the API for each of the compression
 *  techniques we're testing. New techniques that are added to the tool should
 *  usually be declared here.
 */
// Test function for LZ4 with the default parameters
Results lz4Test(int num_iterations, Dataset& data);

// Test function for LZ4 in its 'slow' mode
Results lz4TestSlow(int num_iterations, Dataset& data);

// Test function for LZ4, compressing the data twice
Results lz4TestTwoPass(int num_iterations, Dataset& data);

// Test function for LZMA with the default parameters
Results lzmaTest(int num_iterations, Dataset& data);

// Test functions for LZMA with fastest and slowest settings
Results lzmaTestSlow(int num_iterations, Dataset& data);
Results lzmaTestFast(int num_iterations, Dataset& data);

// Test function for DEFLATE (zlib) with the default parameters
Results deflateTest(int num_iterations, Dataset& data);

// Test functions for DEFLATE with fastest and slowest settings
Results deflateTestFast(int num_iterations, Dataset& data);
Results deflateTestSlow(int num_iterations, Dataset& data);

// Test function for QuickLZ
Results quicklzTest(int num_iterations, Dataset& data);

// Test function for Snappy
Results snappyTest(int num_iterations, Dataset& data);

#endif // COMPRESSION_TEST_HEADER_
