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

// And we'll also need vectors (they're handy), and timing
#include <vector>
#include <ctime>

Results lz4Test(int num_iterations, Dataset& data) {
  // Time vector for compression
  std::vector<double> compression_times;
  // Time vector for decompression
  std::vector<double> decompression_times;
  // Buffers needed for compression and decompression
  char* decompressed = new char[data.data_length];
  char* compressed = new char[LZ4_compressBound(data.data_length)];
  
  // Copy source data into decompressed buffer
  for(int i = 0; i < data.data_length; i++)
    decompressed[i] = data.data[i];
    
  // Run tests
  for(int i = 0; i < num_iterations; i++) {
    /// Compression
    // Get time
    time_t init_compress = time(NULL); 
    // Test compression
    int compressed_bytes = LZ4_compress(decompressed, compressed, data.data_length);
    // Get time and calculate difference
    time_t compress_time = time(NULL);
    compression_times.push_back(difftime(compress_time, init_compress));

    /// Decompression
    // Get time
    time_t init_decompress = time(NULL);
    // Test decompression
    int decompressed_bytes = LZ4_decompress_safe(compressed, decompressed, compressed_bytes, data.data_length);
    // Get time and calculate difference
    time_t decompress_time = time(NULL);
    decompression_times.push_back(difftime(decompress_time, init_decompress));
  }

  // Fill in results
  Results results;

  // ... and we're done.
  return results;
}
