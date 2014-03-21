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

// Include LZMA
#include "lzma/C/LzmaLib.h"

// Include our header
#include "compression_test.h"

// And we'll also need vectors (they're handy)
#include <vector>

// Lastly, OMP, for its timing features
#include <omp.h>

Results lzmaTestFast(int num_iterations, Dataset& data) {
  Results results;
  results.error = 0;
  // Time vector for compression
  std::vector<double> compression_times;
  // Time vector for decompression
  std::vector<double> decompression_times;
  // Variable to hold the compression ratio
  double compression_ratio;
  // Prepare data structures
  int data_length = data.bytes.size();
  unsigned char* uncompressed = new unsigned char[data_length];
  unsigned char* decompressed = new unsigned char[data_length];
  unsigned char* compressed = new unsigned char[data_length];
  
  // Write size to result
  results.dataset_size = data_length;  
  
  // Copy source data into decompressed buffer
  for(int i = 0; i < data_length; i++)
    uncompressed[i] = data.bytes.at(i);
    
  // Run tests
  for(int i = 0; i < num_iterations; i++) {
    /// Compression
    long unsigned int compressed_bytes = data_length;
    int compress_return = SZ_OK;
    long unsigned props_size = 5;
    unsigned char lzma_props[10];
    // Get time
    double init_compress = omp_get_wtime(); 
    // Test compression
    compress_return = LzmaCompress(compressed, &compressed_bytes, uncompressed, 
                                   data_length, lzma_props, &props_size, 
                                   0, 0, -1, -1, -1, -1, -1 );
    
    // Get time and calculate difference
    double compress_time = omp_get_wtime();
    compression_times.push_back(compress_time - init_compress);

    /// Decompression
    long unsigned int decompressed_bytes = data_length;
    int decompress_return = SZ_OK;
    // Get time
    double init_decompress = omp_get_wtime();
    // Test decompression
    decompress_return = LzmaUncompress(decompressed, &decompressed_bytes, 
                                       compressed, &compressed_bytes,
                                       lzma_props, props_size);
    
    // Get time and calculate difference
    double decompress_time = omp_get_wtime();
    decompression_times.push_back(decompress_time - init_decompress);
    // Set the ratio on the first iteration
    if(i == 0) compression_ratio = ((double)data.bytes.size())/((double)compressed_bytes);

    /// Check for errors
    if(compress_return != SZ_OK || decompress_return != SZ_OK)
      results.error = 1;
    else 
      if(decompressed_bytes != data_length)
        results.error = 2;
      else
        for(int i = 0; i < data_length; i++)
          if(decompressed[i] != uncompressed[i]) results.error = 2;
  }
  
  // Clean up
  delete compressed;
  delete decompressed;
  delete uncompressed;
  

  // Fill in results
  // Name
  results.dataset_name = data.name;
  results.technique_name = "LZMA Fast";
  // Number of iterations
  results.num_iterations = num_iterations;
  // Compression ratio
  results.compression_ratio = compression_ratio;
  // Times
  FillInTimes(compression_times, decompression_times, &results);

  // ... and we're done.
  return results;
}

