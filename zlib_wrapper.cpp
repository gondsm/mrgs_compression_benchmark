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
#include "zlib/zlib.h"

// Include our header
#include "compression_test.h"

// And we'll also need vectors (they're handy), and timing
#include <vector>
#include <ctime>

// Lastly, OMP, for its timing features
#include <omp.h>

Results deflateTest(int num_iterations, Dataset& data) {
  Results results;
  // Time vector for compression
  std::vector<double> compression_times;
  // Time vector for decompression
  std::vector<double> decompression_times;
  // Variable to hold the compression ratio
  double compression_ratio;
  // Prepare data structures
  int data_length = data.bytes.size();
  unsigned char* decompressed = new unsigned char[data_length];
  unsigned char* compressed = new unsigned char[data_length];
  z_stream compress_stream, decompress_stream;
  compress_stream.zalloc = decompress_stream.zalloc = Z_NULL;
  compress_stream.zfree = decompress_stream.zfree = Z_NULL;
  compress_stream.opaque = decompress_stream.opaque = Z_NULL;
  // Initiate deflate and inflate with default compression level
  int deflate_return = deflateInit(&compress_stream, -1);
  int inflate_return = inflateInit(&decompress_stream);
  // If init goes wrong, we return an error struct
  if (deflate_return != Z_OK || inflate_return != Z_OK) {
    results.dataset_name = data.name;
    results.technique_name = "DEFLATE (zlib)";
    results.error = true;
    return results;
  }
  compress_stream.avail_in = data_length;
  compress_stream.next_in = decompressed;
  compress_stream.avail_out = data_length;
  compress_stream.next_out = compressed;
  decompress_stream.avail_in = data_length;
  decompress_stream.next_in = compressed;
  decompress_stream.avail_out = data_length;
  decompress_stream.next_out = decompressed;
  
  // Copy source data into decompressed buffer
  for(int i = 0; i < data_length; i++)
    decompressed[i] = data.bytes.at(i);
    
  // Run tests
  for(int i = 0; i < num_iterations; i++) {
    /// Compression
    // Get time
    double init_compress = omp_get_wtime(); 
    // Test compression
    int compressed_bytes;
    deflate_return = deflate(&compress_stream, Z_FINISH);
    compressed_bytes = compress_stream.total_out;
    // Get time and calculate difference
    double compress_time = omp_get_wtime();
    compression_times.push_back(compress_time - init_compress);

    /// Decompression
    // Get time
    double init_decompress = omp_get_wtime();
    // Test decompression
    int decompressed_bytes;
    inflate_return = inflate(&decompress_stream, Z_FINISH);
    decompressed_bytes = decompress_stream.total_out;
    // Get time and calculate difference
    double decompress_time = omp_get_wtime();
    decompression_times.push_back(decompress_time - init_decompress);
    // Set the ratio on the first iteration
    if(i == 0) compression_ratio = ((double)data.bytes.size())/((double)compressed_bytes);

    /// Check for errors
    if(inflate_return != Z_STREAM_END || deflate_return != Z_STREAM_END)
      results.error = true;
  }
  
  // Clean up
  delete compressed;
  delete decompressed;
  deflateEnd(&compress_stream);
  inflateEnd(&decompress_stream);
  

  // Fill in results
  // Name
  results.dataset_name = data.name;
  results.technique_name = "DEFLATE (zlib)";
  // Compression ratio
  results.compression_ratio = compression_ratio;
  // Times
  FillInTimes(compression_times, decompression_times, &results);

  // ... and we're done.
  return results;
}
