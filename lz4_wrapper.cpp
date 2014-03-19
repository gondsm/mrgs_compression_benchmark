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

// Include lz4
#include "lz4/lz4.h"

// Include our header
#include "compression_test.h"

// And we'll also need vectors (they're handy)
#include <vector>

// Lastly, OMP, for its timing features
#include <omp.h>

Results lz4Test(int num_iterations, Dataset& data) {
  Results results;
  results.error = false;  
  // Time vector for compression
  std::vector<double> compression_times;
  // Time vector for decompression
  std::vector<double> decompression_times;
  // Buffers needed for compression and decompression
  int data_length = data.bytes.size();
  char* decompressed = new char[data_length];
  char* uncompressed = new char[data_length];
  char* compressed = new char[LZ4_compressBound(data_length)];
  
  // Variable to hold the compression ratio
  double compression_ratio;

  // Copy source data into decompressed buffer
  for(int i = 0; i < data_length; i++)
    uncompressed[i] = data.bytes.at(i);
    
  // Run tests
  for(int i = 0; i < num_iterations; i++) {
    /// Compression
    // Get time
    double init_compress = omp_get_wtime(); 
    // Test compression
    int compressed_bytes = LZ4_compress(uncompressed, compressed, data_length);
    // Get time and calculate difference
    double compress_time = omp_get_wtime();
    compression_times.push_back(compress_time - init_compress);

    /// Decompression
    // Get time
    double init_decompress = omp_get_wtime();
    // Test decompression
    int decompressed_bytes = LZ4_decompress_safe(compressed, decompressed, compressed_bytes, data_length);
    // Get time and calculate difference
    double decompress_time = omp_get_wtime();
    decompression_times.push_back(decompress_time - init_decompress);
    // Set the ratio on the first iteration
    if(i == 0) compression_ratio = ((double)data.bytes.size())/((double)compressed_bytes);

  }
  
  // Clean up
  delete compressed;
  delete decompressed;

  // Fill in results
  // Name
  results.dataset_name = data.name;
  results.technique_name = "LZ4";
  // Compression ratio
  results.compression_ratio = compression_ratio;
  // Times
  FillInTimes(compression_times, decompression_times, &results);

  // ... and we're done.
  return results;
}
