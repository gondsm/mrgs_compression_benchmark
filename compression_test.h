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

/* Structures:
 *  These structures form a standardized way of interacting with the algorithms
 *  we intend to test.
 */
// This structure defines the way datasets are provided to the algorithms.
struct Dataset {
  // Dataset's name (its filename)
  std::string name;
  // The dataset's length
  int data_length;
  // The data we want to (de)compress, in row-major order
  char* data;
};

struct Results {
  // Compression stats
  double avg_time_compression;
  double std_deviation_compression;
  // Decompression stats
  double avg_time_decompression;
  double std_deviation_decompression;
  // Compression ratio achieved
  double compression_ratio;
  // Number of iterations we've concluded
  int num_iterations;
  // Were there any errors?
  bool error;	
};

typedef struct Dataset Dataset;
typedef struct Results Results;

/* Functions
 *  These declarations are defined in their respective cpps, and are here for
 *  reference.
 */
// This function loads a dataset from a given filename.
// The file must be a text file in which values are separated by spaces
// and lines by a line break.
// The file is read line-by-line and each "word" is attributed to a "cell".
bool LoadDataset(std::string filename, Dataset * dataset);

Results lz4Test(int num_iterations, Dataset& data);

Results lzmaTest(int num_iterations, Dataset& data);

Results deflate_test(int num_iterations, Dataset data);
#endif // COMPRESSION_TEST_HEADER_
