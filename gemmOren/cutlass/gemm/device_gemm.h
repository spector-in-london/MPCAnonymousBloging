/***************************************************************************************************
* Copyright (c) 2017-2018, NVIDIA CORPORATION.  All rights reserved.
*
* Redistribution and use in source and binary forms, with or without modification, are permitted
* provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright notice, this list of
*       conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright notice, this list of
*       conditions and the following disclaimer in the documentation and/or other materials
*       provided with the distribution.
*     * Neither the name of the NVIDIA CORPORATION nor the names of its contributors may be used
*       to endorse or promote products derived from this software without specific prior written
*       permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
* FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL NVIDIA CORPORATION BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
* OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TOR (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
**************************************************************************************************/
/*! \file
\brief device level GEMM implemented by more than one kernels.
*/
#pragma once

#if !defined(__CUDACC_RTC__)
#include <cuda.h>
#endif

#include "cutlass/coord.h"
#include "cutlass/util/platform.h"
namespace cutlass {
namespace gemm {

template<typename DeviceGemmTraits_ >
struct DeviceGemm {
  /// The Traits
  typedef DeviceGemmTraits_ Traits;
  /// Use the params object defined in traits
  typedef typename Traits::Params Params;

  /// Support for NVRTC
#if !defined(__CUDACC_RTC__)
  /// Launch the kernels in order
  static __host__ cudaError_t launch(Params const& params) {
    Traits::GemmTraits::KernelClass::launch(params.GemmParams);
    cudaError_t err = cudaGetLastError();
    if (err != cudaSuccess)
      return err;
    Traits::ReductionTraits::KernelClass::launch(params.ReductionParams);
    return cudaGetLastError();
  }
#endif

  ///
  /// Methods
  ///

  /// Ctor.
  CUTLASS_DEVICE DeviceGemm() {}
};
} // namespace device_gemm
} // namespace cutalss
