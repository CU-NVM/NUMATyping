//===--- opencl-c.h - OpenCL C language builtin function header -----------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef _OPENCL_H_
#define _OPENCL_H_

#include "opencl-c-base.h"

#if defined(__opencl_c_images)
#ifndef cl_khr_depth_images
#define cl_khr_depth_images
#endif //cl_khr_depth_images
#endif //defined(__opencl_c_images)

#if __OPENCL_C_VERSION__ < CL_VERSION_2_0
#ifdef cl_khr_3d_image_writes
#pragma OPENCL EXTENSION cl_khr_3d_image_writes : enable
#endif //cl_khr_3d_image_writes
#endif //__OPENCL_C_VERSION__ < CL_VERSION_2_0

#if (defined(__OPENCL_CPP_VERSION__) ||                                        \
     (__OPENCL_C_VERSION__ >= CL_VERSION_1_2)) &&                              \
    (defined(__SPIR__) || defined(__SPIRV__))
#pragma OPENCL EXTENSION cl_intel_planar_yuv : begin
#pragma OPENCL EXTENSION cl_intel_planar_yuv : end
#endif // (defined(__OPENCL_CPP_VERSION__) ||
       //  (__OPENCL_C_VERSION__ >= CL_VERSION_1_2)) &&
       // (defined(__SPIR__) || defined(__SPIRV__))

#define __ovld __attribute__((overloadable))
#define __conv __attribute__((convergent))

// Optimizations
#define __purefn __attribute__((pure))
#define __cnfn __attribute__((const))


// OpenCL v1.1/1.2/2.0 s6.2.3 - Explicit conversions

char __ovld __cnfn convert_char_rte(char);
char __ovld __cnfn convert_char_sat_rte(char);
char __ovld __cnfn convert_char_rtz(char);
char __ovld __cnfn convert_char_sat_rtz(char);
char __ovld __cnfn convert_char_rtp(char);
char __ovld __cnfn convert_char_sat_rtp(char);
char __ovld __cnfn convert_char_rtn(char);
char __ovld __cnfn convert_char_sat_rtn(char);
char __ovld __cnfn convert_char(char);
char __ovld __cnfn convert_char_sat(char);
char __ovld __cnfn convert_char_rte(uchar);
char __ovld __cnfn convert_char_sat_rte(uchar);
char __ovld __cnfn convert_char_rtz(uchar);
char __ovld __cnfn convert_char_sat_rtz(uchar);
char __ovld __cnfn convert_char_rtp(uchar);
char __ovld __cnfn convert_char_sat_rtp(uchar);
char __ovld __cnfn convert_char_rtn(uchar);
char __ovld __cnfn convert_char_sat_rtn(uchar);
char __ovld __cnfn convert_char(uchar);
char __ovld __cnfn convert_char_sat(uchar);
char __ovld __cnfn convert_char_rte(short);
char __ovld __cnfn convert_char_sat_rte(short);
char __ovld __cnfn convert_char_rtz(short);
char __ovld __cnfn convert_char_sat_rtz(short);
char __ovld __cnfn convert_char_rtp(short);
char __ovld __cnfn convert_char_sat_rtp(short);
char __ovld __cnfn convert_char_rtn(short);
char __ovld __cnfn convert_char_sat_rtn(short);
char __ovld __cnfn convert_char(short);
char __ovld __cnfn convert_char_sat(short);
char __ovld __cnfn convert_char_rte(ushort);
char __ovld __cnfn convert_char_sat_rte(ushort);
char __ovld __cnfn convert_char_rtz(ushort);
char __ovld __cnfn convert_char_sat_rtz(ushort);
char __ovld __cnfn convert_char_rtp(ushort);
char __ovld __cnfn convert_char_sat_rtp(ushort);
char __ovld __cnfn convert_char_rtn(ushort);
char __ovld __cnfn convert_char_sat_rtn(ushort);
char __ovld __cnfn convert_char(ushort);
char __ovld __cnfn convert_char_sat(ushort);
char __ovld __cnfn convert_char_rte(int);
char __ovld __cnfn convert_char_sat_rte(int);
char __ovld __cnfn convert_char_rtz(int);
char __ovld __cnfn convert_char_sat_rtz(int);
char __ovld __cnfn convert_char_rtp(int);
char __ovld __cnfn convert_char_sat_rtp(int);
char __ovld __cnfn convert_char_rtn(int);
char __ovld __cnfn convert_char_sat_rtn(int);
char __ovld __cnfn convert_char(int);
char __ovld __cnfn convert_char_sat(int);
char __ovld __cnfn convert_char_rte(uint);
char __ovld __cnfn convert_char_sat_rte(uint);
char __ovld __cnfn convert_char_rtz(uint);
char __ovld __cnfn convert_char_sat_rtz(uint);
char __ovld __cnfn convert_char_rtp(uint);
char __ovld __cnfn convert_char_sat_rtp(uint);
char __ovld __cnfn convert_char_rtn(uint);
char __ovld __cnfn convert_char_sat_rtn(uint);
char __ovld __cnfn convert_char(uint);
char __ovld __cnfn convert_char_sat(uint);
char __ovld __cnfn convert_char_rte(long);
char __ovld __cnfn convert_char_sat_rte(long);
char __ovld __cnfn convert_char_rtz(long);
char __ovld __cnfn convert_char_sat_rtz(long);
char __ovld __cnfn convert_char_rtp(long);
char __ovld __cnfn convert_char_sat_rtp(long);
char __ovld __cnfn convert_char_rtn(long);
char __ovld __cnfn convert_char_sat_rtn(long);
char __ovld __cnfn convert_char(long);
char __ovld __cnfn convert_char_sat(long);
char __ovld __cnfn convert_char_rte(ulong);
char __ovld __cnfn convert_char_sat_rte(ulong);
char __ovld __cnfn convert_char_rtz(ulong);
char __ovld __cnfn convert_char_sat_rtz(ulong);
char __ovld __cnfn convert_char_rtp(ulong);
char __ovld __cnfn convert_char_sat_rtp(ulong);
char __ovld __cnfn convert_char_rtn(ulong);
char __ovld __cnfn convert_char_sat_rtn(ulong);
char __ovld __cnfn convert_char(ulong);
char __ovld __cnfn convert_char_sat(ulong);
char __ovld __cnfn convert_char_rte(float);
char __ovld __cnfn convert_char_sat_rte(float);
char __ovld __cnfn convert_char_rtz(float);
char __ovld __cnfn convert_char_sat_rtz(float);
char __ovld __cnfn convert_char_rtp(float);
char __ovld __cnfn convert_char_sat_rtp(float);
char __ovld __cnfn convert_char_rtn(float);
char __ovld __cnfn convert_char_sat_rtn(float);
char __ovld __cnfn convert_char(float);
char __ovld __cnfn convert_char_sat(float);
uchar __ovld __cnfn convert_uchar_rte(char);
uchar __ovld __cnfn convert_uchar_sat_rte(char);
uchar __ovld __cnfn convert_uchar_rtz(char);
uchar __ovld __cnfn convert_uchar_sat_rtz(char);
uchar __ovld __cnfn convert_uchar_rtp(char);
uchar __ovld __cnfn convert_uchar_sat_rtp(char);
uchar __ovld __cnfn convert_uchar_rtn(char);
uchar __ovld __cnfn convert_uchar_sat_rtn(char);
uchar __ovld __cnfn convert_uchar(char);
uchar __ovld __cnfn convert_uchar_sat(char);
uchar __ovld __cnfn convert_uchar_rte(uchar);
uchar __ovld __cnfn convert_uchar_sat_rte(uchar);
uchar __ovld __cnfn convert_uchar_rtz(uchar);
uchar __ovld __cnfn convert_uchar_sat_rtz(uchar);
uchar __ovld __cnfn convert_uchar_rtp(uchar);
uchar __ovld __cnfn convert_uchar_sat_rtp(uchar);
uchar __ovld __cnfn convert_uchar_rtn(uchar);
uchar __ovld __cnfn convert_uchar_sat_rtn(uchar);
uchar __ovld __cnfn convert_uchar(uchar);
uchar __ovld __cnfn convert_uchar_sat(uchar);
uchar __ovld __cnfn convert_uchar_rte(short);
uchar __ovld __cnfn convert_uchar_sat_rte(short);
uchar __ovld __cnfn convert_uchar_rtz(short);
uchar __ovld __cnfn convert_uchar_sat_rtz(short);
uchar __ovld __cnfn convert_uchar_rtp(short);
uchar __ovld __cnfn convert_uchar_sat_rtp(short);
uchar __ovld __cnfn convert_uchar_rtn(short);
uchar __ovld __cnfn convert_uchar_sat_rtn(short);
uchar __ovld __cnfn convert_uchar(short);
uchar __ovld __cnfn convert_uchar_sat(short);
uchar __ovld __cnfn convert_uchar_rte(ushort);
uchar __ovld __cnfn convert_uchar_sat_rte(ushort);
uchar __ovld __cnfn convert_uchar_rtz(ushort);
uchar __ovld __cnfn convert_uchar_sat_rtz(ushort);
uchar __ovld __cnfn convert_uchar_rtp(ushort);
uchar __ovld __cnfn convert_uchar_sat_rtp(ushort);
uchar __ovld __cnfn convert_uchar_rtn(ushort);
uchar __ovld __cnfn convert_uchar_sat_rtn(ushort);
uchar __ovld __cnfn convert_uchar(ushort);
uchar __ovld __cnfn convert_uchar_sat(ushort);
uchar __ovld __cnfn convert_uchar_rte(int);
uchar __ovld __cnfn convert_uchar_sat_rte(int);
uchar __ovld __cnfn convert_uchar_rtz(int);
uchar __ovld __cnfn convert_uchar_sat_rtz(int);
uchar __ovld __cnfn convert_uchar_rtp(int);
uchar __ovld __cnfn convert_uchar_sat_rtp(int);
uchar __ovld __cnfn convert_uchar_rtn(int);
uchar __ovld __cnfn convert_uchar_sat_rtn(int);
uchar __ovld __cnfn convert_uchar(int);
uchar __ovld __cnfn convert_uchar_sat(int);
uchar __ovld __cnfn convert_uchar_rte(uint);
uchar __ovld __cnfn convert_uchar_sat_rte(uint);
uchar __ovld __cnfn convert_uchar_rtz(uint);
uchar __ovld __cnfn convert_uchar_sat_rtz(uint);
uchar __ovld __cnfn convert_uchar_rtp(uint);
uchar __ovld __cnfn convert_uchar_sat_rtp(uint);
uchar __ovld __cnfn convert_uchar_rtn(uint);
uchar __ovld __cnfn convert_uchar_sat_rtn(uint);
uchar __ovld __cnfn convert_uchar(uint);
uchar __ovld __cnfn convert_uchar_sat(uint);
uchar __ovld __cnfn convert_uchar_rte(long);
uchar __ovld __cnfn convert_uchar_sat_rte(long);
uchar __ovld __cnfn convert_uchar_rtz(long);
uchar __ovld __cnfn convert_uchar_sat_rtz(long);
uchar __ovld __cnfn convert_uchar_rtp(long);
uchar __ovld __cnfn convert_uchar_sat_rtp(long);
uchar __ovld __cnfn convert_uchar_rtn(long);
uchar __ovld __cnfn convert_uchar_sat_rtn(long);
uchar __ovld __cnfn convert_uchar(long);
uchar __ovld __cnfn convert_uchar_sat(long);
uchar __ovld __cnfn convert_uchar_rte(ulong);
uchar __ovld __cnfn convert_uchar_sat_rte(ulong);
uchar __ovld __cnfn convert_uchar_rtz(ulong);
uchar __ovld __cnfn convert_uchar_sat_rtz(ulong);
uchar __ovld __cnfn convert_uchar_rtp(ulong);
uchar __ovld __cnfn convert_uchar_sat_rtp(ulong);
uchar __ovld __cnfn convert_uchar_rtn(ulong);
uchar __ovld __cnfn convert_uchar_sat_rtn(ulong);
uchar __ovld __cnfn convert_uchar(ulong);
uchar __ovld __cnfn convert_uchar_sat(ulong);
uchar __ovld __cnfn convert_uchar_rte(float);
uchar __ovld __cnfn convert_uchar_sat_rte(float);
uchar __ovld __cnfn convert_uchar_rtz(float);
uchar __ovld __cnfn convert_uchar_sat_rtz(float);
uchar __ovld __cnfn convert_uchar_rtp(float);
uchar __ovld __cnfn convert_uchar_sat_rtp(float);
uchar __ovld __cnfn convert_uchar_rtn(float);
uchar __ovld __cnfn convert_uchar_sat_rtn(float);
uchar __ovld __cnfn convert_uchar(float);
uchar __ovld __cnfn convert_uchar_sat(float);

short __ovld __cnfn convert_short_rte(char);
short __ovld __cnfn convert_short_sat_rte(char);
short __ovld __cnfn convert_short_rtz(char);
short __ovld __cnfn convert_short_sat_rtz(char);
short __ovld __cnfn convert_short_rtp(char);
short __ovld __cnfn convert_short_sat_rtp(char);
short __ovld __cnfn convert_short_rtn(char);
short __ovld __cnfn convert_short_sat_rtn(char);
short __ovld __cnfn convert_short(char);
short __ovld __cnfn convert_short_sat(char);
short __ovld __cnfn convert_short_rte(uchar);
short __ovld __cnfn convert_short_sat_rte(uchar);
short __ovld __cnfn convert_short_rtz(uchar);
short __ovld __cnfn convert_short_sat_rtz(uchar);
short __ovld __cnfn convert_short_rtp(uchar);
short __ovld __cnfn convert_short_sat_rtp(uchar);
short __ovld __cnfn convert_short_rtn(uchar);
short __ovld __cnfn convert_short_sat_rtn(uchar);
short __ovld __cnfn convert_short(uchar);
short __ovld __cnfn convert_short_sat(uchar);
short __ovld __cnfn convert_short_rte(short);
short __ovld __cnfn convert_short_sat_rte(short);
short __ovld __cnfn convert_short_rtz(short);
short __ovld __cnfn convert_short_sat_rtz(short);
short __ovld __cnfn convert_short_rtp(short);
short __ovld __cnfn convert_short_sat_rtp(short);
short __ovld __cnfn convert_short_rtn(short);
short __ovld __cnfn convert_short_sat_rtn(short);
short __ovld __cnfn convert_short(short);
short __ovld __cnfn convert_short_sat(short);
short __ovld __cnfn convert_short_rte(ushort);
short __ovld __cnfn convert_short_sat_rte(ushort);
short __ovld __cnfn convert_short_rtz(ushort);
short __ovld __cnfn convert_short_sat_rtz(ushort);
short __ovld __cnfn convert_short_rtp(ushort);
short __ovld __cnfn convert_short_sat_rtp(ushort);
short __ovld __cnfn convert_short_rtn(ushort);
short __ovld __cnfn convert_short_sat_rtn(ushort);
short __ovld __cnfn convert_short(ushort);
short __ovld __cnfn convert_short_sat(ushort);
short __ovld __cnfn convert_short_rte(int);
short __ovld __cnfn convert_short_sat_rte(int);
short __ovld __cnfn convert_short_rtz(int);
short __ovld __cnfn convert_short_sat_rtz(int);
short __ovld __cnfn convert_short_rtp(int);
short __ovld __cnfn convert_short_sat_rtp(int);
short __ovld __cnfn convert_short_rtn(int);
short __ovld __cnfn convert_short_sat_rtn(int);
short __ovld __cnfn convert_short(int);
short __ovld __cnfn convert_short_sat(int);
short __ovld __cnfn convert_short_rte(uint);
short __ovld __cnfn convert_short_sat_rte(uint);
short __ovld __cnfn convert_short_rtz(uint);
short __ovld __cnfn convert_short_sat_rtz(uint);
short __ovld __cnfn convert_short_rtp(uint);
short __ovld __cnfn convert_short_sat_rtp(uint);
short __ovld __cnfn convert_short_rtn(uint);
short __ovld __cnfn convert_short_sat_rtn(uint);
short __ovld __cnfn convert_short(uint);
short __ovld __cnfn convert_short_sat(uint);
short __ovld __cnfn convert_short_rte(long);
short __ovld __cnfn convert_short_sat_rte(long);
short __ovld __cnfn convert_short_rtz(long);
short __ovld __cnfn convert_short_sat_rtz(long);
short __ovld __cnfn convert_short_rtp(long);
short __ovld __cnfn convert_short_sat_rtp(long);
short __ovld __cnfn convert_short_rtn(long);
short __ovld __cnfn convert_short_sat_rtn(long);
short __ovld __cnfn convert_short(long);
short __ovld __cnfn convert_short_sat(long);
short __ovld __cnfn convert_short_rte(ulong);
short __ovld __cnfn convert_short_sat_rte(ulong);
short __ovld __cnfn convert_short_rtz(ulong);
short __ovld __cnfn convert_short_sat_rtz(ulong);
short __ovld __cnfn convert_short_rtp(ulong);
short __ovld __cnfn convert_short_sat_rtp(ulong);
short __ovld __cnfn convert_short_rtn(ulong);
short __ovld __cnfn convert_short_sat_rtn(ulong);
short __ovld __cnfn convert_short(ulong);
short __ovld __cnfn convert_short_sat(ulong);
short __ovld __cnfn convert_short_rte(float);
short __ovld __cnfn convert_short_sat_rte(float);
short __ovld __cnfn convert_short_rtz(float);
short __ovld __cnfn convert_short_sat_rtz(float);
short __ovld __cnfn convert_short_rtp(float);
short __ovld __cnfn convert_short_sat_rtp(float);
short __ovld __cnfn convert_short_rtn(float);
short __ovld __cnfn convert_short_sat_rtn(float);
short __ovld __cnfn convert_short(float);
short __ovld __cnfn convert_short_sat(float);
ushort __ovld __cnfn convert_ushort_rte(char);
ushort __ovld __cnfn convert_ushort_sat_rte(char);
ushort __ovld __cnfn convert_ushort_rtz(char);
ushort __ovld __cnfn convert_ushort_sat_rtz(char);
ushort __ovld __cnfn convert_ushort_rtp(char);
ushort __ovld __cnfn convert_ushort_sat_rtp(char);
ushort __ovld __cnfn convert_ushort_rtn(char);
ushort __ovld __cnfn convert_ushort_sat_rtn(char);
ushort __ovld __cnfn convert_ushort(char);
ushort __ovld __cnfn convert_ushort_sat(char);
ushort __ovld __cnfn convert_ushort_rte(uchar);
ushort __ovld __cnfn convert_ushort_sat_rte(uchar);
ushort __ovld __cnfn convert_ushort_rtz(uchar);
ushort __ovld __cnfn convert_ushort_sat_rtz(uchar);
ushort __ovld __cnfn convert_ushort_rtp(uchar);
ushort __ovld __cnfn convert_ushort_sat_rtp(uchar);
ushort __ovld __cnfn convert_ushort_rtn(uchar);
ushort __ovld __cnfn convert_ushort_sat_rtn(uchar);
ushort __ovld __cnfn convert_ushort(uchar);
ushort __ovld __cnfn convert_ushort_sat(uchar);
ushort __ovld __cnfn convert_ushort_rte(short);
ushort __ovld __cnfn convert_ushort_sat_rte(short);
ushort __ovld __cnfn convert_ushort_rtz(short);
ushort __ovld __cnfn convert_ushort_sat_rtz(short);
ushort __ovld __cnfn convert_ushort_rtp(short);
ushort __ovld __cnfn convert_ushort_sat_rtp(short);
ushort __ovld __cnfn convert_ushort_rtn(short);
ushort __ovld __cnfn convert_ushort_sat_rtn(short);
ushort __ovld __cnfn convert_ushort(short);
ushort __ovld __cnfn convert_ushort_sat(short);
ushort __ovld __cnfn convert_ushort_rte(ushort);
ushort __ovld __cnfn convert_ushort_sat_rte(ushort);
ushort __ovld __cnfn convert_ushort_rtz(ushort);
ushort __ovld __cnfn convert_ushort_sat_rtz(ushort);
ushort __ovld __cnfn convert_ushort_rtp(ushort);
ushort __ovld __cnfn convert_ushort_sat_rtp(ushort);
ushort __ovld __cnfn convert_ushort_rtn(ushort);
ushort __ovld __cnfn convert_ushort_sat_rtn(ushort);
ushort __ovld __cnfn convert_ushort(ushort);
ushort __ovld __cnfn convert_ushort_sat(ushort);
ushort __ovld __cnfn convert_ushort_rte(int);
ushort __ovld __cnfn convert_ushort_sat_rte(int);
ushort __ovld __cnfn convert_ushort_rtz(int);
ushort __ovld __cnfn convert_ushort_sat_rtz(int);
ushort __ovld __cnfn convert_ushort_rtp(int);
ushort __ovld __cnfn convert_ushort_sat_rtp(int);
ushort __ovld __cnfn convert_ushort_rtn(int);
ushort __ovld __cnfn convert_ushort_sat_rtn(int);
ushort __ovld __cnfn convert_ushort(int);
ushort __ovld __cnfn convert_ushort_sat(int);
ushort __ovld __cnfn convert_ushort_rte(uint);
ushort __ovld __cnfn convert_ushort_sat_rte(uint);
ushort __ovld __cnfn convert_ushort_rtz(uint);
ushort __ovld __cnfn convert_ushort_sat_rtz(uint);
ushort __ovld __cnfn convert_ushort_rtp(uint);
ushort __ovld __cnfn convert_ushort_sat_rtp(uint);
ushort __ovld __cnfn convert_ushort_rtn(uint);
ushort __ovld __cnfn convert_ushort_sat_rtn(uint);
ushort __ovld __cnfn convert_ushort(uint);
ushort __ovld __cnfn convert_ushort_sat(uint);
ushort __ovld __cnfn convert_ushort_rte(long);
ushort __ovld __cnfn convert_ushort_sat_rte(long);
ushort __ovld __cnfn convert_ushort_rtz(long);
ushort __ovld __cnfn convert_ushort_sat_rtz(long);
ushort __ovld __cnfn convert_ushort_rtp(long);
ushort __ovld __cnfn convert_ushort_sat_rtp(long);
ushort __ovld __cnfn convert_ushort_rtn(long);
ushort __ovld __cnfn convert_ushort_sat_rtn(long);
ushort __ovld __cnfn convert_ushort(long);
ushort __ovld __cnfn convert_ushort_sat(long);
ushort __ovld __cnfn convert_ushort_rte(ulong);
ushort __ovld __cnfn convert_ushort_sat_rte(ulong);
ushort __ovld __cnfn convert_ushort_rtz(ulong);
ushort __ovld __cnfn convert_ushort_sat_rtz(ulong);
ushort __ovld __cnfn convert_ushort_rtp(ulong);
ushort __ovld __cnfn convert_ushort_sat_rtp(ulong);
ushort __ovld __cnfn convert_ushort_rtn(ulong);
ushort __ovld __cnfn convert_ushort_sat_rtn(ulong);
ushort __ovld __cnfn convert_ushort(ulong);
ushort __ovld __cnfn convert_ushort_sat(ulong);
ushort __ovld __cnfn convert_ushort_rte(float);
ushort __ovld __cnfn convert_ushort_sat_rte(float);
ushort __ovld __cnfn convert_ushort_rtz(float);
ushort __ovld __cnfn convert_ushort_sat_rtz(float);
ushort __ovld __cnfn convert_ushort_rtp(float);
ushort __ovld __cnfn convert_ushort_sat_rtp(float);
ushort __ovld __cnfn convert_ushort_rtn(float);
ushort __ovld __cnfn convert_ushort_sat_rtn(float);
ushort __ovld __cnfn convert_ushort(float);
ushort __ovld __cnfn convert_ushort_sat(float);
int __ovld __cnfn convert_int_rte(char);
int __ovld __cnfn convert_int_sat_rte(char);
int __ovld __cnfn convert_int_rtz(char);
int __ovld __cnfn convert_int_sat_rtz(char);
int __ovld __cnfn convert_int_rtp(char);
int __ovld __cnfn convert_int_sat_rtp(char);
int __ovld __cnfn convert_int_rtn(char);
int __ovld __cnfn convert_int_sat_rtn(char);
int __ovld __cnfn convert_int(char);
int __ovld __cnfn convert_int_sat(char);
int __ovld __cnfn convert_int_rte(uchar);
int __ovld __cnfn convert_int_sat_rte(uchar);
int __ovld __cnfn convert_int_rtz(uchar);
int __ovld __cnfn convert_int_sat_rtz(uchar);
int __ovld __cnfn convert_int_rtp(uchar);
int __ovld __cnfn convert_int_sat_rtp(uchar);
int __ovld __cnfn convert_int_rtn(uchar);
int __ovld __cnfn convert_int_sat_rtn(uchar);
int __ovld __cnfn convert_int(uchar);
int __ovld __cnfn convert_int_sat(uchar);
int __ovld __cnfn convert_int_rte(short);
int __ovld __cnfn convert_int_sat_rte(short);
int __ovld __cnfn convert_int_rtz(short);
int __ovld __cnfn convert_int_sat_rtz(short);
int __ovld __cnfn convert_int_rtp(short);
int __ovld __cnfn convert_int_sat_rtp(short);
int __ovld __cnfn convert_int_rtn(short);
int __ovld __cnfn convert_int_sat_rtn(short);
int __ovld __cnfn convert_int(short);
int __ovld __cnfn convert_int_sat(short);
int __ovld __cnfn convert_int_rte(ushort);
int __ovld __cnfn convert_int_sat_rte(ushort);
int __ovld __cnfn convert_int_rtz(ushort);
int __ovld __cnfn convert_int_sat_rtz(ushort);
int __ovld __cnfn convert_int_rtp(ushort);
int __ovld __cnfn convert_int_sat_rtp(ushort);
int __ovld __cnfn convert_int_rtn(ushort);
int __ovld __cnfn convert_int_sat_rtn(ushort);
int __ovld __cnfn convert_int(ushort);
int __ovld __cnfn convert_int_sat(ushort);
int __ovld __cnfn convert_int_rte(int);
int __ovld __cnfn convert_int_sat_rte(int);
int __ovld __cnfn convert_int_rtz(int);
int __ovld __cnfn convert_int_sat_rtz(int);
int __ovld __cnfn convert_int_rtp(int);
int __ovld __cnfn convert_int_sat_rtp(int);
int __ovld __cnfn convert_int_rtn(int);
int __ovld __cnfn convert_int_sat_rtn(int);
int __ovld __cnfn convert_int(int);
int __ovld __cnfn convert_int_sat(int);
int __ovld __cnfn convert_int_rte(uint);
int __ovld __cnfn convert_int_sat_rte(uint);
int __ovld __cnfn convert_int_rtz(uint);
int __ovld __cnfn convert_int_sat_rtz(uint);
int __ovld __cnfn convert_int_rtp(uint);
int __ovld __cnfn convert_int_sat_rtp(uint);
int __ovld __cnfn convert_int_rtn(uint);
int __ovld __cnfn convert_int_sat_rtn(uint);
int __ovld __cnfn convert_int(uint);
int __ovld __cnfn convert_int_sat(uint);
int __ovld __cnfn convert_int_rte(long);
int __ovld __cnfn convert_int_sat_rte(long);
int __ovld __cnfn convert_int_rtz(long);
int __ovld __cnfn convert_int_sat_rtz(long);
int __ovld __cnfn convert_int_rtp(long);
int __ovld __cnfn convert_int_sat_rtp(long);
int __ovld __cnfn convert_int_rtn(long);
int __ovld __cnfn convert_int_sat_rtn(long);
int __ovld __cnfn convert_int(long);
int __ovld __cnfn convert_int_sat(long);
int __ovld __cnfn convert_int_rte(ulong);
int __ovld __cnfn convert_int_sat_rte(ulong);
int __ovld __cnfn convert_int_rtz(ulong);
int __ovld __cnfn convert_int_sat_rtz(ulong);
int __ovld __cnfn convert_int_rtp(ulong);
int __ovld __cnfn convert_int_sat_rtp(ulong);
int __ovld __cnfn convert_int_rtn(ulong);
int __ovld __cnfn convert_int_sat_rtn(ulong);
int __ovld __cnfn convert_int(ulong);
int __ovld __cnfn convert_int_sat(ulong);
int __ovld __cnfn convert_int_rte(float);
int __ovld __cnfn convert_int_sat_rte(float);
int __ovld __cnfn convert_int_rtz(float);
int __ovld __cnfn convert_int_sat_rtz(float);
int __ovld __cnfn convert_int_rtp(float);
int __ovld __cnfn convert_int_sat_rtp(float);
int __ovld __cnfn convert_int_rtn(float);
int __ovld __cnfn convert_int_sat_rtn(float);
int __ovld __cnfn convert_int(float);
int __ovld __cnfn convert_int_sat(float);
uint __ovld __cnfn convert_uint_rte(char);
uint __ovld __cnfn convert_uint_sat_rte(char);
uint __ovld __cnfn convert_uint_rtz(char);
uint __ovld __cnfn convert_uint_sat_rtz(char);
uint __ovld __cnfn convert_uint_rtp(char);
uint __ovld __cnfn convert_uint_sat_rtp(char);
uint __ovld __cnfn convert_uint_rtn(char);
uint __ovld __cnfn convert_uint_sat_rtn(char);
uint __ovld __cnfn convert_uint(char);
uint __ovld __cnfn convert_uint_sat(char);
uint __ovld __cnfn convert_uint_rte(uchar);
uint __ovld __cnfn convert_uint_sat_rte(uchar);
uint __ovld __cnfn convert_uint_rtz(uchar);
uint __ovld __cnfn convert_uint_sat_rtz(uchar);
uint __ovld __cnfn convert_uint_rtp(uchar);
uint __ovld __cnfn convert_uint_sat_rtp(uchar);
uint __ovld __cnfn convert_uint_rtn(uchar);
uint __ovld __cnfn convert_uint_sat_rtn(uchar);
uint __ovld __cnfn convert_uint(uchar);
uint __ovld __cnfn convert_uint_sat(uchar);
uint __ovld __cnfn convert_uint_rte(short);
uint __ovld __cnfn convert_uint_sat_rte(short);
uint __ovld __cnfn convert_uint_rtz(short);
uint __ovld __cnfn convert_uint_sat_rtz(short);
uint __ovld __cnfn convert_uint_rtp(short);
uint __ovld __cnfn convert_uint_sat_rtp(short);
uint __ovld __cnfn convert_uint_rtn(short);
uint __ovld __cnfn convert_uint_sat_rtn(short);
uint __ovld __cnfn convert_uint(short);
uint __ovld __cnfn convert_uint_sat(short);
uint __ovld __cnfn convert_uint_rte(ushort);
uint __ovld __cnfn convert_uint_sat_rte(ushort);
uint __ovld __cnfn convert_uint_rtz(ushort);
uint __ovld __cnfn convert_uint_sat_rtz(ushort);
uint __ovld __cnfn convert_uint_rtp(ushort);
uint __ovld __cnfn convert_uint_sat_rtp(ushort);
uint __ovld __cnfn convert_uint_rtn(ushort);
uint __ovld __cnfn convert_uint_sat_rtn(ushort);
uint __ovld __cnfn convert_uint(ushort);
uint __ovld __cnfn convert_uint_sat(ushort);
uint __ovld __cnfn convert_uint_rte(int);
uint __ovld __cnfn convert_uint_sat_rte(int);
uint __ovld __cnfn convert_uint_rtz(int);
uint __ovld __cnfn convert_uint_sat_rtz(int);
uint __ovld __cnfn convert_uint_rtp(int);
uint __ovld __cnfn convert_uint_sat_rtp(int);
uint __ovld __cnfn convert_uint_rtn(int);
uint __ovld __cnfn convert_uint_sat_rtn(int);
uint __ovld __cnfn convert_uint(int);
uint __ovld __cnfn convert_uint_sat(int);
uint __ovld __cnfn convert_uint_rte(uint);
uint __ovld __cnfn convert_uint_sat_rte(uint);
uint __ovld __cnfn convert_uint_rtz(uint);
uint __ovld __cnfn convert_uint_sat_rtz(uint);
uint __ovld __cnfn convert_uint_rtp(uint);
uint __ovld __cnfn convert_uint_sat_rtp(uint);
uint __ovld __cnfn convert_uint_rtn(uint);
uint __ovld __cnfn convert_uint_sat_rtn(uint);
uint __ovld __cnfn convert_uint(uint);
uint __ovld __cnfn convert_uint_sat(uint);
uint __ovld __cnfn convert_uint_rte(long);
uint __ovld __cnfn convert_uint_sat_rte(long);
uint __ovld __cnfn convert_uint_rtz(long);
uint __ovld __cnfn convert_uint_sat_rtz(long);
uint __ovld __cnfn convert_uint_rtp(long);
uint __ovld __cnfn convert_uint_sat_rtp(long);
uint __ovld __cnfn convert_uint_rtn(long);
uint __ovld __cnfn convert_uint_sat_rtn(long);
uint __ovld __cnfn convert_uint(long);
uint __ovld __cnfn convert_uint_sat(long);
uint __ovld __cnfn convert_uint_rte(ulong);
uint __ovld __cnfn convert_uint_sat_rte(ulong);
uint __ovld __cnfn convert_uint_rtz(ulong);
uint __ovld __cnfn convert_uint_sat_rtz(ulong);
uint __ovld __cnfn convert_uint_rtp(ulong);
uint __ovld __cnfn convert_uint_sat_rtp(ulong);
uint __ovld __cnfn convert_uint_rtn(ulong);
uint __ovld __cnfn convert_uint_sat_rtn(ulong);
uint __ovld __cnfn convert_uint(ulong);
uint __ovld __cnfn convert_uint_sat(ulong);
uint __ovld __cnfn convert_uint_rte(float);
uint __ovld __cnfn convert_uint_sat_rte(float);
uint __ovld __cnfn convert_uint_rtz(float);
uint __ovld __cnfn convert_uint_sat_rtz(float);
uint __ovld __cnfn convert_uint_rtp(float);
uint __ovld __cnfn convert_uint_sat_rtp(float);
uint __ovld __cnfn convert_uint_rtn(float);
uint __ovld __cnfn convert_uint_sat_rtn(float);
uint __ovld __cnfn convert_uint(float);
uint __ovld __cnfn convert_uint_sat(float);
long __ovld __cnfn convert_long_rte(char);
long __ovld __cnfn convert_long_sat_rte(char);
long __ovld __cnfn convert_long_rtz(char);
long __ovld __cnfn convert_long_sat_rtz(char);
long __ovld __cnfn convert_long_rtp(char);
long __ovld __cnfn convert_long_sat_rtp(char);
long __ovld __cnfn convert_long_rtn(char);
long __ovld __cnfn convert_long_sat_rtn(char);
long __ovld __cnfn convert_long(char);
long __ovld __cnfn convert_long_sat(char);
long __ovld __cnfn convert_long_rte(uchar);
long __ovld __cnfn convert_long_sat_rte(uchar);
long __ovld __cnfn convert_long_rtz(uchar);
long __ovld __cnfn convert_long_sat_rtz(uchar);
long __ovld __cnfn convert_long_rtp(uchar);
long __ovld __cnfn convert_long_sat_rtp(uchar);
long __ovld __cnfn convert_long_rtn(uchar);
long __ovld __cnfn convert_long_sat_rtn(uchar);
long __ovld __cnfn convert_long(uchar);
long __ovld __cnfn convert_long_sat(uchar);
long __ovld __cnfn convert_long_rte(short);
long __ovld __cnfn convert_long_sat_rte(short);
long __ovld __cnfn convert_long_rtz(short);
long __ovld __cnfn convert_long_sat_rtz(short);
long __ovld __cnfn convert_long_rtp(short);
long __ovld __cnfn convert_long_sat_rtp(short);
long __ovld __cnfn convert_long_rtn(short);
long __ovld __cnfn convert_long_sat_rtn(short);
long __ovld __cnfn convert_long(short);
long __ovld __cnfn convert_long_sat(short);
long __ovld __cnfn convert_long_rte(ushort);
long __ovld __cnfn convert_long_sat_rte(ushort);
long __ovld __cnfn convert_long_rtz(ushort);
long __ovld __cnfn convert_long_sat_rtz(ushort);
long __ovld __cnfn convert_long_rtp(ushort);
long __ovld __cnfn convert_long_sat_rtp(ushort);
long __ovld __cnfn convert_long_rtn(ushort);
long __ovld __cnfn convert_long_sat_rtn(ushort);
long __ovld __cnfn convert_long(ushort);
long __ovld __cnfn convert_long_sat(ushort);
long __ovld __cnfn convert_long_rte(int);
long __ovld __cnfn convert_long_sat_rte(int);
long __ovld __cnfn convert_long_rtz(int);
long __ovld __cnfn convert_long_sat_rtz(int);
long __ovld __cnfn convert_long_rtp(int);
long __ovld __cnfn convert_long_sat_rtp(int);
long __ovld __cnfn convert_long_rtn(int);
long __ovld __cnfn convert_long_sat_rtn(int);
long __ovld __cnfn convert_long(int);
long __ovld __cnfn convert_long_sat(int);
long __ovld __cnfn convert_long_rte(uint);
long __ovld __cnfn convert_long_sat_rte(uint);
long __ovld __cnfn convert_long_rtz(uint);
long __ovld __cnfn convert_long_sat_rtz(uint);
long __ovld __cnfn convert_long_rtp(uint);
long __ovld __cnfn convert_long_sat_rtp(uint);
long __ovld __cnfn convert_long_rtn(uint);
long __ovld __cnfn convert_long_sat_rtn(uint);
long __ovld __cnfn convert_long(uint);
long __ovld __cnfn convert_long_sat(uint);
long __ovld __cnfn convert_long_rte(long);
long __ovld __cnfn convert_long_sat_rte(long);
long __ovld __cnfn convert_long_rtz(long);
long __ovld __cnfn convert_long_sat_rtz(long);
long __ovld __cnfn convert_long_rtp(long);
long __ovld __cnfn convert_long_sat_rtp(long);
long __ovld __cnfn convert_long_rtn(long);
long __ovld __cnfn convert_long_sat_rtn(long);
long __ovld __cnfn convert_long(long);
long __ovld __cnfn convert_long_sat(long);
long __ovld __cnfn convert_long_rte(ulong);
long __ovld __cnfn convert_long_sat_rte(ulong);
long __ovld __cnfn convert_long_rtz(ulong);
long __ovld __cnfn convert_long_sat_rtz(ulong);
long __ovld __cnfn convert_long_rtp(ulong);
long __ovld __cnfn convert_long_sat_rtp(ulong);
long __ovld __cnfn convert_long_rtn(ulong);
long __ovld __cnfn convert_long_sat_rtn(ulong);
long __ovld __cnfn convert_long(ulong);
long __ovld __cnfn convert_long_sat(ulong);
long __ovld __cnfn convert_long_rte(float);
long __ovld __cnfn convert_long_sat_rte(float);
long __ovld __cnfn convert_long_rtz(float);
long __ovld __cnfn convert_long_sat_rtz(float);
long __ovld __cnfn convert_long_rtp(float);
long __ovld __cnfn convert_long_sat_rtp(float);
long __ovld __cnfn convert_long_rtn(float);
long __ovld __cnfn convert_long_sat_rtn(float);
long __ovld __cnfn convert_long(float);
long __ovld __cnfn convert_long_sat(float);
ulong __ovld __cnfn convert_ulong_rte(char);
ulong __ovld __cnfn convert_ulong_sat_rte(char);
ulong __ovld __cnfn convert_ulong_rtz(char);
ulong __ovld __cnfn convert_ulong_sat_rtz(char);
ulong __ovld __cnfn convert_ulong_rtp(char);
ulong __ovld __cnfn convert_ulong_sat_rtp(char);
ulong __ovld __cnfn convert_ulong_rtn(char);
ulong __ovld __cnfn convert_ulong_sat_rtn(char);
ulong __ovld __cnfn convert_ulong(char);
ulong __ovld __cnfn convert_ulong_sat(char);
ulong __ovld __cnfn convert_ulong_rte(uchar);
ulong __ovld __cnfn convert_ulong_sat_rte(uchar);
ulong __ovld __cnfn convert_ulong_rtz(uchar);
ulong __ovld __cnfn convert_ulong_sat_rtz(uchar);
ulong __ovld __cnfn convert_ulong_rtp(uchar);
ulong __ovld __cnfn convert_ulong_sat_rtp(uchar);
ulong __ovld __cnfn convert_ulong_rtn(uchar);
ulong __ovld __cnfn convert_ulong_sat_rtn(uchar);
ulong __ovld __cnfn convert_ulong(uchar);
ulong __ovld __cnfn convert_ulong_sat(uchar);
ulong __ovld __cnfn convert_ulong_rte(short);
ulong __ovld __cnfn convert_ulong_sat_rte(short);
ulong __ovld __cnfn convert_ulong_rtz(short);
ulong __ovld __cnfn convert_ulong_sat_rtz(short);
ulong __ovld __cnfn convert_ulong_rtp(short);
ulong __ovld __cnfn convert_ulong_sat_rtp(short);
ulong __ovld __cnfn convert_ulong_rtn(short);
ulong __ovld __cnfn convert_ulong_sat_rtn(short);
ulong __ovld __cnfn convert_ulong(short);
ulong __ovld __cnfn convert_ulong_sat(short);
ulong __ovld __cnfn convert_ulong_rte(ushort);
ulong __ovld __cnfn convert_ulong_sat_rte(ushort);
ulong __ovld __cnfn convert_ulong_rtz(ushort);
ulong __ovld __cnfn convert_ulong_sat_rtz(ushort);
ulong __ovld __cnfn convert_ulong_rtp(ushort);
ulong __ovld __cnfn convert_ulong_sat_rtp(ushort);
ulong __ovld __cnfn convert_ulong_rtn(ushort);
ulong __ovld __cnfn convert_ulong_sat_rtn(ushort);
ulong __ovld __cnfn convert_ulong(ushort);
ulong __ovld __cnfn convert_ulong_sat(ushort);
ulong __ovld __cnfn convert_ulong_rte(int);
ulong __ovld __cnfn convert_ulong_sat_rte(int);
ulong __ovld __cnfn convert_ulong_rtz(int);
ulong __ovld __cnfn convert_ulong_sat_rtz(int);
ulong __ovld __cnfn convert_ulong_rtp(int);
ulong __ovld __cnfn convert_ulong_sat_rtp(int);
ulong __ovld __cnfn convert_ulong_rtn(int);
ulong __ovld __cnfn convert_ulong_sat_rtn(int);
ulong __ovld __cnfn convert_ulong(int);
ulong __ovld __cnfn convert_ulong_sat(int);
ulong __ovld __cnfn convert_ulong_rte(uint);
ulong __ovld __cnfn convert_ulong_sat_rte(uint);
ulong __ovld __cnfn convert_ulong_rtz(uint);
ulong __ovld __cnfn convert_ulong_sat_rtz(uint);
ulong __ovld __cnfn convert_ulong_rtp(uint);
ulong __ovld __cnfn convert_ulong_sat_rtp(uint);
ulong __ovld __cnfn convert_ulong_rtn(uint);
ulong __ovld __cnfn convert_ulong_sat_rtn(uint);
ulong __ovld __cnfn convert_ulong(uint);
ulong __ovld __cnfn convert_ulong_sat(uint);
ulong __ovld __cnfn convert_ulong_rte(long);
ulong __ovld __cnfn convert_ulong_sat_rte(long);
ulong __ovld __cnfn convert_ulong_rtz(long);
ulong __ovld __cnfn convert_ulong_sat_rtz(long);
ulong __ovld __cnfn convert_ulong_rtp(long);
ulong __ovld __cnfn convert_ulong_sat_rtp(long);
ulong __ovld __cnfn convert_ulong_rtn(long);
ulong __ovld __cnfn convert_ulong_sat_rtn(long);
ulong __ovld __cnfn convert_ulong(long);
ulong __ovld __cnfn convert_ulong_sat(long);
ulong __ovld __cnfn convert_ulong_rte(ulong);
ulong __ovld __cnfn convert_ulong_sat_rte(ulong);
ulong __ovld __cnfn convert_ulong_rtz(ulong);
ulong __ovld __cnfn convert_ulong_sat_rtz(ulong);
ulong __ovld __cnfn convert_ulong_rtp(ulong);
ulong __ovld __cnfn convert_ulong_sat_rtp(ulong);
ulong __ovld __cnfn convert_ulong_rtn(ulong);
ulong __ovld __cnfn convert_ulong_sat_rtn(ulong);
ulong __ovld __cnfn convert_ulong(ulong);
ulong __ovld __cnfn convert_ulong_sat(ulong);
ulong __ovld __cnfn convert_ulong_rte(float);
ulong __ovld __cnfn convert_ulong_sat_rte(float);
ulong __ovld __cnfn convert_ulong_rtz(float);
ulong __ovld __cnfn convert_ulong_sat_rtz(float);
ulong __ovld __cnfn convert_ulong_rtp(float);
ulong __ovld __cnfn convert_ulong_sat_rtp(float);
ulong __ovld __cnfn convert_ulong_rtn(float);
ulong __ovld __cnfn convert_ulong_sat_rtn(float);
ulong __ovld __cnfn convert_ulong(float);
ulong __ovld __cnfn convert_ulong_sat(float);
float __ovld __cnfn convert_float_rte(char);
float __ovld __cnfn convert_float_rtz(char);
float __ovld __cnfn convert_float_rtp(char);
float __ovld __cnfn convert_float_rtn(char);
float __ovld __cnfn convert_float(char);
float __ovld __cnfn convert_float_rte(uchar);
float __ovld __cnfn convert_float_rtz(uchar);
float __ovld __cnfn convert_float_rtp(uchar);
float __ovld __cnfn convert_float_rtn(uchar);
float __ovld __cnfn convert_float(uchar);
float __ovld __cnfn convert_float_rte(short);
float __ovld __cnfn convert_float_rtz(short);
float __ovld __cnfn convert_float_rtp(short);
float __ovld __cnfn convert_float_rtn(short);
float __ovld __cnfn convert_float(short);
float __ovld __cnfn convert_float_rte(ushort);
float __ovld __cnfn convert_float_rtz(ushort);
float __ovld __cnfn convert_float_rtp(ushort);
float __ovld __cnfn convert_float_rtn(ushort);
float __ovld __cnfn convert_float(ushort);
float __ovld __cnfn convert_float_rte(int);
float __ovld __cnfn convert_float_rtz(int);
float __ovld __cnfn convert_float_rtp(int);
float __ovld __cnfn convert_float_rtn(int);
float __ovld __cnfn convert_float(int);
float __ovld __cnfn convert_float_rte(uint);
float __ovld __cnfn convert_float_rtz(uint);
float __ovld __cnfn convert_float_rtp(uint);
float __ovld __cnfn convert_float_rtn(uint);
float __ovld __cnfn convert_float(uint);
float __ovld __cnfn convert_float_rte(long);
float __ovld __cnfn convert_float_rtz(long);
float __ovld __cnfn convert_float_rtp(long);
float __ovld __cnfn convert_float_rtn(long);
float __ovld __cnfn convert_float(long);
float __ovld __cnfn convert_float_rte(ulong);
float __ovld __cnfn convert_float_rtz(ulong);
float __ovld __cnfn convert_float_rtp(ulong);
float __ovld __cnfn convert_float_rtn(ulong);
float __ovld __cnfn convert_float(ulong);
float __ovld __cnfn convert_float_rte(float);
float __ovld __cnfn convert_float_rtz(float);
float __ovld __cnfn convert_float_rtp(float);
float __ovld __cnfn convert_float_rtn(float);
float __ovld __cnfn convert_float(float);
char2 __ovld __cnfn convert_char2_rte(char2);
char2 __ovld __cnfn convert_char2_sat_rte(char2);
char2 __ovld __cnfn convert_char2_rtz(char2);
char2 __ovld __cnfn convert_char2_sat_rtz(char2);
char2 __ovld __cnfn convert_char2_rtp(char2);
char2 __ovld __cnfn convert_char2_sat_rtp(char2);
char2 __ovld __cnfn convert_char2_rtn(char2);
char2 __ovld __cnfn convert_char2_sat_rtn(char2);
char2 __ovld __cnfn convert_char2(char2);
char2 __ovld __cnfn convert_char2_sat(char2);
char2 __ovld __cnfn convert_char2_rte(uchar2);
char2 __ovld __cnfn convert_char2_sat_rte(uchar2);
char2 __ovld __cnfn convert_char2_rtz(uchar2);
char2 __ovld __cnfn convert_char2_sat_rtz(uchar2);
char2 __ovld __cnfn convert_char2_rtp(uchar2);
char2 __ovld __cnfn convert_char2_sat_rtp(uchar2);
char2 __ovld __cnfn convert_char2_rtn(uchar2);
char2 __ovld __cnfn convert_char2_sat_rtn(uchar2);
char2 __ovld __cnfn convert_char2(uchar2);
char2 __ovld __cnfn convert_char2_sat(uchar2);
char2 __ovld __cnfn convert_char2_rte(short2);
char2 __ovld __cnfn convert_char2_sat_rte(short2);
char2 __ovld __cnfn convert_char2_rtz(short2);
char2 __ovld __cnfn convert_char2_sat_rtz(short2);
char2 __ovld __cnfn convert_char2_rtp(short2);
char2 __ovld __cnfn convert_char2_sat_rtp(short2);
char2 __ovld __cnfn convert_char2_rtn(short2);
char2 __ovld __cnfn convert_char2_sat_rtn(short2);
char2 __ovld __cnfn convert_char2(short2);
char2 __ovld __cnfn convert_char2_sat(short2);
char2 __ovld __cnfn convert_char2_rte(ushort2);
char2 __ovld __cnfn convert_char2_sat_rte(ushort2);
char2 __ovld __cnfn convert_char2_rtz(ushort2);
char2 __ovld __cnfn convert_char2_sat_rtz(ushort2);
char2 __ovld __cnfn convert_char2_rtp(ushort2);
char2 __ovld __cnfn convert_char2_sat_rtp(ushort2);
char2 __ovld __cnfn convert_char2_rtn(ushort2);
char2 __ovld __cnfn convert_char2_sat_rtn(ushort2);
char2 __ovld __cnfn convert_char2(ushort2);
char2 __ovld __cnfn convert_char2_sat(ushort2);
char2 __ovld __cnfn convert_char2_rte(int2);
char2 __ovld __cnfn convert_char2_sat_rte(int2);
char2 __ovld __cnfn convert_char2_rtz(int2);
char2 __ovld __cnfn convert_char2_sat_rtz(int2);
char2 __ovld __cnfn convert_char2_rtp(int2);
char2 __ovld __cnfn convert_char2_sat_rtp(int2);
char2 __ovld __cnfn convert_char2_rtn(int2);
char2 __ovld __cnfn convert_char2_sat_rtn(int2);
char2 __ovld __cnfn convert_char2(int2);
char2 __ovld __cnfn convert_char2_sat(int2);
char2 __ovld __cnfn convert_char2_rte(uint2);
char2 __ovld __cnfn convert_char2_sat_rte(uint2);
char2 __ovld __cnfn convert_char2_rtz(uint2);
char2 __ovld __cnfn convert_char2_sat_rtz(uint2);
char2 __ovld __cnfn convert_char2_rtp(uint2);
char2 __ovld __cnfn convert_char2_sat_rtp(uint2);
char2 __ovld __cnfn convert_char2_rtn(uint2);
char2 __ovld __cnfn convert_char2_sat_rtn(uint2);
char2 __ovld __cnfn convert_char2(uint2);
char2 __ovld __cnfn convert_char2_sat(uint2);
char2 __ovld __cnfn convert_char2_rte(long2);
char2 __ovld __cnfn convert_char2_sat_rte(long2);
char2 __ovld __cnfn convert_char2_rtz(long2);
char2 __ovld __cnfn convert_char2_sat_rtz(long2);
char2 __ovld __cnfn convert_char2_rtp(long2);
char2 __ovld __cnfn convert_char2_sat_rtp(long2);
char2 __ovld __cnfn convert_char2_rtn(long2);
char2 __ovld __cnfn convert_char2_sat_rtn(long2);
char2 __ovld __cnfn convert_char2(long2);
char2 __ovld __cnfn convert_char2_sat(long2);
char2 __ovld __cnfn convert_char2_rte(ulong2);
char2 __ovld __cnfn convert_char2_sat_rte(ulong2);
char2 __ovld __cnfn convert_char2_rtz(ulong2);
char2 __ovld __cnfn convert_char2_sat_rtz(ulong2);
char2 __ovld __cnfn convert_char2_rtp(ulong2);
char2 __ovld __cnfn convert_char2_sat_rtp(ulong2);
char2 __ovld __cnfn convert_char2_rtn(ulong2);
char2 __ovld __cnfn convert_char2_sat_rtn(ulong2);
char2 __ovld __cnfn convert_char2(ulong2);
char2 __ovld __cnfn convert_char2_sat(ulong2);
char2 __ovld __cnfn convert_char2_rte(float2);
char2 __ovld __cnfn convert_char2_sat_rte(float2);
char2 __ovld __cnfn convert_char2_rtz(float2);
char2 __ovld __cnfn convert_char2_sat_rtz(float2);
char2 __ovld __cnfn convert_char2_rtp(float2);
char2 __ovld __cnfn convert_char2_sat_rtp(float2);
char2 __ovld __cnfn convert_char2_rtn(float2);
char2 __ovld __cnfn convert_char2_sat_rtn(float2);
char2 __ovld __cnfn convert_char2(float2);
char2 __ovld __cnfn convert_char2_sat(float2);
uchar2 __ovld __cnfn convert_uchar2_rte(char2);
uchar2 __ovld __cnfn convert_uchar2_sat_rte(char2);
uchar2 __ovld __cnfn convert_uchar2_rtz(char2);
uchar2 __ovld __cnfn convert_uchar2_sat_rtz(char2);
uchar2 __ovld __cnfn convert_uchar2_rtp(char2);
uchar2 __ovld __cnfn convert_uchar2_sat_rtp(char2);
uchar2 __ovld __cnfn convert_uchar2_rtn(char2);
uchar2 __ovld __cnfn convert_uchar2_sat_rtn(char2);
uchar2 __ovld __cnfn convert_uchar2(char2);
uchar2 __ovld __cnfn convert_uchar2_sat(char2);
uchar2 __ovld __cnfn convert_uchar2_rte(uchar2);
uchar2 __ovld __cnfn convert_uchar2_sat_rte(uchar2);
uchar2 __ovld __cnfn convert_uchar2_rtz(uchar2);
uchar2 __ovld __cnfn convert_uchar2_sat_rtz(uchar2);
uchar2 __ovld __cnfn convert_uchar2_rtp(uchar2);
uchar2 __ovld __cnfn convert_uchar2_sat_rtp(uchar2);
uchar2 __ovld __cnfn convert_uchar2_rtn(uchar2);
uchar2 __ovld __cnfn convert_uchar2_sat_rtn(uchar2);
uchar2 __ovld __cnfn convert_uchar2(uchar2);
uchar2 __ovld __cnfn convert_uchar2_sat(uchar2);
uchar2 __ovld __cnfn convert_uchar2_rte(short2);
uchar2 __ovld __cnfn convert_uchar2_sat_rte(short2);
uchar2 __ovld __cnfn convert_uchar2_rtz(short2);
uchar2 __ovld __cnfn convert_uchar2_sat_rtz(short2);
uchar2 __ovld __cnfn convert_uchar2_rtp(short2);
uchar2 __ovld __cnfn convert_uchar2_sat_rtp(short2);
uchar2 __ovld __cnfn convert_uchar2_rtn(short2);
uchar2 __ovld __cnfn convert_uchar2_sat_rtn(short2);
uchar2 __ovld __cnfn convert_uchar2(short2);
uchar2 __ovld __cnfn convert_uchar2_sat(short2);
uchar2 __ovld __cnfn convert_uchar2_rte(ushort2);
uchar2 __ovld __cnfn convert_uchar2_sat_rte(ushort2);
uchar2 __ovld __cnfn convert_uchar2_rtz(ushort2);
uchar2 __ovld __cnfn convert_uchar2_sat_rtz(ushort2);
uchar2 __ovld __cnfn convert_uchar2_rtp(ushort2);
uchar2 __ovld __cnfn convert_uchar2_sat_rtp(ushort2);
uchar2 __ovld __cnfn convert_uchar2_rtn(ushort2);
uchar2 __ovld __cnfn convert_uchar2_sat_rtn(ushort2);
uchar2 __ovld __cnfn convert_uchar2(ushort2);
uchar2 __ovld __cnfn convert_uchar2_sat(ushort2);
uchar2 __ovld __cnfn convert_uchar2_rte(int2);
uchar2 __ovld __cnfn convert_uchar2_sat_rte(int2);
uchar2 __ovld __cnfn convert_uchar2_rtz(int2);
uchar2 __ovld __cnfn convert_uchar2_sat_rtz(int2);
uchar2 __ovld __cnfn convert_uchar2_rtp(int2);
uchar2 __ovld __cnfn convert_uchar2_sat_rtp(int2);
uchar2 __ovld __cnfn convert_uchar2_rtn(int2);
uchar2 __ovld __cnfn convert_uchar2_sat_rtn(int2);
uchar2 __ovld __cnfn convert_uchar2(int2);
uchar2 __ovld __cnfn convert_uchar2_sat(int2);
uchar2 __ovld __cnfn convert_uchar2_rte(uint2);
uchar2 __ovld __cnfn convert_uchar2_sat_rte(uint2);
uchar2 __ovld __cnfn convert_uchar2_rtz(uint2);
uchar2 __ovld __cnfn convert_uchar2_sat_rtz(uint2);
uchar2 __ovld __cnfn convert_uchar2_rtp(uint2);
uchar2 __ovld __cnfn convert_uchar2_sat_rtp(uint2);
uchar2 __ovld __cnfn convert_uchar2_rtn(uint2);
uchar2 __ovld __cnfn convert_uchar2_sat_rtn(uint2);
uchar2 __ovld __cnfn convert_uchar2(uint2);
uchar2 __ovld __cnfn convert_uchar2_sat(uint2);
uchar2 __ovld __cnfn convert_uchar2_rte(long2);
uchar2 __ovld __cnfn convert_uchar2_sat_rte(long2);
uchar2 __ovld __cnfn convert_uchar2_rtz(long2);
uchar2 __ovld __cnfn convert_uchar2_sat_rtz(long2);
uchar2 __ovld __cnfn convert_uchar2_rtp(long2);
uchar2 __ovld __cnfn convert_uchar2_sat_rtp(long2);
uchar2 __ovld __cnfn convert_uchar2_rtn(long2);
uchar2 __ovld __cnfn convert_uchar2_sat_rtn(long2);
uchar2 __ovld __cnfn convert_uchar2(long2);
uchar2 __ovld __cnfn convert_uchar2_sat(long2);
uchar2 __ovld __cnfn convert_uchar2_rte(ulong2);
uchar2 __ovld __cnfn convert_uchar2_sat_rte(ulong2);
uchar2 __ovld __cnfn convert_uchar2_rtz(ulong2);
uchar2 __ovld __cnfn convert_uchar2_sat_rtz(ulong2);
uchar2 __ovld __cnfn convert_uchar2_rtp(ulong2);
uchar2 __ovld __cnfn convert_uchar2_sat_rtp(ulong2);
uchar2 __ovld __cnfn convert_uchar2_rtn(ulong2);
uchar2 __ovld __cnfn convert_uchar2_sat_rtn(ulong2);
uchar2 __ovld __cnfn convert_uchar2(ulong2);
uchar2 __ovld __cnfn convert_uchar2_sat(ulong2);
uchar2 __ovld __cnfn convert_uchar2_rte(float2);
uchar2 __ovld __cnfn convert_uchar2_sat_rte(float2);
uchar2 __ovld __cnfn convert_uchar2_rtz(float2);
uchar2 __ovld __cnfn convert_uchar2_sat_rtz(float2);
uchar2 __ovld __cnfn convert_uchar2_rtp(float2);
uchar2 __ovld __cnfn convert_uchar2_sat_rtp(float2);
uchar2 __ovld __cnfn convert_uchar2_rtn(float2);
uchar2 __ovld __cnfn convert_uchar2_sat_rtn(float2);
uchar2 __ovld __cnfn convert_uchar2(float2);
uchar2 __ovld __cnfn convert_uchar2_sat(float2);
short2 __ovld __cnfn convert_short2_rte(char2);
short2 __ovld __cnfn convert_short2_sat_rte(char2);
short2 __ovld __cnfn convert_short2_rtz(char2);
short2 __ovld __cnfn convert_short2_sat_rtz(char2);
short2 __ovld __cnfn convert_short2_rtp(char2);
short2 __ovld __cnfn convert_short2_sat_rtp(char2);
short2 __ovld __cnfn convert_short2_rtn(char2);
short2 __ovld __cnfn convert_short2_sat_rtn(char2);
short2 __ovld __cnfn convert_short2(char2);
short2 __ovld __cnfn convert_short2_sat(char2);
short2 __ovld __cnfn convert_short2_rte(uchar2);
short2 __ovld __cnfn convert_short2_sat_rte(uchar2);
short2 __ovld __cnfn convert_short2_rtz(uchar2);
short2 __ovld __cnfn convert_short2_sat_rtz(uchar2);
short2 __ovld __cnfn convert_short2_rtp(uchar2);
short2 __ovld __cnfn convert_short2_sat_rtp(uchar2);
short2 __ovld __cnfn convert_short2_rtn(uchar2);
short2 __ovld __cnfn convert_short2_sat_rtn(uchar2);
short2 __ovld __cnfn convert_short2(uchar2);
short2 __ovld __cnfn convert_short2_sat(uchar2);
short2 __ovld __cnfn convert_short2_rte(short2);
short2 __ovld __cnfn convert_short2_sat_rte(short2);
short2 __ovld __cnfn convert_short2_rtz(short2);
short2 __ovld __cnfn convert_short2_sat_rtz(short2);
short2 __ovld __cnfn convert_short2_rtp(short2);
short2 __ovld __cnfn convert_short2_sat_rtp(short2);
short2 __ovld __cnfn convert_short2_rtn(short2);
short2 __ovld __cnfn convert_short2_sat_rtn(short2);
short2 __ovld __cnfn convert_short2(short2);
short2 __ovld __cnfn convert_short2_sat(short2);
short2 __ovld __cnfn convert_short2_rte(ushort2);
short2 __ovld __cnfn convert_short2_sat_rte(ushort2);
short2 __ovld __cnfn convert_short2_rtz(ushort2);
short2 __ovld __cnfn convert_short2_sat_rtz(ushort2);
short2 __ovld __cnfn convert_short2_rtp(ushort2);
short2 __ovld __cnfn convert_short2_sat_rtp(ushort2);
short2 __ovld __cnfn convert_short2_rtn(ushort2);
short2 __ovld __cnfn convert_short2_sat_rtn(ushort2);
short2 __ovld __cnfn convert_short2(ushort2);
short2 __ovld __cnfn convert_short2_sat(ushort2);
short2 __ovld __cnfn convert_short2_rte(int2);
short2 __ovld __cnfn convert_short2_sat_rte(int2);
short2 __ovld __cnfn convert_short2_rtz(int2);
short2 __ovld __cnfn convert_short2_sat_rtz(int2);
short2 __ovld __cnfn convert_short2_rtp(int2);
short2 __ovld __cnfn convert_short2_sat_rtp(int2);
short2 __ovld __cnfn convert_short2_rtn(int2);
short2 __ovld __cnfn convert_short2_sat_rtn(int2);
short2 __ovld __cnfn convert_short2(int2);
short2 __ovld __cnfn convert_short2_sat(int2);
short2 __ovld __cnfn convert_short2_rte(uint2);
short2 __ovld __cnfn convert_short2_sat_rte(uint2);
short2 __ovld __cnfn convert_short2_rtz(uint2);
short2 __ovld __cnfn convert_short2_sat_rtz(uint2);
short2 __ovld __cnfn convert_short2_rtp(uint2);
short2 __ovld __cnfn convert_short2_sat_rtp(uint2);
short2 __ovld __cnfn convert_short2_rtn(uint2);
short2 __ovld __cnfn convert_short2_sat_rtn(uint2);
short2 __ovld __cnfn convert_short2(uint2);
short2 __ovld __cnfn convert_short2_sat(uint2);
short2 __ovld __cnfn convert_short2_rte(long2);
short2 __ovld __cnfn convert_short2_sat_rte(long2);
short2 __ovld __cnfn convert_short2_rtz(long2);
short2 __ovld __cnfn convert_short2_sat_rtz(long2);
short2 __ovld __cnfn convert_short2_rtp(long2);
short2 __ovld __cnfn convert_short2_sat_rtp(long2);
short2 __ovld __cnfn convert_short2_rtn(long2);
short2 __ovld __cnfn convert_short2_sat_rtn(long2);
short2 __ovld __cnfn convert_short2(long2);
short2 __ovld __cnfn convert_short2_sat(long2);
short2 __ovld __cnfn convert_short2_rte(ulong2);
short2 __ovld __cnfn convert_short2_sat_rte(ulong2);
short2 __ovld __cnfn convert_short2_rtz(ulong2);
short2 __ovld __cnfn convert_short2_sat_rtz(ulong2);
short2 __ovld __cnfn convert_short2_rtp(ulong2);
short2 __ovld __cnfn convert_short2_sat_rtp(ulong2);
short2 __ovld __cnfn convert_short2_rtn(ulong2);
short2 __ovld __cnfn convert_short2_sat_rtn(ulong2);
short2 __ovld __cnfn convert_short2(ulong2);
short2 __ovld __cnfn convert_short2_sat(ulong2);
short2 __ovld __cnfn convert_short2_rte(float2);
short2 __ovld __cnfn convert_short2_sat_rte(float2);
short2 __ovld __cnfn convert_short2_rtz(float2);
short2 __ovld __cnfn convert_short2_sat_rtz(float2);
short2 __ovld __cnfn convert_short2_rtp(float2);
short2 __ovld __cnfn convert_short2_sat_rtp(float2);
short2 __ovld __cnfn convert_short2_rtn(float2);
short2 __ovld __cnfn convert_short2_sat_rtn(float2);
short2 __ovld __cnfn convert_short2(float2);
short2 __ovld __cnfn convert_short2_sat(float2);
ushort2 __ovld __cnfn convert_ushort2_rte(char2);
ushort2 __ovld __cnfn convert_ushort2_sat_rte(char2);
ushort2 __ovld __cnfn convert_ushort2_rtz(char2);
ushort2 __ovld __cnfn convert_ushort2_sat_rtz(char2);
ushort2 __ovld __cnfn convert_ushort2_rtp(char2);
ushort2 __ovld __cnfn convert_ushort2_sat_rtp(char2);
ushort2 __ovld __cnfn convert_ushort2_rtn(char2);
ushort2 __ovld __cnfn convert_ushort2_sat_rtn(char2);
ushort2 __ovld __cnfn convert_ushort2(char2);
ushort2 __ovld __cnfn convert_ushort2_sat(char2);
ushort2 __ovld __cnfn convert_ushort2_rte(uchar2);
ushort2 __ovld __cnfn convert_ushort2_sat_rte(uchar2);
ushort2 __ovld __cnfn convert_ushort2_rtz(uchar2);
ushort2 __ovld __cnfn convert_ushort2_sat_rtz(uchar2);
ushort2 __ovld __cnfn convert_ushort2_rtp(uchar2);
ushort2 __ovld __cnfn convert_ushort2_sat_rtp(uchar2);
ushort2 __ovld __cnfn convert_ushort2_rtn(uchar2);
ushort2 __ovld __cnfn convert_ushort2_sat_rtn(uchar2);
ushort2 __ovld __cnfn convert_ushort2(uchar2);
ushort2 __ovld __cnfn convert_ushort2_sat(uchar2);
ushort2 __ovld __cnfn convert_ushort2_rte(short2);
ushort2 __ovld __cnfn convert_ushort2_sat_rte(short2);
ushort2 __ovld __cnfn convert_ushort2_rtz(short2);
ushort2 __ovld __cnfn convert_ushort2_sat_rtz(short2);
ushort2 __ovld __cnfn convert_ushort2_rtp(short2);
ushort2 __ovld __cnfn convert_ushort2_sat_rtp(short2);
ushort2 __ovld __cnfn convert_ushort2_rtn(short2);
ushort2 __ovld __cnfn convert_ushort2_sat_rtn(short2);
ushort2 __ovld __cnfn convert_ushort2(short2);
ushort2 __ovld __cnfn convert_ushort2_sat(short2);
ushort2 __ovld __cnfn convert_ushort2_rte(ushort2);
ushort2 __ovld __cnfn convert_ushort2_sat_rte(ushort2);
ushort2 __ovld __cnfn convert_ushort2_rtz(ushort2);
ushort2 __ovld __cnfn convert_ushort2_sat_rtz(ushort2);
ushort2 __ovld __cnfn convert_ushort2_rtp(ushort2);
ushort2 __ovld __cnfn convert_ushort2_sat_rtp(ushort2);
ushort2 __ovld __cnfn convert_ushort2_rtn(ushort2);
ushort2 __ovld __cnfn convert_ushort2_sat_rtn(ushort2);
ushort2 __ovld __cnfn convert_ushort2(ushort2);
ushort2 __ovld __cnfn convert_ushort2_sat(ushort2);
ushort2 __ovld __cnfn convert_ushort2_rte(int2);
ushort2 __ovld __cnfn convert_ushort2_sat_rte(int2);
ushort2 __ovld __cnfn convert_ushort2_rtz(int2);
ushort2 __ovld __cnfn convert_ushort2_sat_rtz(int2);
ushort2 __ovld __cnfn convert_ushort2_rtp(int2);
ushort2 __ovld __cnfn convert_ushort2_sat_rtp(int2);
ushort2 __ovld __cnfn convert_ushort2_rtn(int2);
ushort2 __ovld __cnfn convert_ushort2_sat_rtn(int2);
ushort2 __ovld __cnfn convert_ushort2(int2);
ushort2 __ovld __cnfn convert_ushort2_sat(int2);
ushort2 __ovld __cnfn convert_ushort2_rte(uint2);
ushort2 __ovld __cnfn convert_ushort2_sat_rte(uint2);
ushort2 __ovld __cnfn convert_ushort2_rtz(uint2);
ushort2 __ovld __cnfn convert_ushort2_sat_rtz(uint2);
ushort2 __ovld __cnfn convert_ushort2_rtp(uint2);
ushort2 __ovld __cnfn convert_ushort2_sat_rtp(uint2);
ushort2 __ovld __cnfn convert_ushort2_rtn(uint2);
ushort2 __ovld __cnfn convert_ushort2_sat_rtn(uint2);
ushort2 __ovld __cnfn convert_ushort2(uint2);
ushort2 __ovld __cnfn convert_ushort2_sat(uint2);
ushort2 __ovld __cnfn convert_ushort2_rte(long2);
ushort2 __ovld __cnfn convert_ushort2_sat_rte(long2);
ushort2 __ovld __cnfn convert_ushort2_rtz(long2);
ushort2 __ovld __cnfn convert_ushort2_sat_rtz(long2);
ushort2 __ovld __cnfn convert_ushort2_rtp(long2);
ushort2 __ovld __cnfn convert_ushort2_sat_rtp(long2);
ushort2 __ovld __cnfn convert_ushort2_rtn(long2);
ushort2 __ovld __cnfn convert_ushort2_sat_rtn(long2);
ushort2 __ovld __cnfn convert_ushort2(long2);
ushort2 __ovld __cnfn convert_ushort2_sat(long2);
ushort2 __ovld __cnfn convert_ushort2_rte(ulong2);
ushort2 __ovld __cnfn convert_ushort2_sat_rte(ulong2);
ushort2 __ovld __cnfn convert_ushort2_rtz(ulong2);
ushort2 __ovld __cnfn convert_ushort2_sat_rtz(ulong2);
ushort2 __ovld __cnfn convert_ushort2_rtp(ulong2);
ushort2 __ovld __cnfn convert_ushort2_sat_rtp(ulong2);
ushort2 __ovld __cnfn convert_ushort2_rtn(ulong2);
ushort2 __ovld __cnfn convert_ushort2_sat_rtn(ulong2);
ushort2 __ovld __cnfn convert_ushort2(ulong2);
ushort2 __ovld __cnfn convert_ushort2_sat(ulong2);
ushort2 __ovld __cnfn convert_ushort2_rte(float2);
ushort2 __ovld __cnfn convert_ushort2_sat_rte(float2);
ushort2 __ovld __cnfn convert_ushort2_rtz(float2);
ushort2 __ovld __cnfn convert_ushort2_sat_rtz(float2);
ushort2 __ovld __cnfn convert_ushort2_rtp(float2);
ushort2 __ovld __cnfn convert_ushort2_sat_rtp(float2);
ushort2 __ovld __cnfn convert_ushort2_rtn(float2);
ushort2 __ovld __cnfn convert_ushort2_sat_rtn(float2);
ushort2 __ovld __cnfn convert_ushort2(float2);
ushort2 __ovld __cnfn convert_ushort2_sat(float2);
int2 __ovld __cnfn convert_int2_rte(char2);
int2 __ovld __cnfn convert_int2_sat_rte(char2);
int2 __ovld __cnfn convert_int2_rtz(char2);
int2 __ovld __cnfn convert_int2_sat_rtz(char2);
int2 __ovld __cnfn convert_int2_rtp(char2);
int2 __ovld __cnfn convert_int2_sat_rtp(char2);
int2 __ovld __cnfn convert_int2_rtn(char2);
int2 __ovld __cnfn convert_int2_sat_rtn(char2);
int2 __ovld __cnfn convert_int2(char2);
int2 __ovld __cnfn convert_int2_sat(char2);
int2 __ovld __cnfn convert_int2_rte(uchar2);
int2 __ovld __cnfn convert_int2_sat_rte(uchar2);
int2 __ovld __cnfn convert_int2_rtz(uchar2);
int2 __ovld __cnfn convert_int2_sat_rtz(uchar2);
int2 __ovld __cnfn convert_int2_rtp(uchar2);
int2 __ovld __cnfn convert_int2_sat_rtp(uchar2);
int2 __ovld __cnfn convert_int2_rtn(uchar2);
int2 __ovld __cnfn convert_int2_sat_rtn(uchar2);
int2 __ovld __cnfn convert_int2(uchar2);
int2 __ovld __cnfn convert_int2_sat(uchar2);
int2 __ovld __cnfn convert_int2_rte(short2);
int2 __ovld __cnfn convert_int2_sat_rte(short2);
int2 __ovld __cnfn convert_int2_rtz(short2);
int2 __ovld __cnfn convert_int2_sat_rtz(short2);
int2 __ovld __cnfn convert_int2_rtp(short2);
int2 __ovld __cnfn convert_int2_sat_rtp(short2);
int2 __ovld __cnfn convert_int2_rtn(short2);
int2 __ovld __cnfn convert_int2_sat_rtn(short2);
int2 __ovld __cnfn convert_int2(short2);
int2 __ovld __cnfn convert_int2_sat(short2);
int2 __ovld __cnfn convert_int2_rte(ushort2);
int2 __ovld __cnfn convert_int2_sat_rte(ushort2);
int2 __ovld __cnfn convert_int2_rtz(ushort2);
int2 __ovld __cnfn convert_int2_sat_rtz(ushort2);
int2 __ovld __cnfn convert_int2_rtp(ushort2);
int2 __ovld __cnfn convert_int2_sat_rtp(ushort2);
int2 __ovld __cnfn convert_int2_rtn(ushort2);
int2 __ovld __cnfn convert_int2_sat_rtn(ushort2);
int2 __ovld __cnfn convert_int2(ushort2);
int2 __ovld __cnfn convert_int2_sat(ushort2);
int2 __ovld __cnfn convert_int2_rte(int2);
int2 __ovld __cnfn convert_int2_sat_rte(int2);
int2 __ovld __cnfn convert_int2_rtz(int2);
int2 __ovld __cnfn convert_int2_sat_rtz(int2);
int2 __ovld __cnfn convert_int2_rtp(int2);
int2 __ovld __cnfn convert_int2_sat_rtp(int2);
int2 __ovld __cnfn convert_int2_rtn(int2);
int2 __ovld __cnfn convert_int2_sat_rtn(int2);
int2 __ovld __cnfn convert_int2(int2);
int2 __ovld __cnfn convert_int2_sat(int2);
int2 __ovld __cnfn convert_int2_rte(uint2);
int2 __ovld __cnfn convert_int2_sat_rte(uint2);
int2 __ovld __cnfn convert_int2_rtz(uint2);
int2 __ovld __cnfn convert_int2_sat_rtz(uint2);
int2 __ovld __cnfn convert_int2_rtp(uint2);
int2 __ovld __cnfn convert_int2_sat_rtp(uint2);
int2 __ovld __cnfn convert_int2_rtn(uint2);
int2 __ovld __cnfn convert_int2_sat_rtn(uint2);
int2 __ovld __cnfn convert_int2(uint2);
int2 __ovld __cnfn convert_int2_sat(uint2);
int2 __ovld __cnfn convert_int2_rte(long2);
int2 __ovld __cnfn convert_int2_sat_rte(long2);
int2 __ovld __cnfn convert_int2_rtz(long2);
int2 __ovld __cnfn convert_int2_sat_rtz(long2);
int2 __ovld __cnfn convert_int2_rtp(long2);
int2 __ovld __cnfn convert_int2_sat_rtp(long2);
int2 __ovld __cnfn convert_int2_rtn(long2);
int2 __ovld __cnfn convert_int2_sat_rtn(long2);
int2 __ovld __cnfn convert_int2(long2);
int2 __ovld __cnfn convert_int2_sat(long2);
int2 __ovld __cnfn convert_int2_rte(ulong2);
int2 __ovld __cnfn convert_int2_sat_rte(ulong2);
int2 __ovld __cnfn convert_int2_rtz(ulong2);
int2 __ovld __cnfn convert_int2_sat_rtz(ulong2);
int2 __ovld __cnfn convert_int2_rtp(ulong2);
int2 __ovld __cnfn convert_int2_sat_rtp(ulong2);
int2 __ovld __cnfn convert_int2_rtn(ulong2);
int2 __ovld __cnfn convert_int2_sat_rtn(ulong2);
int2 __ovld __cnfn convert_int2(ulong2);
int2 __ovld __cnfn convert_int2_sat(ulong2);
int2 __ovld __cnfn convert_int2_rte(float2);
int2 __ovld __cnfn convert_int2_sat_rte(float2);
int2 __ovld __cnfn convert_int2_rtz(float2);
int2 __ovld __cnfn convert_int2_sat_rtz(float2);
int2 __ovld __cnfn convert_int2_rtp(float2);
int2 __ovld __cnfn convert_int2_sat_rtp(float2);
int2 __ovld __cnfn convert_int2_rtn(float2);
int2 __ovld __cnfn convert_int2_sat_rtn(float2);
int2 __ovld __cnfn convert_int2(float2);
int2 __ovld __cnfn convert_int2_sat(float2);
uint2 __ovld __cnfn convert_uint2_rte(char2);
uint2 __ovld __cnfn convert_uint2_sat_rte(char2);
uint2 __ovld __cnfn convert_uint2_rtz(char2);
uint2 __ovld __cnfn convert_uint2_sat_rtz(char2);
uint2 __ovld __cnfn convert_uint2_rtp(char2);
uint2 __ovld __cnfn convert_uint2_sat_rtp(char2);
uint2 __ovld __cnfn convert_uint2_rtn(char2);
uint2 __ovld __cnfn convert_uint2_sat_rtn(char2);
uint2 __ovld __cnfn convert_uint2(char2);
uint2 __ovld __cnfn convert_uint2_sat(char2);
uint2 __ovld __cnfn convert_uint2_rte(uchar2);
uint2 __ovld __cnfn convert_uint2_sat_rte(uchar2);
uint2 __ovld __cnfn convert_uint2_rtz(uchar2);
uint2 __ovld __cnfn convert_uint2_sat_rtz(uchar2);
uint2 __ovld __cnfn convert_uint2_rtp(uchar2);
uint2 __ovld __cnfn convert_uint2_sat_rtp(uchar2);
uint2 __ovld __cnfn convert_uint2_rtn(uchar2);
uint2 __ovld __cnfn convert_uint2_sat_rtn(uchar2);
uint2 __ovld __cnfn convert_uint2(uchar2);
uint2 __ovld __cnfn convert_uint2_sat(uchar2);
uint2 __ovld __cnfn convert_uint2_rte(short2);
uint2 __ovld __cnfn convert_uint2_sat_rte(short2);
uint2 __ovld __cnfn convert_uint2_rtz(short2);
uint2 __ovld __cnfn convert_uint2_sat_rtz(short2);
uint2 __ovld __cnfn convert_uint2_rtp(short2);
uint2 __ovld __cnfn convert_uint2_sat_rtp(short2);
uint2 __ovld __cnfn convert_uint2_rtn(short2);
uint2 __ovld __cnfn convert_uint2_sat_rtn(short2);
uint2 __ovld __cnfn convert_uint2(short2);
uint2 __ovld __cnfn convert_uint2_sat(short2);
uint2 __ovld __cnfn convert_uint2_rte(ushort2);
uint2 __ovld __cnfn convert_uint2_sat_rte(ushort2);
uint2 __ovld __cnfn convert_uint2_rtz(ushort2);
uint2 __ovld __cnfn convert_uint2_sat_rtz(ushort2);
uint2 __ovld __cnfn convert_uint2_rtp(ushort2);
uint2 __ovld __cnfn convert_uint2_sat_rtp(ushort2);
uint2 __ovld __cnfn convert_uint2_rtn(ushort2);
uint2 __ovld __cnfn convert_uint2_sat_rtn(ushort2);
uint2 __ovld __cnfn convert_uint2(ushort2);
uint2 __ovld __cnfn convert_uint2_sat(ushort2);
uint2 __ovld __cnfn convert_uint2_rte(int2);
uint2 __ovld __cnfn convert_uint2_sat_rte(int2);
uint2 __ovld __cnfn convert_uint2_rtz(int2);
uint2 __ovld __cnfn convert_uint2_sat_rtz(int2);
uint2 __ovld __cnfn convert_uint2_rtp(int2);
uint2 __ovld __cnfn convert_uint2_sat_rtp(int2);
uint2 __ovld __cnfn convert_uint2_rtn(int2);
uint2 __ovld __cnfn convert_uint2_sat_rtn(int2);
uint2 __ovld __cnfn convert_uint2(int2);
uint2 __ovld __cnfn convert_uint2_sat(int2);
uint2 __ovld __cnfn convert_uint2_rte(uint2);
uint2 __ovld __cnfn convert_uint2_sat_rte(uint2);
uint2 __ovld __cnfn convert_uint2_rtz(uint2);
uint2 __ovld __cnfn convert_uint2_sat_rtz(uint2);
uint2 __ovld __cnfn convert_uint2_rtp(uint2);
uint2 __ovld __cnfn convert_uint2_sat_rtp(uint2);
uint2 __ovld __cnfn convert_uint2_rtn(uint2);
uint2 __ovld __cnfn convert_uint2_sat_rtn(uint2);
uint2 __ovld __cnfn convert_uint2(uint2);
uint2 __ovld __cnfn convert_uint2_sat(uint2);
uint2 __ovld __cnfn convert_uint2_rte(long2);
uint2 __ovld __cnfn convert_uint2_sat_rte(long2);
uint2 __ovld __cnfn convert_uint2_rtz(long2);
uint2 __ovld __cnfn convert_uint2_sat_rtz(long2);
uint2 __ovld __cnfn convert_uint2_rtp(long2);
uint2 __ovld __cnfn convert_uint2_sat_rtp(long2);
uint2 __ovld __cnfn convert_uint2_rtn(long2);
uint2 __ovld __cnfn convert_uint2_sat_rtn(long2);
uint2 __ovld __cnfn convert_uint2(long2);
uint2 __ovld __cnfn convert_uint2_sat(long2);
uint2 __ovld __cnfn convert_uint2_rte(ulong2);
uint2 __ovld __cnfn convert_uint2_sat_rte(ulong2);
uint2 __ovld __cnfn convert_uint2_rtz(ulong2);
uint2 __ovld __cnfn convert_uint2_sat_rtz(ulong2);
uint2 __ovld __cnfn convert_uint2_rtp(ulong2);
uint2 __ovld __cnfn convert_uint2_sat_rtp(ulong2);
uint2 __ovld __cnfn convert_uint2_rtn(ulong2);
uint2 __ovld __cnfn convert_uint2_sat_rtn(ulong2);
uint2 __ovld __cnfn convert_uint2(ulong2);
uint2 __ovld __cnfn convert_uint2_sat(ulong2);
uint2 __ovld __cnfn convert_uint2_rte(float2);
uint2 __ovld __cnfn convert_uint2_sat_rte(float2);
uint2 __ovld __cnfn convert_uint2_rtz(float2);
uint2 __ovld __cnfn convert_uint2_sat_rtz(float2);
uint2 __ovld __cnfn convert_uint2_rtp(float2);
uint2 __ovld __cnfn convert_uint2_sat_rtp(float2);
uint2 __ovld __cnfn convert_uint2_rtn(float2);
uint2 __ovld __cnfn convert_uint2_sat_rtn(float2);
uint2 __ovld __cnfn convert_uint2(float2);
uint2 __ovld __cnfn convert_uint2_sat(float2);
long2 __ovld __cnfn convert_long2_rte(char2);
long2 __ovld __cnfn convert_long2_sat_rte(char2);
long2 __ovld __cnfn convert_long2_rtz(char2);
long2 __ovld __cnfn convert_long2_sat_rtz(char2);
long2 __ovld __cnfn convert_long2_rtp(char2);
long2 __ovld __cnfn convert_long2_sat_rtp(char2);
long2 __ovld __cnfn convert_long2_rtn(char2);
long2 __ovld __cnfn convert_long2_sat_rtn(char2);
long2 __ovld __cnfn convert_long2(char2);
long2 __ovld __cnfn convert_long2_sat(char2);
long2 __ovld __cnfn convert_long2_rte(uchar2);
long2 __ovld __cnfn convert_long2_sat_rte(uchar2);
long2 __ovld __cnfn convert_long2_rtz(uchar2);
long2 __ovld __cnfn convert_long2_sat_rtz(uchar2);
long2 __ovld __cnfn convert_long2_rtp(uchar2);
long2 __ovld __cnfn convert_long2_sat_rtp(uchar2);
long2 __ovld __cnfn convert_long2_rtn(uchar2);
long2 __ovld __cnfn convert_long2_sat_rtn(uchar2);
long2 __ovld __cnfn convert_long2(uchar2);
long2 __ovld __cnfn convert_long2_sat(uchar2);
long2 __ovld __cnfn convert_long2_rte(short2);
long2 __ovld __cnfn convert_long2_sat_rte(short2);
long2 __ovld __cnfn convert_long2_rtz(short2);
long2 __ovld __cnfn convert_long2_sat_rtz(short2);
long2 __ovld __cnfn convert_long2_rtp(short2);
long2 __ovld __cnfn convert_long2_sat_rtp(short2);
long2 __ovld __cnfn convert_long2_rtn(short2);
long2 __ovld __cnfn convert_long2_sat_rtn(short2);
long2 __ovld __cnfn convert_long2(short2);
long2 __ovld __cnfn convert_long2_sat(short2);
long2 __ovld __cnfn convert_long2_rte(ushort2);
long2 __ovld __cnfn convert_long2_sat_rte(ushort2);
long2 __ovld __cnfn convert_long2_rtz(ushort2);
long2 __ovld __cnfn convert_long2_sat_rtz(ushort2);
long2 __ovld __cnfn convert_long2_rtp(ushort2);
long2 __ovld __cnfn convert_long2_sat_rtp(ushort2);
long2 __ovld __cnfn convert_long2_rtn(ushort2);
long2 __ovld __cnfn convert_long2_sat_rtn(ushort2);
long2 __ovld __cnfn convert_long2(ushort2);
long2 __ovld __cnfn convert_long2_sat(ushort2);
long2 __ovld __cnfn convert_long2_rte(int2);
long2 __ovld __cnfn convert_long2_sat_rte(int2);
long2 __ovld __cnfn convert_long2_rtz(int2);
long2 __ovld __cnfn convert_long2_sat_rtz(int2);
long2 __ovld __cnfn convert_long2_rtp(int2);
long2 __ovld __cnfn convert_long2_sat_rtp(int2);
long2 __ovld __cnfn convert_long2_rtn(int2);
long2 __ovld __cnfn convert_long2_sat_rtn(int2);
long2 __ovld __cnfn convert_long2(int2);
long2 __ovld __cnfn convert_long2_sat(int2);
long2 __ovld __cnfn convert_long2_rte(uint2);
long2 __ovld __cnfn convert_long2_sat_rte(uint2);
long2 __ovld __cnfn convert_long2_rtz(uint2);
long2 __ovld __cnfn convert_long2_sat_rtz(uint2);
long2 __ovld __cnfn convert_long2_rtp(uint2);
long2 __ovld __cnfn convert_long2_sat_rtp(uint2);
long2 __ovld __cnfn convert_long2_rtn(uint2);
long2 __ovld __cnfn convert_long2_sat_rtn(uint2);
long2 __ovld __cnfn convert_long2(uint2);
long2 __ovld __cnfn convert_long2_sat(uint2);
long2 __ovld __cnfn convert_long2_rte(long2);
long2 __ovld __cnfn convert_long2_sat_rte(long2);
long2 __ovld __cnfn convert_long2_rtz(long2);
long2 __ovld __cnfn convert_long2_sat_rtz(long2);
long2 __ovld __cnfn convert_long2_rtp(long2);
long2 __ovld __cnfn convert_long2_sat_rtp(long2);
long2 __ovld __cnfn convert_long2_rtn(long2);
long2 __ovld __cnfn convert_long2_sat_rtn(long2);
long2 __ovld __cnfn convert_long2(long2);
long2 __ovld __cnfn convert_long2_sat(long2);
long2 __ovld __cnfn convert_long2_rte(ulong2);
long2 __ovld __cnfn convert_long2_sat_rte(ulong2);
long2 __ovld __cnfn convert_long2_rtz(ulong2);
long2 __ovld __cnfn convert_long2_sat_rtz(ulong2);
long2 __ovld __cnfn convert_long2_rtp(ulong2);
long2 __ovld __cnfn convert_long2_sat_rtp(ulong2);
long2 __ovld __cnfn convert_long2_rtn(ulong2);
long2 __ovld __cnfn convert_long2_sat_rtn(ulong2);
long2 __ovld __cnfn convert_long2(ulong2);
long2 __ovld __cnfn convert_long2_sat(ulong2);
long2 __ovld __cnfn convert_long2_rte(float2);
long2 __ovld __cnfn convert_long2_sat_rte(float2);
long2 __ovld __cnfn convert_long2_rtz(float2);
long2 __ovld __cnfn convert_long2_sat_rtz(float2);
long2 __ovld __cnfn convert_long2_rtp(float2);
long2 __ovld __cnfn convert_long2_sat_rtp(float2);
long2 __ovld __cnfn convert_long2_rtn(float2);
long2 __ovld __cnfn convert_long2_sat_rtn(float2);
long2 __ovld __cnfn convert_long2(float2);
long2 __ovld __cnfn convert_long2_sat(float2);
ulong2 __ovld __cnfn convert_ulong2_rte(char2);
ulong2 __ovld __cnfn convert_ulong2_sat_rte(char2);
ulong2 __ovld __cnfn convert_ulong2_rtz(char2);
ulong2 __ovld __cnfn convert_ulong2_sat_rtz(char2);
ulong2 __ovld __cnfn convert_ulong2_rtp(char2);
ulong2 __ovld __cnfn convert_ulong2_sat_rtp(char2);
ulong2 __ovld __cnfn convert_ulong2_rtn(char2);
ulong2 __ovld __cnfn convert_ulong2_sat_rtn(char2);
ulong2 __ovld __cnfn convert_ulong2(char2);
ulong2 __ovld __cnfn convert_ulong2_sat(char2);
ulong2 __ovld __cnfn convert_ulong2_rte(uchar2);
ulong2 __ovld __cnfn convert_ulong2_sat_rte(uchar2);
ulong2 __ovld __cnfn convert_ulong2_rtz(uchar2);
ulong2 __ovld __cnfn convert_ulong2_sat_rtz(uchar2);
ulong2 __ovld __cnfn convert_ulong2_rtp(uchar2);
ulong2 __ovld __cnfn convert_ulong2_sat_rtp(uchar2);
ulong2 __ovld __cnfn convert_ulong2_rtn(uchar2);
ulong2 __ovld __cnfn convert_ulong2_sat_rtn(uchar2);
ulong2 __ovld __cnfn convert_ulong2(uchar2);
ulong2 __ovld __cnfn convert_ulong2_sat(uchar2);
ulong2 __ovld __cnfn convert_ulong2_rte(short2);
ulong2 __ovld __cnfn convert_ulong2_sat_rte(short2);
ulong2 __ovld __cnfn convert_ulong2_rtz(short2);
ulong2 __ovld __cnfn convert_ulong2_sat_rtz(short2);
ulong2 __ovld __cnfn convert_ulong2_rtp(short2);
ulong2 __ovld __cnfn convert_ulong2_sat_rtp(short2);
ulong2 __ovld __cnfn convert_ulong2_rtn(short2);
ulong2 __ovld __cnfn convert_ulong2_sat_rtn(short2);
ulong2 __ovld __cnfn convert_ulong2(short2);
ulong2 __ovld __cnfn convert_ulong2_sat(short2);
ulong2 __ovld __cnfn convert_ulong2_rte(ushort2);
ulong2 __ovld __cnfn convert_ulong2_sat_rte(ushort2);
ulong2 __ovld __cnfn convert_ulong2_rtz(ushort2);
ulong2 __ovld __cnfn convert_ulong2_sat_rtz(ushort2);
ulong2 __ovld __cnfn convert_ulong2_rtp(ushort2);
ulong2 __ovld __cnfn convert_ulong2_sat_rtp(ushort2);
ulong2 __ovld __cnfn convert_ulong2_rtn(ushort2);
ulong2 __ovld __cnfn convert_ulong2_sat_rtn(ushort2);
ulong2 __ovld __cnfn convert_ulong2(ushort2);
ulong2 __ovld __cnfn convert_ulong2_sat(ushort2);
ulong2 __ovld __cnfn convert_ulong2_rte(int2);
ulong2 __ovld __cnfn convert_ulong2_sat_rte(int2);
ulong2 __ovld __cnfn convert_ulong2_rtz(int2);
ulong2 __ovld __cnfn convert_ulong2_sat_rtz(int2);
ulong2 __ovld __cnfn convert_ulong2_rtp(int2);
ulong2 __ovld __cnfn convert_ulong2_sat_rtp(int2);
ulong2 __ovld __cnfn convert_ulong2_rtn(int2);
ulong2 __ovld __cnfn convert_ulong2_sat_rtn(int2);
ulong2 __ovld __cnfn convert_ulong2(int2);
ulong2 __ovld __cnfn convert_ulong2_sat(int2);
ulong2 __ovld __cnfn convert_ulong2_rte(uint2);
ulong2 __ovld __cnfn convert_ulong2_sat_rte(uint2);
ulong2 __ovld __cnfn convert_ulong2_rtz(uint2);
ulong2 __ovld __cnfn convert_ulong2_sat_rtz(uint2);
ulong2 __ovld __cnfn convert_ulong2_rtp(uint2);
ulong2 __ovld __cnfn convert_ulong2_sat_rtp(uint2);
ulong2 __ovld __cnfn convert_ulong2_rtn(uint2);
ulong2 __ovld __cnfn convert_ulong2_sat_rtn(uint2);
ulong2 __ovld __cnfn convert_ulong2(uint2);
ulong2 __ovld __cnfn convert_ulong2_sat(uint2);
ulong2 __ovld __cnfn convert_ulong2_rte(long2);
ulong2 __ovld __cnfn convert_ulong2_sat_rte(long2);
ulong2 __ovld __cnfn convert_ulong2_rtz(long2);
ulong2 __ovld __cnfn convert_ulong2_sat_rtz(long2);
ulong2 __ovld __cnfn convert_ulong2_rtp(long2);
ulong2 __ovld __cnfn convert_ulong2_sat_rtp(long2);
ulong2 __ovld __cnfn convert_ulong2_rtn(long2);
ulong2 __ovld __cnfn convert_ulong2_sat_rtn(long2);
ulong2 __ovld __cnfn convert_ulong2(long2);
ulong2 __ovld __cnfn convert_ulong2_sat(long2);
ulong2 __ovld __cnfn convert_ulong2_rte(ulong2);
ulong2 __ovld __cnfn convert_ulong2_sat_rte(ulong2);
ulong2 __ovld __cnfn convert_ulong2_rtz(ulong2);
ulong2 __ovld __cnfn convert_ulong2_sat_rtz(ulong2);
ulong2 __ovld __cnfn convert_ulong2_rtp(ulong2);
ulong2 __ovld __cnfn convert_ulong2_sat_rtp(ulong2);
ulong2 __ovld __cnfn convert_ulong2_rtn(ulong2);
ulong2 __ovld __cnfn convert_ulong2_sat_rtn(ulong2);
ulong2 __ovld __cnfn convert_ulong2(ulong2);
ulong2 __ovld __cnfn convert_ulong2_sat(ulong2);
ulong2 __ovld __cnfn convert_ulong2_rte(float2);
ulong2 __ovld __cnfn convert_ulong2_sat_rte(float2);
ulong2 __ovld __cnfn convert_ulong2_rtz(float2);
ulong2 __ovld __cnfn convert_ulong2_sat_rtz(float2);
ulong2 __ovld __cnfn convert_ulong2_rtp(float2);
ulong2 __ovld __cnfn convert_ulong2_sat_rtp(float2);
ulong2 __ovld __cnfn convert_ulong2_rtn(float2);
ulong2 __ovld __cnfn convert_ulong2_sat_rtn(float2);
ulong2 __ovld __cnfn convert_ulong2(float2);
ulong2 __ovld __cnfn convert_ulong2_sat(float2);
float2 __ovld __cnfn convert_float2_rte(char2);
float2 __ovld __cnfn convert_float2_rtz(char2);
float2 __ovld __cnfn convert_float2_rtp(char2);
float2 __ovld __cnfn convert_float2_rtn(char2);
float2 __ovld __cnfn convert_float2(char2);
float2 __ovld __cnfn convert_float2_rte(uchar2);
float2 __ovld __cnfn convert_float2_rtz(uchar2);
float2 __ovld __cnfn convert_float2_rtp(uchar2);
float2 __ovld __cnfn convert_float2_rtn(uchar2);
float2 __ovld __cnfn convert_float2(uchar2);
float2 __ovld __cnfn convert_float2_rte(short2);
float2 __ovld __cnfn convert_float2_rtz(short2);
float2 __ovld __cnfn convert_float2_rtp(short2);
float2 __ovld __cnfn convert_float2_rtn(short2);
float2 __ovld __cnfn convert_float2(short2);
float2 __ovld __cnfn convert_float2_rte(ushort2);
float2 __ovld __cnfn convert_float2_rtz(ushort2);
float2 __ovld __cnfn convert_float2_rtp(ushort2);
float2 __ovld __cnfn convert_float2_rtn(ushort2);
float2 __ovld __cnfn convert_float2(ushort2);
float2 __ovld __cnfn convert_float2_rte(int2);
float2 __ovld __cnfn convert_float2_rtz(int2);
float2 __ovld __cnfn convert_float2_rtp(int2);
float2 __ovld __cnfn convert_float2_rtn(int2);
float2 __ovld __cnfn convert_float2(int2);
float2 __ovld __cnfn convert_float2_rte(uint2);
float2 __ovld __cnfn convert_float2_rtz(uint2);
float2 __ovld __cnfn convert_float2_rtp(uint2);
float2 __ovld __cnfn convert_float2_rtn(uint2);
float2 __ovld __cnfn convert_float2(uint2);
float2 __ovld __cnfn convert_float2_rte(long2);
float2 __ovld __cnfn convert_float2_rtz(long2);
float2 __ovld __cnfn convert_float2_rtp(long2);
float2 __ovld __cnfn convert_float2_rtn(long2);
float2 __ovld __cnfn convert_float2(long2);
float2 __ovld __cnfn convert_float2_rte(ulong2);
float2 __ovld __cnfn convert_float2_rtz(ulong2);
float2 __ovld __cnfn convert_float2_rtp(ulong2);
float2 __ovld __cnfn convert_float2_rtn(ulong2);
float2 __ovld __cnfn convert_float2(ulong2);
float2 __ovld __cnfn convert_float2_rte(float2);
float2 __ovld __cnfn convert_float2_rtz(float2);
float2 __ovld __cnfn convert_float2_rtp(float2);
float2 __ovld __cnfn convert_float2_rtn(float2);
float2 __ovld __cnfn convert_float2(float2);
char3 __ovld __cnfn convert_char3_rte(char3);
char3 __ovld __cnfn convert_char3_sat_rte(char3);
char3 __ovld __cnfn convert_char3_rtz(char3);
char3 __ovld __cnfn convert_char3_sat_rtz(char3);
char3 __ovld __cnfn convert_char3_rtp(char3);
char3 __ovld __cnfn convert_char3_sat_rtp(char3);
char3 __ovld __cnfn convert_char3_rtn(char3);
char3 __ovld __cnfn convert_char3_sat_rtn(char3);
char3 __ovld __cnfn convert_char3(char3);
char3 __ovld __cnfn convert_char3_sat(char3);
char3 __ovld __cnfn convert_char3_rte(uchar3);
char3 __ovld __cnfn convert_char3_sat_rte(uchar3);
char3 __ovld __cnfn convert_char3_rtz(uchar3);
char3 __ovld __cnfn convert_char3_sat_rtz(uchar3);
char3 __ovld __cnfn convert_char3_rtp(uchar3);
char3 __ovld __cnfn convert_char3_sat_rtp(uchar3);
char3 __ovld __cnfn convert_char3_rtn(uchar3);
char3 __ovld __cnfn convert_char3_sat_rtn(uchar3);
char3 __ovld __cnfn convert_char3(uchar3);
char3 __ovld __cnfn convert_char3_sat(uchar3);
char3 __ovld __cnfn convert_char3_rte(short3);
char3 __ovld __cnfn convert_char3_sat_rte(short3);
char3 __ovld __cnfn convert_char3_rtz(short3);
char3 __ovld __cnfn convert_char3_sat_rtz(short3);
char3 __ovld __cnfn convert_char3_rtp(short3);
char3 __ovld __cnfn convert_char3_sat_rtp(short3);
char3 __ovld __cnfn convert_char3_rtn(short3);
char3 __ovld __cnfn convert_char3_sat_rtn(short3);
char3 __ovld __cnfn convert_char3(short3);
char3 __ovld __cnfn convert_char3_sat(short3);
char3 __ovld __cnfn convert_char3_rte(ushort3);
char3 __ovld __cnfn convert_char3_sat_rte(ushort3);
char3 __ovld __cnfn convert_char3_rtz(ushort3);
char3 __ovld __cnfn convert_char3_sat_rtz(ushort3);
char3 __ovld __cnfn convert_char3_rtp(ushort3);
char3 __ovld __cnfn convert_char3_sat_rtp(ushort3);
char3 __ovld __cnfn convert_char3_rtn(ushort3);
char3 __ovld __cnfn convert_char3_sat_rtn(ushort3);
char3 __ovld __cnfn convert_char3(ushort3);
char3 __ovld __cnfn convert_char3_sat(ushort3);
char3 __ovld __cnfn convert_char3_rte(int3);
char3 __ovld __cnfn convert_char3_sat_rte(int3);
char3 __ovld __cnfn convert_char3_rtz(int3);
char3 __ovld __cnfn convert_char3_sat_rtz(int3);
char3 __ovld __cnfn convert_char3_rtp(int3);
char3 __ovld __cnfn convert_char3_sat_rtp(int3);
char3 __ovld __cnfn convert_char3_rtn(int3);
char3 __ovld __cnfn convert_char3_sat_rtn(int3);
char3 __ovld __cnfn convert_char3(int3);
char3 __ovld __cnfn convert_char3_sat(int3);
char3 __ovld __cnfn convert_char3_rte(uint3);
char3 __ovld __cnfn convert_char3_sat_rte(uint3);
char3 __ovld __cnfn convert_char3_rtz(uint3);
char3 __ovld __cnfn convert_char3_sat_rtz(uint3);
char3 __ovld __cnfn convert_char3_rtp(uint3);
char3 __ovld __cnfn convert_char3_sat_rtp(uint3);
char3 __ovld __cnfn convert_char3_rtn(uint3);
char3 __ovld __cnfn convert_char3_sat_rtn(uint3);
char3 __ovld __cnfn convert_char3(uint3);
char3 __ovld __cnfn convert_char3_sat(uint3);
char3 __ovld __cnfn convert_char3_rte(long3);
char3 __ovld __cnfn convert_char3_sat_rte(long3);
char3 __ovld __cnfn convert_char3_rtz(long3);
char3 __ovld __cnfn convert_char3_sat_rtz(long3);
char3 __ovld __cnfn convert_char3_rtp(long3);
char3 __ovld __cnfn convert_char3_sat_rtp(long3);
char3 __ovld __cnfn convert_char3_rtn(long3);
char3 __ovld __cnfn convert_char3_sat_rtn(long3);
char3 __ovld __cnfn convert_char3(long3);
char3 __ovld __cnfn convert_char3_sat(long3);
char3 __ovld __cnfn convert_char3_rte(ulong3);
char3 __ovld __cnfn convert_char3_sat_rte(ulong3);
char3 __ovld __cnfn convert_char3_rtz(ulong3);
char3 __ovld __cnfn convert_char3_sat_rtz(ulong3);
char3 __ovld __cnfn convert_char3_rtp(ulong3);
char3 __ovld __cnfn convert_char3_sat_rtp(ulong3);
char3 __ovld __cnfn convert_char3_rtn(ulong3);
char3 __ovld __cnfn convert_char3_sat_rtn(ulong3);
char3 __ovld __cnfn convert_char3(ulong3);
char3 __ovld __cnfn convert_char3_sat(ulong3);
char3 __ovld __cnfn convert_char3_rte(float3);
char3 __ovld __cnfn convert_char3_sat_rte(float3);
char3 __ovld __cnfn convert_char3_rtz(float3);
char3 __ovld __cnfn convert_char3_sat_rtz(float3);
char3 __ovld __cnfn convert_char3_rtp(float3);
char3 __ovld __cnfn convert_char3_sat_rtp(float3);
char3 __ovld __cnfn convert_char3_rtn(float3);
char3 __ovld __cnfn convert_char3_sat_rtn(float3);
char3 __ovld __cnfn convert_char3(float3);
char3 __ovld __cnfn convert_char3_sat(float3);
uchar3 __ovld __cnfn convert_uchar3_rte(char3);
uchar3 __ovld __cnfn convert_uchar3_sat_rte(char3);
uchar3 __ovld __cnfn convert_uchar3_rtz(char3);
uchar3 __ovld __cnfn convert_uchar3_sat_rtz(char3);
uchar3 __ovld __cnfn convert_uchar3_rtp(char3);
uchar3 __ovld __cnfn convert_uchar3_sat_rtp(char3);
uchar3 __ovld __cnfn convert_uchar3_rtn(char3);
uchar3 __ovld __cnfn convert_uchar3_sat_rtn(char3);
uchar3 __ovld __cnfn convert_uchar3(char3);
uchar3 __ovld __cnfn convert_uchar3_sat(char3);
uchar3 __ovld __cnfn convert_uchar3_rte(uchar3);
uchar3 __ovld __cnfn convert_uchar3_sat_rte(uchar3);
uchar3 __ovld __cnfn convert_uchar3_rtz(uchar3);
uchar3 __ovld __cnfn convert_uchar3_sat_rtz(uchar3);
uchar3 __ovld __cnfn convert_uchar3_rtp(uchar3);
uchar3 __ovld __cnfn convert_uchar3_sat_rtp(uchar3);
uchar3 __ovld __cnfn convert_uchar3_rtn(uchar3);
uchar3 __ovld __cnfn convert_uchar3_sat_rtn(uchar3);
uchar3 __ovld __cnfn convert_uchar3(uchar3);
uchar3 __ovld __cnfn convert_uchar3_sat(uchar3);
uchar3 __ovld __cnfn convert_uchar3_rte(short3);
uchar3 __ovld __cnfn convert_uchar3_sat_rte(short3);
uchar3 __ovld __cnfn convert_uchar3_rtz(short3);
uchar3 __ovld __cnfn convert_uchar3_sat_rtz(short3);
uchar3 __ovld __cnfn convert_uchar3_rtp(short3);
uchar3 __ovld __cnfn convert_uchar3_sat_rtp(short3);
uchar3 __ovld __cnfn convert_uchar3_rtn(short3);
uchar3 __ovld __cnfn convert_uchar3_sat_rtn(short3);
uchar3 __ovld __cnfn convert_uchar3(short3);
uchar3 __ovld __cnfn convert_uchar3_sat(short3);
uchar3 __ovld __cnfn convert_uchar3_rte(ushort3);
uchar3 __ovld __cnfn convert_uchar3_sat_rte(ushort3);
uchar3 __ovld __cnfn convert_uchar3_rtz(ushort3);
uchar3 __ovld __cnfn convert_uchar3_sat_rtz(ushort3);
uchar3 __ovld __cnfn convert_uchar3_rtp(ushort3);
uchar3 __ovld __cnfn convert_uchar3_sat_rtp(ushort3);
uchar3 __ovld __cnfn convert_uchar3_rtn(ushort3);
uchar3 __ovld __cnfn convert_uchar3_sat_rtn(ushort3);
uchar3 __ovld __cnfn convert_uchar3(ushort3);
uchar3 __ovld __cnfn convert_uchar3_sat(ushort3);
uchar3 __ovld __cnfn convert_uchar3_rte(int3);
uchar3 __ovld __cnfn convert_uchar3_sat_rte(int3);
uchar3 __ovld __cnfn convert_uchar3_rtz(int3);
uchar3 __ovld __cnfn convert_uchar3_sat_rtz(int3);
uchar3 __ovld __cnfn convert_uchar3_rtp(int3);
uchar3 __ovld __cnfn convert_uchar3_sat_rtp(int3);
uchar3 __ovld __cnfn convert_uchar3_rtn(int3);
uchar3 __ovld __cnfn convert_uchar3_sat_rtn(int3);
uchar3 __ovld __cnfn convert_uchar3(int3);
uchar3 __ovld __cnfn convert_uchar3_sat(int3);
uchar3 __ovld __cnfn convert_uchar3_rte(uint3);
uchar3 __ovld __cnfn convert_uchar3_sat_rte(uint3);
uchar3 __ovld __cnfn convert_uchar3_rtz(uint3);
uchar3 __ovld __cnfn convert_uchar3_sat_rtz(uint3);
uchar3 __ovld __cnfn convert_uchar3_rtp(uint3);
uchar3 __ovld __cnfn convert_uchar3_sat_rtp(uint3);
uchar3 __ovld __cnfn convert_uchar3_rtn(uint3);
uchar3 __ovld __cnfn convert_uchar3_sat_rtn(uint3);
uchar3 __ovld __cnfn convert_uchar3(uint3);
uchar3 __ovld __cnfn convert_uchar3_sat(uint3);
uchar3 __ovld __cnfn convert_uchar3_rte(long3);
uchar3 __ovld __cnfn convert_uchar3_sat_rte(long3);
uchar3 __ovld __cnfn convert_uchar3_rtz(long3);
uchar3 __ovld __cnfn convert_uchar3_sat_rtz(long3);
uchar3 __ovld __cnfn convert_uchar3_rtp(long3);
uchar3 __ovld __cnfn convert_uchar3_sat_rtp(long3);
uchar3 __ovld __cnfn convert_uchar3_rtn(long3);
uchar3 __ovld __cnfn convert_uchar3_sat_rtn(long3);
uchar3 __ovld __cnfn convert_uchar3(long3);
uchar3 __ovld __cnfn convert_uchar3_sat(long3);
uchar3 __ovld __cnfn convert_uchar3_rte(ulong3);
uchar3 __ovld __cnfn convert_uchar3_sat_rte(ulong3);
uchar3 __ovld __cnfn convert_uchar3_rtz(ulong3);
uchar3 __ovld __cnfn convert_uchar3_sat_rtz(ulong3);
uchar3 __ovld __cnfn convert_uchar3_rtp(ulong3);
uchar3 __ovld __cnfn convert_uchar3_sat_rtp(ulong3);
uchar3 __ovld __cnfn convert_uchar3_rtn(ulong3);
uchar3 __ovld __cnfn convert_uchar3_sat_rtn(ulong3);
uchar3 __ovld __cnfn convert_uchar3(ulong3);
uchar3 __ovld __cnfn convert_uchar3_sat(ulong3);
uchar3 __ovld __cnfn convert_uchar3_rte(float3);
uchar3 __ovld __cnfn convert_uchar3_sat_rte(float3);
uchar3 __ovld __cnfn convert_uchar3_rtz(float3);
uchar3 __ovld __cnfn convert_uchar3_sat_rtz(float3);
uchar3 __ovld __cnfn convert_uchar3_rtp(float3);
uchar3 __ovld __cnfn convert_uchar3_sat_rtp(float3);
uchar3 __ovld __cnfn convert_uchar3_rtn(float3);
uchar3 __ovld __cnfn convert_uchar3_sat_rtn(float3);
uchar3 __ovld __cnfn convert_uchar3(float3);
uchar3 __ovld __cnfn convert_uchar3_sat(float3);
short3 __ovld __cnfn convert_short3_rte(char3);
short3 __ovld __cnfn convert_short3_sat_rte(char3);
short3 __ovld __cnfn convert_short3_rtz(char3);
short3 __ovld __cnfn convert_short3_sat_rtz(char3);
short3 __ovld __cnfn convert_short3_rtp(char3);
short3 __ovld __cnfn convert_short3_sat_rtp(char3);
short3 __ovld __cnfn convert_short3_rtn(char3);
short3 __ovld __cnfn convert_short3_sat_rtn(char3);
short3 __ovld __cnfn convert_short3(char3);
short3 __ovld __cnfn convert_short3_sat(char3);
short3 __ovld __cnfn convert_short3_rte(uchar3);
short3 __ovld __cnfn convert_short3_sat_rte(uchar3);
short3 __ovld __cnfn convert_short3_rtz(uchar3);
short3 __ovld __cnfn convert_short3_sat_rtz(uchar3);
short3 __ovld __cnfn convert_short3_rtp(uchar3);
short3 __ovld __cnfn convert_short3_sat_rtp(uchar3);
short3 __ovld __cnfn convert_short3_rtn(uchar3);
short3 __ovld __cnfn convert_short3_sat_rtn(uchar3);
short3 __ovld __cnfn convert_short3(uchar3);
short3 __ovld __cnfn convert_short3_sat(uchar3);
short3 __ovld __cnfn convert_short3_rte(short3);
short3 __ovld __cnfn convert_short3_sat_rte(short3);
short3 __ovld __cnfn convert_short3_rtz(short3);
short3 __ovld __cnfn convert_short3_sat_rtz(short3);
short3 __ovld __cnfn convert_short3_rtp(short3);
short3 __ovld __cnfn convert_short3_sat_rtp(short3);
short3 __ovld __cnfn convert_short3_rtn(short3);
short3 __ovld __cnfn convert_short3_sat_rtn(short3);
short3 __ovld __cnfn convert_short3(short3);
short3 __ovld __cnfn convert_short3_sat(short3);
short3 __ovld __cnfn convert_short3_rte(ushort3);
short3 __ovld __cnfn convert_short3_sat_rte(ushort3);
short3 __ovld __cnfn convert_short3_rtz(ushort3);
short3 __ovld __cnfn convert_short3_sat_rtz(ushort3);
short3 __ovld __cnfn convert_short3_rtp(ushort3);
short3 __ovld __cnfn convert_short3_sat_rtp(ushort3);
short3 __ovld __cnfn convert_short3_rtn(ushort3);
short3 __ovld __cnfn convert_short3_sat_rtn(ushort3);
short3 __ovld __cnfn convert_short3(ushort3);
short3 __ovld __cnfn convert_short3_sat(ushort3);
short3 __ovld __cnfn convert_short3_rte(int3);
short3 __ovld __cnfn convert_short3_sat_rte(int3);
short3 __ovld __cnfn convert_short3_rtz(int3);
short3 __ovld __cnfn convert_short3_sat_rtz(int3);
short3 __ovld __cnfn convert_short3_rtp(int3);
short3 __ovld __cnfn convert_short3_sat_rtp(int3);
short3 __ovld __cnfn convert_short3_rtn(int3);
short3 __ovld __cnfn convert_short3_sat_rtn(int3);
short3 __ovld __cnfn convert_short3(int3);
short3 __ovld __cnfn convert_short3_sat(int3);
short3 __ovld __cnfn convert_short3_rte(uint3);
short3 __ovld __cnfn convert_short3_sat_rte(uint3);
short3 __ovld __cnfn convert_short3_rtz(uint3);
short3 __ovld __cnfn convert_short3_sat_rtz(uint3);
short3 __ovld __cnfn convert_short3_rtp(uint3);
short3 __ovld __cnfn convert_short3_sat_rtp(uint3);
short3 __ovld __cnfn convert_short3_rtn(uint3);
short3 __ovld __cnfn convert_short3_sat_rtn(uint3);
short3 __ovld __cnfn convert_short3(uint3);
short3 __ovld __cnfn convert_short3_sat(uint3);
short3 __ovld __cnfn convert_short3_rte(long3);
short3 __ovld __cnfn convert_short3_sat_rte(long3);
short3 __ovld __cnfn convert_short3_rtz(long3);
short3 __ovld __cnfn convert_short3_sat_rtz(long3);
short3 __ovld __cnfn convert_short3_rtp(long3);
short3 __ovld __cnfn convert_short3_sat_rtp(long3);
short3 __ovld __cnfn convert_short3_rtn(long3);
short3 __ovld __cnfn convert_short3_sat_rtn(long3);
short3 __ovld __cnfn convert_short3(long3);
short3 __ovld __cnfn convert_short3_sat(long3);
short3 __ovld __cnfn convert_short3_rte(ulong3);
short3 __ovld __cnfn convert_short3_sat_rte(ulong3);
short3 __ovld __cnfn convert_short3_rtz(ulong3);
short3 __ovld __cnfn convert_short3_sat_rtz(ulong3);
short3 __ovld __cnfn convert_short3_rtp(ulong3);
short3 __ovld __cnfn convert_short3_sat_rtp(ulong3);
short3 __ovld __cnfn convert_short3_rtn(ulong3);
short3 __ovld __cnfn convert_short3_sat_rtn(ulong3);
short3 __ovld __cnfn convert_short3(ulong3);
short3 __ovld __cnfn convert_short3_sat(ulong3);
short3 __ovld __cnfn convert_short3_rte(float3);
short3 __ovld __cnfn convert_short3_sat_rte(float3);
short3 __ovld __cnfn convert_short3_rtz(float3);
short3 __ovld __cnfn convert_short3_sat_rtz(float3);
short3 __ovld __cnfn convert_short3_rtp(float3);
short3 __ovld __cnfn convert_short3_sat_rtp(float3);
short3 __ovld __cnfn convert_short3_rtn(float3);
short3 __ovld __cnfn convert_short3_sat_rtn(float3);
short3 __ovld __cnfn convert_short3(float3);
short3 __ovld __cnfn convert_short3_sat(float3);
ushort3 __ovld __cnfn convert_ushort3_rte(char3);
ushort3 __ovld __cnfn convert_ushort3_sat_rte(char3);
ushort3 __ovld __cnfn convert_ushort3_rtz(char3);
ushort3 __ovld __cnfn convert_ushort3_sat_rtz(char3);
ushort3 __ovld __cnfn convert_ushort3_rtp(char3);
ushort3 __ovld __cnfn convert_ushort3_sat_rtp(char3);
ushort3 __ovld __cnfn convert_ushort3_rtn(char3);
ushort3 __ovld __cnfn convert_ushort3_sat_rtn(char3);
ushort3 __ovld __cnfn convert_ushort3(char3);
ushort3 __ovld __cnfn convert_ushort3_sat(char3);
ushort3 __ovld __cnfn convert_ushort3_rte(uchar3);
ushort3 __ovld __cnfn convert_ushort3_sat_rte(uchar3);
ushort3 __ovld __cnfn convert_ushort3_rtz(uchar3);
ushort3 __ovld __cnfn convert_ushort3_sat_rtz(uchar3);
ushort3 __ovld __cnfn convert_ushort3_rtp(uchar3);
ushort3 __ovld __cnfn convert_ushort3_sat_rtp(uchar3);
ushort3 __ovld __cnfn convert_ushort3_rtn(uchar3);
ushort3 __ovld __cnfn convert_ushort3_sat_rtn(uchar3);
ushort3 __ovld __cnfn convert_ushort3(uchar3);
ushort3 __ovld __cnfn convert_ushort3_sat(uchar3);
ushort3 __ovld __cnfn convert_ushort3_rte(short3);
ushort3 __ovld __cnfn convert_ushort3_sat_rte(short3);
ushort3 __ovld __cnfn convert_ushort3_rtz(short3);
ushort3 __ovld __cnfn convert_ushort3_sat_rtz(short3);
ushort3 __ovld __cnfn convert_ushort3_rtp(short3);
ushort3 __ovld __cnfn convert_ushort3_sat_rtp(short3);
ushort3 __ovld __cnfn convert_ushort3_rtn(short3);
ushort3 __ovld __cnfn convert_ushort3_sat_rtn(short3);
ushort3 __ovld __cnfn convert_ushort3(short3);
ushort3 __ovld __cnfn convert_ushort3_sat(short3);
ushort3 __ovld __cnfn convert_ushort3_rte(ushort3);
ushort3 __ovld __cnfn convert_ushort3_sat_rte(ushort3);
ushort3 __ovld __cnfn convert_ushort3_rtz(ushort3);
ushort3 __ovld __cnfn convert_ushort3_sat_rtz(ushort3);
ushort3 __ovld __cnfn convert_ushort3_rtp(ushort3);
ushort3 __ovld __cnfn convert_ushort3_sat_rtp(ushort3);
ushort3 __ovld __cnfn convert_ushort3_rtn(ushort3);
ushort3 __ovld __cnfn convert_ushort3_sat_rtn(ushort3);
ushort3 __ovld __cnfn convert_ushort3(ushort3);
ushort3 __ovld __cnfn convert_ushort3_sat(ushort3);
ushort3 __ovld __cnfn convert_ushort3_rte(int3);
ushort3 __ovld __cnfn convert_ushort3_sat_rte(int3);
ushort3 __ovld __cnfn convert_ushort3_rtz(int3);
ushort3 __ovld __cnfn convert_ushort3_sat_rtz(int3);
ushort3 __ovld __cnfn convert_ushort3_rtp(int3);
ushort3 __ovld __cnfn convert_ushort3_sat_rtp(int3);
ushort3 __ovld __cnfn convert_ushort3_rtn(int3);
ushort3 __ovld __cnfn convert_ushort3_sat_rtn(int3);
ushort3 __ovld __cnfn convert_ushort3(int3);
ushort3 __ovld __cnfn convert_ushort3_sat(int3);
ushort3 __ovld __cnfn convert_ushort3_rte(uint3);
ushort3 __ovld __cnfn convert_ushort3_sat_rte(uint3);
ushort3 __ovld __cnfn convert_ushort3_rtz(uint3);
ushort3 __ovld __cnfn convert_ushort3_sat_rtz(uint3);
ushort3 __ovld __cnfn convert_ushort3_rtp(uint3);
ushort3 __ovld __cnfn convert_ushort3_sat_rtp(uint3);
ushort3 __ovld __cnfn convert_ushort3_rtn(uint3);
ushort3 __ovld __cnfn convert_ushort3_sat_rtn(uint3);
ushort3 __ovld __cnfn convert_ushort3(uint3);
ushort3 __ovld __cnfn convert_ushort3_sat(uint3);
ushort3 __ovld __cnfn convert_ushort3_rte(long3);
ushort3 __ovld __cnfn convert_ushort3_sat_rte(long3);
ushort3 __ovld __cnfn convert_ushort3_rtz(long3);
ushort3 __ovld __cnfn convert_ushort3_sat_rtz(long3);
ushort3 __ovld __cnfn convert_ushort3_rtp(long3);
ushort3 __ovld __cnfn convert_ushort3_sat_rtp(long3);
ushort3 __ovld __cnfn convert_ushort3_rtn(long3);
ushort3 __ovld __cnfn convert_ushort3_sat_rtn(long3);
ushort3 __ovld __cnfn convert_ushort3(long3);
ushort3 __ovld __cnfn convert_ushort3_sat(long3);
ushort3 __ovld __cnfn convert_ushort3_rte(ulong3);
ushort3 __ovld __cnfn convert_ushort3_sat_rte(ulong3);
ushort3 __ovld __cnfn convert_ushort3_rtz(ulong3);
ushort3 __ovld __cnfn convert_ushort3_sat_rtz(ulong3);
ushort3 __ovld __cnfn convert_ushort3_rtp(ulong3);
ushort3 __ovld __cnfn convert_ushort3_sat_rtp(ulong3);
ushort3 __ovld __cnfn convert_ushort3_rtn(ulong3);
ushort3 __ovld __cnfn convert_ushort3_sat_rtn(ulong3);
ushort3 __ovld __cnfn convert_ushort3(ulong3);
ushort3 __ovld __cnfn convert_ushort3_sat(ulong3);
ushort3 __ovld __cnfn convert_ushort3_rte(float3);
ushort3 __ovld __cnfn convert_ushort3_sat_rte(float3);
ushort3 __ovld __cnfn convert_ushort3_rtz(float3);
ushort3 __ovld __cnfn convert_ushort3_sat_rtz(float3);
ushort3 __ovld __cnfn convert_ushort3_rtp(float3);
ushort3 __ovld __cnfn convert_ushort3_sat_rtp(float3);
ushort3 __ovld __cnfn convert_ushort3_rtn(float3);
ushort3 __ovld __cnfn convert_ushort3_sat_rtn(float3);
ushort3 __ovld __cnfn convert_ushort3(float3);
ushort3 __ovld __cnfn convert_ushort3_sat(float3);
int3 __ovld __cnfn convert_int3_rte(char3);
int3 __ovld __cnfn convert_int3_sat_rte(char3);
int3 __ovld __cnfn convert_int3_rtz(char3);
int3 __ovld __cnfn convert_int3_sat_rtz(char3);
int3 __ovld __cnfn convert_int3_rtp(char3);
int3 __ovld __cnfn convert_int3_sat_rtp(char3);
int3 __ovld __cnfn convert_int3_rtn(char3);
int3 __ovld __cnfn convert_int3_sat_rtn(char3);
int3 __ovld __cnfn convert_int3(char3);
int3 __ovld __cnfn convert_int3_sat(char3);
int3 __ovld __cnfn convert_int3_rte(uchar3);
int3 __ovld __cnfn convert_int3_sat_rte(uchar3);
int3 __ovld __cnfn convert_int3_rtz(uchar3);
int3 __ovld __cnfn convert_int3_sat_rtz(uchar3);
int3 __ovld __cnfn convert_int3_rtp(uchar3);
int3 __ovld __cnfn convert_int3_sat_rtp(uchar3);
int3 __ovld __cnfn convert_int3_rtn(uchar3);
int3 __ovld __cnfn convert_int3_sat_rtn(uchar3);
int3 __ovld __cnfn convert_int3(uchar3);
int3 __ovld __cnfn convert_int3_sat(uchar3);
int3 __ovld __cnfn convert_int3_rte(short3);
int3 __ovld __cnfn convert_int3_sat_rte(short3);
int3 __ovld __cnfn convert_int3_rtz(short3);
int3 __ovld __cnfn convert_int3_sat_rtz(short3);
int3 __ovld __cnfn convert_int3_rtp(short3);
int3 __ovld __cnfn convert_int3_sat_rtp(short3);
int3 __ovld __cnfn convert_int3_rtn(short3);
int3 __ovld __cnfn convert_int3_sat_rtn(short3);
int3 __ovld __cnfn convert_int3(short3);
int3 __ovld __cnfn convert_int3_sat(short3);
int3 __ovld __cnfn convert_int3_rte(ushort3);
int3 __ovld __cnfn convert_int3_sat_rte(ushort3);
int3 __ovld __cnfn convert_int3_rtz(ushort3);
int3 __ovld __cnfn convert_int3_sat_rtz(ushort3);
int3 __ovld __cnfn convert_int3_rtp(ushort3);
int3 __ovld __cnfn convert_int3_sat_rtp(ushort3);
int3 __ovld __cnfn convert_int3_rtn(ushort3);
int3 __ovld __cnfn convert_int3_sat_rtn(ushort3);
int3 __ovld __cnfn convert_int3(ushort3);
int3 __ovld __cnfn convert_int3_sat(ushort3);
int3 __ovld __cnfn convert_int3_rte(int3);
int3 __ovld __cnfn convert_int3_sat_rte(int3);
int3 __ovld __cnfn convert_int3_rtz(int3);
int3 __ovld __cnfn convert_int3_sat_rtz(int3);
int3 __ovld __cnfn convert_int3_rtp(int3);
int3 __ovld __cnfn convert_int3_sat_rtp(int3);
int3 __ovld __cnfn convert_int3_rtn(int3);
int3 __ovld __cnfn convert_int3_sat_rtn(int3);
int3 __ovld __cnfn convert_int3(int3);
int3 __ovld __cnfn convert_int3_sat(int3);
int3 __ovld __cnfn convert_int3_rte(uint3);
int3 __ovld __cnfn convert_int3_sat_rte(uint3);
int3 __ovld __cnfn convert_int3_rtz(uint3);
int3 __ovld __cnfn convert_int3_sat_rtz(uint3);
int3 __ovld __cnfn convert_int3_rtp(uint3);
int3 __ovld __cnfn convert_int3_sat_rtp(uint3);
int3 __ovld __cnfn convert_int3_rtn(uint3);
int3 __ovld __cnfn convert_int3_sat_rtn(uint3);
int3 __ovld __cnfn convert_int3(uint3);
int3 __ovld __cnfn convert_int3_sat(uint3);
int3 __ovld __cnfn convert_int3_rte(long3);
int3 __ovld __cnfn convert_int3_sat_rte(long3);
int3 __ovld __cnfn convert_int3_rtz(long3);
int3 __ovld __cnfn convert_int3_sat_rtz(long3);
int3 __ovld __cnfn convert_int3_rtp(long3);
int3 __ovld __cnfn convert_int3_sat_rtp(long3);
int3 __ovld __cnfn convert_int3_rtn(long3);
int3 __ovld __cnfn convert_int3_sat_rtn(long3);
int3 __ovld __cnfn convert_int3(long3);
int3 __ovld __cnfn convert_int3_sat(long3);
int3 __ovld __cnfn convert_int3_rte(ulong3);
int3 __ovld __cnfn convert_int3_sat_rte(ulong3);
int3 __ovld __cnfn convert_int3_rtz(ulong3);
int3 __ovld __cnfn convert_int3_sat_rtz(ulong3);
int3 __ovld __cnfn convert_int3_rtp(ulong3);
int3 __ovld __cnfn convert_int3_sat_rtp(ulong3);
int3 __ovld __cnfn convert_int3_rtn(ulong3);
int3 __ovld __cnfn convert_int3_sat_rtn(ulong3);
int3 __ovld __cnfn convert_int3(ulong3);
int3 __ovld __cnfn convert_int3_sat(ulong3);
int3 __ovld __cnfn convert_int3_rte(float3);
int3 __ovld __cnfn convert_int3_sat_rte(float3);
int3 __ovld __cnfn convert_int3_rtz(float3);
int3 __ovld __cnfn convert_int3_sat_rtz(float3);
int3 __ovld __cnfn convert_int3_rtp(float3);
int3 __ovld __cnfn convert_int3_sat_rtp(float3);
int3 __ovld __cnfn convert_int3_rtn(float3);
int3 __ovld __cnfn convert_int3_sat_rtn(float3);
int3 __ovld __cnfn convert_int3(float3);
int3 __ovld __cnfn convert_int3_sat(float3);
uint3 __ovld __cnfn convert_uint3_rte(char3);
uint3 __ovld __cnfn convert_uint3_sat_rte(char3);
uint3 __ovld __cnfn convert_uint3_rtz(char3);
uint3 __ovld __cnfn convert_uint3_sat_rtz(char3);
uint3 __ovld __cnfn convert_uint3_rtp(char3);
uint3 __ovld __cnfn convert_uint3_sat_rtp(char3);
uint3 __ovld __cnfn convert_uint3_rtn(char3);
uint3 __ovld __cnfn convert_uint3_sat_rtn(char3);
uint3 __ovld __cnfn convert_uint3(char3);
uint3 __ovld __cnfn convert_uint3_sat(char3);
uint3 __ovld __cnfn convert_uint3_rte(uchar3);
uint3 __ovld __cnfn convert_uint3_sat_rte(uchar3);
uint3 __ovld __cnfn convert_uint3_rtz(uchar3);
uint3 __ovld __cnfn convert_uint3_sat_rtz(uchar3);
uint3 __ovld __cnfn convert_uint3_rtp(uchar3);
uint3 __ovld __cnfn convert_uint3_sat_rtp(uchar3);
uint3 __ovld __cnfn convert_uint3_rtn(uchar3);
uint3 __ovld __cnfn convert_uint3_sat_rtn(uchar3);
uint3 __ovld __cnfn convert_uint3(uchar3);
uint3 __ovld __cnfn convert_uint3_sat(uchar3);
uint3 __ovld __cnfn convert_uint3_rte(short3);
uint3 __ovld __cnfn convert_uint3_sat_rte(short3);
uint3 __ovld __cnfn convert_uint3_rtz(short3);
uint3 __ovld __cnfn convert_uint3_sat_rtz(short3);
uint3 __ovld __cnfn convert_uint3_rtp(short3);
uint3 __ovld __cnfn convert_uint3_sat_rtp(short3);
uint3 __ovld __cnfn convert_uint3_rtn(short3);
uint3 __ovld __cnfn convert_uint3_sat_rtn(short3);
uint3 __ovld __cnfn convert_uint3(short3);
uint3 __ovld __cnfn convert_uint3_sat(short3);
uint3 __ovld __cnfn convert_uint3_rte(ushort3);
uint3 __ovld __cnfn convert_uint3_sat_rte(ushort3);
uint3 __ovld __cnfn convert_uint3_rtz(ushort3);
uint3 __ovld __cnfn convert_uint3_sat_rtz(ushort3);
uint3 __ovld __cnfn convert_uint3_rtp(ushort3);
uint3 __ovld __cnfn convert_uint3_sat_rtp(ushort3);
uint3 __ovld __cnfn convert_uint3_rtn(ushort3);
uint3 __ovld __cnfn convert_uint3_sat_rtn(ushort3);
uint3 __ovld __cnfn convert_uint3(ushort3);
uint3 __ovld __cnfn convert_uint3_sat(ushort3);
uint3 __ovld __cnfn convert_uint3_rte(int3);
uint3 __ovld __cnfn convert_uint3_sat_rte(int3);
uint3 __ovld __cnfn convert_uint3_rtz(int3);
uint3 __ovld __cnfn convert_uint3_sat_rtz(int3);
uint3 __ovld __cnfn convert_uint3_rtp(int3);
uint3 __ovld __cnfn convert_uint3_sat_rtp(int3);
uint3 __ovld __cnfn convert_uint3_rtn(int3);
uint3 __ovld __cnfn convert_uint3_sat_rtn(int3);
uint3 __ovld __cnfn convert_uint3(int3);
uint3 __ovld __cnfn convert_uint3_sat(int3);
uint3 __ovld __cnfn convert_uint3_rte(uint3);
uint3 __ovld __cnfn convert_uint3_sat_rte(uint3);
uint3 __ovld __cnfn convert_uint3_rtz(uint3);
uint3 __ovld __cnfn convert_uint3_sat_rtz(uint3);
uint3 __ovld __cnfn convert_uint3_rtp(uint3);
uint3 __ovld __cnfn convert_uint3_sat_rtp(uint3);
uint3 __ovld __cnfn convert_uint3_rtn(uint3);
uint3 __ovld __cnfn convert_uint3_sat_rtn(uint3);
uint3 __ovld __cnfn convert_uint3(uint3);
uint3 __ovld __cnfn convert_uint3_sat(uint3);
uint3 __ovld __cnfn convert_uint3_rte(long3);
uint3 __ovld __cnfn convert_uint3_sat_rte(long3);
uint3 __ovld __cnfn convert_uint3_rtz(long3);
uint3 __ovld __cnfn convert_uint3_sat_rtz(long3);
uint3 __ovld __cnfn convert_uint3_rtp(long3);
uint3 __ovld __cnfn convert_uint3_sat_rtp(long3);
uint3 __ovld __cnfn convert_uint3_rtn(long3);
uint3 __ovld __cnfn convert_uint3_sat_rtn(long3);
uint3 __ovld __cnfn convert_uint3(long3);
uint3 __ovld __cnfn convert_uint3_sat(long3);
uint3 __ovld __cnfn convert_uint3_rte(ulong3);
uint3 __ovld __cnfn convert_uint3_sat_rte(ulong3);
uint3 __ovld __cnfn convert_uint3_rtz(ulong3);
uint3 __ovld __cnfn convert_uint3_sat_rtz(ulong3);
uint3 __ovld __cnfn convert_uint3_rtp(ulong3);
uint3 __ovld __cnfn convert_uint3_sat_rtp(ulong3);
uint3 __ovld __cnfn convert_uint3_rtn(ulong3);
uint3 __ovld __cnfn convert_uint3_sat_rtn(ulong3);
uint3 __ovld __cnfn convert_uint3(ulong3);
uint3 __ovld __cnfn convert_uint3_sat(ulong3);
uint3 __ovld __cnfn convert_uint3_rte(float3);
uint3 __ovld __cnfn convert_uint3_sat_rte(float3);
uint3 __ovld __cnfn convert_uint3_rtz(float3);
uint3 __ovld __cnfn convert_uint3_sat_rtz(float3);
uint3 __ovld __cnfn convert_uint3_rtp(float3);
uint3 __ovld __cnfn convert_uint3_sat_rtp(float3);
uint3 __ovld __cnfn convert_uint3_rtn(float3);
uint3 __ovld __cnfn convert_uint3_sat_rtn(float3);
uint3 __ovld __cnfn convert_uint3(float3);
uint3 __ovld __cnfn convert_uint3_sat(float3);
long3 __ovld __cnfn convert_long3_rte(char3);
long3 __ovld __cnfn convert_long3_sat_rte(char3);
long3 __ovld __cnfn convert_long3_rtz(char3);
long3 __ovld __cnfn convert_long3_sat_rtz(char3);
long3 __ovld __cnfn convert_long3_rtp(char3);
long3 __ovld __cnfn convert_long3_sat_rtp(char3);
long3 __ovld __cnfn convert_long3_rtn(char3);
long3 __ovld __cnfn convert_long3_sat_rtn(char3);
long3 __ovld __cnfn convert_long3(char3);
long3 __ovld __cnfn convert_long3_sat(char3);
long3 __ovld __cnfn convert_long3_rte(uchar3);
long3 __ovld __cnfn convert_long3_sat_rte(uchar3);
long3 __ovld __cnfn convert_long3_rtz(uchar3);
long3 __ovld __cnfn convert_long3_sat_rtz(uchar3);
long3 __ovld __cnfn convert_long3_rtp(uchar3);
long3 __ovld __cnfn convert_long3_sat_rtp(uchar3);
long3 __ovld __cnfn convert_long3_rtn(uchar3);
long3 __ovld __cnfn convert_long3_sat_rtn(uchar3);
long3 __ovld __cnfn convert_long3(uchar3);
long3 __ovld __cnfn convert_long3_sat(uchar3);
long3 __ovld __cnfn convert_long3_rte(short3);
long3 __ovld __cnfn convert_long3_sat_rte(short3);
long3 __ovld __cnfn convert_long3_rtz(short3);
long3 __ovld __cnfn convert_long3_sat_rtz(short3);
long3 __ovld __cnfn convert_long3_rtp(short3);
long3 __ovld __cnfn convert_long3_sat_rtp(short3);
long3 __ovld __cnfn convert_long3_rtn(short3);
long3 __ovld __cnfn convert_long3_sat_rtn(short3);
long3 __ovld __cnfn convert_long3(short3);
long3 __ovld __cnfn convert_long3_sat(short3);
long3 __ovld __cnfn convert_long3_rte(ushort3);
long3 __ovld __cnfn convert_long3_sat_rte(ushort3);
long3 __ovld __cnfn convert_long3_rtz(ushort3);
long3 __ovld __cnfn convert_long3_sat_rtz(ushort3);
long3 __ovld __cnfn convert_long3_rtp(ushort3);
long3 __ovld __cnfn convert_long3_sat_rtp(ushort3);
long3 __ovld __cnfn convert_long3_rtn(ushort3);
long3 __ovld __cnfn convert_long3_sat_rtn(ushort3);
long3 __ovld __cnfn convert_long3(ushort3);
long3 __ovld __cnfn convert_long3_sat(ushort3);
long3 __ovld __cnfn convert_long3_rte(int3);
long3 __ovld __cnfn convert_long3_sat_rte(int3);
long3 __ovld __cnfn convert_long3_rtz(int3);
long3 __ovld __cnfn convert_long3_sat_rtz(int3);
long3 __ovld __cnfn convert_long3_rtp(int3);
long3 __ovld __cnfn convert_long3_sat_rtp(int3);
long3 __ovld __cnfn convert_long3_rtn(int3);
long3 __ovld __cnfn convert_long3_sat_rtn(int3);
long3 __ovld __cnfn convert_long3(int3);
long3 __ovld __cnfn convert_long3_sat(int3);
long3 __ovld __cnfn convert_long3_rte(uint3);
long3 __ovld __cnfn convert_long3_sat_rte(uint3);
long3 __ovld __cnfn convert_long3_rtz(uint3);
long3 __ovld __cnfn convert_long3_sat_rtz(uint3);
long3 __ovld __cnfn convert_long3_rtp(uint3);
long3 __ovld __cnfn convert_long3_sat_rtp(uint3);
long3 __ovld __cnfn convert_long3_rtn(uint3);
long3 __ovld __cnfn convert_long3_sat_rtn(uint3);
long3 __ovld __cnfn convert_long3(uint3);
long3 __ovld __cnfn convert_long3_sat(uint3);
long3 __ovld __cnfn convert_long3_rte(long3);
long3 __ovld __cnfn convert_long3_sat_rte(long3);
long3 __ovld __cnfn convert_long3_rtz(long3);
long3 __ovld __cnfn convert_long3_sat_rtz(long3);
long3 __ovld __cnfn convert_long3_rtp(long3);
long3 __ovld __cnfn convert_long3_sat_rtp(long3);
long3 __ovld __cnfn convert_long3_rtn(long3);
long3 __ovld __cnfn convert_long3_sat_rtn(long3);
long3 __ovld __cnfn convert_long3(long3);
long3 __ovld __cnfn convert_long3_sat(long3);
long3 __ovld __cnfn convert_long3_rte(ulong3);
long3 __ovld __cnfn convert_long3_sat_rte(ulong3);
long3 __ovld __cnfn convert_long3_rtz(ulong3);
long3 __ovld __cnfn convert_long3_sat_rtz(ulong3);
long3 __ovld __cnfn convert_long3_rtp(ulong3);
long3 __ovld __cnfn convert_long3_sat_rtp(ulong3);
long3 __ovld __cnfn convert_long3_rtn(ulong3);
long3 __ovld __cnfn convert_long3_sat_rtn(ulong3);
long3 __ovld __cnfn convert_long3(ulong3);
long3 __ovld __cnfn convert_long3_sat(ulong3);
long3 __ovld __cnfn convert_long3_rte(float3);
long3 __ovld __cnfn convert_long3_sat_rte(float3);
long3 __ovld __cnfn convert_long3_rtz(float3);
long3 __ovld __cnfn convert_long3_sat_rtz(float3);
long3 __ovld __cnfn convert_long3_rtp(float3);
long3 __ovld __cnfn convert_long3_sat_rtp(float3);
long3 __ovld __cnfn convert_long3_rtn(float3);
long3 __ovld __cnfn convert_long3_sat_rtn(float3);
long3 __ovld __cnfn convert_long3(float3);
long3 __ovld __cnfn convert_long3_sat(float3);
ulong3 __ovld __cnfn convert_ulong3_rte(char3);
ulong3 __ovld __cnfn convert_ulong3_sat_rte(char3);
ulong3 __ovld __cnfn convert_ulong3_rtz(char3);
ulong3 __ovld __cnfn convert_ulong3_sat_rtz(char3);
ulong3 __ovld __cnfn convert_ulong3_rtp(char3);
ulong3 __ovld __cnfn convert_ulong3_sat_rtp(char3);
ulong3 __ovld __cnfn convert_ulong3_rtn(char3);
ulong3 __ovld __cnfn convert_ulong3_sat_rtn(char3);
ulong3 __ovld __cnfn convert_ulong3(char3);
ulong3 __ovld __cnfn convert_ulong3_sat(char3);
ulong3 __ovld __cnfn convert_ulong3_rte(uchar3);
ulong3 __ovld __cnfn convert_ulong3_sat_rte(uchar3);
ulong3 __ovld __cnfn convert_ulong3_rtz(uchar3);
ulong3 __ovld __cnfn convert_ulong3_sat_rtz(uchar3);
ulong3 __ovld __cnfn convert_ulong3_rtp(uchar3);
ulong3 __ovld __cnfn convert_ulong3_sat_rtp(uchar3);
ulong3 __ovld __cnfn convert_ulong3_rtn(uchar3);
ulong3 __ovld __cnfn convert_ulong3_sat_rtn(uchar3);
ulong3 __ovld __cnfn convert_ulong3(uchar3);
ulong3 __ovld __cnfn convert_ulong3_sat(uchar3);
ulong3 __ovld __cnfn convert_ulong3_rte(short3);
ulong3 __ovld __cnfn convert_ulong3_sat_rte(short3);
ulong3 __ovld __cnfn convert_ulong3_rtz(short3);
ulong3 __ovld __cnfn convert_ulong3_sat_rtz(short3);
ulong3 __ovld __cnfn convert_ulong3_rtp(short3);
ulong3 __ovld __cnfn convert_ulong3_sat_rtp(short3);
ulong3 __ovld __cnfn convert_ulong3_rtn(short3);
ulong3 __ovld __cnfn convert_ulong3_sat_rtn(short3);
ulong3 __ovld __cnfn convert_ulong3(short3);
ulong3 __ovld __cnfn convert_ulong3_sat(short3);
ulong3 __ovld __cnfn convert_ulong3_rte(ushort3);
ulong3 __ovld __cnfn convert_ulong3_sat_rte(ushort3);
ulong3 __ovld __cnfn convert_ulong3_rtz(ushort3);
ulong3 __ovld __cnfn convert_ulong3_sat_rtz(ushort3);
ulong3 __ovld __cnfn convert_ulong3_rtp(ushort3);
ulong3 __ovld __cnfn convert_ulong3_sat_rtp(ushort3);
ulong3 __ovld __cnfn convert_ulong3_rtn(ushort3);
ulong3 __ovld __cnfn convert_ulong3_sat_rtn(ushort3);
ulong3 __ovld __cnfn convert_ulong3(ushort3);
ulong3 __ovld __cnfn convert_ulong3_sat(ushort3);
ulong3 __ovld __cnfn convert_ulong3_rte(int3);
ulong3 __ovld __cnfn convert_ulong3_sat_rte(int3);
ulong3 __ovld __cnfn convert_ulong3_rtz(int3);
ulong3 __ovld __cnfn convert_ulong3_sat_rtz(int3);
ulong3 __ovld __cnfn convert_ulong3_rtp(int3);
ulong3 __ovld __cnfn convert_ulong3_sat_rtp(int3);
ulong3 __ovld __cnfn convert_ulong3_rtn(int3);
ulong3 __ovld __cnfn convert_ulong3_sat_rtn(int3);
ulong3 __ovld __cnfn convert_ulong3(int3);
ulong3 __ovld __cnfn convert_ulong3_sat(int3);
ulong3 __ovld __cnfn convert_ulong3_rte(uint3);
ulong3 __ovld __cnfn convert_ulong3_sat_rte(uint3);
ulong3 __ovld __cnfn convert_ulong3_rtz(uint3);
ulong3 __ovld __cnfn convert_ulong3_sat_rtz(uint3);
ulong3 __ovld __cnfn convert_ulong3_rtp(uint3);
ulong3 __ovld __cnfn convert_ulong3_sat_rtp(uint3);
ulong3 __ovld __cnfn convert_ulong3_rtn(uint3);
ulong3 __ovld __cnfn convert_ulong3_sat_rtn(uint3);
ulong3 __ovld __cnfn convert_ulong3(uint3);
ulong3 __ovld __cnfn convert_ulong3_sat(uint3);
ulong3 __ovld __cnfn convert_ulong3_rte(long3);
ulong3 __ovld __cnfn convert_ulong3_sat_rte(long3);
ulong3 __ovld __cnfn convert_ulong3_rtz(long3);
ulong3 __ovld __cnfn convert_ulong3_sat_rtz(long3);
ulong3 __ovld __cnfn convert_ulong3_rtp(long3);
ulong3 __ovld __cnfn convert_ulong3_sat_rtp(long3);
ulong3 __ovld __cnfn convert_ulong3_rtn(long3);
ulong3 __ovld __cnfn convert_ulong3_sat_rtn(long3);
ulong3 __ovld __cnfn convert_ulong3(long3);
ulong3 __ovld __cnfn convert_ulong3_sat(long3);
ulong3 __ovld __cnfn convert_ulong3_rte(ulong3);
ulong3 __ovld __cnfn convert_ulong3_sat_rte(ulong3);
ulong3 __ovld __cnfn convert_ulong3_rtz(ulong3);
ulong3 __ovld __cnfn convert_ulong3_sat_rtz(ulong3);
ulong3 __ovld __cnfn convert_ulong3_rtp(ulong3);
ulong3 __ovld __cnfn convert_ulong3_sat_rtp(ulong3);
ulong3 __ovld __cnfn convert_ulong3_rtn(ulong3);
ulong3 __ovld __cnfn convert_ulong3_sat_rtn(ulong3);
ulong3 __ovld __cnfn convert_ulong3(ulong3);
ulong3 __ovld __cnfn convert_ulong3_sat(ulong3);
ulong3 __ovld __cnfn convert_ulong3_rte(float3);
ulong3 __ovld __cnfn convert_ulong3_sat_rte(float3);
ulong3 __ovld __cnfn convert_ulong3_rtz(float3);
ulong3 __ovld __cnfn convert_ulong3_sat_rtz(float3);
ulong3 __ovld __cnfn convert_ulong3_rtp(float3);
ulong3 __ovld __cnfn convert_ulong3_sat_rtp(float3);
ulong3 __ovld __cnfn convert_ulong3_rtn(float3);
ulong3 __ovld __cnfn convert_ulong3_sat_rtn(float3);
ulong3 __ovld __cnfn convert_ulong3(float3);
ulong3 __ovld __cnfn convert_ulong3_sat(float3);
float3 __ovld __cnfn convert_float3_rte(char3);
float3 __ovld __cnfn convert_float3_rtz(char3);
float3 __ovld __cnfn convert_float3_rtp(char3);
float3 __ovld __cnfn convert_float3_rtn(char3);
float3 __ovld __cnfn convert_float3(char3);
float3 __ovld __cnfn convert_float3_rte(uchar3);
float3 __ovld __cnfn convert_float3_rtz(uchar3);
float3 __ovld __cnfn convert_float3_rtp(uchar3);
float3 __ovld __cnfn convert_float3_rtn(uchar3);
float3 __ovld __cnfn convert_float3(uchar3);
float3 __ovld __cnfn convert_float3_rte(short3);
float3 __ovld __cnfn convert_float3_rtz(short3);
float3 __ovld __cnfn convert_float3_rtp(short3);
float3 __ovld __cnfn convert_float3_rtn(short3);
float3 __ovld __cnfn convert_float3(short3);
float3 __ovld __cnfn convert_float3_rte(ushort3);
float3 __ovld __cnfn convert_float3_rtz(ushort3);
float3 __ovld __cnfn convert_float3_rtp(ushort3);
float3 __ovld __cnfn convert_float3_rtn(ushort3);
float3 __ovld __cnfn convert_float3(ushort3);
float3 __ovld __cnfn convert_float3_rte(int3);
float3 __ovld __cnfn convert_float3_rtz(int3);
float3 __ovld __cnfn convert_float3_rtp(int3);
float3 __ovld __cnfn convert_float3_rtn(int3);
float3 __ovld __cnfn convert_float3(int3);
float3 __ovld __cnfn convert_float3_rte(uint3);
float3 __ovld __cnfn convert_float3_rtz(uint3);
float3 __ovld __cnfn convert_float3_rtp(uint3);
float3 __ovld __cnfn convert_float3_rtn(uint3);
float3 __ovld __cnfn convert_float3(uint3);
float3 __ovld __cnfn convert_float3_rte(long3);
float3 __ovld __cnfn convert_float3_rtz(long3);
float3 __ovld __cnfn convert_float3_rtp(long3);
float3 __ovld __cnfn convert_float3_rtn(long3);
float3 __ovld __cnfn convert_float3(long3);
float3 __ovld __cnfn convert_float3_rte(ulong3);
float3 __ovld __cnfn convert_float3_rtz(ulong3);
float3 __ovld __cnfn convert_float3_rtp(ulong3);
float3 __ovld __cnfn convert_float3_rtn(ulong3);
float3 __ovld __cnfn convert_float3(ulong3);
float3 __ovld __cnfn convert_float3_rte(float3);
float3 __ovld __cnfn convert_float3_rtz(float3);
float3 __ovld __cnfn convert_float3_rtp(float3);
float3 __ovld __cnfn convert_float3_rtn(float3);
float3 __ovld __cnfn convert_float3(float3);
char4 __ovld __cnfn convert_char4_rte(char4);
char4 __ovld __cnfn convert_char4_sat_rte(char4);
char4 __ovld __cnfn convert_char4_rtz(char4);
char4 __ovld __cnfn convert_char4_sat_rtz(char4);
char4 __ovld __cnfn convert_char4_rtp(char4);
char4 __ovld __cnfn convert_char4_sat_rtp(char4);
char4 __ovld __cnfn convert_char4_rtn(char4);
char4 __ovld __cnfn convert_char4_sat_rtn(char4);
char4 __ovld __cnfn convert_char4(char4);
char4 __ovld __cnfn convert_char4_sat(char4);
char4 __ovld __cnfn convert_char4_rte(uchar4);
char4 __ovld __cnfn convert_char4_sat_rte(uchar4);
char4 __ovld __cnfn convert_char4_rtz(uchar4);
char4 __ovld __cnfn convert_char4_sat_rtz(uchar4);
char4 __ovld __cnfn convert_char4_rtp(uchar4);
char4 __ovld __cnfn convert_char4_sat_rtp(uchar4);
char4 __ovld __cnfn convert_char4_rtn(uchar4);
char4 __ovld __cnfn convert_char4_sat_rtn(uchar4);
char4 __ovld __cnfn convert_char4(uchar4);
char4 __ovld __cnfn convert_char4_sat(uchar4);
char4 __ovld __cnfn convert_char4_rte(short4);
char4 __ovld __cnfn convert_char4_sat_rte(short4);
char4 __ovld __cnfn convert_char4_rtz(short4);
char4 __ovld __cnfn convert_char4_sat_rtz(short4);
char4 __ovld __cnfn convert_char4_rtp(short4);
char4 __ovld __cnfn convert_char4_sat_rtp(short4);
char4 __ovld __cnfn convert_char4_rtn(short4);
char4 __ovld __cnfn convert_char4_sat_rtn(short4);
char4 __ovld __cnfn convert_char4(short4);
char4 __ovld __cnfn convert_char4_sat(short4);
char4 __ovld __cnfn convert_char4_rte(ushort4);
char4 __ovld __cnfn convert_char4_sat_rte(ushort4);
char4 __ovld __cnfn convert_char4_rtz(ushort4);
char4 __ovld __cnfn convert_char4_sat_rtz(ushort4);
char4 __ovld __cnfn convert_char4_rtp(ushort4);
char4 __ovld __cnfn convert_char4_sat_rtp(ushort4);
char4 __ovld __cnfn convert_char4_rtn(ushort4);
char4 __ovld __cnfn convert_char4_sat_rtn(ushort4);
char4 __ovld __cnfn convert_char4(ushort4);
char4 __ovld __cnfn convert_char4_sat(ushort4);
char4 __ovld __cnfn convert_char4_rte(int4);
char4 __ovld __cnfn convert_char4_sat_rte(int4);
char4 __ovld __cnfn convert_char4_rtz(int4);
char4 __ovld __cnfn convert_char4_sat_rtz(int4);
char4 __ovld __cnfn convert_char4_rtp(int4);
char4 __ovld __cnfn convert_char4_sat_rtp(int4);
char4 __ovld __cnfn convert_char4_rtn(int4);
char4 __ovld __cnfn convert_char4_sat_rtn(int4);
char4 __ovld __cnfn convert_char4(int4);
char4 __ovld __cnfn convert_char4_sat(int4);
char4 __ovld __cnfn convert_char4_rte(uint4);
char4 __ovld __cnfn convert_char4_sat_rte(uint4);
char4 __ovld __cnfn convert_char4_rtz(uint4);
char4 __ovld __cnfn convert_char4_sat_rtz(uint4);
char4 __ovld __cnfn convert_char4_rtp(uint4);
char4 __ovld __cnfn convert_char4_sat_rtp(uint4);
char4 __ovld __cnfn convert_char4_rtn(uint4);
char4 __ovld __cnfn convert_char4_sat_rtn(uint4);
char4 __ovld __cnfn convert_char4(uint4);
char4 __ovld __cnfn convert_char4_sat(uint4);
char4 __ovld __cnfn convert_char4_rte(long4);
char4 __ovld __cnfn convert_char4_sat_rte(long4);
char4 __ovld __cnfn convert_char4_rtz(long4);
char4 __ovld __cnfn convert_char4_sat_rtz(long4);
char4 __ovld __cnfn convert_char4_rtp(long4);
char4 __ovld __cnfn convert_char4_sat_rtp(long4);
char4 __ovld __cnfn convert_char4_rtn(long4);
char4 __ovld __cnfn convert_char4_sat_rtn(long4);
char4 __ovld __cnfn convert_char4(long4);
char4 __ovld __cnfn convert_char4_sat(long4);
char4 __ovld __cnfn convert_char4_rte(ulong4);
char4 __ovld __cnfn convert_char4_sat_rte(ulong4);
char4 __ovld __cnfn convert_char4_rtz(ulong4);
char4 __ovld __cnfn convert_char4_sat_rtz(ulong4);
char4 __ovld __cnfn convert_char4_rtp(ulong4);
char4 __ovld __cnfn convert_char4_sat_rtp(ulong4);
char4 __ovld __cnfn convert_char4_rtn(ulong4);
char4 __ovld __cnfn convert_char4_sat_rtn(ulong4);
char4 __ovld __cnfn convert_char4(ulong4);
char4 __ovld __cnfn convert_char4_sat(ulong4);
char4 __ovld __cnfn convert_char4_rte(float4);
char4 __ovld __cnfn convert_char4_sat_rte(float4);
char4 __ovld __cnfn convert_char4_rtz(float4);
char4 __ovld __cnfn convert_char4_sat_rtz(float4);
char4 __ovld __cnfn convert_char4_rtp(float4);
char4 __ovld __cnfn convert_char4_sat_rtp(float4);
char4 __ovld __cnfn convert_char4_rtn(float4);
char4 __ovld __cnfn convert_char4_sat_rtn(float4);
char4 __ovld __cnfn convert_char4(float4);
char4 __ovld __cnfn convert_char4_sat(float4);
uchar4 __ovld __cnfn convert_uchar4_rte(char4);
uchar4 __ovld __cnfn convert_uchar4_sat_rte(char4);
uchar4 __ovld __cnfn convert_uchar4_rtz(char4);
uchar4 __ovld __cnfn convert_uchar4_sat_rtz(char4);
uchar4 __ovld __cnfn convert_uchar4_rtp(char4);
uchar4 __ovld __cnfn convert_uchar4_sat_rtp(char4);
uchar4 __ovld __cnfn convert_uchar4_rtn(char4);
uchar4 __ovld __cnfn convert_uchar4_sat_rtn(char4);
uchar4 __ovld __cnfn convert_uchar4(char4);
uchar4 __ovld __cnfn convert_uchar4_sat(char4);
uchar4 __ovld __cnfn convert_uchar4_rte(uchar4);
uchar4 __ovld __cnfn convert_uchar4_sat_rte(uchar4);
uchar4 __ovld __cnfn convert_uchar4_rtz(uchar4);
uchar4 __ovld __cnfn convert_uchar4_sat_rtz(uchar4);
uchar4 __ovld __cnfn convert_uchar4_rtp(uchar4);
uchar4 __ovld __cnfn convert_uchar4_sat_rtp(uchar4);
uchar4 __ovld __cnfn convert_uchar4_rtn(uchar4);
uchar4 __ovld __cnfn convert_uchar4_sat_rtn(uchar4);
uchar4 __ovld __cnfn convert_uchar4(uchar4);
uchar4 __ovld __cnfn convert_uchar4_sat(uchar4);
uchar4 __ovld __cnfn convert_uchar4_rte(short4);
uchar4 __ovld __cnfn convert_uchar4_sat_rte(short4);
uchar4 __ovld __cnfn convert_uchar4_rtz(short4);
uchar4 __ovld __cnfn convert_uchar4_sat_rtz(short4);
uchar4 __ovld __cnfn convert_uchar4_rtp(short4);
uchar4 __ovld __cnfn convert_uchar4_sat_rtp(short4);
uchar4 __ovld __cnfn convert_uchar4_rtn(short4);
uchar4 __ovld __cnfn convert_uchar4_sat_rtn(short4);
uchar4 __ovld __cnfn convert_uchar4(short4);
uchar4 __ovld __cnfn convert_uchar4_sat(short4);
uchar4 __ovld __cnfn convert_uchar4_rte(ushort4);
uchar4 __ovld __cnfn convert_uchar4_sat_rte(ushort4);
uchar4 __ovld __cnfn convert_uchar4_rtz(ushort4);
uchar4 __ovld __cnfn convert_uchar4_sat_rtz(ushort4);
uchar4 __ovld __cnfn convert_uchar4_rtp(ushort4);
uchar4 __ovld __cnfn convert_uchar4_sat_rtp(ushort4);
uchar4 __ovld __cnfn convert_uchar4_rtn(ushort4);
uchar4 __ovld __cnfn convert_uchar4_sat_rtn(ushort4);
uchar4 __ovld __cnfn convert_uchar4(ushort4);
uchar4 __ovld __cnfn convert_uchar4_sat(ushort4);
uchar4 __ovld __cnfn convert_uchar4_rte(int4);
uchar4 __ovld __cnfn convert_uchar4_sat_rte(int4);
uchar4 __ovld __cnfn convert_uchar4_rtz(int4);
uchar4 __ovld __cnfn convert_uchar4_sat_rtz(int4);
uchar4 __ovld __cnfn convert_uchar4_rtp(int4);
uchar4 __ovld __cnfn convert_uchar4_sat_rtp(int4);
uchar4 __ovld __cnfn convert_uchar4_rtn(int4);
uchar4 __ovld __cnfn convert_uchar4_sat_rtn(int4);
uchar4 __ovld __cnfn convert_uchar4(int4);
uchar4 __ovld __cnfn convert_uchar4_sat(int4);
uchar4 __ovld __cnfn convert_uchar4_rte(uint4);
uchar4 __ovld __cnfn convert_uchar4_sat_rte(uint4);
uchar4 __ovld __cnfn convert_uchar4_rtz(uint4);
uchar4 __ovld __cnfn convert_uchar4_sat_rtz(uint4);
uchar4 __ovld __cnfn convert_uchar4_rtp(uint4);
uchar4 __ovld __cnfn convert_uchar4_sat_rtp(uint4);
uchar4 __ovld __cnfn convert_uchar4_rtn(uint4);
uchar4 __ovld __cnfn convert_uchar4_sat_rtn(uint4);
uchar4 __ovld __cnfn convert_uchar4(uint4);
uchar4 __ovld __cnfn convert_uchar4_sat(uint4);
uchar4 __ovld __cnfn convert_uchar4_rte(long4);
uchar4 __ovld __cnfn convert_uchar4_sat_rte(long4);
uchar4 __ovld __cnfn convert_uchar4_rtz(long4);
uchar4 __ovld __cnfn convert_uchar4_sat_rtz(long4);
uchar4 __ovld __cnfn convert_uchar4_rtp(long4);
uchar4 __ovld __cnfn convert_uchar4_sat_rtp(long4);
uchar4 __ovld __cnfn convert_uchar4_rtn(long4);
uchar4 __ovld __cnfn convert_uchar4_sat_rtn(long4);
uchar4 __ovld __cnfn convert_uchar4(long4);
uchar4 __ovld __cnfn convert_uchar4_sat(long4);
uchar4 __ovld __cnfn convert_uchar4_rte(ulong4);
uchar4 __ovld __cnfn convert_uchar4_sat_rte(ulong4);
uchar4 __ovld __cnfn convert_uchar4_rtz(ulong4);
uchar4 __ovld __cnfn convert_uchar4_sat_rtz(ulong4);
uchar4 __ovld __cnfn convert_uchar4_rtp(ulong4);
uchar4 __ovld __cnfn convert_uchar4_sat_rtp(ulong4);
uchar4 __ovld __cnfn convert_uchar4_rtn(ulong4);
uchar4 __ovld __cnfn convert_uchar4_sat_rtn(ulong4);
uchar4 __ovld __cnfn convert_uchar4(ulong4);
uchar4 __ovld __cnfn convert_uchar4_sat(ulong4);
uchar4 __ovld __cnfn convert_uchar4_rte(float4);
uchar4 __ovld __cnfn convert_uchar4_sat_rte(float4);
uchar4 __ovld __cnfn convert_uchar4_rtz(float4);
uchar4 __ovld __cnfn convert_uchar4_sat_rtz(float4);
uchar4 __ovld __cnfn convert_uchar4_rtp(float4);
uchar4 __ovld __cnfn convert_uchar4_sat_rtp(float4);
uchar4 __ovld __cnfn convert_uchar4_rtn(float4);
uchar4 __ovld __cnfn convert_uchar4_sat_rtn(float4);
uchar4 __ovld __cnfn convert_uchar4(float4);
uchar4 __ovld __cnfn convert_uchar4_sat(float4);
short4 __ovld __cnfn convert_short4_rte(char4);
short4 __ovld __cnfn convert_short4_sat_rte(char4);
short4 __ovld __cnfn convert_short4_rtz(char4);
short4 __ovld __cnfn convert_short4_sat_rtz(char4);
short4 __ovld __cnfn convert_short4_rtp(char4);
short4 __ovld __cnfn convert_short4_sat_rtp(char4);
short4 __ovld __cnfn convert_short4_rtn(char4);
short4 __ovld __cnfn convert_short4_sat_rtn(char4);
short4 __ovld __cnfn convert_short4(char4);
short4 __ovld __cnfn convert_short4_sat(char4);
short4 __ovld __cnfn convert_short4_rte(uchar4);
short4 __ovld __cnfn convert_short4_sat_rte(uchar4);
short4 __ovld __cnfn convert_short4_rtz(uchar4);
short4 __ovld __cnfn convert_short4_sat_rtz(uchar4);
short4 __ovld __cnfn convert_short4_rtp(uchar4);
short4 __ovld __cnfn convert_short4_sat_rtp(uchar4);
short4 __ovld __cnfn convert_short4_rtn(uchar4);
short4 __ovld __cnfn convert_short4_sat_rtn(uchar4);
short4 __ovld __cnfn convert_short4(uchar4);
short4 __ovld __cnfn convert_short4_sat(uchar4);
short4 __ovld __cnfn convert_short4_rte(short4);
short4 __ovld __cnfn convert_short4_sat_rte(short4);
short4 __ovld __cnfn convert_short4_rtz(short4);
short4 __ovld __cnfn convert_short4_sat_rtz(short4);
short4 __ovld __cnfn convert_short4_rtp(short4);
short4 __ovld __cnfn convert_short4_sat_rtp(short4);
short4 __ovld __cnfn convert_short4_rtn(short4);
short4 __ovld __cnfn convert_short4_sat_rtn(short4);
short4 __ovld __cnfn convert_short4(short4);
short4 __ovld __cnfn convert_short4_sat(short4);
short4 __ovld __cnfn convert_short4_rte(ushort4);
short4 __ovld __cnfn convert_short4_sat_rte(ushort4);
short4 __ovld __cnfn convert_short4_rtz(ushort4);
short4 __ovld __cnfn convert_short4_sat_rtz(ushort4);
short4 __ovld __cnfn convert_short4_rtp(ushort4);
short4 __ovld __cnfn convert_short4_sat_rtp(ushort4);
short4 __ovld __cnfn convert_short4_rtn(ushort4);
short4 __ovld __cnfn convert_short4_sat_rtn(ushort4);
short4 __ovld __cnfn convert_short4(ushort4);
short4 __ovld __cnfn convert_short4_sat(ushort4);
short4 __ovld __cnfn convert_short4_rte(int4);
short4 __ovld __cnfn convert_short4_sat_rte(int4);
short4 __ovld __cnfn convert_short4_rtz(int4);
short4 __ovld __cnfn convert_short4_sat_rtz(int4);
short4 __ovld __cnfn convert_short4_rtp(int4);
short4 __ovld __cnfn convert_short4_sat_rtp(int4);
short4 __ovld __cnfn convert_short4_rtn(int4);
short4 __ovld __cnfn convert_short4_sat_rtn(int4);
short4 __ovld __cnfn convert_short4(int4);
short4 __ovld __cnfn convert_short4_sat(int4);
short4 __ovld __cnfn convert_short4_rte(uint4);
short4 __ovld __cnfn convert_short4_sat_rte(uint4);
short4 __ovld __cnfn convert_short4_rtz(uint4);
short4 __ovld __cnfn convert_short4_sat_rtz(uint4);
short4 __ovld __cnfn convert_short4_rtp(uint4);
short4 __ovld __cnfn convert_short4_sat_rtp(uint4);
short4 __ovld __cnfn convert_short4_rtn(uint4);
short4 __ovld __cnfn convert_short4_sat_rtn(uint4);
short4 __ovld __cnfn convert_short4(uint4);
short4 __ovld __cnfn convert_short4_sat(uint4);
short4 __ovld __cnfn convert_short4_rte(long4);
short4 __ovld __cnfn convert_short4_sat_rte(long4);
short4 __ovld __cnfn convert_short4_rtz(long4);
short4 __ovld __cnfn convert_short4_sat_rtz(long4);
short4 __ovld __cnfn convert_short4_rtp(long4);
short4 __ovld __cnfn convert_short4_sat_rtp(long4);
short4 __ovld __cnfn convert_short4_rtn(long4);
short4 __ovld __cnfn convert_short4_sat_rtn(long4);
short4 __ovld __cnfn convert_short4(long4);
short4 __ovld __cnfn convert_short4_sat(long4);
short4 __ovld __cnfn convert_short4_rte(ulong4);
short4 __ovld __cnfn convert_short4_sat_rte(ulong4);
short4 __ovld __cnfn convert_short4_rtz(ulong4);
short4 __ovld __cnfn convert_short4_sat_rtz(ulong4);
short4 __ovld __cnfn convert_short4_rtp(ulong4);
short4 __ovld __cnfn convert_short4_sat_rtp(ulong4);
short4 __ovld __cnfn convert_short4_rtn(ulong4);
short4 __ovld __cnfn convert_short4_sat_rtn(ulong4);
short4 __ovld __cnfn convert_short4(ulong4);
short4 __ovld __cnfn convert_short4_sat(ulong4);
short4 __ovld __cnfn convert_short4_rte(float4);
short4 __ovld __cnfn convert_short4_sat_rte(float4);
short4 __ovld __cnfn convert_short4_rtz(float4);
short4 __ovld __cnfn convert_short4_sat_rtz(float4);
short4 __ovld __cnfn convert_short4_rtp(float4);
short4 __ovld __cnfn convert_short4_sat_rtp(float4);
short4 __ovld __cnfn convert_short4_rtn(float4);
short4 __ovld __cnfn convert_short4_sat_rtn(float4);
short4 __ovld __cnfn convert_short4(float4);
short4 __ovld __cnfn convert_short4_sat(float4);
ushort4 __ovld __cnfn convert_ushort4_rte(char4);
ushort4 __ovld __cnfn convert_ushort4_sat_rte(char4);
ushort4 __ovld __cnfn convert_ushort4_rtz(char4);
ushort4 __ovld __cnfn convert_ushort4_sat_rtz(char4);
ushort4 __ovld __cnfn convert_ushort4_rtp(char4);
ushort4 __ovld __cnfn convert_ushort4_sat_rtp(char4);
ushort4 __ovld __cnfn convert_ushort4_rtn(char4);
ushort4 __ovld __cnfn convert_ushort4_sat_rtn(char4);
ushort4 __ovld __cnfn convert_ushort4(char4);
ushort4 __ovld __cnfn convert_ushort4_sat(char4);
ushort4 __ovld __cnfn convert_ushort4_rte(uchar4);
ushort4 __ovld __cnfn convert_ushort4_sat_rte(uchar4);
ushort4 __ovld __cnfn convert_ushort4_rtz(uchar4);
ushort4 __ovld __cnfn convert_ushort4_sat_rtz(uchar4);
ushort4 __ovld __cnfn convert_ushort4_rtp(uchar4);
ushort4 __ovld __cnfn convert_ushort4_sat_rtp(uchar4);
ushort4 __ovld __cnfn convert_ushort4_rtn(uchar4);
ushort4 __ovld __cnfn convert_ushort4_sat_rtn(uchar4);
ushort4 __ovld __cnfn convert_ushort4(uchar4);
ushort4 __ovld __cnfn convert_ushort4_sat(uchar4);
ushort4 __ovld __cnfn convert_ushort4_rte(short4);
ushort4 __ovld __cnfn convert_ushort4_sat_rte(short4);
ushort4 __ovld __cnfn convert_ushort4_rtz(short4);
ushort4 __ovld __cnfn convert_ushort4_sat_rtz(short4);
ushort4 __ovld __cnfn convert_ushort4_rtp(short4);
ushort4 __ovld __cnfn convert_ushort4_sat_rtp(short4);
ushort4 __ovld __cnfn convert_ushort4_rtn(short4);
ushort4 __ovld __cnfn convert_ushort4_sat_rtn(short4);
ushort4 __ovld __cnfn convert_ushort4(short4);
ushort4 __ovld __cnfn convert_ushort4_sat(short4);
ushort4 __ovld __cnfn convert_ushort4_rte(ushort4);
ushort4 __ovld __cnfn convert_ushort4_sat_rte(ushort4);
ushort4 __ovld __cnfn convert_ushort4_rtz(ushort4);
ushort4 __ovld __cnfn convert_ushort4_sat_rtz(ushort4);
ushort4 __ovld __cnfn convert_ushort4_rtp(ushort4);
ushort4 __ovld __cnfn convert_ushort4_sat_rtp(ushort4);
ushort4 __ovld __cnfn convert_ushort4_rtn(ushort4);
ushort4 __ovld __cnfn convert_ushort4_sat_rtn(ushort4);
ushort4 __ovld __cnfn convert_ushort4(ushort4);
ushort4 __ovld __cnfn convert_ushort4_sat(ushort4);
ushort4 __ovld __cnfn convert_ushort4_rte(int4);
ushort4 __ovld __cnfn convert_ushort4_sat_rte(int4);
ushort4 __ovld __cnfn convert_ushort4_rtz(int4);
ushort4 __ovld __cnfn convert_ushort4_sat_rtz(int4);
ushort4 __ovld __cnfn convert_ushort4_rtp(int4);
ushort4 __ovld __cnfn convert_ushort4_sat_rtp(int4);
ushort4 __ovld __cnfn convert_ushort4_rtn(int4);
ushort4 __ovld __cnfn convert_ushort4_sat_rtn(int4);
ushort4 __ovld __cnfn convert_ushort4(int4);
ushort4 __ovld __cnfn convert_ushort4_sat(int4);
ushort4 __ovld __cnfn convert_ushort4_rte(uint4);
ushort4 __ovld __cnfn convert_ushort4_sat_rte(uint4);
ushort4 __ovld __cnfn convert_ushort4_rtz(uint4);
ushort4 __ovld __cnfn convert_ushort4_sat_rtz(uint4);
ushort4 __ovld __cnfn convert_ushort4_rtp(uint4);
ushort4 __ovld __cnfn convert_ushort4_sat_rtp(uint4);
ushort4 __ovld __cnfn convert_ushort4_rtn(uint4);
ushort4 __ovld __cnfn convert_ushort4_sat_rtn(uint4);
ushort4 __ovld __cnfn convert_ushort4(uint4);
ushort4 __ovld __cnfn convert_ushort4_sat(uint4);
ushort4 __ovld __cnfn convert_ushort4_rte(long4);
ushort4 __ovld __cnfn convert_ushort4_sat_rte(long4);
ushort4 __ovld __cnfn convert_ushort4_rtz(long4);
ushort4 __ovld __cnfn convert_ushort4_sat_rtz(long4);
ushort4 __ovld __cnfn convert_ushort4_rtp(long4);
ushort4 __ovld __cnfn convert_ushort4_sat_rtp(long4);
ushort4 __ovld __cnfn convert_ushort4_rtn(long4);
ushort4 __ovld __cnfn convert_ushort4_sat_rtn(long4);
ushort4 __ovld __cnfn convert_ushort4(long4);
ushort4 __ovld __cnfn convert_ushort4_sat(long4);
ushort4 __ovld __cnfn convert_ushort4_rte(ulong4);
ushort4 __ovld __cnfn convert_ushort4_sat_rte(ulong4);
ushort4 __ovld __cnfn convert_ushort4_rtz(ulong4);
ushort4 __ovld __cnfn convert_ushort4_sat_rtz(ulong4);
ushort4 __ovld __cnfn convert_ushort4_rtp(ulong4);
ushort4 __ovld __cnfn convert_ushort4_sat_rtp(ulong4);
ushort4 __ovld __cnfn convert_ushort4_rtn(ulong4);
ushort4 __ovld __cnfn convert_ushort4_sat_rtn(ulong4);
ushort4 __ovld __cnfn convert_ushort4(ulong4);
ushort4 __ovld __cnfn convert_ushort4_sat(ulong4);
ushort4 __ovld __cnfn convert_ushort4_rte(float4);
ushort4 __ovld __cnfn convert_ushort4_sat_rte(float4);
ushort4 __ovld __cnfn convert_ushort4_rtz(float4);
ushort4 __ovld __cnfn convert_ushort4_sat_rtz(float4);
ushort4 __ovld __cnfn convert_ushort4_rtp(float4);
ushort4 __ovld __cnfn convert_ushort4_sat_rtp(float4);
ushort4 __ovld __cnfn convert_ushort4_rtn(float4);
ushort4 __ovld __cnfn convert_ushort4_sat_rtn(float4);
ushort4 __ovld __cnfn convert_ushort4(float4);
ushort4 __ovld __cnfn convert_ushort4_sat(float4);
int4 __ovld __cnfn convert_int4_rte(char4);
int4 __ovld __cnfn convert_int4_sat_rte(char4);
int4 __ovld __cnfn convert_int4_rtz(char4);
int4 __ovld __cnfn convert_int4_sat_rtz(char4);
int4 __ovld __cnfn convert_int4_rtp(char4);
int4 __ovld __cnfn convert_int4_sat_rtp(char4);
int4 __ovld __cnfn convert_int4_rtn(char4);
int4 __ovld __cnfn convert_int4_sat_rtn(char4);
int4 __ovld __cnfn convert_int4(char4);
int4 __ovld __cnfn convert_int4_sat(char4);
int4 __ovld __cnfn convert_int4_rte(uchar4);
int4 __ovld __cnfn convert_int4_sat_rte(uchar4);
int4 __ovld __cnfn convert_int4_rtz(uchar4);
int4 __ovld __cnfn convert_int4_sat_rtz(uchar4);
int4 __ovld __cnfn convert_int4_rtp(uchar4);
int4 __ovld __cnfn convert_int4_sat_rtp(uchar4);
int4 __ovld __cnfn convert_int4_rtn(uchar4);
int4 __ovld __cnfn convert_int4_sat_rtn(uchar4);
int4 __ovld __cnfn convert_int4(uchar4);
int4 __ovld __cnfn convert_int4_sat(uchar4);
int4 __ovld __cnfn convert_int4_rte(short4);
int4 __ovld __cnfn convert_int4_sat_rte(short4);
int4 __ovld __cnfn convert_int4_rtz(short4);
int4 __ovld __cnfn convert_int4_sat_rtz(short4);
int4 __ovld __cnfn convert_int4_rtp(short4);
int4 __ovld __cnfn convert_int4_sat_rtp(short4);
int4 __ovld __cnfn convert_int4_rtn(short4);
int4 __ovld __cnfn convert_int4_sat_rtn(short4);
int4 __ovld __cnfn convert_int4(short4);
int4 __ovld __cnfn convert_int4_sat(short4);
int4 __ovld __cnfn convert_int4_rte(ushort4);
int4 __ovld __cnfn convert_int4_sat_rte(ushort4);
int4 __ovld __cnfn convert_int4_rtz(ushort4);
int4 __ovld __cnfn convert_int4_sat_rtz(ushort4);
int4 __ovld __cnfn convert_int4_rtp(ushort4);
int4 __ovld __cnfn convert_int4_sat_rtp(ushort4);
int4 __ovld __cnfn convert_int4_rtn(ushort4);
int4 __ovld __cnfn convert_int4_sat_rtn(ushort4);
int4 __ovld __cnfn convert_int4(ushort4);
int4 __ovld __cnfn convert_int4_sat(ushort4);
int4 __ovld __cnfn convert_int4_rte(int4);
int4 __ovld __cnfn convert_int4_sat_rte(int4);
int4 __ovld __cnfn convert_int4_rtz(int4);
int4 __ovld __cnfn convert_int4_sat_rtz(int4);
int4 __ovld __cnfn convert_int4_rtp(int4);
int4 __ovld __cnfn convert_int4_sat_rtp(int4);
int4 __ovld __cnfn convert_int4_rtn(int4);
int4 __ovld __cnfn convert_int4_sat_rtn(int4);
int4 __ovld __cnfn convert_int4(int4);
int4 __ovld __cnfn convert_int4_sat(int4);
int4 __ovld __cnfn convert_int4_rte(uint4);
int4 __ovld __cnfn convert_int4_sat_rte(uint4);
int4 __ovld __cnfn convert_int4_rtz(uint4);
int4 __ovld __cnfn convert_int4_sat_rtz(uint4);
int4 __ovld __cnfn convert_int4_rtp(uint4);
int4 __ovld __cnfn convert_int4_sat_rtp(uint4);
int4 __ovld __cnfn convert_int4_rtn(uint4);
int4 __ovld __cnfn convert_int4_sat_rtn(uint4);
int4 __ovld __cnfn convert_int4(uint4);
int4 __ovld __cnfn convert_int4_sat(uint4);
int4 __ovld __cnfn convert_int4_rte(long4);
int4 __ovld __cnfn convert_int4_sat_rte(long4);
int4 __ovld __cnfn convert_int4_rtz(long4);
int4 __ovld __cnfn convert_int4_sat_rtz(long4);
int4 __ovld __cnfn convert_int4_rtp(long4);
int4 __ovld __cnfn convert_int4_sat_rtp(long4);
int4 __ovld __cnfn convert_int4_rtn(long4);
int4 __ovld __cnfn convert_int4_sat_rtn(long4);
int4 __ovld __cnfn convert_int4(long4);
int4 __ovld __cnfn convert_int4_sat(long4);
int4 __ovld __cnfn convert_int4_rte(ulong4);
int4 __ovld __cnfn convert_int4_sat_rte(ulong4);
int4 __ovld __cnfn convert_int4_rtz(ulong4);
int4 __ovld __cnfn convert_int4_sat_rtz(ulong4);
int4 __ovld __cnfn convert_int4_rtp(ulong4);
int4 __ovld __cnfn convert_int4_sat_rtp(ulong4);
int4 __ovld __cnfn convert_int4_rtn(ulong4);
int4 __ovld __cnfn convert_int4_sat_rtn(ulong4);
int4 __ovld __cnfn convert_int4(ulong4);
int4 __ovld __cnfn convert_int4_sat(ulong4);
int4 __ovld __cnfn convert_int4_rte(float4);
int4 __ovld __cnfn convert_int4_sat_rte(float4);
int4 __ovld __cnfn convert_int4_rtz(float4);
int4 __ovld __cnfn convert_int4_sat_rtz(float4);
int4 __ovld __cnfn convert_int4_rtp(float4);
int4 __ovld __cnfn convert_int4_sat_rtp(float4);
int4 __ovld __cnfn convert_int4_rtn(float4);
int4 __ovld __cnfn convert_int4_sat_rtn(float4);
int4 __ovld __cnfn convert_int4(float4);
int4 __ovld __cnfn convert_int4_sat(float4);
uint4 __ovld __cnfn convert_uint4_rte(char4);
uint4 __ovld __cnfn convert_uint4_sat_rte(char4);
uint4 __ovld __cnfn convert_uint4_rtz(char4);
uint4 __ovld __cnfn convert_uint4_sat_rtz(char4);
uint4 __ovld __cnfn convert_uint4_rtp(char4);
uint4 __ovld __cnfn convert_uint4_sat_rtp(char4);
uint4 __ovld __cnfn convert_uint4_rtn(char4);
uint4 __ovld __cnfn convert_uint4_sat_rtn(char4);
uint4 __ovld __cnfn convert_uint4(char4);
uint4 __ovld __cnfn convert_uint4_sat(char4);
uint4 __ovld __cnfn convert_uint4_rte(uchar4);
uint4 __ovld __cnfn convert_uint4_sat_rte(uchar4);
uint4 __ovld __cnfn convert_uint4_rtz(uchar4);
uint4 __ovld __cnfn convert_uint4_sat_rtz(uchar4);
uint4 __ovld __cnfn convert_uint4_rtp(uchar4);
uint4 __ovld __cnfn convert_uint4_sat_rtp(uchar4);
uint4 __ovld __cnfn convert_uint4_rtn(uchar4);
uint4 __ovld __cnfn convert_uint4_sat_rtn(uchar4);
uint4 __ovld __cnfn convert_uint4(uchar4);
uint4 __ovld __cnfn convert_uint4_sat(uchar4);
uint4 __ovld __cnfn convert_uint4_rte(short4);
uint4 __ovld __cnfn convert_uint4_sat_rte(short4);
uint4 __ovld __cnfn convert_uint4_rtz(short4);
uint4 __ovld __cnfn convert_uint4_sat_rtz(short4);
uint4 __ovld __cnfn convert_uint4_rtp(short4);
uint4 __ovld __cnfn convert_uint4_sat_rtp(short4);
uint4 __ovld __cnfn convert_uint4_rtn(short4);
uint4 __ovld __cnfn convert_uint4_sat_rtn(short4);
uint4 __ovld __cnfn convert_uint4(short4);
uint4 __ovld __cnfn convert_uint4_sat(short4);
uint4 __ovld __cnfn convert_uint4_rte(ushort4);
uint4 __ovld __cnfn convert_uint4_sat_rte(ushort4);
uint4 __ovld __cnfn convert_uint4_rtz(ushort4);
uint4 __ovld __cnfn convert_uint4_sat_rtz(ushort4);
uint4 __ovld __cnfn convert_uint4_rtp(ushort4);
uint4 __ovld __cnfn convert_uint4_sat_rtp(ushort4);
uint4 __ovld __cnfn convert_uint4_rtn(ushort4);
uint4 __ovld __cnfn convert_uint4_sat_rtn(ushort4);
uint4 __ovld __cnfn convert_uint4(ushort4);
uint4 __ovld __cnfn convert_uint4_sat(ushort4);
uint4 __ovld __cnfn convert_uint4_rte(int4);
uint4 __ovld __cnfn convert_uint4_sat_rte(int4);
uint4 __ovld __cnfn convert_uint4_rtz(int4);
uint4 __ovld __cnfn convert_uint4_sat_rtz(int4);
uint4 __ovld __cnfn convert_uint4_rtp(int4);
uint4 __ovld __cnfn convert_uint4_sat_rtp(int4);
uint4 __ovld __cnfn convert_uint4_rtn(int4);
uint4 __ovld __cnfn convert_uint4_sat_rtn(int4);
uint4 __ovld __cnfn convert_uint4(int4);
uint4 __ovld __cnfn convert_uint4_sat(int4);
uint4 __ovld __cnfn convert_uint4_rte(uint4);
uint4 __ovld __cnfn convert_uint4_sat_rte(uint4);
uint4 __ovld __cnfn convert_uint4_rtz(uint4);
uint4 __ovld __cnfn convert_uint4_sat_rtz(uint4);
uint4 __ovld __cnfn convert_uint4_rtp(uint4);
uint4 __ovld __cnfn convert_uint4_sat_rtp(uint4);
uint4 __ovld __cnfn convert_uint4_rtn(uint4);
uint4 __ovld __cnfn convert_uint4_sat_rtn(uint4);
uint4 __ovld __cnfn convert_uint4(uint4);
uint4 __ovld __cnfn convert_uint4_sat(uint4);
uint4 __ovld __cnfn convert_uint4_rte(long4);
uint4 __ovld __cnfn convert_uint4_sat_rte(long4);
uint4 __ovld __cnfn convert_uint4_rtz(long4);
uint4 __ovld __cnfn convert_uint4_sat_rtz(long4);
uint4 __ovld __cnfn convert_uint4_rtp(long4);
uint4 __ovld __cnfn convert_uint4_sat_rtp(long4);
uint4 __ovld __cnfn convert_uint4_rtn(long4);
uint4 __ovld __cnfn convert_uint4_sat_rtn(long4);
uint4 __ovld __cnfn convert_uint4(long4);
uint4 __ovld __cnfn convert_uint4_sat(long4);
uint4 __ovld __cnfn convert_uint4_rte(ulong4);
uint4 __ovld __cnfn convert_uint4_sat_rte(ulong4);
uint4 __ovld __cnfn convert_uint4_rtz(ulong4);
uint4 __ovld __cnfn convert_uint4_sat_rtz(ulong4);
uint4 __ovld __cnfn convert_uint4_rtp(ulong4);
uint4 __ovld __cnfn convert_uint4_sat_rtp(ulong4);
uint4 __ovld __cnfn convert_uint4_rtn(ulong4);
uint4 __ovld __cnfn convert_uint4_sat_rtn(ulong4);
uint4 __ovld __cnfn convert_uint4(ulong4);
uint4 __ovld __cnfn convert_uint4_sat(ulong4);
uint4 __ovld __cnfn convert_uint4_rte(float4);
uint4 __ovld __cnfn convert_uint4_sat_rte(float4);
uint4 __ovld __cnfn convert_uint4_rtz(float4);
uint4 __ovld __cnfn convert_uint4_sat_rtz(float4);
uint4 __ovld __cnfn convert_uint4_rtp(float4);
uint4 __ovld __cnfn convert_uint4_sat_rtp(float4);
uint4 __ovld __cnfn convert_uint4_rtn(float4);
uint4 __ovld __cnfn convert_uint4_sat_rtn(float4);
uint4 __ovld __cnfn convert_uint4(float4);
uint4 __ovld __cnfn convert_uint4_sat(float4);
long4 __ovld __cnfn convert_long4_rte(char4);
long4 __ovld __cnfn convert_long4_sat_rte(char4);
long4 __ovld __cnfn convert_long4_rtz(char4);
long4 __ovld __cnfn convert_long4_sat_rtz(char4);
long4 __ovld __cnfn convert_long4_rtp(char4);
long4 __ovld __cnfn convert_long4_sat_rtp(char4);
long4 __ovld __cnfn convert_long4_rtn(char4);
long4 __ovld __cnfn convert_long4_sat_rtn(char4);
long4 __ovld __cnfn convert_long4(char4);
long4 __ovld __cnfn convert_long4_sat(char4);
long4 __ovld __cnfn convert_long4_rte(uchar4);
long4 __ovld __cnfn convert_long4_sat_rte(uchar4);
long4 __ovld __cnfn convert_long4_rtz(uchar4);
long4 __ovld __cnfn convert_long4_sat_rtz(uchar4);
long4 __ovld __cnfn convert_long4_rtp(uchar4);
long4 __ovld __cnfn convert_long4_sat_rtp(uchar4);
long4 __ovld __cnfn convert_long4_rtn(uchar4);
long4 __ovld __cnfn convert_long4_sat_rtn(uchar4);
long4 __ovld __cnfn convert_long4(uchar4);
long4 __ovld __cnfn convert_long4_sat(uchar4);
long4 __ovld __cnfn convert_long4_rte(short4);
long4 __ovld __cnfn convert_long4_sat_rte(short4);
long4 __ovld __cnfn convert_long4_rtz(short4);
long4 __ovld __cnfn convert_long4_sat_rtz(short4);
long4 __ovld __cnfn convert_long4_rtp(short4);
long4 __ovld __cnfn convert_long4_sat_rtp(short4);
long4 __ovld __cnfn convert_long4_rtn(short4);
long4 __ovld __cnfn convert_long4_sat_rtn(short4);
long4 __ovld __cnfn convert_long4(short4);
long4 __ovld __cnfn convert_long4_sat(short4);
long4 __ovld __cnfn convert_long4_rte(ushort4);
long4 __ovld __cnfn convert_long4_sat_rte(ushort4);
long4 __ovld __cnfn convert_long4_rtz(ushort4);
long4 __ovld __cnfn convert_long4_sat_rtz(ushort4);
long4 __ovld __cnfn convert_long4_rtp(ushort4);
long4 __ovld __cnfn convert_long4_sat_rtp(ushort4);
long4 __ovld __cnfn convert_long4_rtn(ushort4);
long4 __ovld __cnfn convert_long4_sat_rtn(ushort4);
long4 __ovld __cnfn convert_long4(ushort4);
long4 __ovld __cnfn convert_long4_sat(ushort4);
long4 __ovld __cnfn convert_long4_rte(int4);
long4 __ovld __cnfn convert_long4_sat_rte(int4);
long4 __ovld __cnfn convert_long4_rtz(int4);
long4 __ovld __cnfn convert_long4_sat_rtz(int4);
long4 __ovld __cnfn convert_long4_rtp(int4);
long4 __ovld __cnfn convert_long4_sat_rtp(int4);
long4 __ovld __cnfn convert_long4_rtn(int4);
long4 __ovld __cnfn convert_long4_sat_rtn(int4);
long4 __ovld __cnfn convert_long4(int4);
long4 __ovld __cnfn convert_long4_sat(int4);
long4 __ovld __cnfn convert_long4_rte(uint4);
long4 __ovld __cnfn convert_long4_sat_rte(uint4);
long4 __ovld __cnfn convert_long4_rtz(uint4);
long4 __ovld __cnfn convert_long4_sat_rtz(uint4);
long4 __ovld __cnfn convert_long4_rtp(uint4);
long4 __ovld __cnfn convert_long4_sat_rtp(uint4);
long4 __ovld __cnfn convert_long4_rtn(uint4);
long4 __ovld __cnfn convert_long4_sat_rtn(uint4);
long4 __ovld __cnfn convert_long4(uint4);
long4 __ovld __cnfn convert_long4_sat(uint4);
long4 __ovld __cnfn convert_long4_rte(long4);
long4 __ovld __cnfn convert_long4_sat_rte(long4);
long4 __ovld __cnfn convert_long4_rtz(long4);
long4 __ovld __cnfn convert_long4_sat_rtz(long4);
long4 __ovld __cnfn convert_long4_rtp(long4);
long4 __ovld __cnfn convert_long4_sat_rtp(long4);
long4 __ovld __cnfn convert_long4_rtn(long4);
long4 __ovld __cnfn convert_long4_sat_rtn(long4);
long4 __ovld __cnfn convert_long4(long4);
long4 __ovld __cnfn convert_long4_sat(long4);
long4 __ovld __cnfn convert_long4_rte(ulong4);
long4 __ovld __cnfn convert_long4_sat_rte(ulong4);
long4 __ovld __cnfn convert_long4_rtz(ulong4);
long4 __ovld __cnfn convert_long4_sat_rtz(ulong4);
long4 __ovld __cnfn convert_long4_rtp(ulong4);
long4 __ovld __cnfn convert_long4_sat_rtp(ulong4);
long4 __ovld __cnfn convert_long4_rtn(ulong4);
long4 __ovld __cnfn convert_long4_sat_rtn(ulong4);
long4 __ovld __cnfn convert_long4(ulong4);
long4 __ovld __cnfn convert_long4_sat(ulong4);
long4 __ovld __cnfn convert_long4_rte(float4);
long4 __ovld __cnfn convert_long4_sat_rte(float4);
long4 __ovld __cnfn convert_long4_rtz(float4);
long4 __ovld __cnfn convert_long4_sat_rtz(float4);
long4 __ovld __cnfn convert_long4_rtp(float4);
long4 __ovld __cnfn convert_long4_sat_rtp(float4);
long4 __ovld __cnfn convert_long4_rtn(float4);
long4 __ovld __cnfn convert_long4_sat_rtn(float4);
long4 __ovld __cnfn convert_long4(float4);
long4 __ovld __cnfn convert_long4_sat(float4);
ulong4 __ovld __cnfn convert_ulong4_rte(char4);
ulong4 __ovld __cnfn convert_ulong4_sat_rte(char4);
ulong4 __ovld __cnfn convert_ulong4_rtz(char4);
ulong4 __ovld __cnfn convert_ulong4_sat_rtz(char4);
ulong4 __ovld __cnfn convert_ulong4_rtp(char4);
ulong4 __ovld __cnfn convert_ulong4_sat_rtp(char4);
ulong4 __ovld __cnfn convert_ulong4_rtn(char4);
ulong4 __ovld __cnfn convert_ulong4_sat_rtn(char4);
ulong4 __ovld __cnfn convert_ulong4(char4);
ulong4 __ovld __cnfn convert_ulong4_sat(char4);
ulong4 __ovld __cnfn convert_ulong4_rte(uchar4);
ulong4 __ovld __cnfn convert_ulong4_sat_rte(uchar4);
ulong4 __ovld __cnfn convert_ulong4_rtz(uchar4);
ulong4 __ovld __cnfn convert_ulong4_sat_rtz(uchar4);
ulong4 __ovld __cnfn convert_ulong4_rtp(uchar4);
ulong4 __ovld __cnfn convert_ulong4_sat_rtp(uchar4);
ulong4 __ovld __cnfn convert_ulong4_rtn(uchar4);
ulong4 __ovld __cnfn convert_ulong4_sat_rtn(uchar4);
ulong4 __ovld __cnfn convert_ulong4(uchar4);
ulong4 __ovld __cnfn convert_ulong4_sat(uchar4);
ulong4 __ovld __cnfn convert_ulong4_rte(short4);
ulong4 __ovld __cnfn convert_ulong4_sat_rte(short4);
ulong4 __ovld __cnfn convert_ulong4_rtz(short4);
ulong4 __ovld __cnfn convert_ulong4_sat_rtz(short4);
ulong4 __ovld __cnfn convert_ulong4_rtp(short4);
ulong4 __ovld __cnfn convert_ulong4_sat_rtp(short4);
ulong4 __ovld __cnfn convert_ulong4_rtn(short4);
ulong4 __ovld __cnfn convert_ulong4_sat_rtn(short4);
ulong4 __ovld __cnfn convert_ulong4(short4);
ulong4 __ovld __cnfn convert_ulong4_sat(short4);
ulong4 __ovld __cnfn convert_ulong4_rte(ushort4);
ulong4 __ovld __cnfn convert_ulong4_sat_rte(ushort4);
ulong4 __ovld __cnfn convert_ulong4_rtz(ushort4);
ulong4 __ovld __cnfn convert_ulong4_sat_rtz(ushort4);
ulong4 __ovld __cnfn convert_ulong4_rtp(ushort4);
ulong4 __ovld __cnfn convert_ulong4_sat_rtp(ushort4);
ulong4 __ovld __cnfn convert_ulong4_rtn(ushort4);
ulong4 __ovld __cnfn convert_ulong4_sat_rtn(ushort4);
ulong4 __ovld __cnfn convert_ulong4(ushort4);
ulong4 __ovld __cnfn convert_ulong4_sat(ushort4);
ulong4 __ovld __cnfn convert_ulong4_rte(int4);
ulong4 __ovld __cnfn convert_ulong4_sat_rte(int4);
ulong4 __ovld __cnfn convert_ulong4_rtz(int4);
ulong4 __ovld __cnfn convert_ulong4_sat_rtz(int4);
ulong4 __ovld __cnfn convert_ulong4_rtp(int4);
ulong4 __ovld __cnfn convert_ulong4_sat_rtp(int4);
ulong4 __ovld __cnfn convert_ulong4_rtn(int4);
ulong4 __ovld __cnfn convert_ulong4_sat_rtn(int4);
ulong4 __ovld __cnfn convert_ulong4(int4);
ulong4 __ovld __cnfn convert_ulong4_sat(int4);
ulong4 __ovld __cnfn convert_ulong4_rte(uint4);
ulong4 __ovld __cnfn convert_ulong4_sat_rte(uint4);
ulong4 __ovld __cnfn convert_ulong4_rtz(uint4);
ulong4 __ovld __cnfn convert_ulong4_sat_rtz(uint4);
ulong4 __ovld __cnfn convert_ulong4_rtp(uint4);
ulong4 __ovld __cnfn convert_ulong4_sat_rtp(uint4);
ulong4 __ovld __cnfn convert_ulong4_rtn(uint4);
ulong4 __ovld __cnfn convert_ulong4_sat_rtn(uint4);
ulong4 __ovld __cnfn convert_ulong4(uint4);
ulong4 __ovld __cnfn convert_ulong4_sat(uint4);
ulong4 __ovld __cnfn convert_ulong4_rte(long4);
ulong4 __ovld __cnfn convert_ulong4_sat_rte(long4);
ulong4 __ovld __cnfn convert_ulong4_rtz(long4);
ulong4 __ovld __cnfn convert_ulong4_sat_rtz(long4);
ulong4 __ovld __cnfn convert_ulong4_rtp(long4);
ulong4 __ovld __cnfn convert_ulong4_sat_rtp(long4);
ulong4 __ovld __cnfn convert_ulong4_rtn(long4);
ulong4 __ovld __cnfn convert_ulong4_sat_rtn(long4);
ulong4 __ovld __cnfn convert_ulong4(long4);
ulong4 __ovld __cnfn convert_ulong4_sat(long4);
ulong4 __ovld __cnfn convert_ulong4_rte(ulong4);
ulong4 __ovld __cnfn convert_ulong4_sat_rte(ulong4);
ulong4 __ovld __cnfn convert_ulong4_rtz(ulong4);
ulong4 __ovld __cnfn convert_ulong4_sat_rtz(ulong4);
ulong4 __ovld __cnfn convert_ulong4_rtp(ulong4);
ulong4 __ovld __cnfn convert_ulong4_sat_rtp(ulong4);
ulong4 __ovld __cnfn convert_ulong4_rtn(ulong4);
ulong4 __ovld __cnfn convert_ulong4_sat_rtn(ulong4);
ulong4 __ovld __cnfn convert_ulong4(ulong4);
ulong4 __ovld __cnfn convert_ulong4_sat(ulong4);
ulong4 __ovld __cnfn convert_ulong4_rte(float4);
ulong4 __ovld __cnfn convert_ulong4_sat_rte(float4);
ulong4 __ovld __cnfn convert_ulong4_rtz(float4);
ulong4 __ovld __cnfn convert_ulong4_sat_rtz(float4);
ulong4 __ovld __cnfn convert_ulong4_rtp(float4);
ulong4 __ovld __cnfn convert_ulong4_sat_rtp(float4);
ulong4 __ovld __cnfn convert_ulong4_rtn(float4);
ulong4 __ovld __cnfn convert_ulong4_sat_rtn(float4);
ulong4 __ovld __cnfn convert_ulong4(float4);
ulong4 __ovld __cnfn convert_ulong4_sat(float4);
float4 __ovld __cnfn convert_float4_rte(char4);
float4 __ovld __cnfn convert_float4_rtz(char4);
float4 __ovld __cnfn convert_float4_rtp(char4);
float4 __ovld __cnfn convert_float4_rtn(char4);
float4 __ovld __cnfn convert_float4(char4);
float4 __ovld __cnfn convert_float4_rte(uchar4);
float4 __ovld __cnfn convert_float4_rtz(uchar4);
float4 __ovld __cnfn convert_float4_rtp(uchar4);
float4 __ovld __cnfn convert_float4_rtn(uchar4);
float4 __ovld __cnfn convert_float4(uchar4);
float4 __ovld __cnfn convert_float4_rte(short4);
float4 __ovld __cnfn convert_float4_rtz(short4);
float4 __ovld __cnfn convert_float4_rtp(short4);
float4 __ovld __cnfn convert_float4_rtn(short4);
float4 __ovld __cnfn convert_float4(short4);
float4 __ovld __cnfn convert_float4_rte(ushort4);
float4 __ovld __cnfn convert_float4_rtz(ushort4);
float4 __ovld __cnfn convert_float4_rtp(ushort4);
float4 __ovld __cnfn convert_float4_rtn(ushort4);
float4 __ovld __cnfn convert_float4(ushort4);
float4 __ovld __cnfn convert_float4_rte(int4);
float4 __ovld __cnfn convert_float4_rtz(int4);
float4 __ovld __cnfn convert_float4_rtp(int4);
float4 __ovld __cnfn convert_float4_rtn(int4);
float4 __ovld __cnfn convert_float4(int4);
float4 __ovld __cnfn convert_float4_rte(uint4);
float4 __ovld __cnfn convert_float4_rtz(uint4);
float4 __ovld __cnfn convert_float4_rtp(uint4);
float4 __ovld __cnfn convert_float4_rtn(uint4);
float4 __ovld __cnfn convert_float4(uint4);
float4 __ovld __cnfn convert_float4_rte(long4);
float4 __ovld __cnfn convert_float4_rtz(long4);
float4 __ovld __cnfn convert_float4_rtp(long4);
float4 __ovld __cnfn convert_float4_rtn(long4);
float4 __ovld __cnfn convert_float4(long4);
float4 __ovld __cnfn convert_float4_rte(ulong4);
float4 __ovld __cnfn convert_float4_rtz(ulong4);
float4 __ovld __cnfn convert_float4_rtp(ulong4);
float4 __ovld __cnfn convert_float4_rtn(ulong4);
float4 __ovld __cnfn convert_float4(ulong4);
float4 __ovld __cnfn convert_float4_rte(float4);
float4 __ovld __cnfn convert_float4_rtz(float4);
float4 __ovld __cnfn convert_float4_rtp(float4);
float4 __ovld __cnfn convert_float4_rtn(float4);
float4 __ovld __cnfn convert_float4(float4);
char8 __ovld __cnfn convert_char8_rte(char8);
char8 __ovld __cnfn convert_char8_sat_rte(char8);
char8 __ovld __cnfn convert_char8_rtz(char8);
char8 __ovld __cnfn convert_char8_sat_rtz(char8);
char8 __ovld __cnfn convert_char8_rtp(char8);
char8 __ovld __cnfn convert_char8_sat_rtp(char8);
char8 __ovld __cnfn convert_char8_rtn(char8);
char8 __ovld __cnfn convert_char8_sat_rtn(char8);
char8 __ovld __cnfn convert_char8(char8);
char8 __ovld __cnfn convert_char8_sat(char8);
char8 __ovld __cnfn convert_char8_rte(uchar8);
char8 __ovld __cnfn convert_char8_sat_rte(uchar8);
char8 __ovld __cnfn convert_char8_rtz(uchar8);
char8 __ovld __cnfn convert_char8_sat_rtz(uchar8);
char8 __ovld __cnfn convert_char8_rtp(uchar8);
char8 __ovld __cnfn convert_char8_sat_rtp(uchar8);
char8 __ovld __cnfn convert_char8_rtn(uchar8);
char8 __ovld __cnfn convert_char8_sat_rtn(uchar8);
char8 __ovld __cnfn convert_char8(uchar8);
char8 __ovld __cnfn convert_char8_sat(uchar8);
char8 __ovld __cnfn convert_char8_rte(short8);
char8 __ovld __cnfn convert_char8_sat_rte(short8);
char8 __ovld __cnfn convert_char8_rtz(short8);
char8 __ovld __cnfn convert_char8_sat_rtz(short8);
char8 __ovld __cnfn convert_char8_rtp(short8);
char8 __ovld __cnfn convert_char8_sat_rtp(short8);
char8 __ovld __cnfn convert_char8_rtn(short8);
char8 __ovld __cnfn convert_char8_sat_rtn(short8);
char8 __ovld __cnfn convert_char8(short8);
char8 __ovld __cnfn convert_char8_sat(short8);
char8 __ovld __cnfn convert_char8_rte(ushort8);
char8 __ovld __cnfn convert_char8_sat_rte(ushort8);
char8 __ovld __cnfn convert_char8_rtz(ushort8);
char8 __ovld __cnfn convert_char8_sat_rtz(ushort8);
char8 __ovld __cnfn convert_char8_rtp(ushort8);
char8 __ovld __cnfn convert_char8_sat_rtp(ushort8);
char8 __ovld __cnfn convert_char8_rtn(ushort8);
char8 __ovld __cnfn convert_char8_sat_rtn(ushort8);
char8 __ovld __cnfn convert_char8(ushort8);
char8 __ovld __cnfn convert_char8_sat(ushort8);
char8 __ovld __cnfn convert_char8_rte(int8);
char8 __ovld __cnfn convert_char8_sat_rte(int8);
char8 __ovld __cnfn convert_char8_rtz(int8);
char8 __ovld __cnfn convert_char8_sat_rtz(int8);
char8 __ovld __cnfn convert_char8_rtp(int8);
char8 __ovld __cnfn convert_char8_sat_rtp(int8);
char8 __ovld __cnfn convert_char8_rtn(int8);
char8 __ovld __cnfn convert_char8_sat_rtn(int8);
char8 __ovld __cnfn convert_char8(int8);
char8 __ovld __cnfn convert_char8_sat(int8);
char8 __ovld __cnfn convert_char8_rte(uint8);
char8 __ovld __cnfn convert_char8_sat_rte(uint8);
char8 __ovld __cnfn convert_char8_rtz(uint8);
char8 __ovld __cnfn convert_char8_sat_rtz(uint8);
char8 __ovld __cnfn convert_char8_rtp(uint8);
char8 __ovld __cnfn convert_char8_sat_rtp(uint8);
char8 __ovld __cnfn convert_char8_rtn(uint8);
char8 __ovld __cnfn convert_char8_sat_rtn(uint8);
char8 __ovld __cnfn convert_char8(uint8);
char8 __ovld __cnfn convert_char8_sat(uint8);
char8 __ovld __cnfn convert_char8_rte(long8);
char8 __ovld __cnfn convert_char8_sat_rte(long8);
char8 __ovld __cnfn convert_char8_rtz(long8);
char8 __ovld __cnfn convert_char8_sat_rtz(long8);
char8 __ovld __cnfn convert_char8_rtp(long8);
char8 __ovld __cnfn convert_char8_sat_rtp(long8);
char8 __ovld __cnfn convert_char8_rtn(long8);
char8 __ovld __cnfn convert_char8_sat_rtn(long8);
char8 __ovld __cnfn convert_char8(long8);
char8 __ovld __cnfn convert_char8_sat(long8);
char8 __ovld __cnfn convert_char8_rte(ulong8);
char8 __ovld __cnfn convert_char8_sat_rte(ulong8);
char8 __ovld __cnfn convert_char8_rtz(ulong8);
char8 __ovld __cnfn convert_char8_sat_rtz(ulong8);
char8 __ovld __cnfn convert_char8_rtp(ulong8);
char8 __ovld __cnfn convert_char8_sat_rtp(ulong8);
char8 __ovld __cnfn convert_char8_rtn(ulong8);
char8 __ovld __cnfn convert_char8_sat_rtn(ulong8);
char8 __ovld __cnfn convert_char8(ulong8);
char8 __ovld __cnfn convert_char8_sat(ulong8);
char8 __ovld __cnfn convert_char8_rte(float8);
char8 __ovld __cnfn convert_char8_sat_rte(float8);
char8 __ovld __cnfn convert_char8_rtz(float8);
char8 __ovld __cnfn convert_char8_sat_rtz(float8);
char8 __ovld __cnfn convert_char8_rtp(float8);
char8 __ovld __cnfn convert_char8_sat_rtp(float8);
char8 __ovld __cnfn convert_char8_rtn(float8);
char8 __ovld __cnfn convert_char8_sat_rtn(float8);
char8 __ovld __cnfn convert_char8(float8);
char8 __ovld __cnfn convert_char8_sat(float8);
uchar8 __ovld __cnfn convert_uchar8_rte(char8);
uchar8 __ovld __cnfn convert_uchar8_sat_rte(char8);
uchar8 __ovld __cnfn convert_uchar8_rtz(char8);
uchar8 __ovld __cnfn convert_uchar8_sat_rtz(char8);
uchar8 __ovld __cnfn convert_uchar8_rtp(char8);
uchar8 __ovld __cnfn convert_uchar8_sat_rtp(char8);
uchar8 __ovld __cnfn convert_uchar8_rtn(char8);
uchar8 __ovld __cnfn convert_uchar8_sat_rtn(char8);
uchar8 __ovld __cnfn convert_uchar8(char8);
uchar8 __ovld __cnfn convert_uchar8_sat(char8);
uchar8 __ovld __cnfn convert_uchar8_rte(uchar8);
uchar8 __ovld __cnfn convert_uchar8_sat_rte(uchar8);
uchar8 __ovld __cnfn convert_uchar8_rtz(uchar8);
uchar8 __ovld __cnfn convert_uchar8_sat_rtz(uchar8);
uchar8 __ovld __cnfn convert_uchar8_rtp(uchar8);
uchar8 __ovld __cnfn convert_uchar8_sat_rtp(uchar8);
uchar8 __ovld __cnfn convert_uchar8_rtn(uchar8);
uchar8 __ovld __cnfn convert_uchar8_sat_rtn(uchar8);
uchar8 __ovld __cnfn convert_uchar8(uchar8);
uchar8 __ovld __cnfn convert_uchar8_sat(uchar8);
uchar8 __ovld __cnfn convert_uchar8_rte(short8);
uchar8 __ovld __cnfn convert_uchar8_sat_rte(short8);
uchar8 __ovld __cnfn convert_uchar8_rtz(short8);
uchar8 __ovld __cnfn convert_uchar8_sat_rtz(short8);
uchar8 __ovld __cnfn convert_uchar8_rtp(short8);
uchar8 __ovld __cnfn convert_uchar8_sat_rtp(short8);
uchar8 __ovld __cnfn convert_uchar8_rtn(short8);
uchar8 __ovld __cnfn convert_uchar8_sat_rtn(short8);
uchar8 __ovld __cnfn convert_uchar8(short8);
uchar8 __ovld __cnfn convert_uchar8_sat(short8);
uchar8 __ovld __cnfn convert_uchar8_rte(ushort8);
uchar8 __ovld __cnfn convert_uchar8_sat_rte(ushort8);
uchar8 __ovld __cnfn convert_uchar8_rtz(ushort8);
uchar8 __ovld __cnfn convert_uchar8_sat_rtz(ushort8);
uchar8 __ovld __cnfn convert_uchar8_rtp(ushort8);
uchar8 __ovld __cnfn convert_uchar8_sat_rtp(ushort8);
uchar8 __ovld __cnfn convert_uchar8_rtn(ushort8);
uchar8 __ovld __cnfn convert_uchar8_sat_rtn(ushort8);
uchar8 __ovld __cnfn convert_uchar8(ushort8);
uchar8 __ovld __cnfn convert_uchar8_sat(ushort8);
uchar8 __ovld __cnfn convert_uchar8_rte(int8);
uchar8 __ovld __cnfn convert_uchar8_sat_rte(int8);
uchar8 __ovld __cnfn convert_uchar8_rtz(int8);
uchar8 __ovld __cnfn convert_uchar8_sat_rtz(int8);
uchar8 __ovld __cnfn convert_uchar8_rtp(int8);
uchar8 __ovld __cnfn convert_uchar8_sat_rtp(int8);
uchar8 __ovld __cnfn convert_uchar8_rtn(int8);
uchar8 __ovld __cnfn convert_uchar8_sat_rtn(int8);
uchar8 __ovld __cnfn convert_uchar8(int8);
uchar8 __ovld __cnfn convert_uchar8_sat(int8);
uchar8 __ovld __cnfn convert_uchar8_rte(uint8);
uchar8 __ovld __cnfn convert_uchar8_sat_rte(uint8);
uchar8 __ovld __cnfn convert_uchar8_rtz(uint8);
uchar8 __ovld __cnfn convert_uchar8_sat_rtz(uint8);
uchar8 __ovld __cnfn convert_uchar8_rtp(uint8);
uchar8 __ovld __cnfn convert_uchar8_sat_rtp(uint8);
uchar8 __ovld __cnfn convert_uchar8_rtn(uint8);
uchar8 __ovld __cnfn convert_uchar8_sat_rtn(uint8);
uchar8 __ovld __cnfn convert_uchar8(uint8);
uchar8 __ovld __cnfn convert_uchar8_sat(uint8);
uchar8 __ovld __cnfn convert_uchar8_rte(long8);
uchar8 __ovld __cnfn convert_uchar8_sat_rte(long8);
uchar8 __ovld __cnfn convert_uchar8_rtz(long8);
uchar8 __ovld __cnfn convert_uchar8_sat_rtz(long8);
uchar8 __ovld __cnfn convert_uchar8_rtp(long8);
uchar8 __ovld __cnfn convert_uchar8_sat_rtp(long8);
uchar8 __ovld __cnfn convert_uchar8_rtn(long8);
uchar8 __ovld __cnfn convert_uchar8_sat_rtn(long8);
uchar8 __ovld __cnfn convert_uchar8(long8);
uchar8 __ovld __cnfn convert_uchar8_sat(long8);
uchar8 __ovld __cnfn convert_uchar8_rte(ulong8);
uchar8 __ovld __cnfn convert_uchar8_sat_rte(ulong8);
uchar8 __ovld __cnfn convert_uchar8_rtz(ulong8);
uchar8 __ovld __cnfn convert_uchar8_sat_rtz(ulong8);
uchar8 __ovld __cnfn convert_uchar8_rtp(ulong8);
uchar8 __ovld __cnfn convert_uchar8_sat_rtp(ulong8);
uchar8 __ovld __cnfn convert_uchar8_rtn(ulong8);
uchar8 __ovld __cnfn convert_uchar8_sat_rtn(ulong8);
uchar8 __ovld __cnfn convert_uchar8(ulong8);
uchar8 __ovld __cnfn convert_uchar8_sat(ulong8);
uchar8 __ovld __cnfn convert_uchar8_rte(float8);
uchar8 __ovld __cnfn convert_uchar8_sat_rte(float8);
uchar8 __ovld __cnfn convert_uchar8_rtz(float8);
uchar8 __ovld __cnfn convert_uchar8_sat_rtz(float8);
uchar8 __ovld __cnfn convert_uchar8_rtp(float8);
uchar8 __ovld __cnfn convert_uchar8_sat_rtp(float8);
uchar8 __ovld __cnfn convert_uchar8_rtn(float8);
uchar8 __ovld __cnfn convert_uchar8_sat_rtn(float8);
uchar8 __ovld __cnfn convert_uchar8(float8);
uchar8 __ovld __cnfn convert_uchar8_sat(float8);
short8 __ovld __cnfn convert_short8_rte(char8);
short8 __ovld __cnfn convert_short8_sat_rte(char8);
short8 __ovld __cnfn convert_short8_rtz(char8);
short8 __ovld __cnfn convert_short8_sat_rtz(char8);
short8 __ovld __cnfn convert_short8_rtp(char8);
short8 __ovld __cnfn convert_short8_sat_rtp(char8);
short8 __ovld __cnfn convert_short8_rtn(char8);
short8 __ovld __cnfn convert_short8_sat_rtn(char8);
short8 __ovld __cnfn convert_short8(char8);
short8 __ovld __cnfn convert_short8_sat(char8);
short8 __ovld __cnfn convert_short8_rte(uchar8);
short8 __ovld __cnfn convert_short8_sat_rte(uchar8);
short8 __ovld __cnfn convert_short8_rtz(uchar8);
short8 __ovld __cnfn convert_short8_sat_rtz(uchar8);
short8 __ovld __cnfn convert_short8_rtp(uchar8);
short8 __ovld __cnfn convert_short8_sat_rtp(uchar8);
short8 __ovld __cnfn convert_short8_rtn(uchar8);
short8 __ovld __cnfn convert_short8_sat_rtn(uchar8);
short8 __ovld __cnfn convert_short8(uchar8);
short8 __ovld __cnfn convert_short8_sat(uchar8);
short8 __ovld __cnfn convert_short8_rte(short8);
short8 __ovld __cnfn convert_short8_sat_rte(short8);
short8 __ovld __cnfn convert_short8_rtz(short8);
short8 __ovld __cnfn convert_short8_sat_rtz(short8);
short8 __ovld __cnfn convert_short8_rtp(short8);
short8 __ovld __cnfn convert_short8_sat_rtp(short8);
short8 __ovld __cnfn convert_short8_rtn(short8);
short8 __ovld __cnfn convert_short8_sat_rtn(short8);
short8 __ovld __cnfn convert_short8(short8);
short8 __ovld __cnfn convert_short8_sat(short8);
short8 __ovld __cnfn convert_short8_rte(ushort8);
short8 __ovld __cnfn convert_short8_sat_rte(ushort8);
short8 __ovld __cnfn convert_short8_rtz(ushort8);
short8 __ovld __cnfn convert_short8_sat_rtz(ushort8);
short8 __ovld __cnfn convert_short8_rtp(ushort8);
short8 __ovld __cnfn convert_short8_sat_rtp(ushort8);
short8 __ovld __cnfn convert_short8_rtn(ushort8);
short8 __ovld __cnfn convert_short8_sat_rtn(ushort8);
short8 __ovld __cnfn convert_short8(ushort8);
short8 __ovld __cnfn convert_short8_sat(ushort8);
short8 __ovld __cnfn convert_short8_rte(int8);
short8 __ovld __cnfn convert_short8_sat_rte(int8);
short8 __ovld __cnfn convert_short8_rtz(int8);
short8 __ovld __cnfn convert_short8_sat_rtz(int8);
short8 __ovld __cnfn convert_short8_rtp(int8);
short8 __ovld __cnfn convert_short8_sat_rtp(int8);
short8 __ovld __cnfn convert_short8_rtn(int8);
short8 __ovld __cnfn convert_short8_sat_rtn(int8);
short8 __ovld __cnfn convert_short8(int8);
short8 __ovld __cnfn convert_short8_sat(int8);
short8 __ovld __cnfn convert_short8_rte(uint8);
short8 __ovld __cnfn convert_short8_sat_rte(uint8);
short8 __ovld __cnfn convert_short8_rtz(uint8);
short8 __ovld __cnfn convert_short8_sat_rtz(uint8);
short8 __ovld __cnfn convert_short8_rtp(uint8);
short8 __ovld __cnfn convert_short8_sat_rtp(uint8);
short8 __ovld __cnfn convert_short8_rtn(uint8);
short8 __ovld __cnfn convert_short8_sat_rtn(uint8);
short8 __ovld __cnfn convert_short8(uint8);
short8 __ovld __cnfn convert_short8_sat(uint8);
short8 __ovld __cnfn convert_short8_rte(long8);
short8 __ovld __cnfn convert_short8_sat_rte(long8);
short8 __ovld __cnfn convert_short8_rtz(long8);
short8 __ovld __cnfn convert_short8_sat_rtz(long8);
short8 __ovld __cnfn convert_short8_rtp(long8);
short8 __ovld __cnfn convert_short8_sat_rtp(long8);
short8 __ovld __cnfn convert_short8_rtn(long8);
short8 __ovld __cnfn convert_short8_sat_rtn(long8);
short8 __ovld __cnfn convert_short8(long8);
short8 __ovld __cnfn convert_short8_sat(long8);
short8 __ovld __cnfn convert_short8_rte(ulong8);
short8 __ovld __cnfn convert_short8_sat_rte(ulong8);
short8 __ovld __cnfn convert_short8_rtz(ulong8);
short8 __ovld __cnfn convert_short8_sat_rtz(ulong8);
short8 __ovld __cnfn convert_short8_rtp(ulong8);
short8 __ovld __cnfn convert_short8_sat_rtp(ulong8);
short8 __ovld __cnfn convert_short8_rtn(ulong8);
short8 __ovld __cnfn convert_short8_sat_rtn(ulong8);
short8 __ovld __cnfn convert_short8(ulong8);
short8 __ovld __cnfn convert_short8_sat(ulong8);
short8 __ovld __cnfn convert_short8_rte(float8);
short8 __ovld __cnfn convert_short8_sat_rte(float8);
short8 __ovld __cnfn convert_short8_rtz(float8);
short8 __ovld __cnfn convert_short8_sat_rtz(float8);
short8 __ovld __cnfn convert_short8_rtp(float8);
short8 __ovld __cnfn convert_short8_sat_rtp(float8);
short8 __ovld __cnfn convert_short8_rtn(float8);
short8 __ovld __cnfn convert_short8_sat_rtn(float8);
short8 __ovld __cnfn convert_short8(float8);
short8 __ovld __cnfn convert_short8_sat(float8);
ushort8 __ovld __cnfn convert_ushort8_rte(char8);
ushort8 __ovld __cnfn convert_ushort8_sat_rte(char8);
ushort8 __ovld __cnfn convert_ushort8_rtz(char8);
ushort8 __ovld __cnfn convert_ushort8_sat_rtz(char8);
ushort8 __ovld __cnfn convert_ushort8_rtp(char8);
ushort8 __ovld __cnfn convert_ushort8_sat_rtp(char8);
ushort8 __ovld __cnfn convert_ushort8_rtn(char8);
ushort8 __ovld __cnfn convert_ushort8_sat_rtn(char8);
ushort8 __ovld __cnfn convert_ushort8(char8);
ushort8 __ovld __cnfn convert_ushort8_sat(char8);
ushort8 __ovld __cnfn convert_ushort8_rte(uchar8);
ushort8 __ovld __cnfn convert_ushort8_sat_rte(uchar8);
ushort8 __ovld __cnfn convert_ushort8_rtz(uchar8);
ushort8 __ovld __cnfn convert_ushort8_sat_rtz(uchar8);
ushort8 __ovld __cnfn convert_ushort8_rtp(uchar8);
ushort8 __ovld __cnfn convert_ushort8_sat_rtp(uchar8);
ushort8 __ovld __cnfn convert_ushort8_rtn(uchar8);
ushort8 __ovld __cnfn convert_ushort8_sat_rtn(uchar8);
ushort8 __ovld __cnfn convert_ushort8(uchar8);
ushort8 __ovld __cnfn convert_ushort8_sat(uchar8);
ushort8 __ovld __cnfn convert_ushort8_rte(short8);
ushort8 __ovld __cnfn convert_ushort8_sat_rte(short8);
ushort8 __ovld __cnfn convert_ushort8_rtz(short8);
ushort8 __ovld __cnfn convert_ushort8_sat_rtz(short8);
ushort8 __ovld __cnfn convert_ushort8_rtp(short8);
ushort8 __ovld __cnfn convert_ushort8_sat_rtp(short8);
ushort8 __ovld __cnfn convert_ushort8_rtn(short8);
ushort8 __ovld __cnfn convert_ushort8_sat_rtn(short8);
ushort8 __ovld __cnfn convert_ushort8(short8);
ushort8 __ovld __cnfn convert_ushort8_sat(short8);
ushort8 __ovld __cnfn convert_ushort8_rte(ushort8);
ushort8 __ovld __cnfn convert_ushort8_sat_rte(ushort8);
ushort8 __ovld __cnfn convert_ushort8_rtz(ushort8);
ushort8 __ovld __cnfn convert_ushort8_sat_rtz(ushort8);
ushort8 __ovld __cnfn convert_ushort8_rtp(ushort8);
ushort8 __ovld __cnfn convert_ushort8_sat_rtp(ushort8);
ushort8 __ovld __cnfn convert_ushort8_rtn(ushort8);
ushort8 __ovld __cnfn convert_ushort8_sat_rtn(ushort8);
ushort8 __ovld __cnfn convert_ushort8(ushort8);
ushort8 __ovld __cnfn convert_ushort8_sat(ushort8);
ushort8 __ovld __cnfn convert_ushort8_rte(int8);
ushort8 __ovld __cnfn convert_ushort8_sat_rte(int8);
ushort8 __ovld __cnfn convert_ushort8_rtz(int8);
ushort8 __ovld __cnfn convert_ushort8_sat_rtz(int8);
ushort8 __ovld __cnfn convert_ushort8_rtp(int8);
ushort8 __ovld __cnfn convert_ushort8_sat_rtp(int8);
ushort8 __ovld __cnfn convert_ushort8_rtn(int8);
ushort8 __ovld __cnfn convert_ushort8_sat_rtn(int8);
ushort8 __ovld __cnfn convert_ushort8(int8);
ushort8 __ovld __cnfn convert_ushort8_sat(int8);
ushort8 __ovld __cnfn convert_ushort8_rte(uint8);
ushort8 __ovld __cnfn convert_ushort8_sat_rte(uint8);
ushort8 __ovld __cnfn convert_ushort8_rtz(uint8);
ushort8 __ovld __cnfn convert_ushort8_sat_rtz(uint8);
ushort8 __ovld __cnfn convert_ushort8_rtp(uint8);
ushort8 __ovld __cnfn convert_ushort8_sat_rtp(uint8);
ushort8 __ovld __cnfn convert_ushort8_rtn(uint8);
ushort8 __ovld __cnfn convert_ushort8_sat_rtn(uint8);
ushort8 __ovld __cnfn convert_ushort8(uint8);
ushort8 __ovld __cnfn convert_ushort8_sat(uint8);
ushort8 __ovld __cnfn convert_ushort8_rte(long8);
ushort8 __ovld __cnfn convert_ushort8_sat_rte(long8);
ushort8 __ovld __cnfn convert_ushort8_rtz(long8);
ushort8 __ovld __cnfn convert_ushort8_sat_rtz(long8);
ushort8 __ovld __cnfn convert_ushort8_rtp(long8);
ushort8 __ovld __cnfn convert_ushort8_sat_rtp(long8);
ushort8 __ovld __cnfn convert_ushort8_rtn(long8);
ushort8 __ovld __cnfn convert_ushort8_sat_rtn(long8);
ushort8 __ovld __cnfn convert_ushort8(long8);
ushort8 __ovld __cnfn convert_ushort8_sat(long8);
ushort8 __ovld __cnfn convert_ushort8_rte(ulong8);
ushort8 __ovld __cnfn convert_ushort8_sat_rte(ulong8);
ushort8 __ovld __cnfn convert_ushort8_rtz(ulong8);
ushort8 __ovld __cnfn convert_ushort8_sat_rtz(ulong8);
ushort8 __ovld __cnfn convert_ushort8_rtp(ulong8);
ushort8 __ovld __cnfn convert_ushort8_sat_rtp(ulong8);
ushort8 __ovld __cnfn convert_ushort8_rtn(ulong8);
ushort8 __ovld __cnfn convert_ushort8_sat_rtn(ulong8);
ushort8 __ovld __cnfn convert_ushort8(ulong8);
ushort8 __ovld __cnfn convert_ushort8_sat(ulong8);
ushort8 __ovld __cnfn convert_ushort8_rte(float8);
ushort8 __ovld __cnfn convert_ushort8_sat_rte(float8);
ushort8 __ovld __cnfn convert_ushort8_rtz(float8);
ushort8 __ovld __cnfn convert_ushort8_sat_rtz(float8);
ushort8 __ovld __cnfn convert_ushort8_rtp(float8);
ushort8 __ovld __cnfn convert_ushort8_sat_rtp(float8);
ushort8 __ovld __cnfn convert_ushort8_rtn(float8);
ushort8 __ovld __cnfn convert_ushort8_sat_rtn(float8);
ushort8 __ovld __cnfn convert_ushort8(float8);
ushort8 __ovld __cnfn convert_ushort8_sat(float8);
int8 __ovld __cnfn convert_int8_rte(char8);
int8 __ovld __cnfn convert_int8_sat_rte(char8);
int8 __ovld __cnfn convert_int8_rtz(char8);
int8 __ovld __cnfn convert_int8_sat_rtz(char8);
int8 __ovld __cnfn convert_int8_rtp(char8);
int8 __ovld __cnfn convert_int8_sat_rtp(char8);
int8 __ovld __cnfn convert_int8_rtn(char8);
int8 __ovld __cnfn convert_int8_sat_rtn(char8);
int8 __ovld __cnfn convert_int8(char8);
int8 __ovld __cnfn convert_int8_sat(char8);
int8 __ovld __cnfn convert_int8_rte(uchar8);
int8 __ovld __cnfn convert_int8_sat_rte(uchar8);
int8 __ovld __cnfn convert_int8_rtz(uchar8);
int8 __ovld __cnfn convert_int8_sat_rtz(uchar8);
int8 __ovld __cnfn convert_int8_rtp(uchar8);
int8 __ovld __cnfn convert_int8_sat_rtp(uchar8);
int8 __ovld __cnfn convert_int8_rtn(uchar8);
int8 __ovld __cnfn convert_int8_sat_rtn(uchar8);
int8 __ovld __cnfn convert_int8(uchar8);
int8 __ovld __cnfn convert_int8_sat(uchar8);
int8 __ovld __cnfn convert_int8_rte(short8);
int8 __ovld __cnfn convert_int8_sat_rte(short8);
int8 __ovld __cnfn convert_int8_rtz(short8);
int8 __ovld __cnfn convert_int8_sat_rtz(short8);
int8 __ovld __cnfn convert_int8_rtp(short8);
int8 __ovld __cnfn convert_int8_sat_rtp(short8);
int8 __ovld __cnfn convert_int8_rtn(short8);
int8 __ovld __cnfn convert_int8_sat_rtn(short8);
int8 __ovld __cnfn convert_int8(short8);
int8 __ovld __cnfn convert_int8_sat(short8);
int8 __ovld __cnfn convert_int8_rte(ushort8);
int8 __ovld __cnfn convert_int8_sat_rte(ushort8);
int8 __ovld __cnfn convert_int8_rtz(ushort8);
int8 __ovld __cnfn convert_int8_sat_rtz(ushort8);
int8 __ovld __cnfn convert_int8_rtp(ushort8);
int8 __ovld __cnfn convert_int8_sat_rtp(ushort8);
int8 __ovld __cnfn convert_int8_rtn(ushort8);
int8 __ovld __cnfn convert_int8_sat_rtn(ushort8);
int8 __ovld __cnfn convert_int8(ushort8);
int8 __ovld __cnfn convert_int8_sat(ushort8);
int8 __ovld __cnfn convert_int8_rte(int8);
int8 __ovld __cnfn convert_int8_sat_rte(int8);
int8 __ovld __cnfn convert_int8_rtz(int8);
int8 __ovld __cnfn convert_int8_sat_rtz(int8);
int8 __ovld __cnfn convert_int8_rtp(int8);
int8 __ovld __cnfn convert_int8_sat_rtp(int8);
int8 __ovld __cnfn convert_int8_rtn(int8);
int8 __ovld __cnfn convert_int8_sat_rtn(int8);
int8 __ovld __cnfn convert_int8(int8);
int8 __ovld __cnfn convert_int8_sat(int8);
int8 __ovld __cnfn convert_int8_rte(uint8);
int8 __ovld __cnfn convert_int8_sat_rte(uint8);
int8 __ovld __cnfn convert_int8_rtz(uint8);
int8 __ovld __cnfn convert_int8_sat_rtz(uint8);
int8 __ovld __cnfn convert_int8_rtp(uint8);
int8 __ovld __cnfn convert_int8_sat_rtp(uint8);
int8 __ovld __cnfn convert_int8_rtn(uint8);
int8 __ovld __cnfn convert_int8_sat_rtn(uint8);
int8 __ovld __cnfn convert_int8(uint8);
int8 __ovld __cnfn convert_int8_sat(uint8);
int8 __ovld __cnfn convert_int8_rte(long8);
int8 __ovld __cnfn convert_int8_sat_rte(long8);
int8 __ovld __cnfn convert_int8_rtz(long8);
int8 __ovld __cnfn convert_int8_sat_rtz(long8);
int8 __ovld __cnfn convert_int8_rtp(long8);
int8 __ovld __cnfn convert_int8_sat_rtp(long8);
int8 __ovld __cnfn convert_int8_rtn(long8);
int8 __ovld __cnfn convert_int8_sat_rtn(long8);
int8 __ovld __cnfn convert_int8(long8);
int8 __ovld __cnfn convert_int8_sat(long8);
int8 __ovld __cnfn convert_int8_rte(ulong8);
int8 __ovld __cnfn convert_int8_sat_rte(ulong8);
int8 __ovld __cnfn convert_int8_rtz(ulong8);
int8 __ovld __cnfn convert_int8_sat_rtz(ulong8);
int8 __ovld __cnfn convert_int8_rtp(ulong8);
int8 __ovld __cnfn convert_int8_sat_rtp(ulong8);
int8 __ovld __cnfn convert_int8_rtn(ulong8);
int8 __ovld __cnfn convert_int8_sat_rtn(ulong8);
int8 __ovld __cnfn convert_int8(ulong8);
int8 __ovld __cnfn convert_int8_sat(ulong8);
int8 __ovld __cnfn convert_int8_rte(float8);
int8 __ovld __cnfn convert_int8_sat_rte(float8);
int8 __ovld __cnfn convert_int8_rtz(float8);
int8 __ovld __cnfn convert_int8_sat_rtz(float8);
int8 __ovld __cnfn convert_int8_rtp(float8);
int8 __ovld __cnfn convert_int8_sat_rtp(float8);
int8 __ovld __cnfn convert_int8_rtn(float8);
int8 __ovld __cnfn convert_int8_sat_rtn(float8);
int8 __ovld __cnfn convert_int8(float8);
int8 __ovld __cnfn convert_int8_sat(float8);
uint8 __ovld __cnfn convert_uint8_rte(char8);
uint8 __ovld __cnfn convert_uint8_sat_rte(char8);
uint8 __ovld __cnfn convert_uint8_rtz(char8);
uint8 __ovld __cnfn convert_uint8_sat_rtz(char8);
uint8 __ovld __cnfn convert_uint8_rtp(char8);
uint8 __ovld __cnfn convert_uint8_sat_rtp(char8);
uint8 __ovld __cnfn convert_uint8_rtn(char8);
uint8 __ovld __cnfn convert_uint8_sat_rtn(char8);
uint8 __ovld __cnfn convert_uint8(char8);
uint8 __ovld __cnfn convert_uint8_sat(char8);
uint8 __ovld __cnfn convert_uint8_rte(uchar8);
uint8 __ovld __cnfn convert_uint8_sat_rte(uchar8);
uint8 __ovld __cnfn convert_uint8_rtz(uchar8);
uint8 __ovld __cnfn convert_uint8_sat_rtz(uchar8);
uint8 __ovld __cnfn convert_uint8_rtp(uchar8);
uint8 __ovld __cnfn convert_uint8_sat_rtp(uchar8);
uint8 __ovld __cnfn convert_uint8_rtn(uchar8);
uint8 __ovld __cnfn convert_uint8_sat_rtn(uchar8);
uint8 __ovld __cnfn convert_uint8(uchar8);
uint8 __ovld __cnfn convert_uint8_sat(uchar8);
uint8 __ovld __cnfn convert_uint8_rte(short8);
uint8 __ovld __cnfn convert_uint8_sat_rte(short8);
uint8 __ovld __cnfn convert_uint8_rtz(short8);
uint8 __ovld __cnfn convert_uint8_sat_rtz(short8);
uint8 __ovld __cnfn convert_uint8_rtp(short8);
uint8 __ovld __cnfn convert_uint8_sat_rtp(short8);
uint8 __ovld __cnfn convert_uint8_rtn(short8);
uint8 __ovld __cnfn convert_uint8_sat_rtn(short8);
uint8 __ovld __cnfn convert_uint8(short8);
uint8 __ovld __cnfn convert_uint8_sat(short8);
uint8 __ovld __cnfn convert_uint8_rte(ushort8);
uint8 __ovld __cnfn convert_uint8_sat_rte(ushort8);
uint8 __ovld __cnfn convert_uint8_rtz(ushort8);
uint8 __ovld __cnfn convert_uint8_sat_rtz(ushort8);
uint8 __ovld __cnfn convert_uint8_rtp(ushort8);
uint8 __ovld __cnfn convert_uint8_sat_rtp(ushort8);
uint8 __ovld __cnfn convert_uint8_rtn(ushort8);
uint8 __ovld __cnfn convert_uint8_sat_rtn(ushort8);
uint8 __ovld __cnfn convert_uint8(ushort8);
uint8 __ovld __cnfn convert_uint8_sat(ushort8);
uint8 __ovld __cnfn convert_uint8_rte(int8);
uint8 __ovld __cnfn convert_uint8_sat_rte(int8);
uint8 __ovld __cnfn convert_uint8_rtz(int8);
uint8 __ovld __cnfn convert_uint8_sat_rtz(int8);
uint8 __ovld __cnfn convert_uint8_rtp(int8);
uint8 __ovld __cnfn convert_uint8_sat_rtp(int8);
uint8 __ovld __cnfn convert_uint8_rtn(int8);
uint8 __ovld __cnfn convert_uint8_sat_rtn(int8);
uint8 __ovld __cnfn convert_uint8(int8);
uint8 __ovld __cnfn convert_uint8_sat(int8);
uint8 __ovld __cnfn convert_uint8_rte(uint8);
uint8 __ovld __cnfn convert_uint8_sat_rte(uint8);
uint8 __ovld __cnfn convert_uint8_rtz(uint8);
uint8 __ovld __cnfn convert_uint8_sat_rtz(uint8);
uint8 __ovld __cnfn convert_uint8_rtp(uint8);
uint8 __ovld __cnfn convert_uint8_sat_rtp(uint8);
uint8 __ovld __cnfn convert_uint8_rtn(uint8);
uint8 __ovld __cnfn convert_uint8_sat_rtn(uint8);
uint8 __ovld __cnfn convert_uint8(uint8);
uint8 __ovld __cnfn convert_uint8_sat(uint8);
uint8 __ovld __cnfn convert_uint8_rte(long8);
uint8 __ovld __cnfn convert_uint8_sat_rte(long8);
uint8 __ovld __cnfn convert_uint8_rtz(long8);
uint8 __ovld __cnfn convert_uint8_sat_rtz(long8);
uint8 __ovld __cnfn convert_uint8_rtp(long8);
uint8 __ovld __cnfn convert_uint8_sat_rtp(long8);
uint8 __ovld __cnfn convert_uint8_rtn(long8);
uint8 __ovld __cnfn convert_uint8_sat_rtn(long8);
uint8 __ovld __cnfn convert_uint8(long8);
uint8 __ovld __cnfn convert_uint8_sat(long8);
uint8 __ovld __cnfn convert_uint8_rte(ulong8);
uint8 __ovld __cnfn convert_uint8_sat_rte(ulong8);
uint8 __ovld __cnfn convert_uint8_rtz(ulong8);
uint8 __ovld __cnfn convert_uint8_sat_rtz(ulong8);
uint8 __ovld __cnfn convert_uint8_rtp(ulong8);
uint8 __ovld __cnfn convert_uint8_sat_rtp(ulong8);
uint8 __ovld __cnfn convert_uint8_rtn(ulong8);
uint8 __ovld __cnfn convert_uint8_sat_rtn(ulong8);
uint8 __ovld __cnfn convert_uint8(ulong8);
uint8 __ovld __cnfn convert_uint8_sat(ulong8);
uint8 __ovld __cnfn convert_uint8_rte(float8);
uint8 __ovld __cnfn convert_uint8_sat_rte(float8);
uint8 __ovld __cnfn convert_uint8_rtz(float8);
uint8 __ovld __cnfn convert_uint8_sat_rtz(float8);
uint8 __ovld __cnfn convert_uint8_rtp(float8);
uint8 __ovld __cnfn convert_uint8_sat_rtp(float8);
uint8 __ovld __cnfn convert_uint8_rtn(float8);
uint8 __ovld __cnfn convert_uint8_sat_rtn(float8);
uint8 __ovld __cnfn convert_uint8(float8);
uint8 __ovld __cnfn convert_uint8_sat(float8);
long8 __ovld __cnfn convert_long8_rte(char8);
long8 __ovld __cnfn convert_long8_sat_rte(char8);
long8 __ovld __cnfn convert_long8_rtz(char8);
long8 __ovld __cnfn convert_long8_sat_rtz(char8);
long8 __ovld __cnfn convert_long8_rtp(char8);
long8 __ovld __cnfn convert_long8_sat_rtp(char8);
long8 __ovld __cnfn convert_long8_rtn(char8);
long8 __ovld __cnfn convert_long8_sat_rtn(char8);
long8 __ovld __cnfn convert_long8(char8);
long8 __ovld __cnfn convert_long8_sat(char8);
long8 __ovld __cnfn convert_long8_rte(uchar8);
long8 __ovld __cnfn convert_long8_sat_rte(uchar8);
long8 __ovld __cnfn convert_long8_rtz(uchar8);
long8 __ovld __cnfn convert_long8_sat_rtz(uchar8);
long8 __ovld __cnfn convert_long8_rtp(uchar8);
long8 __ovld __cnfn convert_long8_sat_rtp(uchar8);
long8 __ovld __cnfn convert_long8_rtn(uchar8);
long8 __ovld __cnfn convert_long8_sat_rtn(uchar8);
long8 __ovld __cnfn convert_long8(uchar8);
long8 __ovld __cnfn convert_long8_sat(uchar8);
long8 __ovld __cnfn convert_long8_rte(short8);
long8 __ovld __cnfn convert_long8_sat_rte(short8);
long8 __ovld __cnfn convert_long8_rtz(short8);
long8 __ovld __cnfn convert_long8_sat_rtz(short8);
long8 __ovld __cnfn convert_long8_rtp(short8);
long8 __ovld __cnfn convert_long8_sat_rtp(short8);
long8 __ovld __cnfn convert_long8_rtn(short8);
long8 __ovld __cnfn convert_long8_sat_rtn(short8);
long8 __ovld __cnfn convert_long8(short8);
long8 __ovld __cnfn convert_long8_sat(short8);
long8 __ovld __cnfn convert_long8_rte(ushort8);
long8 __ovld __cnfn convert_long8_sat_rte(ushort8);
long8 __ovld __cnfn convert_long8_rtz(ushort8);
long8 __ovld __cnfn convert_long8_sat_rtz(ushort8);
long8 __ovld __cnfn convert_long8_rtp(ushort8);
long8 __ovld __cnfn convert_long8_sat_rtp(ushort8);
long8 __ovld __cnfn convert_long8_rtn(ushort8);
long8 __ovld __cnfn convert_long8_sat_rtn(ushort8);
long8 __ovld __cnfn convert_long8(ushort8);
long8 __ovld __cnfn convert_long8_sat(ushort8);
long8 __ovld __cnfn convert_long8_rte(int8);
long8 __ovld __cnfn convert_long8_sat_rte(int8);
long8 __ovld __cnfn convert_long8_rtz(int8);
long8 __ovld __cnfn convert_long8_sat_rtz(int8);
long8 __ovld __cnfn convert_long8_rtp(int8);
long8 __ovld __cnfn convert_long8_sat_rtp(int8);
long8 __ovld __cnfn convert_long8_rtn(int8);
long8 __ovld __cnfn convert_long8_sat_rtn(int8);
long8 __ovld __cnfn convert_long8(int8);
long8 __ovld __cnfn convert_long8_sat(int8);
long8 __ovld __cnfn convert_long8_rte(uint8);
long8 __ovld __cnfn convert_long8_sat_rte(uint8);
long8 __ovld __cnfn convert_long8_rtz(uint8);
long8 __ovld __cnfn convert_long8_sat_rtz(uint8);
long8 __ovld __cnfn convert_long8_rtp(uint8);
long8 __ovld __cnfn convert_long8_sat_rtp(uint8);
long8 __ovld __cnfn convert_long8_rtn(uint8);
long8 __ovld __cnfn convert_long8_sat_rtn(uint8);
long8 __ovld __cnfn convert_long8(uint8);
long8 __ovld __cnfn convert_long8_sat(uint8);
long8 __ovld __cnfn convert_long8_rte(long8);
long8 __ovld __cnfn convert_long8_sat_rte(long8);
long8 __ovld __cnfn convert_long8_rtz(long8);
long8 __ovld __cnfn convert_long8_sat_rtz(long8);
long8 __ovld __cnfn convert_long8_rtp(long8);
long8 __ovld __cnfn convert_long8_sat_rtp(long8);
long8 __ovld __cnfn convert_long8_rtn(long8);
long8 __ovld __cnfn convert_long8_sat_rtn(long8);
long8 __ovld __cnfn convert_long8(long8);
long8 __ovld __cnfn convert_long8_sat(long8);
long8 __ovld __cnfn convert_long8_rte(ulong8);
long8 __ovld __cnfn convert_long8_sat_rte(ulong8);
long8 __ovld __cnfn convert_long8_rtz(ulong8);
long8 __ovld __cnfn convert_long8_sat_rtz(ulong8);
long8 __ovld __cnfn convert_long8_rtp(ulong8);
long8 __ovld __cnfn convert_long8_sat_rtp(ulong8);
long8 __ovld __cnfn convert_long8_rtn(ulong8);
long8 __ovld __cnfn convert_long8_sat_rtn(ulong8);
long8 __ovld __cnfn convert_long8(ulong8);
long8 __ovld __cnfn convert_long8_sat(ulong8);
long8 __ovld __cnfn convert_long8_rte(float8);
long8 __ovld __cnfn convert_long8_sat_rte(float8);
long8 __ovld __cnfn convert_long8_rtz(float8);
long8 __ovld __cnfn convert_long8_sat_rtz(float8);
long8 __ovld __cnfn convert_long8_rtp(float8);
long8 __ovld __cnfn convert_long8_sat_rtp(float8);
long8 __ovld __cnfn convert_long8_rtn(float8);
long8 __ovld __cnfn convert_long8_sat_rtn(float8);
long8 __ovld __cnfn convert_long8(float8);
long8 __ovld __cnfn convert_long8_sat(float8);
ulong8 __ovld __cnfn convert_ulong8_rte(char8);
ulong8 __ovld __cnfn convert_ulong8_sat_rte(char8);
ulong8 __ovld __cnfn convert_ulong8_rtz(char8);
ulong8 __ovld __cnfn convert_ulong8_sat_rtz(char8);
ulong8 __ovld __cnfn convert_ulong8_rtp(char8);
ulong8 __ovld __cnfn convert_ulong8_sat_rtp(char8);
ulong8 __ovld __cnfn convert_ulong8_rtn(char8);
ulong8 __ovld __cnfn convert_ulong8_sat_rtn(char8);
ulong8 __ovld __cnfn convert_ulong8(char8);
ulong8 __ovld __cnfn convert_ulong8_sat(char8);
ulong8 __ovld __cnfn convert_ulong8_rte(uchar8);
ulong8 __ovld __cnfn convert_ulong8_sat_rte(uchar8);
ulong8 __ovld __cnfn convert_ulong8_rtz(uchar8);
ulong8 __ovld __cnfn convert_ulong8_sat_rtz(uchar8);
ulong8 __ovld __cnfn convert_ulong8_rtp(uchar8);
ulong8 __ovld __cnfn convert_ulong8_sat_rtp(uchar8);
ulong8 __ovld __cnfn convert_ulong8_rtn(uchar8);
ulong8 __ovld __cnfn convert_ulong8_sat_rtn(uchar8);
ulong8 __ovld __cnfn convert_ulong8(uchar8);
ulong8 __ovld __cnfn convert_ulong8_sat(uchar8);
ulong8 __ovld __cnfn convert_ulong8_rte(short8);
ulong8 __ovld __cnfn convert_ulong8_sat_rte(short8);
ulong8 __ovld __cnfn convert_ulong8_rtz(short8);
ulong8 __ovld __cnfn convert_ulong8_sat_rtz(short8);
ulong8 __ovld __cnfn convert_ulong8_rtp(short8);
ulong8 __ovld __cnfn convert_ulong8_sat_rtp(short8);
ulong8 __ovld __cnfn convert_ulong8_rtn(short8);
ulong8 __ovld __cnfn convert_ulong8_sat_rtn(short8);
ulong8 __ovld __cnfn convert_ulong8(short8);
ulong8 __ovld __cnfn convert_ulong8_sat(short8);
ulong8 __ovld __cnfn convert_ulong8_rte(ushort8);
ulong8 __ovld __cnfn convert_ulong8_sat_rte(ushort8);
ulong8 __ovld __cnfn convert_ulong8_rtz(ushort8);
ulong8 __ovld __cnfn convert_ulong8_sat_rtz(ushort8);
ulong8 __ovld __cnfn convert_ulong8_rtp(ushort8);
ulong8 __ovld __cnfn convert_ulong8_sat_rtp(ushort8);
ulong8 __ovld __cnfn convert_ulong8_rtn(ushort8);
ulong8 __ovld __cnfn convert_ulong8_sat_rtn(ushort8);
ulong8 __ovld __cnfn convert_ulong8(ushort8);
ulong8 __ovld __cnfn convert_ulong8_sat(ushort8);
ulong8 __ovld __cnfn convert_ulong8_rte(int8);
ulong8 __ovld __cnfn convert_ulong8_sat_rte(int8);
ulong8 __ovld __cnfn convert_ulong8_rtz(int8);
ulong8 __ovld __cnfn convert_ulong8_sat_rtz(int8);
ulong8 __ovld __cnfn convert_ulong8_rtp(int8);
ulong8 __ovld __cnfn convert_ulong8_sat_rtp(int8);
ulong8 __ovld __cnfn convert_ulong8_rtn(int8);
ulong8 __ovld __cnfn convert_ulong8_sat_rtn(int8);
ulong8 __ovld __cnfn convert_ulong8(int8);
ulong8 __ovld __cnfn convert_ulong8_sat(int8);
ulong8 __ovld __cnfn convert_ulong8_rte(uint8);
ulong8 __ovld __cnfn convert_ulong8_sat_rte(uint8);
ulong8 __ovld __cnfn convert_ulong8_rtz(uint8);
ulong8 __ovld __cnfn convert_ulong8_sat_rtz(uint8);
ulong8 __ovld __cnfn convert_ulong8_rtp(uint8);
ulong8 __ovld __cnfn convert_ulong8_sat_rtp(uint8);
ulong8 __ovld __cnfn convert_ulong8_rtn(uint8);
ulong8 __ovld __cnfn convert_ulong8_sat_rtn(uint8);
ulong8 __ovld __cnfn convert_ulong8(uint8);
ulong8 __ovld __cnfn convert_ulong8_sat(uint8);
ulong8 __ovld __cnfn convert_ulong8_rte(long8);
ulong8 __ovld __cnfn convert_ulong8_sat_rte(long8);
ulong8 __ovld __cnfn convert_ulong8_rtz(long8);
ulong8 __ovld __cnfn convert_ulong8_sat_rtz(long8);
ulong8 __ovld __cnfn convert_ulong8_rtp(long8);
ulong8 __ovld __cnfn convert_ulong8_sat_rtp(long8);
ulong8 __ovld __cnfn convert_ulong8_rtn(long8);
ulong8 __ovld __cnfn convert_ulong8_sat_rtn(long8);
ulong8 __ovld __cnfn convert_ulong8(long8);
ulong8 __ovld __cnfn convert_ulong8_sat(long8);
ulong8 __ovld __cnfn convert_ulong8_rte(ulong8);
ulong8 __ovld __cnfn convert_ulong8_sat_rte(ulong8);
ulong8 __ovld __cnfn convert_ulong8_rtz(ulong8);
ulong8 __ovld __cnfn convert_ulong8_sat_rtz(ulong8);
ulong8 __ovld __cnfn convert_ulong8_rtp(ulong8);
ulong8 __ovld __cnfn convert_ulong8_sat_rtp(ulong8);
ulong8 __ovld __cnfn convert_ulong8_rtn(ulong8);
ulong8 __ovld __cnfn convert_ulong8_sat_rtn(ulong8);
ulong8 __ovld __cnfn convert_ulong8(ulong8);
ulong8 __ovld __cnfn convert_ulong8_sat(ulong8);
ulong8 __ovld __cnfn convert_ulong8_rte(float8);
ulong8 __ovld __cnfn convert_ulong8_sat_rte(float8);
ulong8 __ovld __cnfn convert_ulong8_rtz(float8);
ulong8 __ovld __cnfn convert_ulong8_sat_rtz(float8);
ulong8 __ovld __cnfn convert_ulong8_rtp(float8);
ulong8 __ovld __cnfn convert_ulong8_sat_rtp(float8);
ulong8 __ovld __cnfn convert_ulong8_rtn(float8);
ulong8 __ovld __cnfn convert_ulong8_sat_rtn(float8);
ulong8 __ovld __cnfn convert_ulong8(float8);
ulong8 __ovld __cnfn convert_ulong8_sat(float8);
float8 __ovld __cnfn convert_float8_rte(char8);
float8 __ovld __cnfn convert_float8_rtz(char8);
float8 __ovld __cnfn convert_float8_rtp(char8);
float8 __ovld __cnfn convert_float8_rtn(char8);
float8 __ovld __cnfn convert_float8(char8);
float8 __ovld __cnfn convert_float8_rte(uchar8);
float8 __ovld __cnfn convert_float8_rtz(uchar8);
float8 __ovld __cnfn convert_float8_rtp(uchar8);
float8 __ovld __cnfn convert_float8_rtn(uchar8);
float8 __ovld __cnfn convert_float8(uchar8);
float8 __ovld __cnfn convert_float8_rte(short8);
float8 __ovld __cnfn convert_float8_rtz(short8);
float8 __ovld __cnfn convert_float8_rtp(short8);
float8 __ovld __cnfn convert_float8_rtn(short8);
float8 __ovld __cnfn convert_float8(short8);
float8 __ovld __cnfn convert_float8_rte(ushort8);
float8 __ovld __cnfn convert_float8_rtz(ushort8);
float8 __ovld __cnfn convert_float8_rtp(ushort8);
float8 __ovld __cnfn convert_float8_rtn(ushort8);
float8 __ovld __cnfn convert_float8(ushort8);
float8 __ovld __cnfn convert_float8_rte(int8);
float8 __ovld __cnfn convert_float8_rtz(int8);
float8 __ovld __cnfn convert_float8_rtp(int8);
float8 __ovld __cnfn convert_float8_rtn(int8);
float8 __ovld __cnfn convert_float8(int8);
float8 __ovld __cnfn convert_float8_rte(uint8);
float8 __ovld __cnfn convert_float8_rtz(uint8);
float8 __ovld __cnfn convert_float8_rtp(uint8);
float8 __ovld __cnfn convert_float8_rtn(uint8);
float8 __ovld __cnfn convert_float8(uint8);
float8 __ovld __cnfn convert_float8_rte(long8);
float8 __ovld __cnfn convert_float8_rtz(long8);
float8 __ovld __cnfn convert_float8_rtp(long8);
float8 __ovld __cnfn convert_float8_rtn(long8);
float8 __ovld __cnfn convert_float8(long8);
float8 __ovld __cnfn convert_float8_rte(ulong8);
float8 __ovld __cnfn convert_float8_rtz(ulong8);
float8 __ovld __cnfn convert_float8_rtp(ulong8);
float8 __ovld __cnfn convert_float8_rtn(ulong8);
float8 __ovld __cnfn convert_float8(ulong8);
float8 __ovld __cnfn convert_float8_rte(float8);
float8 __ovld __cnfn convert_float8_rtz(float8);
float8 __ovld __cnfn convert_float8_rtp(float8);
float8 __ovld __cnfn convert_float8_rtn(float8);
float8 __ovld __cnfn convert_float8(float8);
char16 __ovld __cnfn convert_char16_rte(char16);
char16 __ovld __cnfn convert_char16_sat_rte(char16);
char16 __ovld __cnfn convert_char16_rtz(char16);
char16 __ovld __cnfn convert_char16_sat_rtz(char16);
char16 __ovld __cnfn convert_char16_rtp(char16);
char16 __ovld __cnfn convert_char16_sat_rtp(char16);
char16 __ovld __cnfn convert_char16_rtn(char16);
char16 __ovld __cnfn convert_char16_sat_rtn(char16);
char16 __ovld __cnfn convert_char16(char16);
char16 __ovld __cnfn convert_char16_sat(char16);
char16 __ovld __cnfn convert_char16_rte(uchar16);
char16 __ovld __cnfn convert_char16_sat_rte(uchar16);
char16 __ovld __cnfn convert_char16_rtz(uchar16);
char16 __ovld __cnfn convert_char16_sat_rtz(uchar16);
char16 __ovld __cnfn convert_char16_rtp(uchar16);
char16 __ovld __cnfn convert_char16_sat_rtp(uchar16);
char16 __ovld __cnfn convert_char16_rtn(uchar16);
char16 __ovld __cnfn convert_char16_sat_rtn(uchar16);
char16 __ovld __cnfn convert_char16(uchar16);
char16 __ovld __cnfn convert_char16_sat(uchar16);
char16 __ovld __cnfn convert_char16_rte(short16);
char16 __ovld __cnfn convert_char16_sat_rte(short16);
char16 __ovld __cnfn convert_char16_rtz(short16);
char16 __ovld __cnfn convert_char16_sat_rtz(short16);
char16 __ovld __cnfn convert_char16_rtp(short16);
char16 __ovld __cnfn convert_char16_sat_rtp(short16);
char16 __ovld __cnfn convert_char16_rtn(short16);
char16 __ovld __cnfn convert_char16_sat_rtn(short16);
char16 __ovld __cnfn convert_char16(short16);
char16 __ovld __cnfn convert_char16_sat(short16);
char16 __ovld __cnfn convert_char16_rte(ushort16);
char16 __ovld __cnfn convert_char16_sat_rte(ushort16);
char16 __ovld __cnfn convert_char16_rtz(ushort16);
char16 __ovld __cnfn convert_char16_sat_rtz(ushort16);
char16 __ovld __cnfn convert_char16_rtp(ushort16);
char16 __ovld __cnfn convert_char16_sat_rtp(ushort16);
char16 __ovld __cnfn convert_char16_rtn(ushort16);
char16 __ovld __cnfn convert_char16_sat_rtn(ushort16);
char16 __ovld __cnfn convert_char16(ushort16);
char16 __ovld __cnfn convert_char16_sat(ushort16);
char16 __ovld __cnfn convert_char16_rte(int16);
char16 __ovld __cnfn convert_char16_sat_rte(int16);
char16 __ovld __cnfn convert_char16_rtz(int16);
char16 __ovld __cnfn convert_char16_sat_rtz(int16);
char16 __ovld __cnfn convert_char16_rtp(int16);
char16 __ovld __cnfn convert_char16_sat_rtp(int16);
char16 __ovld __cnfn convert_char16_rtn(int16);
char16 __ovld __cnfn convert_char16_sat_rtn(int16);
char16 __ovld __cnfn convert_char16(int16);
char16 __ovld __cnfn convert_char16_sat(int16);
char16 __ovld __cnfn convert_char16_rte(uint16);
char16 __ovld __cnfn convert_char16_sat_rte(uint16);
char16 __ovld __cnfn convert_char16_rtz(uint16);
char16 __ovld __cnfn convert_char16_sat_rtz(uint16);
char16 __ovld __cnfn convert_char16_rtp(uint16);
char16 __ovld __cnfn convert_char16_sat_rtp(uint16);
char16 __ovld __cnfn convert_char16_rtn(uint16);
char16 __ovld __cnfn convert_char16_sat_rtn(uint16);
char16 __ovld __cnfn convert_char16(uint16);
char16 __ovld __cnfn convert_char16_sat(uint16);
char16 __ovld __cnfn convert_char16_rte(long16);
char16 __ovld __cnfn convert_char16_sat_rte(long16);
char16 __ovld __cnfn convert_char16_rtz(long16);
char16 __ovld __cnfn convert_char16_sat_rtz(long16);
char16 __ovld __cnfn convert_char16_rtp(long16);
char16 __ovld __cnfn convert_char16_sat_rtp(long16);
char16 __ovld __cnfn convert_char16_rtn(long16);
char16 __ovld __cnfn convert_char16_sat_rtn(long16);
char16 __ovld __cnfn convert_char16(long16);
char16 __ovld __cnfn convert_char16_sat(long16);
char16 __ovld __cnfn convert_char16_rte(ulong16);
char16 __ovld __cnfn convert_char16_sat_rte(ulong16);
char16 __ovld __cnfn convert_char16_rtz(ulong16);
char16 __ovld __cnfn convert_char16_sat_rtz(ulong16);
char16 __ovld __cnfn convert_char16_rtp(ulong16);
char16 __ovld __cnfn convert_char16_sat_rtp(ulong16);
char16 __ovld __cnfn convert_char16_rtn(ulong16);
char16 __ovld __cnfn convert_char16_sat_rtn(ulong16);
char16 __ovld __cnfn convert_char16(ulong16);
char16 __ovld __cnfn convert_char16_sat(ulong16);
char16 __ovld __cnfn convert_char16_rte(float16);
char16 __ovld __cnfn convert_char16_sat_rte(float16);
char16 __ovld __cnfn convert_char16_rtz(float16);
char16 __ovld __cnfn convert_char16_sat_rtz(float16);
char16 __ovld __cnfn convert_char16_rtp(float16);
char16 __ovld __cnfn convert_char16_sat_rtp(float16);
char16 __ovld __cnfn convert_char16_rtn(float16);
char16 __ovld __cnfn convert_char16_sat_rtn(float16);
char16 __ovld __cnfn convert_char16(float16);
char16 __ovld __cnfn convert_char16_sat(float16);
uchar16 __ovld __cnfn convert_uchar16_rte(char16);
uchar16 __ovld __cnfn convert_uchar16_sat_rte(char16);
uchar16 __ovld __cnfn convert_uchar16_rtz(char16);
uchar16 __ovld __cnfn convert_uchar16_sat_rtz(char16);
uchar16 __ovld __cnfn convert_uchar16_rtp(char16);
uchar16 __ovld __cnfn convert_uchar16_sat_rtp(char16);
uchar16 __ovld __cnfn convert_uchar16_rtn(char16);
uchar16 __ovld __cnfn convert_uchar16_sat_rtn(char16);
uchar16 __ovld __cnfn convert_uchar16(char16);
uchar16 __ovld __cnfn convert_uchar16_sat(char16);
uchar16 __ovld __cnfn convert_uchar16_rte(uchar16);
uchar16 __ovld __cnfn convert_uchar16_sat_rte(uchar16);
uchar16 __ovld __cnfn convert_uchar16_rtz(uchar16);
uchar16 __ovld __cnfn convert_uchar16_sat_rtz(uchar16);
uchar16 __ovld __cnfn convert_uchar16_rtp(uchar16);
uchar16 __ovld __cnfn convert_uchar16_sat_rtp(uchar16);
uchar16 __ovld __cnfn convert_uchar16_rtn(uchar16);
uchar16 __ovld __cnfn convert_uchar16_sat_rtn(uchar16);
uchar16 __ovld __cnfn convert_uchar16(uchar16);
uchar16 __ovld __cnfn convert_uchar16_sat(uchar16);
uchar16 __ovld __cnfn convert_uchar16_rte(short16);
uchar16 __ovld __cnfn convert_uchar16_sat_rte(short16);
uchar16 __ovld __cnfn convert_uchar16_rtz(short16);
uchar16 __ovld __cnfn convert_uchar16_sat_rtz(short16);
uchar16 __ovld __cnfn convert_uchar16_rtp(short16);
uchar16 __ovld __cnfn convert_uchar16_sat_rtp(short16);
uchar16 __ovld __cnfn convert_uchar16_rtn(short16);
uchar16 __ovld __cnfn convert_uchar16_sat_rtn(short16);
uchar16 __ovld __cnfn convert_uchar16(short16);
uchar16 __ovld __cnfn convert_uchar16_sat(short16);
uchar16 __ovld __cnfn convert_uchar16_rte(ushort16);
uchar16 __ovld __cnfn convert_uchar16_sat_rte(ushort16);
uchar16 __ovld __cnfn convert_uchar16_rtz(ushort16);
uchar16 __ovld __cnfn convert_uchar16_sat_rtz(ushort16);
uchar16 __ovld __cnfn convert_uchar16_rtp(ushort16);
uchar16 __ovld __cnfn convert_uchar16_sat_rtp(ushort16);
uchar16 __ovld __cnfn convert_uchar16_rtn(ushort16);
uchar16 __ovld __cnfn convert_uchar16_sat_rtn(ushort16);
uchar16 __ovld __cnfn convert_uchar16(ushort16);
uchar16 __ovld __cnfn convert_uchar16_sat(ushort16);
uchar16 __ovld __cnfn convert_uchar16_rte(int16);
uchar16 __ovld __cnfn convert_uchar16_sat_rte(int16);
uchar16 __ovld __cnfn convert_uchar16_rtz(int16);
uchar16 __ovld __cnfn convert_uchar16_sat_rtz(int16);
uchar16 __ovld __cnfn convert_uchar16_rtp(int16);
uchar16 __ovld __cnfn convert_uchar16_sat_rtp(int16);
uchar16 __ovld __cnfn convert_uchar16_rtn(int16);
uchar16 __ovld __cnfn convert_uchar16_sat_rtn(int16);
uchar16 __ovld __cnfn convert_uchar16(int16);
uchar16 __ovld __cnfn convert_uchar16_sat(int16);
uchar16 __ovld __cnfn convert_uchar16_rte(uint16);
uchar16 __ovld __cnfn convert_uchar16_sat_rte(uint16);
uchar16 __ovld __cnfn convert_uchar16_rtz(uint16);
uchar16 __ovld __cnfn convert_uchar16_sat_rtz(uint16);
uchar16 __ovld __cnfn convert_uchar16_rtp(uint16);
uchar16 __ovld __cnfn convert_uchar16_sat_rtp(uint16);
uchar16 __ovld __cnfn convert_uchar16_rtn(uint16);
uchar16 __ovld __cnfn convert_uchar16_sat_rtn(uint16);
uchar16 __ovld __cnfn convert_uchar16(uint16);
uchar16 __ovld __cnfn convert_uchar16_sat(uint16);
uchar16 __ovld __cnfn convert_uchar16_rte(long16);
uchar16 __ovld __cnfn convert_uchar16_sat_rte(long16);
uchar16 __ovld __cnfn convert_uchar16_rtz(long16);
uchar16 __ovld __cnfn convert_uchar16_sat_rtz(long16);
uchar16 __ovld __cnfn convert_uchar16_rtp(long16);
uchar16 __ovld __cnfn convert_uchar16_sat_rtp(long16);
uchar16 __ovld __cnfn convert_uchar16_rtn(long16);
uchar16 __ovld __cnfn convert_uchar16_sat_rtn(long16);
uchar16 __ovld __cnfn convert_uchar16(long16);
uchar16 __ovld __cnfn convert_uchar16_sat(long16);
uchar16 __ovld __cnfn convert_uchar16_rte(ulong16);
uchar16 __ovld __cnfn convert_uchar16_sat_rte(ulong16);
uchar16 __ovld __cnfn convert_uchar16_rtz(ulong16);
uchar16 __ovld __cnfn convert_uchar16_sat_rtz(ulong16);
uchar16 __ovld __cnfn convert_uchar16_rtp(ulong16);
uchar16 __ovld __cnfn convert_uchar16_sat_rtp(ulong16);
uchar16 __ovld __cnfn convert_uchar16_rtn(ulong16);
uchar16 __ovld __cnfn convert_uchar16_sat_rtn(ulong16);
uchar16 __ovld __cnfn convert_uchar16(ulong16);
uchar16 __ovld __cnfn convert_uchar16_sat(ulong16);
uchar16 __ovld __cnfn convert_uchar16_rte(float16);
uchar16 __ovld __cnfn convert_uchar16_sat_rte(float16);
uchar16 __ovld __cnfn convert_uchar16_rtz(float16);
uchar16 __ovld __cnfn convert_uchar16_sat_rtz(float16);
uchar16 __ovld __cnfn convert_uchar16_rtp(float16);
uchar16 __ovld __cnfn convert_uchar16_sat_rtp(float16);
uchar16 __ovld __cnfn convert_uchar16_rtn(float16);
uchar16 __ovld __cnfn convert_uchar16_sat_rtn(float16);
uchar16 __ovld __cnfn convert_uchar16(float16);
uchar16 __ovld __cnfn convert_uchar16_sat(float16);
short16 __ovld __cnfn convert_short16_rte(char16);
short16 __ovld __cnfn convert_short16_sat_rte(char16);
short16 __ovld __cnfn convert_short16_rtz(char16);
short16 __ovld __cnfn convert_short16_sat_rtz(char16);
short16 __ovld __cnfn convert_short16_rtp(char16);
short16 __ovld __cnfn convert_short16_sat_rtp(char16);
short16 __ovld __cnfn convert_short16_rtn(char16);
short16 __ovld __cnfn convert_short16_sat_rtn(char16);
short16 __ovld __cnfn convert_short16(char16);
short16 __ovld __cnfn convert_short16_sat(char16);
short16 __ovld __cnfn convert_short16_rte(uchar16);
short16 __ovld __cnfn convert_short16_sat_rte(uchar16);
short16 __ovld __cnfn convert_short16_rtz(uchar16);
short16 __ovld __cnfn convert_short16_sat_rtz(uchar16);
short16 __ovld __cnfn convert_short16_rtp(uchar16);
short16 __ovld __cnfn convert_short16_sat_rtp(uchar16);
short16 __ovld __cnfn convert_short16_rtn(uchar16);
short16 __ovld __cnfn convert_short16_sat_rtn(uchar16);
short16 __ovld __cnfn convert_short16(uchar16);
short16 __ovld __cnfn convert_short16_sat(uchar16);
short16 __ovld __cnfn convert_short16_rte(short16);
short16 __ovld __cnfn convert_short16_sat_rte(short16);
short16 __ovld __cnfn convert_short16_rtz(short16);
short16 __ovld __cnfn convert_short16_sat_rtz(short16);
short16 __ovld __cnfn convert_short16_rtp(short16);
short16 __ovld __cnfn convert_short16_sat_rtp(short16);
short16 __ovld __cnfn convert_short16_rtn(short16);
short16 __ovld __cnfn convert_short16_sat_rtn(short16);
short16 __ovld __cnfn convert_short16(short16);
short16 __ovld __cnfn convert_short16_sat(short16);
short16 __ovld __cnfn convert_short16_rte(ushort16);
short16 __ovld __cnfn convert_short16_sat_rte(ushort16);
short16 __ovld __cnfn convert_short16_rtz(ushort16);
short16 __ovld __cnfn convert_short16_sat_rtz(ushort16);
short16 __ovld __cnfn convert_short16_rtp(ushort16);
short16 __ovld __cnfn convert_short16_sat_rtp(ushort16);
short16 __ovld __cnfn convert_short16_rtn(ushort16);
short16 __ovld __cnfn convert_short16_sat_rtn(ushort16);
short16 __ovld __cnfn convert_short16(ushort16);
short16 __ovld __cnfn convert_short16_sat(ushort16);
short16 __ovld __cnfn convert_short16_rte(int16);
short16 __ovld __cnfn convert_short16_sat_rte(int16);
short16 __ovld __cnfn convert_short16_rtz(int16);
short16 __ovld __cnfn convert_short16_sat_rtz(int16);
short16 __ovld __cnfn convert_short16_rtp(int16);
short16 __ovld __cnfn convert_short16_sat_rtp(int16);
short16 __ovld __cnfn convert_short16_rtn(int16);
short16 __ovld __cnfn convert_short16_sat_rtn(int16);
short16 __ovld __cnfn convert_short16(int16);
short16 __ovld __cnfn convert_short16_sat(int16);
short16 __ovld __cnfn convert_short16_rte(uint16);
short16 __ovld __cnfn convert_short16_sat_rte(uint16);
short16 __ovld __cnfn convert_short16_rtz(uint16);
short16 __ovld __cnfn convert_short16_sat_rtz(uint16);
short16 __ovld __cnfn convert_short16_rtp(uint16);
short16 __ovld __cnfn convert_short16_sat_rtp(uint16);
short16 __ovld __cnfn convert_short16_rtn(uint16);
short16 __ovld __cnfn convert_short16_sat_rtn(uint16);
short16 __ovld __cnfn convert_short16(uint16);
short16 __ovld __cnfn convert_short16_sat(uint16);
short16 __ovld __cnfn convert_short16_rte(long16);
short16 __ovld __cnfn convert_short16_sat_rte(long16);
short16 __ovld __cnfn convert_short16_rtz(long16);
short16 __ovld __cnfn convert_short16_sat_rtz(long16);
short16 __ovld __cnfn convert_short16_rtp(long16);
short16 __ovld __cnfn convert_short16_sat_rtp(long16);
short16 __ovld __cnfn convert_short16_rtn(long16);
short16 __ovld __cnfn convert_short16_sat_rtn(long16);
short16 __ovld __cnfn convert_short16(long16);
short16 __ovld __cnfn convert_short16_sat(long16);
short16 __ovld __cnfn convert_short16_rte(ulong16);
short16 __ovld __cnfn convert_short16_sat_rte(ulong16);
short16 __ovld __cnfn convert_short16_rtz(ulong16);
short16 __ovld __cnfn convert_short16_sat_rtz(ulong16);
short16 __ovld __cnfn convert_short16_rtp(ulong16);
short16 __ovld __cnfn convert_short16_sat_rtp(ulong16);
short16 __ovld __cnfn convert_short16_rtn(ulong16);
short16 __ovld __cnfn convert_short16_sat_rtn(ulong16);
short16 __ovld __cnfn convert_short16(ulong16);
short16 __ovld __cnfn convert_short16_sat(ulong16);
short16 __ovld __cnfn convert_short16_rte(float16);
short16 __ovld __cnfn convert_short16_sat_rte(float16);
short16 __ovld __cnfn convert_short16_rtz(float16);
short16 __ovld __cnfn convert_short16_sat_rtz(float16);
short16 __ovld __cnfn convert_short16_rtp(float16);
short16 __ovld __cnfn convert_short16_sat_rtp(float16);
short16 __ovld __cnfn convert_short16_rtn(float16);
short16 __ovld __cnfn convert_short16_sat_rtn(float16);
short16 __ovld __cnfn convert_short16(float16);
short16 __ovld __cnfn convert_short16_sat(float16);
ushort16 __ovld __cnfn convert_ushort16_rte(char16);
ushort16 __ovld __cnfn convert_ushort16_sat_rte(char16);
ushort16 __ovld __cnfn convert_ushort16_rtz(char16);
ushort16 __ovld __cnfn convert_ushort16_sat_rtz(char16);
ushort16 __ovld __cnfn convert_ushort16_rtp(char16);
ushort16 __ovld __cnfn convert_ushort16_sat_rtp(char16);
ushort16 __ovld __cnfn convert_ushort16_rtn(char16);
ushort16 __ovld __cnfn convert_ushort16_sat_rtn(char16);
ushort16 __ovld __cnfn convert_ushort16(char16);
ushort16 __ovld __cnfn convert_ushort16_sat(char16);
ushort16 __ovld __cnfn convert_ushort16_rte(uchar16);
ushort16 __ovld __cnfn convert_ushort16_sat_rte(uchar16);
ushort16 __ovld __cnfn convert_ushort16_rtz(uchar16);
ushort16 __ovld __cnfn convert_ushort16_sat_rtz(uchar16);
ushort16 __ovld __cnfn convert_ushort16_rtp(uchar16);
ushort16 __ovld __cnfn convert_ushort16_sat_rtp(uchar16);
ushort16 __ovld __cnfn convert_ushort16_rtn(uchar16);
ushort16 __ovld __cnfn convert_ushort16_sat_rtn(uchar16);
ushort16 __ovld __cnfn convert_ushort16(uchar16);
ushort16 __ovld __cnfn convert_ushort16_sat(uchar16);
ushort16 __ovld __cnfn convert_ushort16_rte(short16);
ushort16 __ovld __cnfn convert_ushort16_sat_rte(short16);
ushort16 __ovld __cnfn convert_ushort16_rtz(short16);
ushort16 __ovld __cnfn convert_ushort16_sat_rtz(short16);
ushort16 __ovld __cnfn convert_ushort16_rtp(short16);
ushort16 __ovld __cnfn convert_ushort16_sat_rtp(short16);
ushort16 __ovld __cnfn convert_ushort16_rtn(short16);
ushort16 __ovld __cnfn convert_ushort16_sat_rtn(short16);
ushort16 __ovld __cnfn convert_ushort16(short16);
ushort16 __ovld __cnfn convert_ushort16_sat(short16);
ushort16 __ovld __cnfn convert_ushort16_rte(ushort16);
ushort16 __ovld __cnfn convert_ushort16_sat_rte(ushort16);
ushort16 __ovld __cnfn convert_ushort16_rtz(ushort16);
ushort16 __ovld __cnfn convert_ushort16_sat_rtz(ushort16);
ushort16 __ovld __cnfn convert_ushort16_rtp(ushort16);
ushort16 __ovld __cnfn convert_ushort16_sat_rtp(ushort16);
ushort16 __ovld __cnfn convert_ushort16_rtn(ushort16);
ushort16 __ovld __cnfn convert_ushort16_sat_rtn(ushort16);
ushort16 __ovld __cnfn convert_ushort16(ushort16);
ushort16 __ovld __cnfn convert_ushort16_sat(ushort16);
ushort16 __ovld __cnfn convert_ushort16_rte(int16);
ushort16 __ovld __cnfn convert_ushort16_sat_rte(int16);
ushort16 __ovld __cnfn convert_ushort16_rtz(int16);
ushort16 __ovld __cnfn convert_ushort16_sat_rtz(int16);
ushort16 __ovld __cnfn convert_ushort16_rtp(int16);
ushort16 __ovld __cnfn convert_ushort16_sat_rtp(int16);
ushort16 __ovld __cnfn convert_ushort16_rtn(int16);
ushort16 __ovld __cnfn convert_ushort16_sat_rtn(int16);
ushort16 __ovld __cnfn convert_ushort16(int16);
ushort16 __ovld __cnfn convert_ushort16_sat(int16);
ushort16 __ovld __cnfn convert_ushort16_rte(uint16);
ushort16 __ovld __cnfn convert_ushort16_sat_rte(uint16);
ushort16 __ovld __cnfn convert_ushort16_rtz(uint16);
ushort16 __ovld __cnfn convert_ushort16_sat_rtz(uint16);
ushort16 __ovld __cnfn convert_ushort16_rtp(uint16);
ushort16 __ovld __cnfn convert_ushort16_sat_rtp(uint16);
ushort16 __ovld __cnfn convert_ushort16_rtn(uint16);
ushort16 __ovld __cnfn convert_ushort16_sat_rtn(uint16);
ushort16 __ovld __cnfn convert_ushort16(uint16);
ushort16 __ovld __cnfn convert_ushort16_sat(uint16);
ushort16 __ovld __cnfn convert_ushort16_rte(long16);
ushort16 __ovld __cnfn convert_ushort16_sat_rte(long16);
ushort16 __ovld __cnfn convert_ushort16_rtz(long16);
ushort16 __ovld __cnfn convert_ushort16_sat_rtz(long16);
ushort16 __ovld __cnfn convert_ushort16_rtp(long16);
ushort16 __ovld __cnfn convert_ushort16_sat_rtp(long16);
ushort16 __ovld __cnfn convert_ushort16_rtn(long16);
ushort16 __ovld __cnfn convert_ushort16_sat_rtn(long16);
ushort16 __ovld __cnfn convert_ushort16(long16);
ushort16 __ovld __cnfn convert_ushort16_sat(long16);
ushort16 __ovld __cnfn convert_ushort16_rte(ulong16);
ushort16 __ovld __cnfn convert_ushort16_sat_rte(ulong16);
ushort16 __ovld __cnfn convert_ushort16_rtz(ulong16);
ushort16 __ovld __cnfn convert_ushort16_sat_rtz(ulong16);
ushort16 __ovld __cnfn convert_ushort16_rtp(ulong16);
ushort16 __ovld __cnfn convert_ushort16_sat_rtp(ulong16);
ushort16 __ovld __cnfn convert_ushort16_rtn(ulong16);
ushort16 __ovld __cnfn convert_ushort16_sat_rtn(ulong16);
ushort16 __ovld __cnfn convert_ushort16(ulong16);
ushort16 __ovld __cnfn convert_ushort16_sat(ulong16);
ushort16 __ovld __cnfn convert_ushort16_rte(float16);
ushort16 __ovld __cnfn convert_ushort16_sat_rte(float16);
ushort16 __ovld __cnfn convert_ushort16_rtz(float16);
ushort16 __ovld __cnfn convert_ushort16_sat_rtz(float16);
ushort16 __ovld __cnfn convert_ushort16_rtp(float16);
ushort16 __ovld __cnfn convert_ushort16_sat_rtp(float16);
ushort16 __ovld __cnfn convert_ushort16_rtn(float16);
ushort16 __ovld __cnfn convert_ushort16_sat_rtn(float16);
ushort16 __ovld __cnfn convert_ushort16(float16);
ushort16 __ovld __cnfn convert_ushort16_sat(float16);
int16 __ovld __cnfn convert_int16_rte(char16);
int16 __ovld __cnfn convert_int16_sat_rte(char16);
int16 __ovld __cnfn convert_int16_rtz(char16);
int16 __ovld __cnfn convert_int16_sat_rtz(char16);
int16 __ovld __cnfn convert_int16_rtp(char16);
int16 __ovld __cnfn convert_int16_sat_rtp(char16);
int16 __ovld __cnfn convert_int16_rtn(char16);
int16 __ovld __cnfn convert_int16_sat_rtn(char16);
int16 __ovld __cnfn convert_int16(char16);
int16 __ovld __cnfn convert_int16_sat(char16);
int16 __ovld __cnfn convert_int16_rte(uchar16);
int16 __ovld __cnfn convert_int16_sat_rte(uchar16);
int16 __ovld __cnfn convert_int16_rtz(uchar16);
int16 __ovld __cnfn convert_int16_sat_rtz(uchar16);
int16 __ovld __cnfn convert_int16_rtp(uchar16);
int16 __ovld __cnfn convert_int16_sat_rtp(uchar16);
int16 __ovld __cnfn convert_int16_rtn(uchar16);
int16 __ovld __cnfn convert_int16_sat_rtn(uchar16);
int16 __ovld __cnfn convert_int16(uchar16);
int16 __ovld __cnfn convert_int16_sat(uchar16);
int16 __ovld __cnfn convert_int16_rte(short16);
int16 __ovld __cnfn convert_int16_sat_rte(short16);
int16 __ovld __cnfn convert_int16_rtz(short16);
int16 __ovld __cnfn convert_int16_sat_rtz(short16);
int16 __ovld __cnfn convert_int16_rtp(short16);
int16 __ovld __cnfn convert_int16_sat_rtp(short16);
int16 __ovld __cnfn convert_int16_rtn(short16);
int16 __ovld __cnfn convert_int16_sat_rtn(short16);
int16 __ovld __cnfn convert_int16(short16);
int16 __ovld __cnfn convert_int16_sat(short16);
int16 __ovld __cnfn convert_int16_rte(ushort16);
int16 __ovld __cnfn convert_int16_sat_rte(ushort16);
int16 __ovld __cnfn convert_int16_rtz(ushort16);
int16 __ovld __cnfn convert_int16_sat_rtz(ushort16);
int16 __ovld __cnfn convert_int16_rtp(ushort16);
int16 __ovld __cnfn convert_int16_sat_rtp(ushort16);
int16 __ovld __cnfn convert_int16_rtn(ushort16);
int16 __ovld __cnfn convert_int16_sat_rtn(ushort16);
int16 __ovld __cnfn convert_int16(ushort16);
int16 __ovld __cnfn convert_int16_sat(ushort16);
int16 __ovld __cnfn convert_int16_rte(int16);
int16 __ovld __cnfn convert_int16_sat_rte(int16);
int16 __ovld __cnfn convert_int16_rtz(int16);
int16 __ovld __cnfn convert_int16_sat_rtz(int16);
int16 __ovld __cnfn convert_int16_rtp(int16);
int16 __ovld __cnfn convert_int16_sat_rtp(int16);
int16 __ovld __cnfn convert_int16_rtn(int16);
int16 __ovld __cnfn convert_int16_sat_rtn(int16);
int16 __ovld __cnfn convert_int16(int16);
int16 __ovld __cnfn convert_int16_sat(int16);
int16 __ovld __cnfn convert_int16_rte(uint16);
int16 __ovld __cnfn convert_int16_sat_rte(uint16);
int16 __ovld __cnfn convert_int16_rtz(uint16);
int16 __ovld __cnfn convert_int16_sat_rtz(uint16);
int16 __ovld __cnfn convert_int16_rtp(uint16);
int16 __ovld __cnfn convert_int16_sat_rtp(uint16);
int16 __ovld __cnfn convert_int16_rtn(uint16);
int16 __ovld __cnfn convert_int16_sat_rtn(uint16);
int16 __ovld __cnfn convert_int16(uint16);
int16 __ovld __cnfn convert_int16_sat(uint16);
int16 __ovld __cnfn convert_int16_rte(long16);
int16 __ovld __cnfn convert_int16_sat_rte(long16);
int16 __ovld __cnfn convert_int16_rtz(long16);
int16 __ovld __cnfn convert_int16_sat_rtz(long16);
int16 __ovld __cnfn convert_int16_rtp(long16);
int16 __ovld __cnfn convert_int16_sat_rtp(long16);
int16 __ovld __cnfn convert_int16_rtn(long16);
int16 __ovld __cnfn convert_int16_sat_rtn(long16);
int16 __ovld __cnfn convert_int16(long16);
int16 __ovld __cnfn convert_int16_sat(long16);
int16 __ovld __cnfn convert_int16_rte(ulong16);
int16 __ovld __cnfn convert_int16_sat_rte(ulong16);
int16 __ovld __cnfn convert_int16_rtz(ulong16);
int16 __ovld __cnfn convert_int16_sat_rtz(ulong16);
int16 __ovld __cnfn convert_int16_rtp(ulong16);
int16 __ovld __cnfn convert_int16_sat_rtp(ulong16);
int16 __ovld __cnfn convert_int16_rtn(ulong16);
int16 __ovld __cnfn convert_int16_sat_rtn(ulong16);
int16 __ovld __cnfn convert_int16(ulong16);
int16 __ovld __cnfn convert_int16_sat(ulong16);
int16 __ovld __cnfn convert_int16_rte(float16);
int16 __ovld __cnfn convert_int16_sat_rte(float16);
int16 __ovld __cnfn convert_int16_rtz(float16);
int16 __ovld __cnfn convert_int16_sat_rtz(float16);
int16 __ovld __cnfn convert_int16_rtp(float16);
int16 __ovld __cnfn convert_int16_sat_rtp(float16);
int16 __ovld __cnfn convert_int16_rtn(float16);
int16 __ovld __cnfn convert_int16_sat_rtn(float16);
int16 __ovld __cnfn convert_int16(float16);
int16 __ovld __cnfn convert_int16_sat(float16);
uint16 __ovld __cnfn convert_uint16_rte(char16);
uint16 __ovld __cnfn convert_uint16_sat_rte(char16);
uint16 __ovld __cnfn convert_uint16_rtz(char16);
uint16 __ovld __cnfn convert_uint16_sat_rtz(char16);
uint16 __ovld __cnfn convert_uint16_rtp(char16);
uint16 __ovld __cnfn convert_uint16_sat_rtp(char16);
uint16 __ovld __cnfn convert_uint16_rtn(char16);
uint16 __ovld __cnfn convert_uint16_sat_rtn(char16);
uint16 __ovld __cnfn convert_uint16(char16);
uint16 __ovld __cnfn convert_uint16_sat(char16);
uint16 __ovld __cnfn convert_uint16_rte(uchar16);
uint16 __ovld __cnfn convert_uint16_sat_rte(uchar16);
uint16 __ovld __cnfn convert_uint16_rtz(uchar16);
uint16 __ovld __cnfn convert_uint16_sat_rtz(uchar16);
uint16 __ovld __cnfn convert_uint16_rtp(uchar16);
uint16 __ovld __cnfn convert_uint16_sat_rtp(uchar16);
uint16 __ovld __cnfn convert_uint16_rtn(uchar16);
uint16 __ovld __cnfn convert_uint16_sat_rtn(uchar16);
uint16 __ovld __cnfn convert_uint16(uchar16);
uint16 __ovld __cnfn convert_uint16_sat(uchar16);
uint16 __ovld __cnfn convert_uint16_rte(short16);
uint16 __ovld __cnfn convert_uint16_sat_rte(short16);
uint16 __ovld __cnfn convert_uint16_rtz(short16);
uint16 __ovld __cnfn convert_uint16_sat_rtz(short16);
uint16 __ovld __cnfn convert_uint16_rtp(short16);
uint16 __ovld __cnfn convert_uint16_sat_rtp(short16);
uint16 __ovld __cnfn convert_uint16_rtn(short16);
uint16 __ovld __cnfn convert_uint16_sat_rtn(short16);
uint16 __ovld __cnfn convert_uint16(short16);
uint16 __ovld __cnfn convert_uint16_sat(short16);
uint16 __ovld __cnfn convert_uint16_rte(ushort16);
uint16 __ovld __cnfn convert_uint16_sat_rte(ushort16);
uint16 __ovld __cnfn convert_uint16_rtz(ushort16);
uint16 __ovld __cnfn convert_uint16_sat_rtz(ushort16);
uint16 __ovld __cnfn convert_uint16_rtp(ushort16);
uint16 __ovld __cnfn convert_uint16_sat_rtp(ushort16);
uint16 __ovld __cnfn convert_uint16_rtn(ushort16);
uint16 __ovld __cnfn convert_uint16_sat_rtn(ushort16);
uint16 __ovld __cnfn convert_uint16(ushort16);
uint16 __ovld __cnfn convert_uint16_sat(ushort16);
uint16 __ovld __cnfn convert_uint16_rte(int16);
uint16 __ovld __cnfn convert_uint16_sat_rte(int16);
uint16 __ovld __cnfn convert_uint16_rtz(int16);
uint16 __ovld __cnfn convert_uint16_sat_rtz(int16);
uint16 __ovld __cnfn convert_uint16_rtp(int16);
uint16 __ovld __cnfn convert_uint16_sat_rtp(int16);
uint16 __ovld __cnfn convert_uint16_rtn(int16);
uint16 __ovld __cnfn convert_uint16_sat_rtn(int16);
uint16 __ovld __cnfn convert_uint16(int16);
uint16 __ovld __cnfn convert_uint16_sat(int16);
uint16 __ovld __cnfn convert_uint16_rte(uint16);
uint16 __ovld __cnfn convert_uint16_sat_rte(uint16);
uint16 __ovld __cnfn convert_uint16_rtz(uint16);
uint16 __ovld __cnfn convert_uint16_sat_rtz(uint16);
uint16 __ovld __cnfn convert_uint16_rtp(uint16);
uint16 __ovld __cnfn convert_uint16_sat_rtp(uint16);
uint16 __ovld __cnfn convert_uint16_rtn(uint16);
uint16 __ovld __cnfn convert_uint16_sat_rtn(uint16);
uint16 __ovld __cnfn convert_uint16(uint16);
uint16 __ovld __cnfn convert_uint16_sat(uint16);
uint16 __ovld __cnfn convert_uint16_rte(long16);
uint16 __ovld __cnfn convert_uint16_sat_rte(long16);
uint16 __ovld __cnfn convert_uint16_rtz(long16);
uint16 __ovld __cnfn convert_uint16_sat_rtz(long16);
uint16 __ovld __cnfn convert_uint16_rtp(long16);
uint16 __ovld __cnfn convert_uint16_sat_rtp(long16);
uint16 __ovld __cnfn convert_uint16_rtn(long16);
uint16 __ovld __cnfn convert_uint16_sat_rtn(long16);
uint16 __ovld __cnfn convert_uint16(long16);
uint16 __ovld __cnfn convert_uint16_sat(long16);
uint16 __ovld __cnfn convert_uint16_rte(ulong16);
uint16 __ovld __cnfn convert_uint16_sat_rte(ulong16);
uint16 __ovld __cnfn convert_uint16_rtz(ulong16);
uint16 __ovld __cnfn convert_uint16_sat_rtz(ulong16);
uint16 __ovld __cnfn convert_uint16_rtp(ulong16);
uint16 __ovld __cnfn convert_uint16_sat_rtp(ulong16);
uint16 __ovld __cnfn convert_uint16_rtn(ulong16);
uint16 __ovld __cnfn convert_uint16_sat_rtn(ulong16);
uint16 __ovld __cnfn convert_uint16(ulong16);
uint16 __ovld __cnfn convert_uint16_sat(ulong16);
uint16 __ovld __cnfn convert_uint16_rte(float16);
uint16 __ovld __cnfn convert_uint16_sat_rte(float16);
uint16 __ovld __cnfn convert_uint16_rtz(float16);
uint16 __ovld __cnfn convert_uint16_sat_rtz(float16);
uint16 __ovld __cnfn convert_uint16_rtp(float16);
uint16 __ovld __cnfn convert_uint16_sat_rtp(float16);
uint16 __ovld __cnfn convert_uint16_rtn(float16);
uint16 __ovld __cnfn convert_uint16_sat_rtn(float16);
uint16 __ovld __cnfn convert_uint16(float16);
uint16 __ovld __cnfn convert_uint16_sat(float16);
long16 __ovld __cnfn convert_long16_rte(char16);
long16 __ovld __cnfn convert_long16_sat_rte(char16);
long16 __ovld __cnfn convert_long16_rtz(char16);
long16 __ovld __cnfn convert_long16_sat_rtz(char16);
long16 __ovld __cnfn convert_long16_rtp(char16);
long16 __ovld __cnfn convert_long16_sat_rtp(char16);
long16 __ovld __cnfn convert_long16_rtn(char16);
long16 __ovld __cnfn convert_long16_sat_rtn(char16);
long16 __ovld __cnfn convert_long16(char16);
long16 __ovld __cnfn convert_long16_sat(char16);
long16 __ovld __cnfn convert_long16_rte(uchar16);
long16 __ovld __cnfn convert_long16_sat_rte(uchar16);
long16 __ovld __cnfn convert_long16_rtz(uchar16);
long16 __ovld __cnfn convert_long16_sat_rtz(uchar16);
long16 __ovld __cnfn convert_long16_rtp(uchar16);
long16 __ovld __cnfn convert_long16_sat_rtp(uchar16);
long16 __ovld __cnfn convert_long16_rtn(uchar16);
long16 __ovld __cnfn convert_long16_sat_rtn(uchar16);
long16 __ovld __cnfn convert_long16(uchar16);
long16 __ovld __cnfn convert_long16_sat(uchar16);
long16 __ovld __cnfn convert_long16_rte(short16);
long16 __ovld __cnfn convert_long16_sat_rte(short16);
long16 __ovld __cnfn convert_long16_rtz(short16);
long16 __ovld __cnfn convert_long16_sat_rtz(short16);
long16 __ovld __cnfn convert_long16_rtp(short16);
long16 __ovld __cnfn convert_long16_sat_rtp(short16);
long16 __ovld __cnfn convert_long16_rtn(short16);
long16 __ovld __cnfn convert_long16_sat_rtn(short16);
long16 __ovld __cnfn convert_long16(short16);
long16 __ovld __cnfn convert_long16_sat(short16);
long16 __ovld __cnfn convert_long16_rte(ushort16);
long16 __ovld __cnfn convert_long16_sat_rte(ushort16);
long16 __ovld __cnfn convert_long16_rtz(ushort16);
long16 __ovld __cnfn convert_long16_sat_rtz(ushort16);
long16 __ovld __cnfn convert_long16_rtp(ushort16);
long16 __ovld __cnfn convert_long16_sat_rtp(ushort16);
long16 __ovld __cnfn convert_long16_rtn(ushort16);
long16 __ovld __cnfn convert_long16_sat_rtn(ushort16);
long16 __ovld __cnfn convert_long16(ushort16);
long16 __ovld __cnfn convert_long16_sat(ushort16);
long16 __ovld __cnfn convert_long16_rte(int16);
long16 __ovld __cnfn convert_long16_sat_rte(int16);
long16 __ovld __cnfn convert_long16_rtz(int16);
long16 __ovld __cnfn convert_long16_sat_rtz(int16);
long16 __ovld __cnfn convert_long16_rtp(int16);
long16 __ovld __cnfn convert_long16_sat_rtp(int16);
long16 __ovld __cnfn convert_long16_rtn(int16);
long16 __ovld __cnfn convert_long16_sat_rtn(int16);
long16 __ovld __cnfn convert_long16(int16);
long16 __ovld __cnfn convert_long16_sat(int16);
long16 __ovld __cnfn convert_long16_rte(uint16);
long16 __ovld __cnfn convert_long16_sat_rte(uint16);
long16 __ovld __cnfn convert_long16_rtz(uint16);
long16 __ovld __cnfn convert_long16_sat_rtz(uint16);
long16 __ovld __cnfn convert_long16_rtp(uint16);
long16 __ovld __cnfn convert_long16_sat_rtp(uint16);
long16 __ovld __cnfn convert_long16_rtn(uint16);
long16 __ovld __cnfn convert_long16_sat_rtn(uint16);
long16 __ovld __cnfn convert_long16(uint16);
long16 __ovld __cnfn convert_long16_sat(uint16);
long16 __ovld __cnfn convert_long16_rte(long16);
long16 __ovld __cnfn convert_long16_sat_rte(long16);
long16 __ovld __cnfn convert_long16_rtz(long16);
long16 __ovld __cnfn convert_long16_sat_rtz(long16);
long16 __ovld __cnfn convert_long16_rtp(long16);
long16 __ovld __cnfn convert_long16_sat_rtp(long16);
long16 __ovld __cnfn convert_long16_rtn(long16);
long16 __ovld __cnfn convert_long16_sat_rtn(long16);
long16 __ovld __cnfn convert_long16(long16);
long16 __ovld __cnfn convert_long16_sat(long16);
long16 __ovld __cnfn convert_long16_rte(ulong16);
long16 __ovld __cnfn convert_long16_sat_rte(ulong16);
long16 __ovld __cnfn convert_long16_rtz(ulong16);
long16 __ovld __cnfn convert_long16_sat_rtz(ulong16);
long16 __ovld __cnfn convert_long16_rtp(ulong16);
long16 __ovld __cnfn convert_long16_sat_rtp(ulong16);
long16 __ovld __cnfn convert_long16_rtn(ulong16);
long16 __ovld __cnfn convert_long16_sat_rtn(ulong16);
long16 __ovld __cnfn convert_long16(ulong16);
long16 __ovld __cnfn convert_long16_sat(ulong16);
long16 __ovld __cnfn convert_long16_rte(float16);
long16 __ovld __cnfn convert_long16_sat_rte(float16);
long16 __ovld __cnfn convert_long16_rtz(float16);
long16 __ovld __cnfn convert_long16_sat_rtz(float16);
long16 __ovld __cnfn convert_long16_rtp(float16);
long16 __ovld __cnfn convert_long16_sat_rtp(float16);
long16 __ovld __cnfn convert_long16_rtn(float16);
long16 __ovld __cnfn convert_long16_sat_rtn(float16);
long16 __ovld __cnfn convert_long16(float16);
long16 __ovld __cnfn convert_long16_sat(float16);
ulong16 __ovld __cnfn convert_ulong16_rte(char16);
ulong16 __ovld __cnfn convert_ulong16_sat_rte(char16);
ulong16 __ovld __cnfn convert_ulong16_rtz(char16);
ulong16 __ovld __cnfn convert_ulong16_sat_rtz(char16);
ulong16 __ovld __cnfn convert_ulong16_rtp(char16);
ulong16 __ovld __cnfn convert_ulong16_sat_rtp(char16);
ulong16 __ovld __cnfn convert_ulong16_rtn(char16);
ulong16 __ovld __cnfn convert_ulong16_sat_rtn(char16);
ulong16 __ovld __cnfn convert_ulong16(char16);
ulong16 __ovld __cnfn convert_ulong16_sat(char16);
ulong16 __ovld __cnfn convert_ulong16_rte(uchar16);
ulong16 __ovld __cnfn convert_ulong16_sat_rte(uchar16);
ulong16 __ovld __cnfn convert_ulong16_rtz(uchar16);
ulong16 __ovld __cnfn convert_ulong16_sat_rtz(uchar16);
ulong16 __ovld __cnfn convert_ulong16_rtp(uchar16);
ulong16 __ovld __cnfn convert_ulong16_sat_rtp(uchar16);
ulong16 __ovld __cnfn convert_ulong16_rtn(uchar16);
ulong16 __ovld __cnfn convert_ulong16_sat_rtn(uchar16);
ulong16 __ovld __cnfn convert_ulong16(uchar16);
ulong16 __ovld __cnfn convert_ulong16_sat(uchar16);
ulong16 __ovld __cnfn convert_ulong16_rte(short16);
ulong16 __ovld __cnfn convert_ulong16_sat_rte(short16);
ulong16 __ovld __cnfn convert_ulong16_rtz(short16);
ulong16 __ovld __cnfn convert_ulong16_sat_rtz(short16);
ulong16 __ovld __cnfn convert_ulong16_rtp(short16);
ulong16 __ovld __cnfn convert_ulong16_sat_rtp(short16);
ulong16 __ovld __cnfn convert_ulong16_rtn(short16);
ulong16 __ovld __cnfn convert_ulong16_sat_rtn(short16);
ulong16 __ovld __cnfn convert_ulong16(short16);
ulong16 __ovld __cnfn convert_ulong16_sat(short16);
ulong16 __ovld __cnfn convert_ulong16_rte(ushort16);
ulong16 __ovld __cnfn convert_ulong16_sat_rte(ushort16);
ulong16 __ovld __cnfn convert_ulong16_rtz(ushort16);
ulong16 __ovld __cnfn convert_ulong16_sat_rtz(ushort16);
ulong16 __ovld __cnfn convert_ulong16_rtp(ushort16);
ulong16 __ovld __cnfn convert_ulong16_sat_rtp(ushort16);
ulong16 __ovld __cnfn convert_ulong16_rtn(ushort16);
ulong16 __ovld __cnfn convert_ulong16_sat_rtn(ushort16);
ulong16 __ovld __cnfn convert_ulong16(ushort16);
ulong16 __ovld __cnfn convert_ulong16_sat(ushort16);
ulong16 __ovld __cnfn convert_ulong16_rte(int16);
ulong16 __ovld __cnfn convert_ulong16_sat_rte(int16);
ulong16 __ovld __cnfn convert_ulong16_rtz(int16);
ulong16 __ovld __cnfn convert_ulong16_sat_rtz(int16);
ulong16 __ovld __cnfn convert_ulong16_rtp(int16);
ulong16 __ovld __cnfn convert_ulong16_sat_rtp(int16);
ulong16 __ovld __cnfn convert_ulong16_rtn(int16);
ulong16 __ovld __cnfn convert_ulong16_sat_rtn(int16);
ulong16 __ovld __cnfn convert_ulong16(int16);
ulong16 __ovld __cnfn convert_ulong16_sat(int16);
ulong16 __ovld __cnfn convert_ulong16_rte(uint16);
ulong16 __ovld __cnfn convert_ulong16_sat_rte(uint16);
ulong16 __ovld __cnfn convert_ulong16_rtz(uint16);
ulong16 __ovld __cnfn convert_ulong16_sat_rtz(uint16);
ulong16 __ovld __cnfn convert_ulong16_rtp(uint16);
ulong16 __ovld __cnfn convert_ulong16_sat_rtp(uint16);
ulong16 __ovld __cnfn convert_ulong16_rtn(uint16);
ulong16 __ovld __cnfn convert_ulong16_sat_rtn(uint16);
ulong16 __ovld __cnfn convert_ulong16(uint16);
ulong16 __ovld __cnfn convert_ulong16_sat(uint16);
ulong16 __ovld __cnfn convert_ulong16_rte(long16);
ulong16 __ovld __cnfn convert_ulong16_sat_rte(long16);
ulong16 __ovld __cnfn convert_ulong16_rtz(long16);
ulong16 __ovld __cnfn convert_ulong16_sat_rtz(long16);
ulong16 __ovld __cnfn convert_ulong16_rtp(long16);
ulong16 __ovld __cnfn convert_ulong16_sat_rtp(long16);
ulong16 __ovld __cnfn convert_ulong16_rtn(long16);
ulong16 __ovld __cnfn convert_ulong16_sat_rtn(long16);
ulong16 __ovld __cnfn convert_ulong16(long16);
ulong16 __ovld __cnfn convert_ulong16_sat(long16);
ulong16 __ovld __cnfn convert_ulong16_rte(ulong16);
ulong16 __ovld __cnfn convert_ulong16_sat_rte(ulong16);
ulong16 __ovld __cnfn convert_ulong16_rtz(ulong16);
ulong16 __ovld __cnfn convert_ulong16_sat_rtz(ulong16);
ulong16 __ovld __cnfn convert_ulong16_rtp(ulong16);
ulong16 __ovld __cnfn convert_ulong16_sat_rtp(ulong16);
ulong16 __ovld __cnfn convert_ulong16_rtn(ulong16);
ulong16 __ovld __cnfn convert_ulong16_sat_rtn(ulong16);
ulong16 __ovld __cnfn convert_ulong16(ulong16);
ulong16 __ovld __cnfn convert_ulong16_sat(ulong16);
ulong16 __ovld __cnfn convert_ulong16_rte(float16);
ulong16 __ovld __cnfn convert_ulong16_sat_rte(float16);
ulong16 __ovld __cnfn convert_ulong16_rtz(float16);
ulong16 __ovld __cnfn convert_ulong16_sat_rtz(float16);
ulong16 __ovld __cnfn convert_ulong16_rtp(float16);
ulong16 __ovld __cnfn convert_ulong16_sat_rtp(float16);
ulong16 __ovld __cnfn convert_ulong16_rtn(float16);
ulong16 __ovld __cnfn convert_ulong16_sat_rtn(float16);
ulong16 __ovld __cnfn convert_ulong16(float16);
ulong16 __ovld __cnfn convert_ulong16_sat(float16);
float16 __ovld __cnfn convert_float16_rte(char16);
float16 __ovld __cnfn convert_float16_rtz(char16);
float16 __ovld __cnfn convert_float16_rtp(char16);
float16 __ovld __cnfn convert_float16_rtn(char16);
float16 __ovld __cnfn convert_float16(char16);
float16 __ovld __cnfn convert_float16_rte(uchar16);
float16 __ovld __cnfn convert_float16_rtz(uchar16);
float16 __ovld __cnfn convert_float16_rtp(uchar16);
float16 __ovld __cnfn convert_float16_rtn(uchar16);
float16 __ovld __cnfn convert_float16(uchar16);
float16 __ovld __cnfn convert_float16_rte(short16);
float16 __ovld __cnfn convert_float16_rtz(short16);
float16 __ovld __cnfn convert_float16_rtp(short16);
float16 __ovld __cnfn convert_float16_rtn(short16);
float16 __ovld __cnfn convert_float16(short16);
float16 __ovld __cnfn convert_float16_rte(ushort16);
float16 __ovld __cnfn convert_float16_rtz(ushort16);
float16 __ovld __cnfn convert_float16_rtp(ushort16);
float16 __ovld __cnfn convert_float16_rtn(ushort16);
float16 __ovld __cnfn convert_float16(ushort16);
float16 __ovld __cnfn convert_float16_rte(int16);
float16 __ovld __cnfn convert_float16_rtz(int16);
float16 __ovld __cnfn convert_float16_rtp(int16);
float16 __ovld __cnfn convert_float16_rtn(int16);
float16 __ovld __cnfn convert_float16(int16);
float16 __ovld __cnfn convert_float16_rte(uint16);
float16 __ovld __cnfn convert_float16_rtz(uint16);
float16 __ovld __cnfn convert_float16_rtp(uint16);
float16 __ovld __cnfn convert_float16_rtn(uint16);
float16 __ovld __cnfn convert_float16(uint16);
float16 __ovld __cnfn convert_float16_rte(long16);
float16 __ovld __cnfn convert_float16_rtz(long16);
float16 __ovld __cnfn convert_float16_rtp(long16);
float16 __ovld __cnfn convert_float16_rtn(long16);
float16 __ovld __cnfn convert_float16(long16);
float16 __ovld __cnfn convert_float16_rte(ulong16);
float16 __ovld __cnfn convert_float16_rtz(ulong16);
float16 __ovld __cnfn convert_float16_rtp(ulong16);
float16 __ovld __cnfn convert_float16_rtn(ulong16);
float16 __ovld __cnfn convert_float16(ulong16);
float16 __ovld __cnfn convert_float16_rte(float16);
float16 __ovld __cnfn convert_float16_rtz(float16);
float16 __ovld __cnfn convert_float16_rtp(float16);
float16 __ovld __cnfn convert_float16_rtn(float16);
float16 __ovld __cnfn convert_float16(float16);

// Conversions with double data type parameters or return value.

#ifdef cl_khr_fp64
#pragma OPENCL EXTENSION cl_khr_fp64 : enable
char __ovld __cnfn convert_char(double);
char __ovld __cnfn convert_char_rte(double);
char __ovld __cnfn convert_char_rtn(double);
char __ovld __cnfn convert_char_rtp(double);
char __ovld __cnfn convert_char_rtz(double);
char __ovld __cnfn convert_char_sat(double);
char __ovld __cnfn convert_char_sat_rte(double);
char __ovld __cnfn convert_char_sat_rtn(double);
char __ovld __cnfn convert_char_sat_rtp(double);
char __ovld __cnfn convert_char_sat_rtz(double);
char2 __ovld __cnfn convert_char2(double2);
char2 __ovld __cnfn convert_char2_rte(double2);
char2 __ovld __cnfn convert_char2_rtn(double2);
char2 __ovld __cnfn convert_char2_rtp(double2);
char2 __ovld __cnfn convert_char2_rtz(double2);
char2 __ovld __cnfn convert_char2_sat(double2);
char2 __ovld __cnfn convert_char2_sat_rte(double2);
char2 __ovld __cnfn convert_char2_sat_rtn(double2);
char2 __ovld __cnfn convert_char2_sat_rtp(double2);
char2 __ovld __cnfn convert_char2_sat_rtz(double2);
char3 __ovld __cnfn convert_char3(double3);
char3 __ovld __cnfn convert_char3_rte(double3);
char3 __ovld __cnfn convert_char3_rtn(double3);
char3 __ovld __cnfn convert_char3_rtp(double3);
char3 __ovld __cnfn convert_char3_rtz(double3);
char3 __ovld __cnfn convert_char3_sat(double3);
char3 __ovld __cnfn convert_char3_sat_rte(double3);
char3 __ovld __cnfn convert_char3_sat_rtn(double3);
char3 __ovld __cnfn convert_char3_sat_rtp(double3);
char3 __ovld __cnfn convert_char3_sat_rtz(double3);
char4 __ovld __cnfn convert_char4(double4);
char4 __ovld __cnfn convert_char4_rte(double4);
char4 __ovld __cnfn convert_char4_rtn(double4);
char4 __ovld __cnfn convert_char4_rtp(double4);
char4 __ovld __cnfn convert_char4_rtz(double4);
char4 __ovld __cnfn convert_char4_sat(double4);
char4 __ovld __cnfn convert_char4_sat_rte(double4);
char4 __ovld __cnfn convert_char4_sat_rtn(double4);
char4 __ovld __cnfn convert_char4_sat_rtp(double4);
char4 __ovld __cnfn convert_char4_sat_rtz(double4);
char8 __ovld __cnfn convert_char8(double8);
char8 __ovld __cnfn convert_char8_rte(double8);
char8 __ovld __cnfn convert_char8_rtn(double8);
char8 __ovld __cnfn convert_char8_rtp(double8);
char8 __ovld __cnfn convert_char8_rtz(double8);
char8 __ovld __cnfn convert_char8_sat(double8);
char8 __ovld __cnfn convert_char8_sat_rte(double8);
char8 __ovld __cnfn convert_char8_sat_rtn(double8);
char8 __ovld __cnfn convert_char8_sat_rtp(double8);
char8 __ovld __cnfn convert_char8_sat_rtz(double8);
char16 __ovld __cnfn convert_char16(double16);
char16 __ovld __cnfn convert_char16_rte(double16);
char16 __ovld __cnfn convert_char16_rtn(double16);
char16 __ovld __cnfn convert_char16_rtp(double16);
char16 __ovld __cnfn convert_char16_rtz(double16);
char16 __ovld __cnfn convert_char16_sat(double16);
char16 __ovld __cnfn convert_char16_sat_rte(double16);
char16 __ovld __cnfn convert_char16_sat_rtn(double16);
char16 __ovld __cnfn convert_char16_sat_rtp(double16);
char16 __ovld __cnfn convert_char16_sat_rtz(double16);

uchar __ovld __cnfn convert_uchar(double);
uchar __ovld __cnfn convert_uchar_rte(double);
uchar __ovld __cnfn convert_uchar_rtn(double);
uchar __ovld __cnfn convert_uchar_rtp(double);
uchar __ovld __cnfn convert_uchar_rtz(double);
uchar __ovld __cnfn convert_uchar_sat(double);
uchar __ovld __cnfn convert_uchar_sat_rte(double);
uchar __ovld __cnfn convert_uchar_sat_rtn(double);
uchar __ovld __cnfn convert_uchar_sat_rtp(double);
uchar __ovld __cnfn convert_uchar_sat_rtz(double);
uchar2 __ovld __cnfn convert_uchar2(double2);
uchar2 __ovld __cnfn convert_uchar2_rte(double2);
uchar2 __ovld __cnfn convert_uchar2_rtn(double2);
uchar2 __ovld __cnfn convert_uchar2_rtp(double2);
uchar2 __ovld __cnfn convert_uchar2_rtz(double2);
uchar2 __ovld __cnfn convert_uchar2_sat(double2);
uchar2 __ovld __cnfn convert_uchar2_sat_rte(double2);
uchar2 __ovld __cnfn convert_uchar2_sat_rtn(double2);
uchar2 __ovld __cnfn convert_uchar2_sat_rtp(double2);
uchar2 __ovld __cnfn convert_uchar2_sat_rtz(double2);
uchar3 __ovld __cnfn convert_uchar3(double3);
uchar3 __ovld __cnfn convert_uchar3_rte(double3);
uchar3 __ovld __cnfn convert_uchar3_rtn(double3);
uchar3 __ovld __cnfn convert_uchar3_rtp(double3);
uchar3 __ovld __cnfn convert_uchar3_rtz(double3);
uchar3 __ovld __cnfn convert_uchar3_sat(double3);
uchar3 __ovld __cnfn convert_uchar3_sat_rte(double3);
uchar3 __ovld __cnfn convert_uchar3_sat_rtn(double3);
uchar3 __ovld __cnfn convert_uchar3_sat_rtp(double3);
uchar3 __ovld __cnfn convert_uchar3_sat_rtz(double3);
uchar4 __ovld __cnfn convert_uchar4(double4);
uchar4 __ovld __cnfn convert_uchar4_rte(double4);
uchar4 __ovld __cnfn convert_uchar4_rtn(double4);
uchar4 __ovld __cnfn convert_uchar4_rtp(double4);
uchar4 __ovld __cnfn convert_uchar4_rtz(double4);
uchar4 __ovld __cnfn convert_uchar4_sat(double4);
uchar4 __ovld __cnfn convert_uchar4_sat_rte(double4);
uchar4 __ovld __cnfn convert_uchar4_sat_rtn(double4);
uchar4 __ovld __cnfn convert_uchar4_sat_rtp(double4);
uchar4 __ovld __cnfn convert_uchar4_sat_rtz(double4);
uchar8 __ovld __cnfn convert_uchar8(double8);
uchar8 __ovld __cnfn convert_uchar8_rte(double8);
uchar8 __ovld __cnfn convert_uchar8_rtn(double8);
uchar8 __ovld __cnfn convert_uchar8_rtp(double8);
uchar8 __ovld __cnfn convert_uchar8_rtz(double8);
uchar8 __ovld __cnfn convert_uchar8_sat(double8);
uchar8 __ovld __cnfn convert_uchar8_sat_rte(double8);
uchar8 __ovld __cnfn convert_uchar8_sat_rtn(double8);
uchar8 __ovld __cnfn convert_uchar8_sat_rtp(double8);
uchar8 __ovld __cnfn convert_uchar8_sat_rtz(double8);
uchar16 __ovld __cnfn convert_uchar16(double16);
uchar16 __ovld __cnfn convert_uchar16_rte(double16);
uchar16 __ovld __cnfn convert_uchar16_rtn(double16);
uchar16 __ovld __cnfn convert_uchar16_rtp(double16);
uchar16 __ovld __cnfn convert_uchar16_rtz(double16);
uchar16 __ovld __cnfn convert_uchar16_sat(double16);
uchar16 __ovld __cnfn convert_uchar16_sat_rte(double16);
uchar16 __ovld __cnfn convert_uchar16_sat_rtn(double16);
uchar16 __ovld __cnfn convert_uchar16_sat_rtp(double16);
uchar16 __ovld __cnfn convert_uchar16_sat_rtz(double16);

short __ovld __cnfn convert_short(double);
short __ovld __cnfn convert_short_rte(double);
short __ovld __cnfn convert_short_rtn(double);
short __ovld __cnfn convert_short_rtp(double);
short __ovld __cnfn convert_short_rtz(double);
short __ovld __cnfn convert_short_sat(double);
short __ovld __cnfn convert_short_sat_rte(double);
short __ovld __cnfn convert_short_sat_rtn(double);
short __ovld __cnfn convert_short_sat_rtp(double);
short __ovld __cnfn convert_short_sat_rtz(double);
short2 __ovld __cnfn convert_short2(double2);
short2 __ovld __cnfn convert_short2_rte(double2);
short2 __ovld __cnfn convert_short2_rtn(double2);
short2 __ovld __cnfn convert_short2_rtp(double2);
short2 __ovld __cnfn convert_short2_rtz(double2);
short2 __ovld __cnfn convert_short2_sat(double2);
short2 __ovld __cnfn convert_short2_sat_rte(double2);
short2 __ovld __cnfn convert_short2_sat_rtn(double2);
short2 __ovld __cnfn convert_short2_sat_rtp(double2);
short2 __ovld __cnfn convert_short2_sat_rtz(double2);
short3 __ovld __cnfn convert_short3(double3);
short3 __ovld __cnfn convert_short3_rte(double3);
short3 __ovld __cnfn convert_short3_rtn(double3);
short3 __ovld __cnfn convert_short3_rtp(double3);
short3 __ovld __cnfn convert_short3_rtz(double3);
short3 __ovld __cnfn convert_short3_sat(double3);
short3 __ovld __cnfn convert_short3_sat_rte(double3);
short3 __ovld __cnfn convert_short3_sat_rtn(double3);
short3 __ovld __cnfn convert_short3_sat_rtp(double3);
short3 __ovld __cnfn convert_short3_sat_rtz(double3);
short4 __ovld __cnfn convert_short4(double4);
short4 __ovld __cnfn convert_short4_rte(double4);
short4 __ovld __cnfn convert_short4_rtn(double4);
short4 __ovld __cnfn convert_short4_rtp(double4);
short4 __ovld __cnfn convert_short4_rtz(double4);
short4 __ovld __cnfn convert_short4_sat(double4);
short4 __ovld __cnfn convert_short4_sat_rte(double4);
short4 __ovld __cnfn convert_short4_sat_rtn(double4);
short4 __ovld __cnfn convert_short4_sat_rtp(double4);
short4 __ovld __cnfn convert_short4_sat_rtz(double4);
short8 __ovld __cnfn convert_short8(double8);
short8 __ovld __cnfn convert_short8_rte(double8);
short8 __ovld __cnfn convert_short8_rtn(double8);
short8 __ovld __cnfn convert_short8_rtp(double8);
short8 __ovld __cnfn convert_short8_rtz(double8);
short8 __ovld __cnfn convert_short8_sat(double8);
short8 __ovld __cnfn convert_short8_sat_rte(double8);
short8 __ovld __cnfn convert_short8_sat_rtn(double8);
short8 __ovld __cnfn convert_short8_sat_rtp(double8);
short8 __ovld __cnfn convert_short8_sat_rtz(double8);
short16 __ovld __cnfn convert_short16(double16);
short16 __ovld __cnfn convert_short16_rte(double16);
short16 __ovld __cnfn convert_short16_rtn(double16);
short16 __ovld __cnfn convert_short16_rtp(double16);
short16 __ovld __cnfn convert_short16_rtz(double16);
short16 __ovld __cnfn convert_short16_sat(double16);
short16 __ovld __cnfn convert_short16_sat_rte(double16);
short16 __ovld __cnfn convert_short16_sat_rtn(double16);
short16 __ovld __cnfn convert_short16_sat_rtp(double16);
short16 __ovld __cnfn convert_short16_sat_rtz(double16);

ushort __ovld __cnfn convert_ushort(double);
ushort __ovld __cnfn convert_ushort_rte(double);
ushort __ovld __cnfn convert_ushort_rtn(double);
ushort __ovld __cnfn convert_ushort_rtp(double);
ushort __ovld __cnfn convert_ushort_rtz(double);
ushort __ovld __cnfn convert_ushort_sat(double);
ushort __ovld __cnfn convert_ushort_sat_rte(double);
ushort __ovld __cnfn convert_ushort_sat_rtn(double);
ushort __ovld __cnfn convert_ushort_sat_rtp(double);
ushort __ovld __cnfn convert_ushort_sat_rtz(double);
ushort2 __ovld __cnfn convert_ushort2(double2);
ushort2 __ovld __cnfn convert_ushort2_rte(double2);
ushort2 __ovld __cnfn convert_ushort2_rtn(double2);
ushort2 __ovld __cnfn convert_ushort2_rtp(double2);
ushort2 __ovld __cnfn convert_ushort2_rtz(double2);
ushort2 __ovld __cnfn convert_ushort2_sat(double2);
ushort2 __ovld __cnfn convert_ushort2_sat_rte(double2);
ushort2 __ovld __cnfn convert_ushort2_sat_rtn(double2);
ushort2 __ovld __cnfn convert_ushort2_sat_rtp(double2);
ushort2 __ovld __cnfn convert_ushort2_sat_rtz(double2);
ushort3 __ovld __cnfn convert_ushort3(double3);
ushort3 __ovld __cnfn convert_ushort3_rte(double3);
ushort3 __ovld __cnfn convert_ushort3_rtn(double3);
ushort3 __ovld __cnfn convert_ushort3_rtp(double3);
ushort3 __ovld __cnfn convert_ushort3_rtz(double3);
ushort3 __ovld __cnfn convert_ushort3_sat(double3);
ushort3 __ovld __cnfn convert_ushort3_sat_rte(double3);
ushort3 __ovld __cnfn convert_ushort3_sat_rtn(double3);
ushort3 __ovld __cnfn convert_ushort3_sat_rtp(double3);
ushort3 __ovld __cnfn convert_ushort3_sat_rtz(double3);
ushort4 __ovld __cnfn convert_ushort4(double4);
ushort4 __ovld __cnfn convert_ushort4_rte(double4);
ushort4 __ovld __cnfn convert_ushort4_rtn(double4);
ushort4 __ovld __cnfn convert_ushort4_rtp(double4);
ushort4 __ovld __cnfn convert_ushort4_rtz(double4);
ushort4 __ovld __cnfn convert_ushort4_sat(double4);
ushort4 __ovld __cnfn convert_ushort4_sat_rte(double4);
ushort4 __ovld __cnfn convert_ushort4_sat_rtn(double4);
ushort4 __ovld __cnfn convert_ushort4_sat_rtp(double4);
ushort4 __ovld __cnfn convert_ushort4_sat_rtz(double4);
ushort8 __ovld __cnfn convert_ushort8(double8);
ushort8 __ovld __cnfn convert_ushort8_rte(double8);
ushort8 __ovld __cnfn convert_ushort8_rtn(double8);
ushort8 __ovld __cnfn convert_ushort8_rtp(double8);
ushort8 __ovld __cnfn convert_ushort8_rtz(double8);
ushort8 __ovld __cnfn convert_ushort8_sat(double8);
ushort8 __ovld __cnfn convert_ushort8_sat_rte(double8);
ushort8 __ovld __cnfn convert_ushort8_sat_rtn(double8);
ushort8 __ovld __cnfn convert_ushort8_sat_rtp(double8);
ushort8 __ovld __cnfn convert_ushort8_sat_rtz(double8);
ushort16 __ovld __cnfn convert_ushort16(double16);
ushort16 __ovld __cnfn convert_ushort16_rte(double16);
ushort16 __ovld __cnfn convert_ushort16_rtn(double16);
ushort16 __ovld __cnfn convert_ushort16_rtp(double16);
ushort16 __ovld __cnfn convert_ushort16_rtz(double16);
ushort16 __ovld __cnfn convert_ushort16_sat(double16);
ushort16 __ovld __cnfn convert_ushort16_sat_rte(double16);
ushort16 __ovld __cnfn convert_ushort16_sat_rtn(double16);
ushort16 __ovld __cnfn convert_ushort16_sat_rtp(double16);
ushort16 __ovld __cnfn convert_ushort16_sat_rtz(double16);

int __ovld __cnfn convert_int(double);
int __ovld __cnfn convert_int_rte(double);
int __ovld __cnfn convert_int_rtn(double);
int __ovld __cnfn convert_int_rtp(double);
int __ovld __cnfn convert_int_rtz(double);
int __ovld __cnfn convert_int_sat(double);
int __ovld __cnfn convert_int_sat_rte(double);
int __ovld __cnfn convert_int_sat_rtn(double);
int __ovld __cnfn convert_int_sat_rtp(double);
int __ovld __cnfn convert_int_sat_rtz(double);
int2 __ovld __cnfn convert_int2(double2);
int2 __ovld __cnfn convert_int2_rte(double2);
int2 __ovld __cnfn convert_int2_rtn(double2);
int2 __ovld __cnfn convert_int2_rtp(double2);
int2 __ovld __cnfn convert_int2_rtz(double2);
int2 __ovld __cnfn convert_int2_sat(double2);
int2 __ovld __cnfn convert_int2_sat_rte(double2);
int2 __ovld __cnfn convert_int2_sat_rtn(double2);
int2 __ovld __cnfn convert_int2_sat_rtp(double2);
int2 __ovld __cnfn convert_int2_sat_rtz(double2);
int3 __ovld __cnfn convert_int3(double3);
int3 __ovld __cnfn convert_int3_rte(double3);
int3 __ovld __cnfn convert_int3_rtn(double3);
int3 __ovld __cnfn convert_int3_rtp(double3);
int3 __ovld __cnfn convert_int3_rtz(double3);
int3 __ovld __cnfn convert_int3_sat(double3);
int3 __ovld __cnfn convert_int3_sat_rte(double3);
int3 __ovld __cnfn convert_int3_sat_rtn(double3);
int3 __ovld __cnfn convert_int3_sat_rtp(double3);
int3 __ovld __cnfn convert_int3_sat_rtz(double3);
int4 __ovld __cnfn convert_int4(double4);
int4 __ovld __cnfn convert_int4_rte(double4);
int4 __ovld __cnfn convert_int4_rtn(double4);
int4 __ovld __cnfn convert_int4_rtp(double4);
int4 __ovld __cnfn convert_int4_rtz(double4);
int4 __ovld __cnfn convert_int4_sat(double4);
int4 __ovld __cnfn convert_int4_sat_rte(double4);
int4 __ovld __cnfn convert_int4_sat_rtn(double4);
int4 __ovld __cnfn convert_int4_sat_rtp(double4);
int4 __ovld __cnfn convert_int4_sat_rtz(double4);
int8 __ovld __cnfn convert_int8(double8);
int8 __ovld __cnfn convert_int8_rte(double8);
int8 __ovld __cnfn convert_int8_rtn(double8);
int8 __ovld __cnfn convert_int8_rtp(double8);
int8 __ovld __cnfn convert_int8_rtz(double8);
int8 __ovld __cnfn convert_int8_sat(double8);
int8 __ovld __cnfn convert_int8_sat_rte(double8);
int8 __ovld __cnfn convert_int8_sat_rtn(double8);
int8 __ovld __cnfn convert_int8_sat_rtp(double8);
int8 __ovld __cnfn convert_int8_sat_rtz(double8);
int16 __ovld __cnfn convert_int16(double16);
int16 __ovld __cnfn convert_int16_rte(double16);
int16 __ovld __cnfn convert_int16_rtn(double16);
int16 __ovld __cnfn convert_int16_rtp(double16);
int16 __ovld __cnfn convert_int16_rtz(double16);
int16 __ovld __cnfn convert_int16_sat(double16);
int16 __ovld __cnfn convert_int16_sat_rte(double16);
int16 __ovld __cnfn convert_int16_sat_rtn(double16);
int16 __ovld __cnfn convert_int16_sat_rtp(double16);
int16 __ovld __cnfn convert_int16_sat_rtz(double16);

uint __ovld __cnfn convert_uint(double);
uint __ovld __cnfn convert_uint_rte(double);
uint __ovld __cnfn convert_uint_rtn(double);
uint __ovld __cnfn convert_uint_rtp(double);
uint __ovld __cnfn convert_uint_rtz(double);
uint __ovld __cnfn convert_uint_sat(double);
uint __ovld __cnfn convert_uint_sat_rte(double);
uint __ovld __cnfn convert_uint_sat_rtn(double);
uint __ovld __cnfn convert_uint_sat_rtp(double);
uint __ovld __cnfn convert_uint_sat_rtz(double);
uint2 __ovld __cnfn convert_uint2(double2);
uint2 __ovld __cnfn convert_uint2_rte(double2);
uint2 __ovld __cnfn convert_uint2_rtn(double2);
uint2 __ovld __cnfn convert_uint2_rtp(double2);
uint2 __ovld __cnfn convert_uint2_rtz(double2);
uint2 __ovld __cnfn convert_uint2_sat(double2);
uint2 __ovld __cnfn convert_uint2_sat_rte(double2);
uint2 __ovld __cnfn convert_uint2_sat_rtn(double2);
uint2 __ovld __cnfn convert_uint2_sat_rtp(double2);
uint2 __ovld __cnfn convert_uint2_sat_rtz(double2);
uint3 __ovld __cnfn convert_uint3(double3);
uint3 __ovld __cnfn convert_uint3_rte(double3);
uint3 __ovld __cnfn convert_uint3_rtn(double3);
uint3 __ovld __cnfn convert_uint3_rtp(double3);
uint3 __ovld __cnfn convert_uint3_rtz(double3);
uint3 __ovld __cnfn convert_uint3_sat(double3);
uint3 __ovld __cnfn convert_uint3_sat_rte(double3);
uint3 __ovld __cnfn convert_uint3_sat_rtn(double3);
uint3 __ovld __cnfn convert_uint3_sat_rtp(double3);
uint3 __ovld __cnfn convert_uint3_sat_rtz(double3);
uint4 __ovld __cnfn convert_uint4(double4);
uint4 __ovld __cnfn convert_uint4_rte(double4);
uint4 __ovld __cnfn convert_uint4_rtn(double4);
uint4 __ovld __cnfn convert_uint4_rtp(double4);
uint4 __ovld __cnfn convert_uint4_rtz(double4);
uint4 __ovld __cnfn convert_uint4_sat(double4);
uint4 __ovld __cnfn convert_uint4_sat_rte(double4);
uint4 __ovld __cnfn convert_uint4_sat_rtn(double4);
uint4 __ovld __cnfn convert_uint4_sat_rtp(double4);
uint4 __ovld __cnfn convert_uint4_sat_rtz(double4);
uint8 __ovld __cnfn convert_uint8(double8);
uint8 __ovld __cnfn convert_uint8_rte(double8);
uint8 __ovld __cnfn convert_uint8_rtn(double8);
uint8 __ovld __cnfn convert_uint8_rtp(double8);
uint8 __ovld __cnfn convert_uint8_rtz(double8);
uint8 __ovld __cnfn convert_uint8_sat(double8);
uint8 __ovld __cnfn convert_uint8_sat_rte(double8);
uint8 __ovld __cnfn convert_uint8_sat_rtn(double8);
uint8 __ovld __cnfn convert_uint8_sat_rtp(double8);
uint8 __ovld __cnfn convert_uint8_sat_rtz(double8);
uint16 __ovld __cnfn convert_uint16(double16);
uint16 __ovld __cnfn convert_uint16_rte(double16);
uint16 __ovld __cnfn convert_uint16_rtn(double16);
uint16 __ovld __cnfn convert_uint16_rtp(double16);
uint16 __ovld __cnfn convert_uint16_rtz(double16);
uint16 __ovld __cnfn convert_uint16_sat(double16);
uint16 __ovld __cnfn convert_uint16_sat_rte(double16);
uint16 __ovld __cnfn convert_uint16_sat_rtn(double16);
uint16 __ovld __cnfn convert_uint16_sat_rtp(double16);
uint16 __ovld __cnfn convert_uint16_sat_rtz(double16);

long __ovld __cnfn convert_long(double);
long __ovld __cnfn convert_long_rte(double);
long __ovld __cnfn convert_long_rtn(double);
long __ovld __cnfn convert_long_rtp(double);
long __ovld __cnfn convert_long_rtz(double);
long __ovld __cnfn convert_long_sat(double);
long __ovld __cnfn convert_long_sat_rte(double);
long __ovld __cnfn convert_long_sat_rtn(double);
long __ovld __cnfn convert_long_sat_rtp(double);
long __ovld __cnfn convert_long_sat_rtz(double);
long2 __ovld __cnfn convert_long2(double2);
long2 __ovld __cnfn convert_long2_rte(double2);
long2 __ovld __cnfn convert_long2_rtn(double2);
long2 __ovld __cnfn convert_long2_rtp(double2);
long2 __ovld __cnfn convert_long2_rtz(double2);
long2 __ovld __cnfn convert_long2_sat(double2);
long2 __ovld __cnfn convert_long2_sat_rte(double2);
long2 __ovld __cnfn convert_long2_sat_rtn(double2);
long2 __ovld __cnfn convert_long2_sat_rtp(double2);
long2 __ovld __cnfn convert_long2_sat_rtz(double2);
long3 __ovld __cnfn convert_long3(double3);
long3 __ovld __cnfn convert_long3_rte(double3);
long3 __ovld __cnfn convert_long3_rtn(double3);
long3 __ovld __cnfn convert_long3_rtp(double3);
long3 __ovld __cnfn convert_long3_rtz(double3);
long3 __ovld __cnfn convert_long3_sat(double3);
long3 __ovld __cnfn convert_long3_sat_rte(double3);
long3 __ovld __cnfn convert_long3_sat_rtn(double3);
long3 __ovld __cnfn convert_long3_sat_rtp(double3);
long3 __ovld __cnfn convert_long3_sat_rtz(double3);
long4 __ovld __cnfn convert_long4(double4);
long4 __ovld __cnfn convert_long4_rte(double4);
long4 __ovld __cnfn convert_long4_rtn(double4);
long4 __ovld __cnfn convert_long4_rtp(double4);
long4 __ovld __cnfn convert_long4_rtz(double4);
long4 __ovld __cnfn convert_long4_sat(double4);
long4 __ovld __cnfn convert_long4_sat_rte(double4);
long4 __ovld __cnfn convert_long4_sat_rtn(double4);
long4 __ovld __cnfn convert_long4_sat_rtp(double4);
long4 __ovld __cnfn convert_long4_sat_rtz(double4);
long8 __ovld __cnfn convert_long8(double8);
long8 __ovld __cnfn convert_long8_rte(double8);
long8 __ovld __cnfn convert_long8_rtn(double8);
long8 __ovld __cnfn convert_long8_rtp(double8);
long8 __ovld __cnfn convert_long8_rtz(double8);
long8 __ovld __cnfn convert_long8_sat(double8);
long8 __ovld __cnfn convert_long8_sat_rte(double8);
long8 __ovld __cnfn convert_long8_sat_rtn(double8);
long8 __ovld __cnfn convert_long8_sat_rtp(double8);
long8 __ovld __cnfn convert_long8_sat_rtz(double8);
long16 __ovld __cnfn convert_long16(double16);
long16 __ovld __cnfn convert_long16_rte(double16);
long16 __ovld __cnfn convert_long16_rtn(double16);
long16 __ovld __cnfn convert_long16_rtp(double16);
long16 __ovld __cnfn convert_long16_rtz(double16);
long16 __ovld __cnfn convert_long16_sat(double16);
long16 __ovld __cnfn convert_long16_sat_rte(double16);
long16 __ovld __cnfn convert_long16_sat_rtn(double16);
long16 __ovld __cnfn convert_long16_sat_rtp(double16);
long16 __ovld __cnfn convert_long16_sat_rtz(double16);

ulong __ovld __cnfn convert_ulong(double);
ulong __ovld __cnfn convert_ulong_rte(double);
ulong __ovld __cnfn convert_ulong_rtn(double);
ulong __ovld __cnfn convert_ulong_rtp(double);
ulong __ovld __cnfn convert_ulong_rtz(double);
ulong __ovld __cnfn convert_ulong_sat(double);
ulong __ovld __cnfn convert_ulong_sat_rte(double);
ulong __ovld __cnfn convert_ulong_sat_rtn(double);
ulong __ovld __cnfn convert_ulong_sat_rtp(double);
ulong __ovld __cnfn convert_ulong_sat_rtz(double);
ulong2 __ovld __cnfn convert_ulong2(double2);
ulong2 __ovld __cnfn convert_ulong2_rte(double2);
ulong2 __ovld __cnfn convert_ulong2_rtn(double2);
ulong2 __ovld __cnfn convert_ulong2_rtp(double2);
ulong2 __ovld __cnfn convert_ulong2_rtz(double2);
ulong2 __ovld __cnfn convert_ulong2_sat(double2);
ulong2 __ovld __cnfn convert_ulong2_sat_rte(double2);
ulong2 __ovld __cnfn convert_ulong2_sat_rtn(double2);
ulong2 __ovld __cnfn convert_ulong2_sat_rtp(double2);
ulong2 __ovld __cnfn convert_ulong2_sat_rtz(double2);
ulong3 __ovld __cnfn convert_ulong3(double3);
ulong3 __ovld __cnfn convert_ulong3_rte(double3);
ulong3 __ovld __cnfn convert_ulong3_rtn(double3);
ulong3 __ovld __cnfn convert_ulong3_rtp(double3);
ulong3 __ovld __cnfn convert_ulong3_rtz(double3);
ulong3 __ovld __cnfn convert_ulong3_sat(double3);
ulong3 __ovld __cnfn convert_ulong3_sat_rte(double3);
ulong3 __ovld __cnfn convert_ulong3_sat_rtn(double3);
ulong3 __ovld __cnfn convert_ulong3_sat_rtp(double3);
ulong3 __ovld __cnfn convert_ulong3_sat_rtz(double3);
ulong4 __ovld __cnfn convert_ulong4(double4);
ulong4 __ovld __cnfn convert_ulong4_rte(double4);
ulong4 __ovld __cnfn convert_ulong4_rtn(double4);
ulong4 __ovld __cnfn convert_ulong4_rtp(double4);
ulong4 __ovld __cnfn convert_ulong4_rtz(double4);
ulong4 __ovld __cnfn convert_ulong4_sat(double4);
ulong4 __ovld __cnfn convert_ulong4_sat_rte(double4);
ulong4 __ovld __cnfn convert_ulong4_sat_rtn(double4);
ulong4 __ovld __cnfn convert_ulong4_sat_rtp(double4);
ulong4 __ovld __cnfn convert_ulong4_sat_rtz(double4);
ulong8 __ovld __cnfn convert_ulong8(double8);
ulong8 __ovld __cnfn convert_ulong8_rte(double8);
ulong8 __ovld __cnfn convert_ulong8_rtn(double8);
ulong8 __ovld __cnfn convert_ulong8_rtp(double8);
ulong8 __ovld __cnfn convert_ulong8_rtz(double8);
ulong8 __ovld __cnfn convert_ulong8_sat(double8);
ulong8 __ovld __cnfn convert_ulong8_sat_rte(double8);
ulong8 __ovld __cnfn convert_ulong8_sat_rtn(double8);
ulong8 __ovld __cnfn convert_ulong8_sat_rtp(double8);
ulong8 __ovld __cnfn convert_ulong8_sat_rtz(double8);
ulong16 __ovld __cnfn convert_ulong16(double16);
ulong16 __ovld __cnfn convert_ulong16_rte(double16);
ulong16 __ovld __cnfn convert_ulong16_rtn(double16);
ulong16 __ovld __cnfn convert_ulong16_rtp(double16);
ulong16 __ovld __cnfn convert_ulong16_rtz(double16);
ulong16 __ovld __cnfn convert_ulong16_sat(double16);
ulong16 __ovld __cnfn convert_ulong16_sat_rte(double16);
ulong16 __ovld __cnfn convert_ulong16_sat_rtn(double16);
ulong16 __ovld __cnfn convert_ulong16_sat_rtp(double16);
ulong16 __ovld __cnfn convert_ulong16_sat_rtz(double16);

float __ovld __cnfn convert_float(double);
float __ovld __cnfn convert_float_rte(double);
float __ovld __cnfn convert_float_rtn(double);
float __ovld __cnfn convert_float_rtp(double);
float __ovld __cnfn convert_float_rtz(double);
float2 __ovld __cnfn convert_float2(double2);
float2 __ovld __cnfn convert_float2_rte(double2);
float2 __ovld __cnfn convert_float2_rtn(double2);
float2 __ovld __cnfn convert_float2_rtp(double2);
float2 __ovld __cnfn convert_float2_rtz(double2);
float3 __ovld __cnfn convert_float3(double3);
float3 __ovld __cnfn convert_float3_rte(double3);
float3 __ovld __cnfn convert_float3_rtn(double3);
float3 __ovld __cnfn convert_float3_rtp(double3);
float3 __ovld __cnfn convert_float3_rtz(double3);
float4 __ovld __cnfn convert_float4(double4);
float4 __ovld __cnfn convert_float4_rte(double4);
float4 __ovld __cnfn convert_float4_rtn(double4);
float4 __ovld __cnfn convert_float4_rtp(double4);
float4 __ovld __cnfn convert_float4_rtz(double4);
float8 __ovld __cnfn convert_float8(double8);
float8 __ovld __cnfn convert_float8_rte(double8);
float8 __ovld __cnfn convert_float8_rtn(double8);
float8 __ovld __cnfn convert_float8_rtp(double8);
float8 __ovld __cnfn convert_float8_rtz(double8);
float16 __ovld __cnfn convert_float16(double16);
float16 __ovld __cnfn convert_float16_rte(double16);
float16 __ovld __cnfn convert_float16_rtn(double16);
float16 __ovld __cnfn convert_float16_rtp(double16);
float16 __ovld __cnfn convert_float16_rtz(double16);

double __ovld __cnfn convert_double(char);
double __ovld __cnfn convert_double(double);
double __ovld __cnfn convert_double(float);
double __ovld __cnfn convert_double(int);
double __ovld __cnfn convert_double(long);
double __ovld __cnfn convert_double(short);
double __ovld __cnfn convert_double(uchar);
double __ovld __cnfn convert_double(uint);
double __ovld __cnfn convert_double(ulong);
double __ovld __cnfn convert_double(ushort);
double __ovld __cnfn convert_double_rte(char);
double __ovld __cnfn convert_double_rte(double);
double __ovld __cnfn convert_double_rte(float);
double __ovld __cnfn convert_double_rte(int);
double __ovld __cnfn convert_double_rte(long);
double __ovld __cnfn convert_double_rte(short);
double __ovld __cnfn convert_double_rte(uchar);
double __ovld __cnfn convert_double_rte(uint);
double __ovld __cnfn convert_double_rte(ulong);
double __ovld __cnfn convert_double_rte(ushort);
double __ovld __cnfn convert_double_rtn(char);
double __ovld __cnfn convert_double_rtn(double);
double __ovld __cnfn convert_double_rtn(float);
double __ovld __cnfn convert_double_rtn(int);
double __ovld __cnfn convert_double_rtn(long);
double __ovld __cnfn convert_double_rtn(short);
double __ovld __cnfn convert_double_rtn(uchar);
double __ovld __cnfn convert_double_rtn(uint);
double __ovld __cnfn convert_double_rtn(ulong);
double __ovld __cnfn convert_double_rtn(ushort);
double __ovld __cnfn convert_double_rtp(char);
double __ovld __cnfn convert_double_rtp(double);
double __ovld __cnfn convert_double_rtp(float);
double __ovld __cnfn convert_double_rtp(int);
double __ovld __cnfn convert_double_rtp(long);
double __ovld __cnfn convert_double_rtp(short);
double __ovld __cnfn convert_double_rtp(uchar);
double __ovld __cnfn convert_double_rtp(uint);
double __ovld __cnfn convert_double_rtp(ulong);
double __ovld __cnfn convert_double_rtp(ushort);
double __ovld __cnfn convert_double_rtz(char);
double __ovld __cnfn convert_double_rtz(double);
double __ovld __cnfn convert_double_rtz(float);
double __ovld __cnfn convert_double_rtz(int);
double __ovld __cnfn convert_double_rtz(long);
double __ovld __cnfn convert_double_rtz(short);
double __ovld __cnfn convert_double_rtz(uchar);
double __ovld __cnfn convert_double_rtz(uint);
double __ovld __cnfn convert_double_rtz(ulong);
double __ovld __cnfn convert_double_rtz(ushort);
double2 __ovld __cnfn convert_double2(char2);
double2 __ovld __cnfn convert_double2(double2);
double2 __ovld __cnfn convert_double2(float2);
double2 __ovld __cnfn convert_double2(int2);
double2 __ovld __cnfn convert_double2(long2);
double2 __ovld __cnfn convert_double2(short2);
double2 __ovld __cnfn convert_double2(uchar2);
double2 __ovld __cnfn convert_double2(uint2);
double2 __ovld __cnfn convert_double2(ulong2);
double2 __ovld __cnfn convert_double2(ushort2);
double2 __ovld __cnfn convert_double2_rte(char2);
double2 __ovld __cnfn convert_double2_rte(double2);
double2 __ovld __cnfn convert_double2_rte(float2);
double2 __ovld __cnfn convert_double2_rte(int2);
double2 __ovld __cnfn convert_double2_rte(long2);
double2 __ovld __cnfn convert_double2_rte(short2);
double2 __ovld __cnfn convert_double2_rte(uchar2);
double2 __ovld __cnfn convert_double2_rte(uint2);
double2 __ovld __cnfn convert_double2_rte(ulong2);
double2 __ovld __cnfn convert_double2_rte(ushort2);
double2 __ovld __cnfn convert_double2_rtn(char2);
double2 __ovld __cnfn convert_double2_rtn(double2);
double2 __ovld __cnfn convert_double2_rtn(float2);
double2 __ovld __cnfn convert_double2_rtn(int2);
double2 __ovld __cnfn convert_double2_rtn(long2);
double2 __ovld __cnfn convert_double2_rtn(short2);
double2 __ovld __cnfn convert_double2_rtn(uchar2);
double2 __ovld __cnfn convert_double2_rtn(uint2);
double2 __ovld __cnfn convert_double2_rtn(ulong2);
double2 __ovld __cnfn convert_double2_rtn(ushort2);
double2 __ovld __cnfn convert_double2_rtp(char2);
double2 __ovld __cnfn convert_double2_rtp(double2);
double2 __ovld __cnfn convert_double2_rtp(float2);
double2 __ovld __cnfn convert_double2_rtp(int2);
double2 __ovld __cnfn convert_double2_rtp(long2);
double2 __ovld __cnfn convert_double2_rtp(short2);
double2 __ovld __cnfn convert_double2_rtp(uchar2);
double2 __ovld __cnfn convert_double2_rtp(uint2);
double2 __ovld __cnfn convert_double2_rtp(ulong2);
double2 __ovld __cnfn convert_double2_rtp(ushort2);
double2 __ovld __cnfn convert_double2_rtz(char2);
double2 __ovld __cnfn convert_double2_rtz(double2);
double2 __ovld __cnfn convert_double2_rtz(float2);
double2 __ovld __cnfn convert_double2_rtz(int2);
double2 __ovld __cnfn convert_double2_rtz(long2);
double2 __ovld __cnfn convert_double2_rtz(short2);
double2 __ovld __cnfn convert_double2_rtz(uchar2);
double2 __ovld __cnfn convert_double2_rtz(uint2);
double2 __ovld __cnfn convert_double2_rtz(ulong2);
double2 __ovld __cnfn convert_double2_rtz(ushort2);
double3 __ovld __cnfn convert_double3(char3);
double3 __ovld __cnfn convert_double3(double3);
double3 __ovld __cnfn convert_double3(float3);
double3 __ovld __cnfn convert_double3(int3);
double3 __ovld __cnfn convert_double3(long3);
double3 __ovld __cnfn convert_double3(short3);
double3 __ovld __cnfn convert_double3(uchar3);
double3 __ovld __cnfn convert_double3(uint3);
double3 __ovld __cnfn convert_double3(ulong3);
double3 __ovld __cnfn convert_double3(ushort3);
double3 __ovld __cnfn convert_double3_rte(char3);
double3 __ovld __cnfn convert_double3_rte(double3);
double3 __ovld __cnfn convert_double3_rte(float3);
double3 __ovld __cnfn convert_double3_rte(int3);
double3 __ovld __cnfn convert_double3_rte(long3);
double3 __ovld __cnfn convert_double3_rte(short3);
double3 __ovld __cnfn convert_double3_rte(uchar3);
double3 __ovld __cnfn convert_double3_rte(uint3);
double3 __ovld __cnfn convert_double3_rte(ulong3);
double3 __ovld __cnfn convert_double3_rte(ushort3);
double3 __ovld __cnfn convert_double3_rtn(char3);
double3 __ovld __cnfn convert_double3_rtn(double3);
double3 __ovld __cnfn convert_double3_rtn(float3);
double3 __ovld __cnfn convert_double3_rtn(int3);
double3 __ovld __cnfn convert_double3_rtn(long3);
double3 __ovld __cnfn convert_double3_rtn(short3);
double3 __ovld __cnfn convert_double3_rtn(uchar3);
double3 __ovld __cnfn convert_double3_rtn(uint3);
double3 __ovld __cnfn convert_double3_rtn(ulong3);
double3 __ovld __cnfn convert_double3_rtn(ushort3);
double3 __ovld __cnfn convert_double3_rtp(char3);
double3 __ovld __cnfn convert_double3_rtp(double3);
double3 __ovld __cnfn convert_double3_rtp(float3);
double3 __ovld __cnfn convert_double3_rtp(int3);
double3 __ovld __cnfn convert_double3_rtp(long3);
double3 __ovld __cnfn convert_double3_rtp(short3);
double3 __ovld __cnfn convert_double3_rtp(uchar3);
double3 __ovld __cnfn convert_double3_rtp(uint3);
double3 __ovld __cnfn convert_double3_rtp(ulong3);
double3 __ovld __cnfn convert_double3_rtp(ushort3);
double3 __ovld __cnfn convert_double3_rtz(char3);
double3 __ovld __cnfn convert_double3_rtz(double3);
double3 __ovld __cnfn convert_double3_rtz(float3);
double3 __ovld __cnfn convert_double3_rtz(int3);
double3 __ovld __cnfn convert_double3_rtz(long3);
double3 __ovld __cnfn convert_double3_rtz(short3);
double3 __ovld __cnfn convert_double3_rtz(uchar3);
double3 __ovld __cnfn convert_double3_rtz(uint3);
double3 __ovld __cnfn convert_double3_rtz(ulong3);
double3 __ovld __cnfn convert_double3_rtz(ushort3);
double4 __ovld __cnfn convert_double4(char4);
double4 __ovld __cnfn convert_double4(double4);
double4 __ovld __cnfn convert_double4(float4);
double4 __ovld __cnfn convert_double4(int4);
double4 __ovld __cnfn convert_double4(long4);
double4 __ovld __cnfn convert_double4(short4);
double4 __ovld __cnfn convert_double4(uchar4);
double4 __ovld __cnfn convert_double4(uint4);
double4 __ovld __cnfn convert_double4(ulong4);
double4 __ovld __cnfn convert_double4(ushort4);
double4 __ovld __cnfn convert_double4_rte(char4);
double4 __ovld __cnfn convert_double4_rte(double4);
double4 __ovld __cnfn convert_double4_rte(float4);
double4 __ovld __cnfn convert_double4_rte(int4);
double4 __ovld __cnfn convert_double4_rte(long4);
double4 __ovld __cnfn convert_double4_rte(short4);
double4 __ovld __cnfn convert_double4_rte(uchar4);
double4 __ovld __cnfn convert_double4_rte(uint4);
double4 __ovld __cnfn convert_double4_rte(ulong4);
double4 __ovld __cnfn convert_double4_rte(ushort4);
double4 __ovld __cnfn convert_double4_rtn(char4);
double4 __ovld __cnfn convert_double4_rtn(double4);
double4 __ovld __cnfn convert_double4_rtn(float4);
double4 __ovld __cnfn convert_double4_rtn(int4);
double4 __ovld __cnfn convert_double4_rtn(long4);
double4 __ovld __cnfn convert_double4_rtn(short4);
double4 __ovld __cnfn convert_double4_rtn(uchar4);
double4 __ovld __cnfn convert_double4_rtn(uint4);
double4 __ovld __cnfn convert_double4_rtn(ulong4);
double4 __ovld __cnfn convert_double4_rtn(ushort4);
double4 __ovld __cnfn convert_double4_rtp(char4);
double4 __ovld __cnfn convert_double4_rtp(double4);
double4 __ovld __cnfn convert_double4_rtp(float4);
double4 __ovld __cnfn convert_double4_rtp(int4);
double4 __ovld __cnfn convert_double4_rtp(long4);
double4 __ovld __cnfn convert_double4_rtp(short4);
double4 __ovld __cnfn convert_double4_rtp(uchar4);
double4 __ovld __cnfn convert_double4_rtp(uint4);
double4 __ovld __cnfn convert_double4_rtp(ulong4);
double4 __ovld __cnfn convert_double4_rtp(ushort4);
double4 __ovld __cnfn convert_double4_rtz(char4);
double4 __ovld __cnfn convert_double4_rtz(double4);
double4 __ovld __cnfn convert_double4_rtz(float4);
double4 __ovld __cnfn convert_double4_rtz(int4);
double4 __ovld __cnfn convert_double4_rtz(long4);
double4 __ovld __cnfn convert_double4_rtz(short4);
double4 __ovld __cnfn convert_double4_rtz(uchar4);
double4 __ovld __cnfn convert_double4_rtz(uint4);
double4 __ovld __cnfn convert_double4_rtz(ulong4);
double4 __ovld __cnfn convert_double4_rtz(ushort4);
double8 __ovld __cnfn convert_double8(char8);
double8 __ovld __cnfn convert_double8(double8);
double8 __ovld __cnfn convert_double8(float8);
double8 __ovld __cnfn convert_double8(int8);
double8 __ovld __cnfn convert_double8(long8);
double8 __ovld __cnfn convert_double8(short8);
double8 __ovld __cnfn convert_double8(uchar8);
double8 __ovld __cnfn convert_double8(uint8);
double8 __ovld __cnfn convert_double8(ulong8);
double8 __ovld __cnfn convert_double8(ushort8);
double8 __ovld __cnfn convert_double8_rte(char8);
double8 __ovld __cnfn convert_double8_rte(double8);
double8 __ovld __cnfn convert_double8_rte(float8);
double8 __ovld __cnfn convert_double8_rte(int8);
double8 __ovld __cnfn convert_double8_rte(long8);
double8 __ovld __cnfn convert_double8_rte(short8);
double8 __ovld __cnfn convert_double8_rte(uchar8);
double8 __ovld __cnfn convert_double8_rte(uint8);
double8 __ovld __cnfn convert_double8_rte(ulong8);
double8 __ovld __cnfn convert_double8_rte(ushort8);
double8 __ovld __cnfn convert_double8_rtn(char8);
double8 __ovld __cnfn convert_double8_rtn(double8);
double8 __ovld __cnfn convert_double8_rtn(float8);
double8 __ovld __cnfn convert_double8_rtn(int8);
double8 __ovld __cnfn convert_double8_rtn(long8);
double8 __ovld __cnfn convert_double8_rtn(short8);
double8 __ovld __cnfn convert_double8_rtn(uchar8);
double8 __ovld __cnfn convert_double8_rtn(uint8);
double8 __ovld __cnfn convert_double8_rtn(ulong8);
double8 __ovld __cnfn convert_double8_rtn(ushort8);
double8 __ovld __cnfn convert_double8_rtp(char8);
double8 __ovld __cnfn convert_double8_rtp(double8);
double8 __ovld __cnfn convert_double8_rtp(float8);
double8 __ovld __cnfn convert_double8_rtp(int8);
double8 __ovld __cnfn convert_double8_rtp(long8);
double8 __ovld __cnfn convert_double8_rtp(short8);
double8 __ovld __cnfn convert_double8_rtp(uchar8);
double8 __ovld __cnfn convert_double8_rtp(uint8);
double8 __ovld __cnfn convert_double8_rtp(ulong8);
double8 __ovld __cnfn convert_double8_rtp(ushort8);
double8 __ovld __cnfn convert_double8_rtz(char8);
double8 __ovld __cnfn convert_double8_rtz(double8);
double8 __ovld __cnfn convert_double8_rtz(float8);
double8 __ovld __cnfn convert_double8_rtz(int8);
double8 __ovld __cnfn convert_double8_rtz(long8);
double8 __ovld __cnfn convert_double8_rtz(short8);
double8 __ovld __cnfn convert_double8_rtz(uchar8);
double8 __ovld __cnfn convert_double8_rtz(uint8);
double8 __ovld __cnfn convert_double8_rtz(ulong8);
double8 __ovld __cnfn convert_double8_rtz(ushort8);
double16 __ovld __cnfn convert_double16(char16);
double16 __ovld __cnfn convert_double16(double16);
double16 __ovld __cnfn convert_double16(float16);
double16 __ovld __cnfn convert_double16(int16);
double16 __ovld __cnfn convert_double16(long16);
double16 __ovld __cnfn convert_double16(short16);
double16 __ovld __cnfn convert_double16(uchar16);
double16 __ovld __cnfn convert_double16(uint16);
double16 __ovld __cnfn convert_double16(ulong16);
double16 __ovld __cnfn convert_double16(ushort16);
double16 __ovld __cnfn convert_double16_rte(char16);
double16 __ovld __cnfn convert_double16_rte(double16);
double16 __ovld __cnfn convert_double16_rte(float16);
double16 __ovld __cnfn convert_double16_rte(int16);
double16 __ovld __cnfn convert_double16_rte(long16);
double16 __ovld __cnfn convert_double16_rte(short16);
double16 __ovld __cnfn convert_double16_rte(uchar16);
double16 __ovld __cnfn convert_double16_rte(uint16);
double16 __ovld __cnfn convert_double16_rte(ulong16);
double16 __ovld __cnfn convert_double16_rte(ushort16);
double16 __ovld __cnfn convert_double16_rtn(char16);
double16 __ovld __cnfn convert_double16_rtn(double16);
double16 __ovld __cnfn convert_double16_rtn(float16);
double16 __ovld __cnfn convert_double16_rtn(int16);
double16 __ovld __cnfn convert_double16_rtn(long16);
double16 __ovld __cnfn convert_double16_rtn(short16);
double16 __ovld __cnfn convert_double16_rtn(uchar16);
double16 __ovld __cnfn convert_double16_rtn(uint16);
double16 __ovld __cnfn convert_double16_rtn(ulong16);
double16 __ovld __cnfn convert_double16_rtn(ushort16);
double16 __ovld __cnfn convert_double16_rtp(char16);
double16 __ovld __cnfn convert_double16_rtp(double16);
double16 __ovld __cnfn convert_double16_rtp(float16);
double16 __ovld __cnfn convert_double16_rtp(int16);
double16 __ovld __cnfn convert_double16_rtp(long16);
double16 __ovld __cnfn convert_double16_rtp(short16);
double16 __ovld __cnfn convert_double16_rtp(uchar16);
double16 __ovld __cnfn convert_double16_rtp(uint16);
double16 __ovld __cnfn convert_double16_rtp(ulong16);
double16 __ovld __cnfn convert_double16_rtp(ushort16);
double16 __ovld __cnfn convert_double16_rtz(char16);
double16 __ovld __cnfn convert_double16_rtz(double16);
double16 __ovld __cnfn convert_double16_rtz(float16);
double16 __ovld __cnfn convert_double16_rtz(int16);
double16 __ovld __cnfn convert_double16_rtz(long16);
double16 __ovld __cnfn convert_double16_rtz(short16);
double16 __ovld __cnfn convert_double16_rtz(uchar16);
double16 __ovld __cnfn convert_double16_rtz(uint16);
double16 __ovld __cnfn convert_double16_rtz(ulong16);
double16 __ovld __cnfn convert_double16_rtz(ushort16);
#endif //cl_khr_fp64

#ifdef cl_khr_fp16
#pragma OPENCL EXTENSION cl_khr_fp16 : enable
// Convert half types to non-double types.
uchar __ovld __cnfn convert_uchar(half);
uchar __ovld __cnfn convert_uchar_rte(half);
uchar __ovld __cnfn convert_uchar_rtp(half);
uchar __ovld __cnfn convert_uchar_rtn(half);
uchar __ovld __cnfn convert_uchar_rtz(half);
uchar __ovld __cnfn convert_uchar_sat(half);
uchar __ovld __cnfn convert_uchar_sat_rte(half);
uchar __ovld __cnfn convert_uchar_sat_rtp(half);
uchar __ovld __cnfn convert_uchar_sat_rtn(half);
uchar __ovld __cnfn convert_uchar_sat_rtz(half);
uchar2 __ovld __cnfn convert_uchar2(half2);
uchar2 __ovld __cnfn convert_uchar2_rte(half2);
uchar2 __ovld __cnfn convert_uchar2_rtp(half2);
uchar2 __ovld __cnfn convert_uchar2_rtn(half2);
uchar2 __ovld __cnfn convert_uchar2_rtz(half2);
uchar2 __ovld __cnfn convert_uchar2_sat(half2);
uchar2 __ovld __cnfn convert_uchar2_sat_rte(half2);
uchar2 __ovld __cnfn convert_uchar2_sat_rtp(half2);
uchar2 __ovld __cnfn convert_uchar2_sat_rtn(half2);
uchar2 __ovld __cnfn convert_uchar2_sat_rtz(half2);
uchar3 __ovld __cnfn convert_uchar3(half3);
uchar3 __ovld __cnfn convert_uchar3_rte(half3);
uchar3 __ovld __cnfn convert_uchar3_rtp(half3);
uchar3 __ovld __cnfn convert_uchar3_rtn(half3);
uchar3 __ovld __cnfn convert_uchar3_rtz(half3);
uchar3 __ovld __cnfn convert_uchar3_sat(half3);
uchar3 __ovld __cnfn convert_uchar3_sat_rte(half3);
uchar3 __ovld __cnfn convert_uchar3_sat_rtp(half3);
uchar3 __ovld __cnfn convert_uchar3_sat_rtn(half3);
uchar3 __ovld __cnfn convert_uchar3_sat_rtz(half3);
uchar4 __ovld __cnfn convert_uchar4(half4);
uchar4 __ovld __cnfn convert_uchar4_rte(half4);
uchar4 __ovld __cnfn convert_uchar4_rtp(half4);
uchar4 __ovld __cnfn convert_uchar4_rtn(half4);
uchar4 __ovld __cnfn convert_uchar4_rtz(half4);
uchar4 __ovld __cnfn convert_uchar4_sat(half4);
uchar4 __ovld __cnfn convert_uchar4_sat_rte(half4);
uchar4 __ovld __cnfn convert_uchar4_sat_rtp(half4);
uchar4 __ovld __cnfn convert_uchar4_sat_rtn(half4);
uchar4 __ovld __cnfn convert_uchar4_sat_rtz(half4);
uchar8 __ovld __cnfn convert_uchar8(half8);
uchar8 __ovld __cnfn convert_uchar8_rte(half8);
uchar8 __ovld __cnfn convert_uchar8_rtp(half8);
uchar8 __ovld __cnfn convert_uchar8_rtn(half8);
uchar8 __ovld __cnfn convert_uchar8_rtz(half8);
uchar8 __ovld __cnfn convert_uchar8_sat(half8);
uchar8 __ovld __cnfn convert_uchar8_sat_rte(half8);
uchar8 __ovld __cnfn convert_uchar8_sat_rtp(half8);
uchar8 __ovld __cnfn convert_uchar8_sat_rtn(half8);
uchar8 __ovld __cnfn convert_uchar8_sat_rtz(half8);
uchar16 __ovld __cnfn convert_uchar16(half16);
uchar16 __ovld __cnfn convert_uchar16_rte(half16);
uchar16 __ovld __cnfn convert_uchar16_rtp(half16);
uchar16 __ovld __cnfn convert_uchar16_rtn(half16);
uchar16 __ovld __cnfn convert_uchar16_rtz(half16);
uchar16 __ovld __cnfn convert_uchar16_sat(half16);
uchar16 __ovld __cnfn convert_uchar16_sat_rte(half16);
uchar16 __ovld __cnfn convert_uchar16_sat_rtp(half16);
uchar16 __ovld __cnfn convert_uchar16_sat_rtn(half16);
uchar16 __ovld __cnfn convert_uchar16_sat_rtz(half16);
ushort __ovld __cnfn convert_ushort(half);
ushort __ovld __cnfn convert_ushort_rte(half);
ushort __ovld __cnfn convert_ushort_rtp(half);
ushort __ovld __cnfn convert_ushort_rtn(half);
ushort __ovld __cnfn convert_ushort_rtz(half);
ushort __ovld __cnfn convert_ushort_sat(half);
ushort __ovld __cnfn convert_ushort_sat_rte(half);
ushort __ovld __cnfn convert_ushort_sat_rtp(half);
ushort __ovld __cnfn convert_ushort_sat_rtn(half);
ushort __ovld __cnfn convert_ushort_sat_rtz(half);
ushort2 __ovld __cnfn convert_ushort2(half2);
ushort2 __ovld __cnfn convert_ushort2_rte(half2);
ushort2 __ovld __cnfn convert_ushort2_rtp(half2);
ushort2 __ovld __cnfn convert_ushort2_rtn(half2);
ushort2 __ovld __cnfn convert_ushort2_rtz(half2);
ushort2 __ovld __cnfn convert_ushort2_sat(half2);
ushort2 __ovld __cnfn convert_ushort2_sat_rte(half2);
ushort2 __ovld __cnfn convert_ushort2_sat_rtp(half2);
ushort2 __ovld __cnfn convert_ushort2_sat_rtn(half2);
ushort2 __ovld __cnfn convert_ushort2_sat_rtz(half2);
ushort3 __ovld __cnfn convert_ushort3(half3);
ushort3 __ovld __cnfn convert_ushort3_rte(half3);
ushort3 __ovld __cnfn convert_ushort3_rtp(half3);
ushort3 __ovld __cnfn convert_ushort3_rtn(half3);
ushort3 __ovld __cnfn convert_ushort3_rtz(half3);
ushort3 __ovld __cnfn convert_ushort3_sat(half3);
ushort3 __ovld __cnfn convert_ushort3_sat_rte(half3);
ushort3 __ovld __cnfn convert_ushort3_sat_rtp(half3);
ushort3 __ovld __cnfn convert_ushort3_sat_rtn(half3);
ushort3 __ovld __cnfn convert_ushort3_sat_rtz(half3);
ushort4 __ovld __cnfn convert_ushort4(half4);
ushort4 __ovld __cnfn convert_ushort4_rte(half4);
ushort4 __ovld __cnfn convert_ushort4_rtp(half4);
ushort4 __ovld __cnfn convert_ushort4_rtn(half4);
ushort4 __ovld __cnfn convert_ushort4_rtz(half4);
ushort4 __ovld __cnfn convert_ushort4_sat(half4);
ushort4 __ovld __cnfn convert_ushort4_sat_rte(half4);
ushort4 __ovld __cnfn convert_ushort4_sat_rtp(half4);
ushort4 __ovld __cnfn convert_ushort4_sat_rtn(half4);
ushort4 __ovld __cnfn convert_ushort4_sat_rtz(half4);
ushort8 __ovld __cnfn convert_ushort8(half8);
ushort8 __ovld __cnfn convert_ushort8_rte(half8);
ushort8 __ovld __cnfn convert_ushort8_rtp(half8);
ushort8 __ovld __cnfn convert_ushort8_rtn(half8);
ushort8 __ovld __cnfn convert_ushort8_rtz(half8);
ushort8 __ovld __cnfn convert_ushort8_sat(half8);
ushort8 __ovld __cnfn convert_ushort8_sat_rte(half8);
ushort8 __ovld __cnfn convert_ushort8_sat_rtp(half8);
ushort8 __ovld __cnfn convert_ushort8_sat_rtn(half8);
ushort8 __ovld __cnfn convert_ushort8_sat_rtz(half8);
ushort16 __ovld __cnfn convert_ushort16(half16);
ushort16 __ovld __cnfn convert_ushort16_rte(half16);
ushort16 __ovld __cnfn convert_ushort16_rtp(half16);
ushort16 __ovld __cnfn convert_ushort16_rtn(half16);
ushort16 __ovld __cnfn convert_ushort16_rtz(half16);
ushort16 __ovld __cnfn convert_ushort16_sat(half16);
ushort16 __ovld __cnfn convert_ushort16_sat_rte(half16);
ushort16 __ovld __cnfn convert_ushort16_sat_rtp(half16);
ushort16 __ovld __cnfn convert_ushort16_sat_rtn(half16);
ushort16 __ovld __cnfn convert_ushort16_sat_rtz(half16);
uint __ovld __cnfn convert_uint(half);
uint __ovld __cnfn convert_uint_rte(half);
uint __ovld __cnfn convert_uint_rtp(half);
uint __ovld __cnfn convert_uint_rtn(half);
uint __ovld __cnfn convert_uint_rtz(half);
uint __ovld __cnfn convert_uint_sat(half);
uint __ovld __cnfn convert_uint_sat_rte(half);
uint __ovld __cnfn convert_uint_sat_rtp(half);
uint __ovld __cnfn convert_uint_sat_rtn(half);
uint __ovld __cnfn convert_uint_sat_rtz(half);
uint2 __ovld __cnfn convert_uint2(half2);
uint2 __ovld __cnfn convert_uint2_rte(half2);
uint2 __ovld __cnfn convert_uint2_rtp(half2);
uint2 __ovld __cnfn convert_uint2_rtn(half2);
uint2 __ovld __cnfn convert_uint2_rtz(half2);
uint2 __ovld __cnfn convert_uint2_sat(half2);
uint2 __ovld __cnfn convert_uint2_sat_rte(half2);
uint2 __ovld __cnfn convert_uint2_sat_rtp(half2);
uint2 __ovld __cnfn convert_uint2_sat_rtn(half2);
uint2 __ovld __cnfn convert_uint2_sat_rtz(half2);
uint3 __ovld __cnfn convert_uint3(half3);
uint3 __ovld __cnfn convert_uint3_rte(half3);
uint3 __ovld __cnfn convert_uint3_rtp(half3);
uint3 __ovld __cnfn convert_uint3_rtn(half3);
uint3 __ovld __cnfn convert_uint3_rtz(half3);
uint3 __ovld __cnfn convert_uint3_sat(half3);
uint3 __ovld __cnfn convert_uint3_sat_rte(half3);
uint3 __ovld __cnfn convert_uint3_sat_rtp(half3);
uint3 __ovld __cnfn convert_uint3_sat_rtn(half3);
uint3 __ovld __cnfn convert_uint3_sat_rtz(half3);
uint4 __ovld __cnfn convert_uint4(half4);
uint4 __ovld __cnfn convert_uint4_rte(half4);
uint4 __ovld __cnfn convert_uint4_rtp(half4);
uint4 __ovld __cnfn convert_uint4_rtn(half4);
uint4 __ovld __cnfn convert_uint4_rtz(half4);
uint4 __ovld __cnfn convert_uint4_sat(half4);
uint4 __ovld __cnfn convert_uint4_sat_rte(half4);
uint4 __ovld __cnfn convert_uint4_sat_rtp(half4);
uint4 __ovld __cnfn convert_uint4_sat_rtn(half4);
uint4 __ovld __cnfn convert_uint4_sat_rtz(half4);
uint8 __ovld __cnfn convert_uint8(half8);
uint8 __ovld __cnfn convert_uint8_rte(half8);
uint8 __ovld __cnfn convert_uint8_rtp(half8);
uint8 __ovld __cnfn convert_uint8_rtn(half8);
uint8 __ovld __cnfn convert_uint8_rtz(half8);
uint8 __ovld __cnfn convert_uint8_sat(half8);
uint8 __ovld __cnfn convert_uint8_sat_rte(half8);
uint8 __ovld __cnfn convert_uint8_sat_rtp(half8);
uint8 __ovld __cnfn convert_uint8_sat_rtn(half8);
uint8 __ovld __cnfn convert_uint8_sat_rtz(half8);
uint16 __ovld __cnfn convert_uint16(half16);
uint16 __ovld __cnfn convert_uint16_rte(half16);
uint16 __ovld __cnfn convert_uint16_rtp(half16);
uint16 __ovld __cnfn convert_uint16_rtn(half16);
uint16 __ovld __cnfn convert_uint16_rtz(half16);
uint16 __ovld __cnfn convert_uint16_sat(half16);
uint16 __ovld __cnfn convert_uint16_sat_rte(half16);
uint16 __ovld __cnfn convert_uint16_sat_rtp(half16);
uint16 __ovld __cnfn convert_uint16_sat_rtn(half16);
uint16 __ovld __cnfn convert_uint16_sat_rtz(half16);
ulong __ovld __cnfn convert_ulong(half);
ulong __ovld __cnfn convert_ulong_rte(half);
ulong __ovld __cnfn convert_ulong_rtp(half);
ulong __ovld __cnfn convert_ulong_rtn(half);
ulong __ovld __cnfn convert_ulong_rtz(half);
ulong __ovld __cnfn convert_ulong_sat(half);
ulong __ovld __cnfn convert_ulong_sat_rte(half);
ulong __ovld __cnfn convert_ulong_sat_rtp(half);
ulong __ovld __cnfn convert_ulong_sat_rtn(half);
ulong __ovld __cnfn convert_ulong_sat_rtz(half);
ulong2 __ovld __cnfn convert_ulong2(half2);
ulong2 __ovld __cnfn convert_ulong2_rte(half2);
ulong2 __ovld __cnfn convert_ulong2_rtp(half2);
ulong2 __ovld __cnfn convert_ulong2_rtn(half2);
ulong2 __ovld __cnfn convert_ulong2_rtz(half2);
ulong2 __ovld __cnfn convert_ulong2_sat(half2);
ulong2 __ovld __cnfn convert_ulong2_sat_rte(half2);
ulong2 __ovld __cnfn convert_ulong2_sat_rtp(half2);
ulong2 __ovld __cnfn convert_ulong2_sat_rtn(half2);
ulong2 __ovld __cnfn convert_ulong2_sat_rtz(half2);
ulong3 __ovld __cnfn convert_ulong3(half3);
ulong3 __ovld __cnfn convert_ulong3_rte(half3);
ulong3 __ovld __cnfn convert_ulong3_rtp(half3);
ulong3 __ovld __cnfn convert_ulong3_rtn(half3);
ulong3 __ovld __cnfn convert_ulong3_rtz(half3);
ulong3 __ovld __cnfn convert_ulong3_sat(half3);
ulong3 __ovld __cnfn convert_ulong3_sat_rte(half3);
ulong3 __ovld __cnfn convert_ulong3_sat_rtp(half3);
ulong3 __ovld __cnfn convert_ulong3_sat_rtn(half3);
ulong3 __ovld __cnfn convert_ulong3_sat_rtz(half3);
ulong4 __ovld __cnfn convert_ulong4(half4);
ulong4 __ovld __cnfn convert_ulong4_rte(half4);
ulong4 __ovld __cnfn convert_ulong4_rtp(half4);
ulong4 __ovld __cnfn convert_ulong4_rtn(half4);
ulong4 __ovld __cnfn convert_ulong4_rtz(half4);
ulong4 __ovld __cnfn convert_ulong4_sat(half4);
ulong4 __ovld __cnfn convert_ulong4_sat_rte(half4);
ulong4 __ovld __cnfn convert_ulong4_sat_rtp(half4);
ulong4 __ovld __cnfn convert_ulong4_sat_rtn(half4);
ulong4 __ovld __cnfn convert_ulong4_sat_rtz(half4);
ulong8 __ovld __cnfn convert_ulong8(half8);
ulong8 __ovld __cnfn convert_ulong8_rte(half8);
ulong8 __ovld __cnfn convert_ulong8_rtp(half8);
ulong8 __ovld __cnfn convert_ulong8_rtn(half8);
ulong8 __ovld __cnfn convert_ulong8_rtz(half8);
ulong8 __ovld __cnfn convert_ulong8_sat(half8);
ulong8 __ovld __cnfn convert_ulong8_sat_rte(half8);
ulong8 __ovld __cnfn convert_ulong8_sat_rtp(half8);
ulong8 __ovld __cnfn convert_ulong8_sat_rtn(half8);
ulong8 __ovld __cnfn convert_ulong8_sat_rtz(half8);
ulong16 __ovld __cnfn convert_ulong16(half16);
ulong16 __ovld __cnfn convert_ulong16_rte(half16);
ulong16 __ovld __cnfn convert_ulong16_rtp(half16);
ulong16 __ovld __cnfn convert_ulong16_rtn(half16);
ulong16 __ovld __cnfn convert_ulong16_rtz(half16);
ulong16 __ovld __cnfn convert_ulong16_sat(half16);
ulong16 __ovld __cnfn convert_ulong16_sat_rte(half16);
ulong16 __ovld __cnfn convert_ulong16_sat_rtp(half16);
ulong16 __ovld __cnfn convert_ulong16_sat_rtn(half16);
ulong16 __ovld __cnfn convert_ulong16_sat_rtz(half16);
char __ovld __cnfn convert_char(half);
char __ovld __cnfn convert_char_rte(half);
char __ovld __cnfn convert_char_rtp(half);
char __ovld __cnfn convert_char_rtn(half);
char __ovld __cnfn convert_char_rtz(half);
char __ovld __cnfn convert_char_sat(half);
char __ovld __cnfn convert_char_sat_rte(half);
char __ovld __cnfn convert_char_sat_rtp(half);
char __ovld __cnfn convert_char_sat_rtn(half);
char __ovld __cnfn convert_char_sat_rtz(half);
char2 __ovld __cnfn convert_char2(half2);
char2 __ovld __cnfn convert_char2_rte(half2);
char2 __ovld __cnfn convert_char2_rtp(half2);
char2 __ovld __cnfn convert_char2_rtn(half2);
char2 __ovld __cnfn convert_char2_rtz(half2);
char2 __ovld __cnfn convert_char2_sat(half2);
char2 __ovld __cnfn convert_char2_sat_rte(half2);
char2 __ovld __cnfn convert_char2_sat_rtp(half2);
char2 __ovld __cnfn convert_char2_sat_rtn(half2);
char2 __ovld __cnfn convert_char2_sat_rtz(half2);
char3 __ovld __cnfn convert_char3(half3);
char3 __ovld __cnfn convert_char3_rte(half3);
char3 __ovld __cnfn convert_char3_rtp(half3);
char3 __ovld __cnfn convert_char3_rtn(half3);
char3 __ovld __cnfn convert_char3_rtz(half3);
char3 __ovld __cnfn convert_char3_sat(half3);
char3 __ovld __cnfn convert_char3_sat_rte(half3);
char3 __ovld __cnfn convert_char3_sat_rtp(half3);
char3 __ovld __cnfn convert_char3_sat_rtn(half3);
char3 __ovld __cnfn convert_char3_sat_rtz(half3);
char4 __ovld __cnfn convert_char4(half4);
char4 __ovld __cnfn convert_char4_rte(half4);
char4 __ovld __cnfn convert_char4_rtp(half4);
char4 __ovld __cnfn convert_char4_rtn(half4);
char4 __ovld __cnfn convert_char4_rtz(half4);
char4 __ovld __cnfn convert_char4_sat(half4);
char4 __ovld __cnfn convert_char4_sat_rte(half4);
char4 __ovld __cnfn convert_char4_sat_rtp(half4);
char4 __ovld __cnfn convert_char4_sat_rtn(half4);
char4 __ovld __cnfn convert_char4_sat_rtz(half4);
char8 __ovld __cnfn convert_char8(half8);
char8 __ovld __cnfn convert_char8_rte(half8);
char8 __ovld __cnfn convert_char8_rtp(half8);
char8 __ovld __cnfn convert_char8_rtn(half8);
char8 __ovld __cnfn convert_char8_rtz(half8);
char8 __ovld __cnfn convert_char8_sat(half8);
char8 __ovld __cnfn convert_char8_sat_rte(half8);
char8 __ovld __cnfn convert_char8_sat_rtp(half8);
char8 __ovld __cnfn convert_char8_sat_rtn(half8);
char8 __ovld __cnfn convert_char8_sat_rtz(half8);
char16 __ovld __cnfn convert_char16(half16);
char16 __ovld __cnfn convert_char16_rte(half16);
char16 __ovld __cnfn convert_char16_rtp(half16);
char16 __ovld __cnfn convert_char16_rtn(half16);
char16 __ovld __cnfn convert_char16_rtz(half16);
char16 __ovld __cnfn convert_char16_sat(half16);
char16 __ovld __cnfn convert_char16_sat_rte(half16);
char16 __ovld __cnfn convert_char16_sat_rtp(half16);
char16 __ovld __cnfn convert_char16_sat_rtn(half16);
char16 __ovld __cnfn convert_char16_sat_rtz(half16);
short __ovld __cnfn convert_short(half);
short __ovld __cnfn convert_short_rte(half);
short __ovld __cnfn convert_short_rtp(half);
short __ovld __cnfn convert_short_rtn(half);
short __ovld __cnfn convert_short_rtz(half);
short __ovld __cnfn convert_short_sat(half);
short __ovld __cnfn convert_short_sat_rte(half);
short __ovld __cnfn convert_short_sat_rtp(half);
short __ovld __cnfn convert_short_sat_rtn(half);
short __ovld __cnfn convert_short_sat_rtz(half);
short2 __ovld __cnfn convert_short2(half2);
short2 __ovld __cnfn convert_short2_rte(half2);
short2 __ovld __cnfn convert_short2_rtp(half2);
short2 __ovld __cnfn convert_short2_rtn(half2);
short2 __ovld __cnfn convert_short2_rtz(half2);
short2 __ovld __cnfn convert_short2_sat(half2);
short2 __ovld __cnfn convert_short2_sat_rte(half2);
short2 __ovld __cnfn convert_short2_sat_rtp(half2);
short2 __ovld __cnfn convert_short2_sat_rtn(half2);
short2 __ovld __cnfn convert_short2_sat_rtz(half2);
short3 __ovld __cnfn convert_short3(half3);
short3 __ovld __cnfn convert_short3_rte(half3);
short3 __ovld __cnfn convert_short3_rtp(half3);
short3 __ovld __cnfn convert_short3_rtn(half3);
short3 __ovld __cnfn convert_short3_rtz(half3);
short3 __ovld __cnfn convert_short3_sat(half3);
short3 __ovld __cnfn convert_short3_sat_rte(half3);
short3 __ovld __cnfn convert_short3_sat_rtp(half3);
short3 __ovld __cnfn convert_short3_sat_rtn(half3);
short3 __ovld __cnfn convert_short3_sat_rtz(half3);
short4 __ovld __cnfn convert_short4(half4);
short4 __ovld __cnfn convert_short4_rte(half4);
short4 __ovld __cnfn convert_short4_rtp(half4);
short4 __ovld __cnfn convert_short4_rtn(half4);
short4 __ovld __cnfn convert_short4_rtz(half4);
short4 __ovld __cnfn convert_short4_sat(half4);
short4 __ovld __cnfn convert_short4_sat_rte(half4);
short4 __ovld __cnfn convert_short4_sat_rtp(half4);
short4 __ovld __cnfn convert_short4_sat_rtn(half4);
short4 __ovld __cnfn convert_short4_sat_rtz(half4);
short8 __ovld __cnfn convert_short8(half8);
short8 __ovld __cnfn convert_short8_rte(half8);
short8 __ovld __cnfn convert_short8_rtp(half8);
short8 __ovld __cnfn convert_short8_rtn(half8);
short8 __ovld __cnfn convert_short8_rtz(half8);
short8 __ovld __cnfn convert_short8_sat(half8);
short8 __ovld __cnfn convert_short8_sat_rte(half8);
short8 __ovld __cnfn convert_short8_sat_rtp(half8);
short8 __ovld __cnfn convert_short8_sat_rtn(half8);
short8 __ovld __cnfn convert_short8_sat_rtz(half8);
short16 __ovld __cnfn convert_short16(half16);
short16 __ovld __cnfn convert_short16_rte(half16);
short16 __ovld __cnfn convert_short16_rtp(half16);
short16 __ovld __cnfn convert_short16_rtn(half16);
short16 __ovld __cnfn convert_short16_rtz(half16);
short16 __ovld __cnfn convert_short16_sat(half16);
short16 __ovld __cnfn convert_short16_sat_rte(half16);
short16 __ovld __cnfn convert_short16_sat_rtp(half16);
short16 __ovld __cnfn convert_short16_sat_rtn(half16);
short16 __ovld __cnfn convert_short16_sat_rtz(half16);
int __ovld __cnfn convert_int(half);
int __ovld __cnfn convert_int_rte(half);
int __ovld __cnfn convert_int_rtp(half);
int __ovld __cnfn convert_int_rtn(half);
int __ovld __cnfn convert_int_rtz(half);
int __ovld __cnfn convert_int_sat(half);
int __ovld __cnfn convert_int_sat_rte(half);
int __ovld __cnfn convert_int_sat_rtp(half);
int __ovld __cnfn convert_int_sat_rtn(half);
int __ovld __cnfn convert_int_sat_rtz(half);
int2 __ovld __cnfn convert_int2(half2);
int2 __ovld __cnfn convert_int2_rte(half2);
int2 __ovld __cnfn convert_int2_rtp(half2);
int2 __ovld __cnfn convert_int2_rtn(half2);
int2 __ovld __cnfn convert_int2_rtz(half2);
int2 __ovld __cnfn convert_int2_sat(half2);
int2 __ovld __cnfn convert_int2_sat_rte(half2);
int2 __ovld __cnfn convert_int2_sat_rtp(half2);
int2 __ovld __cnfn convert_int2_sat_rtn(half2);
int2 __ovld __cnfn convert_int2_sat_rtz(half2);
int3 __ovld __cnfn convert_int3(half3);
int3 __ovld __cnfn convert_int3_rte(half3);
int3 __ovld __cnfn convert_int3_rtp(half3);
int3 __ovld __cnfn convert_int3_rtn(half3);
int3 __ovld __cnfn convert_int3_rtz(half3);
int3 __ovld __cnfn convert_int3_sat(half3);
int3 __ovld __cnfn convert_int3_sat_rte(half3);
int3 __ovld __cnfn convert_int3_sat_rtp(half3);
int3 __ovld __cnfn convert_int3_sat_rtn(half3);
int3 __ovld __cnfn convert_int3_sat_rtz(half3);
int4 __ovld __cnfn convert_int4(half4);
int4 __ovld __cnfn convert_int4_rte(half4);
int4 __ovld __cnfn convert_int4_rtp(half4);
int4 __ovld __cnfn convert_int4_rtn(half4);
int4 __ovld __cnfn convert_int4_rtz(half4);
int4 __ovld __cnfn convert_int4_sat(half4);
int4 __ovld __cnfn convert_int4_sat_rte(half4);
int4 __ovld __cnfn convert_int4_sat_rtp(half4);
int4 __ovld __cnfn convert_int4_sat_rtn(half4);
int4 __ovld __cnfn convert_int4_sat_rtz(half4);
int8 __ovld __cnfn convert_int8(half8);
int8 __ovld __cnfn convert_int8_rte(half8);
int8 __ovld __cnfn convert_int8_rtp(half8);
int8 __ovld __cnfn convert_int8_rtn(half8);
int8 __ovld __cnfn convert_int8_rtz(half8);
int8 __ovld __cnfn convert_int8_sat(half8);
int8 __ovld __cnfn convert_int8_sat_rte(half8);
int8 __ovld __cnfn convert_int8_sat_rtp(half8);
int8 __ovld __cnfn convert_int8_sat_rtn(half8);
int8 __ovld __cnfn convert_int8_sat_rtz(half8);
int16 __ovld __cnfn convert_int16(half16);
int16 __ovld __cnfn convert_int16_rte(half16);
int16 __ovld __cnfn convert_int16_rtp(half16);
int16 __ovld __cnfn convert_int16_rtn(half16);
int16 __ovld __cnfn convert_int16_rtz(half16);
int16 __ovld __cnfn convert_int16_sat(half16);
int16 __ovld __cnfn convert_int16_sat_rte(half16);
int16 __ovld __cnfn convert_int16_sat_rtp(half16);
int16 __ovld __cnfn convert_int16_sat_rtn(half16);
int16 __ovld __cnfn convert_int16_sat_rtz(half16);
long __ovld __cnfn convert_long(half);
long __ovld __cnfn convert_long_rte(half);
long __ovld __cnfn convert_long_rtp(half);
long __ovld __cnfn convert_long_rtn(half);
long __ovld __cnfn convert_long_rtz(half);
long __ovld __cnfn convert_long_sat(half);
long __ovld __cnfn convert_long_sat_rte(half);
long __ovld __cnfn convert_long_sat_rtp(half);
long __ovld __cnfn convert_long_sat_rtn(half);
long __ovld __cnfn convert_long_sat_rtz(half);
long2 __ovld __cnfn convert_long2(half2);
long2 __ovld __cnfn convert_long2_rte(half2);
long2 __ovld __cnfn convert_long2_rtp(half2);
long2 __ovld __cnfn convert_long2_rtn(half2);
long2 __ovld __cnfn convert_long2_rtz(half2);
long2 __ovld __cnfn convert_long2_sat(half2);
long2 __ovld __cnfn convert_long2_sat_rte(half2);
long2 __ovld __cnfn convert_long2_sat_rtp(half2);
long2 __ovld __cnfn convert_long2_sat_rtn(half2);
long2 __ovld __cnfn convert_long2_sat_rtz(half2);
long3 __ovld __cnfn convert_long3(half3);
long3 __ovld __cnfn convert_long3_rte(half3);
long3 __ovld __cnfn convert_long3_rtp(half3);
long3 __ovld __cnfn convert_long3_rtn(half3);
long3 __ovld __cnfn convert_long3_rtz(half3);
long3 __ovld __cnfn convert_long3_sat(half3);
long3 __ovld __cnfn convert_long3_sat_rte(half3);
long3 __ovld __cnfn convert_long3_sat_rtp(half3);
long3 __ovld __cnfn convert_long3_sat_rtn(half3);
long3 __ovld __cnfn convert_long3_sat_rtz(half3);
long4 __ovld __cnfn convert_long4(half4);
long4 __ovld __cnfn convert_long4_rte(half4);
long4 __ovld __cnfn convert_long4_rtp(half4);
long4 __ovld __cnfn convert_long4_rtn(half4);
long4 __ovld __cnfn convert_long4_rtz(half4);
long4 __ovld __cnfn convert_long4_sat(half4);
long4 __ovld __cnfn convert_long4_sat_rte(half4);
long4 __ovld __cnfn convert_long4_sat_rtp(half4);
long4 __ovld __cnfn convert_long4_sat_rtn(half4);
long4 __ovld __cnfn convert_long4_sat_rtz(half4);
long8 __ovld __cnfn convert_long8(half8);
long8 __ovld __cnfn convert_long8_rte(half8);
long8 __ovld __cnfn convert_long8_rtp(half8);
long8 __ovld __cnfn convert_long8_rtn(half8);
long8 __ovld __cnfn convert_long8_rtz(half8);
long8 __ovld __cnfn convert_long8_sat(half8);
long8 __ovld __cnfn convert_long8_sat_rte(half8);
long8 __ovld __cnfn convert_long8_sat_rtp(half8);
long8 __ovld __cnfn convert_long8_sat_rtn(half8);
long8 __ovld __cnfn convert_long8_sat_rtz(half8);
long16 __ovld __cnfn convert_long16(half16);
long16 __ovld __cnfn convert_long16_rte(half16);
long16 __ovld __cnfn convert_long16_rtp(half16);
long16 __ovld __cnfn convert_long16_rtn(half16);
long16 __ovld __cnfn convert_long16_rtz(half16);
long16 __ovld __cnfn convert_long16_sat(half16);
long16 __ovld __cnfn convert_long16_sat_rte(half16);
long16 __ovld __cnfn convert_long16_sat_rtp(half16);
long16 __ovld __cnfn convert_long16_sat_rtn(half16);
long16 __ovld __cnfn convert_long16_sat_rtz(half16);
float __ovld __cnfn convert_float(half);
float __ovld __cnfn convert_float_rte(half);
float __ovld __cnfn convert_float_rtp(half);
float __ovld __cnfn convert_float_rtn(half);
float __ovld __cnfn convert_float_rtz(half);
float2 __ovld __cnfn convert_float2(half2);
float2 __ovld __cnfn convert_float2_rte(half2);
float2 __ovld __cnfn convert_float2_rtp(half2);
float2 __ovld __cnfn convert_float2_rtn(half2);
float2 __ovld __cnfn convert_float2_rtz(half2);
float3 __ovld __cnfn convert_float3(half3);
float3 __ovld __cnfn convert_float3_rte(half3);
float3 __ovld __cnfn convert_float3_rtp(half3);
float3 __ovld __cnfn convert_float3_rtn(half3);
float3 __ovld __cnfn convert_float3_rtz(half3);
float4 __ovld __cnfn convert_float4(half4);
float4 __ovld __cnfn convert_float4_rte(half4);
float4 __ovld __cnfn convert_float4_rtp(half4);
float4 __ovld __cnfn convert_float4_rtn(half4);
float4 __ovld __cnfn convert_float4_rtz(half4);
float8 __ovld __cnfn convert_float8(half8);
float8 __ovld __cnfn convert_float8_rte(half8);
float8 __ovld __cnfn convert_float8_rtp(half8);
float8 __ovld __cnfn convert_float8_rtn(half8);
float8 __ovld __cnfn convert_float8_rtz(half8);
float16 __ovld __cnfn convert_float16(half16);
float16 __ovld __cnfn convert_float16_rte(half16);
float16 __ovld __cnfn convert_float16_rtp(half16);
float16 __ovld __cnfn convert_float16_rtn(half16);
float16 __ovld __cnfn convert_float16_rtz(half16);

// Convert non-double types to half types.
half __ovld __cnfn convert_half(uchar);
half __ovld __cnfn convert_half(ushort);
half __ovld __cnfn convert_half(uint);
half __ovld __cnfn convert_half(ulong);
half __ovld __cnfn convert_half(char);
half __ovld __cnfn convert_half(short);
half __ovld __cnfn convert_half(int);
half __ovld __cnfn convert_half(long);
half __ovld __cnfn convert_half(float);
half __ovld __cnfn convert_half(half);
half __ovld __cnfn convert_half_rte(uchar);
half __ovld __cnfn convert_half_rte(ushort);
half __ovld __cnfn convert_half_rte(uint);
half __ovld __cnfn convert_half_rte(ulong);
half __ovld __cnfn convert_half_rte(char);
half __ovld __cnfn convert_half_rte(short);
half __ovld __cnfn convert_half_rte(int);
half __ovld __cnfn convert_half_rte(long);
half __ovld __cnfn convert_half_rte(float);
half __ovld __cnfn convert_half_rte(half);
half __ovld __cnfn convert_half_rtp(uchar);
half __ovld __cnfn convert_half_rtp(ushort);
half __ovld __cnfn convert_half_rtp(uint);
half __ovld __cnfn convert_half_rtp(ulong);
half __ovld __cnfn convert_half_rtp(char);
half __ovld __cnfn convert_half_rtp(short);
half __ovld __cnfn convert_half_rtp(int);
half __ovld __cnfn convert_half_rtp(long);
half __ovld __cnfn convert_half_rtp(float);
half __ovld __cnfn convert_half_rtp(half);
half __ovld __cnfn convert_half_rtn(uchar);
half __ovld __cnfn convert_half_rtn(ushort);
half __ovld __cnfn convert_half_rtn(uint);
half __ovld __cnfn convert_half_rtn(ulong);
half __ovld __cnfn convert_half_rtn(char);
half __ovld __cnfn convert_half_rtn(short);
half __ovld __cnfn convert_half_rtn(int);
half __ovld __cnfn convert_half_rtn(long);
half __ovld __cnfn convert_half_rtn(float);
half __ovld __cnfn convert_half_rtn(half);
half __ovld __cnfn convert_half_rtz(uchar);
half __ovld __cnfn convert_half_rtz(ushort);
half __ovld __cnfn convert_half_rtz(uint);
half __ovld __cnfn convert_half_rtz(ulong);
half __ovld __cnfn convert_half_rtz(char);
half __ovld __cnfn convert_half_rtz(short);
half __ovld __cnfn convert_half_rtz(int);
half __ovld __cnfn convert_half_rtz(long);
half __ovld __cnfn convert_half_rtz(float);
half __ovld __cnfn convert_half_rtz(half);
half2 __ovld __cnfn convert_half2(char2);
half2 __ovld __cnfn convert_half2(uchar2);
half2 __ovld __cnfn convert_half2(short2);
half2 __ovld __cnfn convert_half2(ushort2);
half2 __ovld __cnfn convert_half2(int2);
half2 __ovld __cnfn convert_half2(uint2);
half2 __ovld __cnfn convert_half2(long2);
half2 __ovld __cnfn convert_half2(ulong2);
half2 __ovld __cnfn convert_half2(float2);
half2 __ovld __cnfn convert_half2(half2);
half2 __ovld __cnfn convert_half2_rte(char2);
half2 __ovld __cnfn convert_half2_rte(uchar2);
half2 __ovld __cnfn convert_half2_rte(short2);
half2 __ovld __cnfn convert_half2_rte(ushort2);
half2 __ovld __cnfn convert_half2_rte(int2);
half2 __ovld __cnfn convert_half2_rte(uint2);
half2 __ovld __cnfn convert_half2_rte(long2);
half2 __ovld __cnfn convert_half2_rte(ulong2);
half2 __ovld __cnfn convert_half2_rte(float2);
half2 __ovld __cnfn convert_half2_rte(half2);
half2 __ovld __cnfn convert_half2_rtp(char2);
half2 __ovld __cnfn convert_half2_rtp(uchar2);
half2 __ovld __cnfn convert_half2_rtp(short2);
half2 __ovld __cnfn convert_half2_rtp(ushort2);
half2 __ovld __cnfn convert_half2_rtp(int2);
half2 __ovld __cnfn convert_half2_rtp(uint2);
half2 __ovld __cnfn convert_half2_rtp(long2);
half2 __ovld __cnfn convert_half2_rtp(ulong2);
half2 __ovld __cnfn convert_half2_rtp(float2);
half2 __ovld __cnfn convert_half2_rtp(half2);
half2 __ovld __cnfn convert_half2_rtn(char2);
half2 __ovld __cnfn convert_half2_rtn(uchar2);
half2 __ovld __cnfn convert_half2_rtn(short2);
half2 __ovld __cnfn convert_half2_rtn(ushort2);
half2 __ovld __cnfn convert_half2_rtn(int2);
half2 __ovld __cnfn convert_half2_rtn(uint2);
half2 __ovld __cnfn convert_half2_rtn(long2);
half2 __ovld __cnfn convert_half2_rtn(ulong2);
half2 __ovld __cnfn convert_half2_rtn(float2);
half2 __ovld __cnfn convert_half2_rtn(half2);
half2 __ovld __cnfn convert_half2_rtz(char2);
half2 __ovld __cnfn convert_half2_rtz(uchar2);
half2 __ovld __cnfn convert_half2_rtz(short2);
half2 __ovld __cnfn convert_half2_rtz(ushort2);
half2 __ovld __cnfn convert_half2_rtz(int2);
half2 __ovld __cnfn convert_half2_rtz(uint2);
half2 __ovld __cnfn convert_half2_rtz(long2);
half2 __ovld __cnfn convert_half2_rtz(ulong2);
half2 __ovld __cnfn convert_half2_rtz(float2);
half2 __ovld __cnfn convert_half2_rtz(half2);
half3 __ovld __cnfn convert_half3(char3);
half3 __ovld __cnfn convert_half3(uchar3);
half3 __ovld __cnfn convert_half3(short3);
half3 __ovld __cnfn convert_half3(ushort3);
half3 __ovld __cnfn convert_half3(int3);
half3 __ovld __cnfn convert_half3(uint3);
half3 __ovld __cnfn convert_half3(long3);
half3 __ovld __cnfn convert_half3(ulong3);
half3 __ovld __cnfn convert_half3(float3);
half3 __ovld __cnfn convert_half3(half3);
half3 __ovld __cnfn convert_half3_rte(char3);
half3 __ovld __cnfn convert_half3_rte(uchar3);
half3 __ovld __cnfn convert_half3_rte(short3);
half3 __ovld __cnfn convert_half3_rte(ushort3);
half3 __ovld __cnfn convert_half3_rte(int3);
half3 __ovld __cnfn convert_half3_rte(uint3);
half3 __ovld __cnfn convert_half3_rte(long3);
half3 __ovld __cnfn convert_half3_rte(ulong3);
half3 __ovld __cnfn convert_half3_rte(float3);
half3 __ovld __cnfn convert_half3_rte(half3);
half3 __ovld __cnfn convert_half3_rtp(char3);
half3 __ovld __cnfn convert_half3_rtp(uchar3);
half3 __ovld __cnfn convert_half3_rtp(short3);
half3 __ovld __cnfn convert_half3_rtp(ushort3);
half3 __ovld __cnfn convert_half3_rtp(int3);
half3 __ovld __cnfn convert_half3_rtp(uint3);
half3 __ovld __cnfn convert_half3_rtp(long3);
half3 __ovld __cnfn convert_half3_rtp(ulong3);
half3 __ovld __cnfn convert_half3_rtp(float3);
half3 __ovld __cnfn convert_half3_rtp(half3);
half3 __ovld __cnfn convert_half3_rtn(char3);
half3 __ovld __cnfn convert_half3_rtn(uchar3);
half3 __ovld __cnfn convert_half3_rtn(short3);
half3 __ovld __cnfn convert_half3_rtn(ushort3);
half3 __ovld __cnfn convert_half3_rtn(int3);
half3 __ovld __cnfn convert_half3_rtn(uint3);
half3 __ovld __cnfn convert_half3_rtn(long3);
half3 __ovld __cnfn convert_half3_rtn(ulong3);
half3 __ovld __cnfn convert_half3_rtn(float3);
half3 __ovld __cnfn convert_half3_rtn(half3);
half3 __ovld __cnfn convert_half3_rtz(char3);
half3 __ovld __cnfn convert_half3_rtz(uchar3);
half3 __ovld __cnfn convert_half3_rtz(short3);
half3 __ovld __cnfn convert_half3_rtz(ushort3);
half3 __ovld __cnfn convert_half3_rtz(int3);
half3 __ovld __cnfn convert_half3_rtz(uint3);
half3 __ovld __cnfn convert_half3_rtz(long3);
half3 __ovld __cnfn convert_half3_rtz(ulong3);
half3 __ovld __cnfn convert_half3_rtz(float3);
half3 __ovld __cnfn convert_half3_rtz(half3);
half4 __ovld __cnfn convert_half4(char4);
half4 __ovld __cnfn convert_half4(uchar4);
half4 __ovld __cnfn convert_half4(short4);
half4 __ovld __cnfn convert_half4(ushort4);
half4 __ovld __cnfn convert_half4(int4);
half4 __ovld __cnfn convert_half4(uint4);
half4 __ovld __cnfn convert_half4(long4);
half4 __ovld __cnfn convert_half4(ulong4);
half4 __ovld __cnfn convert_half4(float4);
half4 __ovld __cnfn convert_half4(half4);
half4 __ovld __cnfn convert_half4_rte(char4);
half4 __ovld __cnfn convert_half4_rte(uchar4);
half4 __ovld __cnfn convert_half4_rte(short4);
half4 __ovld __cnfn convert_half4_rte(ushort4);
half4 __ovld __cnfn convert_half4_rte(int4);
half4 __ovld __cnfn convert_half4_rte(uint4);
half4 __ovld __cnfn convert_half4_rte(long4);
half4 __ovld __cnfn convert_half4_rte(ulong4);
half4 __ovld __cnfn convert_half4_rte(float4);
half4 __ovld __cnfn convert_half4_rte(half4);
half4 __ovld __cnfn convert_half4_rtp(char4);
half4 __ovld __cnfn convert_half4_rtp(uchar4);
half4 __ovld __cnfn convert_half4_rtp(short4);
half4 __ovld __cnfn convert_half4_rtp(ushort4);
half4 __ovld __cnfn convert_half4_rtp(int4);
half4 __ovld __cnfn convert_half4_rtp(uint4);
half4 __ovld __cnfn convert_half4_rtp(long4);
half4 __ovld __cnfn convert_half4_rtp(ulong4);
half4 __ovld __cnfn convert_half4_rtp(float4);
half4 __ovld __cnfn convert_half4_rtp(half4);
half4 __ovld __cnfn convert_half4_rtn(char4);
half4 __ovld __cnfn convert_half4_rtn(uchar4);
half4 __ovld __cnfn convert_half4_rtn(short4);
half4 __ovld __cnfn convert_half4_rtn(ushort4);
half4 __ovld __cnfn convert_half4_rtn(int4);
half4 __ovld __cnfn convert_half4_rtn(uint4);
half4 __ovld __cnfn convert_half4_rtn(long4);
half4 __ovld __cnfn convert_half4_rtn(ulong4);
half4 __ovld __cnfn convert_half4_rtn(float4);
half4 __ovld __cnfn convert_half4_rtn(half4);
half4 __ovld __cnfn convert_half4_rtz(char4);
half4 __ovld __cnfn convert_half4_rtz(uchar4);
half4 __ovld __cnfn convert_half4_rtz(short4);
half4 __ovld __cnfn convert_half4_rtz(ushort4);
half4 __ovld __cnfn convert_half4_rtz(int4);
half4 __ovld __cnfn convert_half4_rtz(uint4);
half4 __ovld __cnfn convert_half4_rtz(long4);
half4 __ovld __cnfn convert_half4_rtz(ulong4);
half4 __ovld __cnfn convert_half4_rtz(float4);
half4 __ovld __cnfn convert_half4_rtz(half4);
half8 __ovld __cnfn convert_half8(char8);
half8 __ovld __cnfn convert_half8(uchar8);
half8 __ovld __cnfn convert_half8(short8);
half8 __ovld __cnfn convert_half8(ushort8);
half8 __ovld __cnfn convert_half8(int8);
half8 __ovld __cnfn convert_half8(uint8);
half8 __ovld __cnfn convert_half8(long8);
half8 __ovld __cnfn convert_half8(ulong8);
half8 __ovld __cnfn convert_half8(float8);
half8 __ovld __cnfn convert_half8(half8);
half8 __ovld __cnfn convert_half8_rte(char8);
half8 __ovld __cnfn convert_half8_rte(uchar8);
half8 __ovld __cnfn convert_half8_rte(short8);
half8 __ovld __cnfn convert_half8_rte(ushort8);
half8 __ovld __cnfn convert_half8_rte(int8);
half8 __ovld __cnfn convert_half8_rte(uint8);
half8 __ovld __cnfn convert_half8_rte(long8);
half8 __ovld __cnfn convert_half8_rte(ulong8);
half8 __ovld __cnfn convert_half8_rte(float8);
half8 __ovld __cnfn convert_half8_rte(half8);
half8 __ovld __cnfn convert_half8_rtp(char8);
half8 __ovld __cnfn convert_half8_rtp(uchar8);
half8 __ovld __cnfn convert_half8_rtp(short8);
half8 __ovld __cnfn convert_half8_rtp(ushort8);
half8 __ovld __cnfn convert_half8_rtp(int8);
half8 __ovld __cnfn convert_half8_rtp(uint8);
half8 __ovld __cnfn convert_half8_rtp(long8);
half8 __ovld __cnfn convert_half8_rtp(ulong8);
half8 __ovld __cnfn convert_half8_rtp(float8);
half8 __ovld __cnfn convert_half8_rtp(half8);
half8 __ovld __cnfn convert_half8_rtn(char8);
half8 __ovld __cnfn convert_half8_rtn(uchar8);
half8 __ovld __cnfn convert_half8_rtn(short8);
half8 __ovld __cnfn convert_half8_rtn(ushort8);
half8 __ovld __cnfn convert_half8_rtn(int8);
half8 __ovld __cnfn convert_half8_rtn(uint8);
half8 __ovld __cnfn convert_half8_rtn(long8);
half8 __ovld __cnfn convert_half8_rtn(ulong8);
half8 __ovld __cnfn convert_half8_rtn(float8);
half8 __ovld __cnfn convert_half8_rtn(half8);
half8 __ovld __cnfn convert_half8_rtz(char8);
half8 __ovld __cnfn convert_half8_rtz(uchar8);
half8 __ovld __cnfn convert_half8_rtz(short8);
half8 __ovld __cnfn convert_half8_rtz(ushort8);
half8 __ovld __cnfn convert_half8_rtz(int8);
half8 __ovld __cnfn convert_half8_rtz(uint8);
half8 __ovld __cnfn convert_half8_rtz(long8);
half8 __ovld __cnfn convert_half8_rtz(ulong8);
half8 __ovld __cnfn convert_half8_rtz(float8);
half8 __ovld __cnfn convert_half8_rtz(half8);
half16 __ovld __cnfn convert_half16(char16);
half16 __ovld __cnfn convert_half16(uchar16);
half16 __ovld __cnfn convert_half16(short16);
half16 __ovld __cnfn convert_half16(ushort16);
half16 __ovld __cnfn convert_half16(int16);
half16 __ovld __cnfn convert_half16(uint16);
half16 __ovld __cnfn convert_half16(long16);
half16 __ovld __cnfn convert_half16(ulong16);
half16 __ovld __cnfn convert_half16(float16);
half16 __ovld __cnfn convert_half16(half16);
half16 __ovld __cnfn convert_half16_rte(char16);
half16 __ovld __cnfn convert_half16_rte(uchar16);
half16 __ovld __cnfn convert_half16_rte(short16);
half16 __ovld __cnfn convert_half16_rte(ushort16);
half16 __ovld __cnfn convert_half16_rte(int16);
half16 __ovld __cnfn convert_half16_rte(uint16);
half16 __ovld __cnfn convert_half16_rte(long16);
half16 __ovld __cnfn convert_half16_rte(ulong16);
half16 __ovld __cnfn convert_half16_rte(float16);
half16 __ovld __cnfn convert_half16_rte(half16);
half16 __ovld __cnfn convert_half16_rtp(char16);
half16 __ovld __cnfn convert_half16_rtp(uchar16);
half16 __ovld __cnfn convert_half16_rtp(short16);
half16 __ovld __cnfn convert_half16_rtp(ushort16);
half16 __ovld __cnfn convert_half16_rtp(int16);
half16 __ovld __cnfn convert_half16_rtp(uint16);
half16 __ovld __cnfn convert_half16_rtp(long16);
half16 __ovld __cnfn convert_half16_rtp(ulong16);
half16 __ovld __cnfn convert_half16_rtp(float16);
half16 __ovld __cnfn convert_half16_rtp(half16);
half16 __ovld __cnfn convert_half16_rtn(char16);
half16 __ovld __cnfn convert_half16_rtn(uchar16);
half16 __ovld __cnfn convert_half16_rtn(short16);
half16 __ovld __cnfn convert_half16_rtn(ushort16);
half16 __ovld __cnfn convert_half16_rtn(int16);
half16 __ovld __cnfn convert_half16_rtn(uint16);
half16 __ovld __cnfn convert_half16_rtn(long16);
half16 __ovld __cnfn convert_half16_rtn(ulong16);
half16 __ovld __cnfn convert_half16_rtn(float16);
half16 __ovld __cnfn convert_half16_rtn(half16);
half16 __ovld __cnfn convert_half16_rtz(char16);
half16 __ovld __cnfn convert_half16_rtz(uchar16);
half16 __ovld __cnfn convert_half16_rtz(short16);
half16 __ovld __cnfn convert_half16_rtz(ushort16);
half16 __ovld __cnfn convert_half16_rtz(int16);
half16 __ovld __cnfn convert_half16_rtz(uint16);
half16 __ovld __cnfn convert_half16_rtz(long16);
half16 __ovld __cnfn convert_half16_rtz(ulong16);
half16 __ovld __cnfn convert_half16_rtz(float16);
half16 __ovld __cnfn convert_half16_rtz(half16);

// Convert half types to double types.
#ifdef cl_khr_fp64
double __ovld __cnfn convert_double(half);
double __ovld __cnfn convert_double_rte(half);
double __ovld __cnfn convert_double_rtp(half);
double __ovld __cnfn convert_double_rtn(half);
double __ovld __cnfn convert_double_rtz(half);
double2 __ovld __cnfn convert_double2(half2);
double2 __ovld __cnfn convert_double2_rte(half2);
double2 __ovld __cnfn convert_double2_rtp(half2);
double2 __ovld __cnfn convert_double2_rtn(half2);
double2 __ovld __cnfn convert_double2_rtz(half2);
double3 __ovld __cnfn convert_double3(half3);
double3 __ovld __cnfn convert_double3_rte(half3);
double3 __ovld __cnfn convert_double3_rtp(half3);
double3 __ovld __cnfn convert_double3_rtn(half3);
double3 __ovld __cnfn convert_double3_rtz(half3);
double4 __ovld __cnfn convert_double4(half4);
double4 __ovld __cnfn convert_double4_rte(half4);
double4 __ovld __cnfn convert_double4_rtp(half4);
double4 __ovld __cnfn convert_double4_rtn(half4);
double4 __ovld __cnfn convert_double4_rtz(half4);
double8 __ovld __cnfn convert_double8(half8);
double8 __ovld __cnfn convert_double8_rte(half8);
double8 __ovld __cnfn convert_double8_rtp(half8);
double8 __ovld __cnfn convert_double8_rtn(half8);
double8 __ovld __cnfn convert_double8_rtz(half8);
double16 __ovld __cnfn convert_double16(half16);
double16 __ovld __cnfn convert_double16_rte(half16);
double16 __ovld __cnfn convert_double16_rtp(half16);
double16 __ovld __cnfn convert_double16_rtn(half16);
double16 __ovld __cnfn convert_double16_rtz(half16);

// Convert double types to half types.
half __ovld __cnfn convert_half(double);
half __ovld __cnfn convert_half_rte(double);
half __ovld __cnfn convert_half_rtp(double);
half __ovld __cnfn convert_half_rtn(double);
half __ovld __cnfn convert_half_rtz(double);
half2 __ovld __cnfn convert_half2(double2);
half2 __ovld __cnfn convert_half2_rte(double2);
half2 __ovld __cnfn convert_half2_rtp(double2);
half2 __ovld __cnfn convert_half2_rtn(double2);
half2 __ovld __cnfn convert_half2_rtz(double2);
half3 __ovld __cnfn convert_half3(double3);
half3 __ovld __cnfn convert_half3_rte(double3);
half3 __ovld __cnfn convert_half3_rtp(double3);
half3 __ovld __cnfn convert_half3_rtn(double3);
half3 __ovld __cnfn convert_half3_rtz(double3);
half4 __ovld __cnfn convert_half4(double4);
half4 __ovld __cnfn convert_half4_rte(double4);
half4 __ovld __cnfn convert_half4_rtp(double4);
half4 __ovld __cnfn convert_half4_rtn(double4);
half4 __ovld __cnfn convert_half4_rtz(double4);
half8 __ovld __cnfn convert_half8(double8);
half8 __ovld __cnfn convert_half8_rte(double8);
half8 __ovld __cnfn convert_half8_rtp(double8);
half8 __ovld __cnfn convert_half8_rtn(double8);
half8 __ovld __cnfn convert_half8_rtz(double8);
half16 __ovld __cnfn convert_half16(double16);
half16 __ovld __cnfn convert_half16_rte(double16);
half16 __ovld __cnfn convert_half16_rtp(double16);
half16 __ovld __cnfn convert_half16_rtn(double16);
half16 __ovld __cnfn convert_half16_rtz(double16);
#endif //cl_khr_fp64

#endif // cl_khr_fp16

// OpenCL v1.1 s6.11.1, v1.2 s6.12.1, v2.0 s6.13.1 - Work-item Functions

/**
 * Returns the number of dimensions in use. This is the
 * value given to the work_dim argument specified in
 * clEnqueueNDRangeKernel.
 * For clEnqueueTask, this returns 1.
 */
uint __ovld __cnfn get_work_dim(void);

/**
 * Returns the number of global work-items specified for
 * dimension identified by dimindx. This value is given by
 * the global_work_size argument to
 * clEnqueueNDRangeKernel. Valid values of dimindx
 * are 0 to get_work_dim() - 1. For other values of
 * dimindx, get_global_size() returns 1.
 * For clEnqueueTask, this always returns 1.
 */
size_t __ovld __cnfn get_global_size(uint);

/**
 * Returns the unique global work-item ID value for
 * dimension identified by dimindx. The global work-item
 * ID specifies the work-item ID based on the number of
 * global work-items specified to execute the kernel. Valid
 * values of dimindx are 0 to get_work_dim() - 1. For
 * other values of dimindx, get_global_id() returns 0.
 * For clEnqueueTask, this returns 0.
 */
size_t __ovld __cnfn get_global_id(uint);

/**
 * Returns the number of local work-items specified in
 * dimension identified by dimindx. This value is given by
 * the local_work_size argument to
 * clEnqueueNDRangeKernel if local_work_size is not
 * NULL; otherwise the OpenCL implementation chooses
 * an appropriate local_work_size value which is returned
 * by this function. Valid values of dimindx are 0 to
 * get_work_dim() - 1. For other values of dimindx,
 * get_local_size() returns 1.
 * For clEnqueueTask, this always returns 1.
 */
size_t __ovld __cnfn get_local_size(uint);

/**
 * Returns the unique local work-item ID i.e. a work-item
 * within a specific work-group for dimension identified by
 * dimindx. Valid values of dimindx are 0 to
 * get_work_dim() - 1. For other values of dimindx,
 * get_local_id() returns 0.
 * For clEnqueueTask, this returns 0.
 */
size_t __ovld __cnfn get_local_id(uint);

/**
 * Returns the number of work-groups that will execute a
 * kernel for dimension identified by dimindx.
 * Valid values of dimindx are 0 to get_work_dim() - 1.
 * For other values of dimindx, get_num_groups() returns 1.
 * For clEnqueueTask, this always returns 1.
 */
size_t __ovld __cnfn get_num_groups(uint);

/**
 * get_group_id returns the work-group ID which is a
 * number from 0 .. get_num_groups(dimindx) - 1.
 * Valid values of dimindx are 0 to get_work_dim() - 1.
 * For other values, get_group_id() returns 0.
 * For clEnqueueTask, this returns 0.
 */
size_t __ovld __cnfn get_group_id(uint);

/**
 * get_global_offset returns the offset values specified in
 * global_work_offset argument to
 * clEnqueueNDRangeKernel.
 * Valid values of dimindx are 0 to get_work_dim() - 1.
 * For other values, get_global_offset() returns 0.
 * For clEnqueueTask, this returns 0.
 */
size_t __ovld __cnfn get_global_offset(uint);

#if defined(__OPENCL_CPP_VERSION__) || (__OPENCL_C_VERSION__ >= CL_VERSION_2_0)
size_t __ovld get_enqueued_local_size(uint);
size_t __ovld get_global_linear_id(void);
size_t __ovld get_local_linear_id(void);
#endif //defined(__OPENCL_CPP_VERSION__) || (__OPENCL_C_VERSION__ >= CL_VERSION_2_0)

// OpenCL v1.1 s6.11.2, v1.2 s6.12.2, v2.0 s6.13.2 - Math functions

/**
 * Arc cosine function.
 */
float __ovld __cnfn acos(float);
float2 __ovld __cnfn acos(float2);
float3 __ovld __cnfn acos(float3);
float4 __ovld __cnfn acos(float4);
float8 __ovld __cnfn acos(float8);
float16 __ovld __cnfn acos(float16);
#ifdef cl_khr_fp64
double __ovld __cnfn acos(double);
double2 __ovld __cnfn acos(double2);
double3 __ovld __cnfn acos(double3);
double4 __ovld __cnfn acos(double4);
double8 __ovld __cnfn acos(double8);
double16 __ovld __cnfn acos(double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn acos(half);
half2 __ovld __cnfn acos(half2);
half3 __ovld __cnfn acos(half3);
half4 __ovld __cnfn acos(half4);
half8 __ovld __cnfn acos(half8);
half16 __ovld __cnfn acos(half16);
#endif //cl_khr_fp16

/**
 * Inverse hyperbolic cosine.
 */
float __ovld __cnfn acosh(float);
float2 __ovld __cnfn acosh(float2);
float3 __ovld __cnfn acosh(float3);
float4 __ovld __cnfn acosh(float4);
float8 __ovld __cnfn acosh(float8);
float16 __ovld __cnfn acosh(float16);
#ifdef cl_khr_fp64
double __ovld __cnfn acosh(double);
double2 __ovld __cnfn acosh(double2);
double3 __ovld __cnfn acosh(double3);
double4 __ovld __cnfn acosh(double4);
double8 __ovld __cnfn acosh(double8);
double16 __ovld __cnfn acosh(double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn acosh(half);
half2 __ovld __cnfn acosh(half2);
half3 __ovld __cnfn acosh(half3);
half4 __ovld __cnfn acosh(half4);
half8 __ovld __cnfn acosh(half8);
half16 __ovld __cnfn acosh(half16);
#endif //cl_khr_fp16

/**
 * Compute acos (x) / PI.
 */
float __ovld __cnfn acospi(float);
float2 __ovld __cnfn acospi(float2);
float3 __ovld __cnfn acospi(float3);
float4 __ovld __cnfn acospi(float4);
float8 __ovld __cnfn acospi(float8);
float16 __ovld __cnfn acospi(float16);
#ifdef cl_khr_fp64
double __ovld __cnfn acospi(double);
double2 __ovld __cnfn acospi(double2);
double3 __ovld __cnfn acospi(double3);
double4 __ovld __cnfn acospi(double4);
double8 __ovld __cnfn acospi(double8);
double16 __ovld __cnfn acospi(double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn acospi(half);
half2 __ovld __cnfn acospi(half2);
half3 __ovld __cnfn acospi(half3);
half4 __ovld __cnfn acospi(half4);
half8 __ovld __cnfn acospi(half8);
half16 __ovld __cnfn acospi(half16);
#endif //cl_khr_fp16

/**
 * Arc sine function.
 */
float __ovld __cnfn asin(float);
float2 __ovld __cnfn asin(float2);
float3 __ovld __cnfn asin(float3);
float4 __ovld __cnfn asin(float4);
float8 __ovld __cnfn asin(float8);
float16 __ovld __cnfn asin(float16);
#ifdef cl_khr_fp64
double __ovld __cnfn asin(double);
double2 __ovld __cnfn asin(double2);
double3 __ovld __cnfn asin(double3);
double4 __ovld __cnfn asin(double4);
double8 __ovld __cnfn asin(double8);
double16 __ovld __cnfn asin(double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn asin(half);
half2 __ovld __cnfn asin(half2);
half3 __ovld __cnfn asin(half3);
half4 __ovld __cnfn asin(half4);
half8 __ovld __cnfn asin(half8);
half16 __ovld __cnfn asin(half16);
#endif //cl_khr_fp16

/**
 * Inverse hyperbolic sine.
 */
float __ovld __cnfn asinh(float);
float2 __ovld __cnfn asinh(float2);
float3 __ovld __cnfn asinh(float3);
float4 __ovld __cnfn asinh(float4);
float8 __ovld __cnfn asinh(float8);
float16 __ovld __cnfn asinh(float16);
#ifdef cl_khr_fp64
double __ovld __cnfn asinh(double);
double2 __ovld __cnfn asinh(double2);
double3 __ovld __cnfn asinh(double3);
double4 __ovld __cnfn asinh(double4);
double8 __ovld __cnfn asinh(double8);
double16 __ovld __cnfn asinh(double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn asinh(half);
half2 __ovld __cnfn asinh(half2);
half3 __ovld __cnfn asinh(half3);
half4 __ovld __cnfn asinh(half4);
half8 __ovld __cnfn asinh(half8);
half16 __ovld __cnfn asinh(half16);
#endif //cl_khr_fp16

/**
 * Compute asin (x) / PI.
 */
float __ovld __cnfn asinpi(float);
float2 __ovld __cnfn asinpi(float2);
float3 __ovld __cnfn asinpi(float3);
float4 __ovld __cnfn asinpi(float4);
float8 __ovld __cnfn asinpi(float8);
float16 __ovld __cnfn asinpi(float16);
#ifdef cl_khr_fp64
double __ovld __cnfn asinpi(double);
double2 __ovld __cnfn asinpi(double2);
double3 __ovld __cnfn asinpi(double3);
double4 __ovld __cnfn asinpi(double4);
double8 __ovld __cnfn asinpi(double8);
double16 __ovld __cnfn asinpi(double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn asinpi(half);
half2 __ovld __cnfn asinpi(half2);
half3 __ovld __cnfn asinpi(half3);
half4 __ovld __cnfn asinpi(half4);
half8 __ovld __cnfn asinpi(half8);
half16 __ovld __cnfn asinpi(half16);
#endif //cl_khr_fp16

/**
 * Arc tangent function.
 */
float __ovld __cnfn atan(float);
float2 __ovld __cnfn atan(float2);
float3 __ovld __cnfn atan(float3);
float4 __ovld __cnfn atan(float4);
float8 __ovld __cnfn atan(float8);
float16 __ovld __cnfn atan(float16);
#ifdef cl_khr_fp64
double __ovld __cnfn atan(double);
double2 __ovld __cnfn atan(double2);
double3 __ovld __cnfn atan(double3);
double4 __ovld __cnfn atan(double4);
double8 __ovld __cnfn atan(double8);
double16 __ovld __cnfn atan(double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn atan(half);
half2 __ovld __cnfn atan(half2);
half3 __ovld __cnfn atan(half3);
half4 __ovld __cnfn atan(half4);
half8 __ovld __cnfn atan(half8);
half16 __ovld __cnfn atan(half16);
#endif //cl_khr_fp16

/**
 * Arc tangent of y / x.
 */
float __ovld __cnfn atan2(float, float);
float2 __ovld __cnfn atan2(float2, float2);
float3 __ovld __cnfn atan2(float3, float3);
float4 __ovld __cnfn atan2(float4, float4);
float8 __ovld __cnfn atan2(float8, float8);
float16 __ovld __cnfn atan2(float16, float16);
#ifdef cl_khr_fp64
double __ovld __cnfn atan2(double, double);
double2 __ovld __cnfn atan2(double2, double2);
double3 __ovld __cnfn atan2(double3, double3);
double4 __ovld __cnfn atan2(double4, double4);
double8 __ovld __cnfn atan2(double8, double8);
double16 __ovld __cnfn atan2(double16, double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn atan2(half, half);
half2 __ovld __cnfn atan2(half2, half2);
half3 __ovld __cnfn atan2(half3, half3);
half4 __ovld __cnfn atan2(half4, half4);
half8 __ovld __cnfn atan2(half8, half8);
half16 __ovld __cnfn atan2(half16, half16);
#endif //cl_khr_fp16

/**
 * Hyperbolic arc tangent.
 */
float __ovld __cnfn atanh(float);
float2 __ovld __cnfn atanh(float2);
float3 __ovld __cnfn atanh(float3);
float4 __ovld __cnfn atanh(float4);
float8 __ovld __cnfn atanh(float8);
float16 __ovld __cnfn atanh(float16);
#ifdef cl_khr_fp64
double __ovld __cnfn atanh(double);
double2 __ovld __cnfn atanh(double2);
double3 __ovld __cnfn atanh(double3);
double4 __ovld __cnfn atanh(double4);
double8 __ovld __cnfn atanh(double8);
double16 __ovld __cnfn atanh(double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn atanh(half);
half2 __ovld __cnfn atanh(half2);
half3 __ovld __cnfn atanh(half3);
half4 __ovld __cnfn atanh(half4);
half8 __ovld __cnfn atanh(half8);
half16 __ovld __cnfn atanh(half16);
#endif //cl_khr_fp16

/**
 * Compute atan (x) / PI.
 */
float __ovld __cnfn atanpi(float);
float2 __ovld __cnfn atanpi(float2);
float3 __ovld __cnfn atanpi(float3);
float4 __ovld __cnfn atanpi(float4);
float8 __ovld __cnfn atanpi(float8);
float16 __ovld __cnfn atanpi(float16);
#ifdef cl_khr_fp64
double __ovld __cnfn atanpi(double);
double2 __ovld __cnfn atanpi(double2);
double3 __ovld __cnfn atanpi(double3);
double4 __ovld __cnfn atanpi(double4);
double8 __ovld __cnfn atanpi(double8);
double16 __ovld __cnfn atanpi(double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn atanpi(half);
half2 __ovld __cnfn atanpi(half2);
half3 __ovld __cnfn atanpi(half3);
half4 __ovld __cnfn atanpi(half4);
half8 __ovld __cnfn atanpi(half8);
half16 __ovld __cnfn atanpi(half16);
#endif //cl_khr_fp16

/**
 * Compute atan2 (y, x) / PI.
 */
float __ovld __cnfn atan2pi(float, float);
float2 __ovld __cnfn atan2pi(float2, float2);
float3 __ovld __cnfn atan2pi(float3, float3);
float4 __ovld __cnfn atan2pi(float4, float4);
float8 __ovld __cnfn atan2pi(float8, float8);
float16 __ovld __cnfn atan2pi(float16, float16);
#ifdef cl_khr_fp64
double __ovld __cnfn atan2pi(double, double);
double2 __ovld __cnfn atan2pi(double2, double2);
double3 __ovld __cnfn atan2pi(double3, double3);
double4 __ovld __cnfn atan2pi(double4, double4);
double8 __ovld __cnfn atan2pi(double8, double8);
double16 __ovld __cnfn atan2pi(double16, double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn atan2pi(half, half);
half2 __ovld __cnfn atan2pi(half2, half2);
half3 __ovld __cnfn atan2pi(half3, half3);
half4 __ovld __cnfn atan2pi(half4, half4);
half8 __ovld __cnfn atan2pi(half8, half8);
half16 __ovld __cnfn atan2pi(half16, half16);
#endif //cl_khr_fp16

/**
 * Compute cube-root.
 */
float __ovld __cnfn cbrt(float);
float2 __ovld __cnfn cbrt(float2);
float3 __ovld __cnfn cbrt(float3);
float4 __ovld __cnfn cbrt(float4);
float8 __ovld __cnfn cbrt(float8);
float16 __ovld __cnfn cbrt(float16);
#ifdef cl_khr_fp64
double __ovld __cnfn cbrt(double);
double2 __ovld __cnfn cbrt(double2);
double3 __ovld __cnfn cbrt(double3);
double4 __ovld __cnfn cbrt(double4);
double8 __ovld __cnfn cbrt(double8);
double16 __ovld __cnfn cbrt(double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn cbrt(half);
half2 __ovld __cnfn cbrt(half2);
half3 __ovld __cnfn cbrt(half3);
half4 __ovld __cnfn cbrt(half4);
half8 __ovld __cnfn cbrt(half8);
half16 __ovld __cnfn cbrt(half16);
#endif //cl_khr_fp16

/**
 * Round to integral value using the round to positive
 * infinity rounding mode.
 */
float __ovld __cnfn ceil(float);
float2 __ovld __cnfn ceil(float2);
float3 __ovld __cnfn ceil(float3);
float4 __ovld __cnfn ceil(float4);
float8 __ovld __cnfn ceil(float8);
float16 __ovld __cnfn ceil(float16);
#ifdef cl_khr_fp64
double __ovld __cnfn ceil(double);
double2 __ovld __cnfn ceil(double2);
double3 __ovld __cnfn ceil(double3);
double4 __ovld __cnfn ceil(double4);
double8 __ovld __cnfn ceil(double8);
double16 __ovld __cnfn ceil(double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn ceil(half);
half2 __ovld __cnfn ceil(half2);
half3 __ovld __cnfn ceil(half3);
half4 __ovld __cnfn ceil(half4);
half8 __ovld __cnfn ceil(half8);
half16 __ovld __cnfn ceil(half16);
#endif //cl_khr_fp16

/**
 * Returns x with its sign changed to match the sign of y.
 */
float __ovld __cnfn copysign(float, float);
float2 __ovld __cnfn copysign(float2, float2);
float3 __ovld __cnfn copysign(float3, float3);
float4 __ovld __cnfn copysign(float4, float4);
float8 __ovld __cnfn copysign(float8, float8);
float16 __ovld __cnfn copysign(float16, float16);
#ifdef cl_khr_fp64
double __ovld __cnfn copysign(double, double);
double2 __ovld __cnfn copysign(double2, double2);
double3 __ovld __cnfn copysign(double3, double3);
double4 __ovld __cnfn copysign(double4, double4);
double8 __ovld __cnfn copysign(double8, double8);
double16 __ovld __cnfn copysign(double16, double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn copysign(half, half);
half2 __ovld __cnfn copysign(half2, half2);
half3 __ovld __cnfn copysign(half3, half3);
half4 __ovld __cnfn copysign(half4, half4);
half8 __ovld __cnfn copysign(half8, half8);
half16 __ovld __cnfn copysign(half16, half16);
#endif //cl_khr_fp16

/**
 * Compute cosine.
 */
float __ovld __cnfn cos(float);
float2 __ovld __cnfn cos(float2);
float3 __ovld __cnfn cos(float3);
float4 __ovld __cnfn cos(float4);
float8 __ovld __cnfn cos(float8);
float16 __ovld __cnfn cos(float16);
#ifdef cl_khr_fp64
double __ovld __cnfn cos(double);
double2 __ovld __cnfn cos(double2);
double3 __ovld __cnfn cos(double3);
double4 __ovld __cnfn cos(double4);
double8 __ovld __cnfn cos(double8);
double16 __ovld __cnfn cos(double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn cos(half);
half2 __ovld __cnfn cos(half2);
half3 __ovld __cnfn cos(half3);
half4 __ovld __cnfn cos(half4);
half8 __ovld __cnfn cos(half8);
half16 __ovld __cnfn cos(half16);
#endif //cl_khr_fp16

/**
 * Compute hyperbolic cosine.
 */
float __ovld __cnfn cosh(float);
float2 __ovld __cnfn cosh(float2);
float3 __ovld __cnfn cosh(float3);
float4 __ovld __cnfn cosh(float4);
float8 __ovld __cnfn cosh(float8);
float16 __ovld __cnfn cosh(float16);
#ifdef cl_khr_fp64
double __ovld __cnfn cosh(double);
double2 __ovld __cnfn cosh(double2);
double3 __ovld __cnfn cosh(double3);
double4 __ovld __cnfn cosh(double4);
double8 __ovld __cnfn cosh(double8);
double16 __ovld __cnfn cosh(double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn cosh(half);
half2 __ovld __cnfn cosh(half2);
half3 __ovld __cnfn cosh(half3);
half4 __ovld __cnfn cosh(half4);
half8 __ovld __cnfn cosh(half8);
half16 __ovld __cnfn cosh(half16);
#endif //cl_khr_fp16

/**
 * Compute cos (PI * x).
 */
float __ovld __cnfn cospi(float);
float2 __ovld __cnfn cospi(float2);
float3 __ovld __cnfn cospi(float3);
float4 __ovld __cnfn cospi(float4);
float8 __ovld __cnfn cospi(float8);
float16 __ovld __cnfn cospi(float16);
#ifdef cl_khr_fp64
double __ovld __cnfn cospi(double);
double2 __ovld __cnfn cospi(double2);
double3 __ovld __cnfn cospi(double3);
double4 __ovld __cnfn cospi(double4);
double8 __ovld __cnfn cospi(double8);
double16 __ovld __cnfn cospi(double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn cospi(half);
half2 __ovld __cnfn cospi(half2);
half3 __ovld __cnfn cospi(half3);
half4 __ovld __cnfn cospi(half4);
half8 __ovld __cnfn cospi(half8);
half16 __ovld __cnfn cospi(half16);
#endif //cl_khr_fp16

/**
 * Complementary error function.
 */
float __ovld __cnfn erfc(float);
float2 __ovld __cnfn erfc(float2);
float3 __ovld __cnfn erfc(float3);
float4 __ovld __cnfn erfc(float4);
float8 __ovld __cnfn erfc(float8);
float16 __ovld __cnfn erfc(float16);
#ifdef cl_khr_fp64
double __ovld __cnfn erfc(double);
double2 __ovld __cnfn erfc(double2);
double3 __ovld __cnfn erfc(double3);
double4 __ovld __cnfn erfc(double4);
double8 __ovld __cnfn erfc(double8);
double16 __ovld __cnfn erfc(double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn erfc(half);
half2 __ovld __cnfn erfc(half2);
half3 __ovld __cnfn erfc(half3);
half4 __ovld __cnfn erfc(half4);
half8 __ovld __cnfn erfc(half8);
half16 __ovld __cnfn erfc(half16);
#endif //cl_khr_fp16

/**
 * Error function encountered in integrating the
 * normal distribution.
 */
float __ovld __cnfn erf(float);
float2 __ovld __cnfn erf(float2);
float3 __ovld __cnfn erf(float3);
float4 __ovld __cnfn erf(float4);
float8 __ovld __cnfn erf(float8);
float16 __ovld __cnfn erf(float16);
#ifdef cl_khr_fp64
double __ovld __cnfn erf(double);
double2 __ovld __cnfn erf(double2);
double3 __ovld __cnfn erf(double3);
double4 __ovld __cnfn erf(double4);
double8 __ovld __cnfn erf(double8);
double16 __ovld __cnfn erf(double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn erf(half);
half2 __ovld __cnfn erf(half2);
half3 __ovld __cnfn erf(half3);
half4 __ovld __cnfn erf(half4);
half8 __ovld __cnfn erf(half8);
half16 __ovld __cnfn erf(half16);
#endif //cl_khr_fp16

/**
 * Compute the base e exponential function of x.
 */
float __ovld __cnfn exp(float);
float2 __ovld __cnfn exp(float2);
float3 __ovld __cnfn exp(float3);
float4 __ovld __cnfn exp(float4);
float8 __ovld __cnfn exp(float8);
float16 __ovld __cnfn exp(float16);
#ifdef cl_khr_fp64
double __ovld __cnfn exp(double);
double2 __ovld __cnfn exp(double2);
double3 __ovld __cnfn exp(double3);
double4 __ovld __cnfn exp(double4);
double8 __ovld __cnfn exp(double8);
double16 __ovld __cnfn exp(double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn exp(half);
half2 __ovld __cnfn exp(half2);
half3 __ovld __cnfn exp(half3);
half4 __ovld __cnfn exp(half4);
half8 __ovld __cnfn exp(half8);
half16 __ovld __cnfn exp(half16);
#endif //cl_khr_fp16

/**
 * Exponential base 2 function.
 */
float __ovld __cnfn exp2(float);
float2 __ovld __cnfn exp2(float2);
float3 __ovld __cnfn exp2(float3);
float4 __ovld __cnfn exp2(float4);
float8 __ovld __cnfn exp2(float8);
float16 __ovld __cnfn exp2(float16);
#ifdef cl_khr_fp64
double __ovld __cnfn exp2(double);
double2 __ovld __cnfn exp2(double2);
double3 __ovld __cnfn exp2(double3);
double4 __ovld __cnfn exp2(double4);
double8 __ovld __cnfn exp2(double8);
double16 __ovld __cnfn exp2(double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn exp2(half);
half2 __ovld __cnfn exp2(half2);
half3 __ovld __cnfn exp2(half3);
half4 __ovld __cnfn exp2(half4);
half8 __ovld __cnfn exp2(half8);
half16 __ovld __cnfn exp2(half16);
#endif //cl_khr_fp16

/**
 * Exponential base 10 function.
 */
float __ovld __cnfn exp10(float);
float2 __ovld __cnfn exp10(float2);
float3 __ovld __cnfn exp10(float3);
float4 __ovld __cnfn exp10(float4);
float8 __ovld __cnfn exp10(float8);
float16 __ovld __cnfn exp10(float16);
#ifdef cl_khr_fp64
double __ovld __cnfn exp10(double);
double2 __ovld __cnfn exp10(double2);
double3 __ovld __cnfn exp10(double3);
double4 __ovld __cnfn exp10(double4);
double8 __ovld __cnfn exp10(double8);
double16 __ovld __cnfn exp10(double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn exp10(half);
half2 __ovld __cnfn exp10(half2);
half3 __ovld __cnfn exp10(half3);
half4 __ovld __cnfn exp10(half4);
half8 __ovld __cnfn exp10(half8);
half16 __ovld __cnfn exp10(half16);
#endif //cl_khr_fp16

/**
 * Compute e^x- 1.0.
 */
float __ovld __cnfn expm1(float);
float2 __ovld __cnfn expm1(float2);
float3 __ovld __cnfn expm1(float3);
float4 __ovld __cnfn expm1(float4);
float8 __ovld __cnfn expm1(float8);
float16 __ovld __cnfn expm1(float16);
#ifdef cl_khr_fp64
double __ovld __cnfn expm1(double);
double2 __ovld __cnfn expm1(double2);
double3 __ovld __cnfn expm1(double3);
double4 __ovld __cnfn expm1(double4);
double8 __ovld __cnfn expm1(double8);
double16 __ovld __cnfn expm1(double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn expm1(half);
half2 __ovld __cnfn expm1(half2);
half3 __ovld __cnfn expm1(half3);
half4 __ovld __cnfn expm1(half4);
half8 __ovld __cnfn expm1(half8);
half16 __ovld __cnfn expm1(half16);
#endif //cl_khr_fp16

/**
 * Compute absolute value of a floating-point number.
 */
float __ovld __cnfn fabs(float);
float2 __ovld __cnfn fabs(float2);
float3 __ovld __cnfn fabs(float3);
float4 __ovld __cnfn fabs(float4);
float8 __ovld __cnfn fabs(float8);
float16 __ovld __cnfn fabs(float16);
#ifdef cl_khr_fp64
double __ovld __cnfn fabs(double);
double2 __ovld __cnfn fabs(double2);
double3 __ovld __cnfn fabs(double3);
double4 __ovld __cnfn fabs(double4);
double8 __ovld __cnfn fabs(double8);
double16 __ovld __cnfn fabs(double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn fabs(half);
half2 __ovld __cnfn fabs(half2);
half3 __ovld __cnfn fabs(half3);
half4 __ovld __cnfn fabs(half4);
half8 __ovld __cnfn fabs(half8);
half16 __ovld __cnfn fabs(half16);
#endif //cl_khr_fp16

/**
 * x - y if x > y, +0 if x is less than or equal to y.
 */
float __ovld __cnfn fdim(float, float);
float2 __ovld __cnfn fdim(float2, float2);
float3 __ovld __cnfn fdim(float3, float3);
float4 __ovld __cnfn fdim(float4, float4);
float8 __ovld __cnfn fdim(float8, float8);
float16 __ovld __cnfn fdim(float16, float16);
#ifdef cl_khr_fp64
double __ovld __cnfn fdim(double, double);
double2 __ovld __cnfn fdim(double2, double2);
double3 __ovld __cnfn fdim(double3, double3);
double4 __ovld __cnfn fdim(double4, double4);
double8 __ovld __cnfn fdim(double8, double8);
double16 __ovld __cnfn fdim(double16, double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn fdim(half, half);
half2 __ovld __cnfn fdim(half2, half2);
half3 __ovld __cnfn fdim(half3, half3);
half4 __ovld __cnfn fdim(half4, half4);
half8 __ovld __cnfn fdim(half8, half8);
half16 __ovld __cnfn fdim(half16, half16);
#endif //cl_khr_fp16

/**
 * Round to integral value using the round to -ve
 * infinity rounding mode.
 */
float __ovld __cnfn floor(float);
float2 __ovld __cnfn floor(float2);
float3 __ovld __cnfn floor(float3);
float4 __ovld __cnfn floor(float4);
float8 __ovld __cnfn floor(float8);
float16 __ovld __cnfn floor(float16);
#ifdef cl_khr_fp64
double __ovld __cnfn floor(double);
double2 __ovld __cnfn floor(double2);
double3 __ovld __cnfn floor(double3);
double4 __ovld __cnfn floor(double4);
double8 __ovld __cnfn floor(double8);
double16 __ovld __cnfn floor(double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn floor(half);
half2 __ovld __cnfn floor(half2);
half3 __ovld __cnfn floor(half3);
half4 __ovld __cnfn floor(half4);
half8 __ovld __cnfn floor(half8);
half16 __ovld __cnfn floor(half16);
#endif //cl_khr_fp16

/**
 * Returns the correctly rounded floating-point
 * representation of the sum of c with the infinitely
 * precise product of a and b. Rounding of
 * intermediate products shall not occur. Edge case
 * behavior is per the IEEE 754-2008 standard.
 */
float __ovld __cnfn fma(float, float, float);
float2 __ovld __cnfn fma(float2, float2, float2);
float3 __ovld __cnfn fma(float3, float3, float3);
float4 __ovld __cnfn fma(float4, float4, float4);
float8 __ovld __cnfn fma(float8, float8, float8);
float16 __ovld __cnfn fma(float16, float16, float16);
#ifdef cl_khr_fp64
double __ovld __cnfn fma(double, double, double);
double2 __ovld __cnfn fma(double2, double2, double2);
double3 __ovld __cnfn fma(double3, double3, double3);
double4 __ovld __cnfn fma(double4, double4, double4);
double8 __ovld __cnfn fma(double8, double8, double8);
double16 __ovld __cnfn fma(double16, double16, double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn fma(half, half, half);
half2 __ovld __cnfn fma(half2, half2, half2);
half3 __ovld __cnfn fma(half3, half3, half3);
half4 __ovld __cnfn fma(half4, half4, half4);
half8 __ovld __cnfn fma(half8, half8, half8);
half16 __ovld __cnfn fma(half16, half16, half16);
#endif //cl_khr_fp16

/**
 * Returns y if x < y, otherwise it returns x. If one
 * argument is a NaN, fmax() returns the other
 * argument. If both arguments are NaNs, fmax()
 * returns a NaN.
 */
float __ovld __cnfn fmax(float, float);
float2 __ovld __cnfn fmax(float2, float2);
float3 __ovld __cnfn fmax(float3, float3);
float4 __ovld __cnfn fmax(float4, float4);
float8 __ovld __cnfn fmax(float8, float8);
float16 __ovld __cnfn fmax(float16, float16);
float2 __ovld __cnfn fmax(float2, float);
float3 __ovld __cnfn fmax(float3, float);
float4 __ovld __cnfn fmax(float4, float);
float8 __ovld __cnfn fmax(float8, float);
float16 __ovld __cnfn fmax(float16, float);
#ifdef cl_khr_fp64
double __ovld __cnfn fmax(double, double);
double2 __ovld __cnfn fmax(double2, double2);
double3 __ovld __cnfn fmax(double3, double3);
double4 __ovld __cnfn fmax(double4, double4);
double8 __ovld __cnfn fmax(double8, double8);
double16 __ovld __cnfn fmax(double16, double16);
double2 __ovld __cnfn fmax(double2, double);
double3 __ovld __cnfn fmax(double3, double);
double4 __ovld __cnfn fmax(double4, double);
double8 __ovld __cnfn fmax(double8, double);
double16 __ovld __cnfn fmax(double16, double);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn fmax(half, half);
half2 __ovld __cnfn fmax(half2, half2);
half3 __ovld __cnfn fmax(half3, half3);
half4 __ovld __cnfn fmax(half4, half4);
half8 __ovld __cnfn fmax(half8, half8);
half16 __ovld __cnfn fmax(half16, half16);
half2 __ovld __cnfn fmax(half2, half);
half3 __ovld __cnfn fmax(half3, half);
half4 __ovld __cnfn fmax(half4, half);
half8 __ovld __cnfn fmax(half8, half);
half16 __ovld __cnfn fmax(half16, half);
#endif //cl_khr_fp16

/**
 * Returns y if y < x, otherwise it returns x. If one
 * argument is a NaN, fmin() returns the other
 * argument. If both arguments are NaNs, fmin()
 * returns a NaN.
 */
float __ovld __cnfn fmin(float, float);
float2 __ovld __cnfn fmin(float2, float2);
float3 __ovld __cnfn fmin(float3, float3);
float4 __ovld __cnfn fmin(float4, float4);
float8 __ovld __cnfn fmin(float8, float8);
float16 __ovld __cnfn fmin(float16, float16);
float2 __ovld __cnfn fmin(float2, float);
float3 __ovld __cnfn fmin(float3, float);
float4 __ovld __cnfn fmin(float4, float);
float8 __ovld __cnfn fmin(float8, float);
float16 __ovld __cnfn fmin(float16, float);
#ifdef cl_khr_fp64
double __ovld __cnfn fmin(double, double);
double2 __ovld __cnfn fmin(double2, double2);
double3 __ovld __cnfn fmin(double3, double3);
double4 __ovld __cnfn fmin(double4, double4);
double8 __ovld __cnfn fmin(double8, double8);
double16 __ovld __cnfn fmin(double16, double16);
double2 __ovld __cnfn fmin(double2, double);
double3 __ovld __cnfn fmin(double3, double);
double4 __ovld __cnfn fmin(double4, double);
double8 __ovld __cnfn fmin(double8, double);
double16 __ovld __cnfn fmin(double16, double);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn fmin(half, half);
half2 __ovld __cnfn fmin(half2, half2);
half3 __ovld __cnfn fmin(half3, half3);
half4 __ovld __cnfn fmin(half4, half4);
half8 __ovld __cnfn fmin(half8, half8);
half16 __ovld __cnfn fmin(half16, half16);
half2 __ovld __cnfn fmin(half2, half);
half3 __ovld __cnfn fmin(half3, half);
half4 __ovld __cnfn fmin(half4, half);
half8 __ovld __cnfn fmin(half8, half);
half16 __ovld __cnfn fmin(half16, half);
#endif //cl_khr_fp16

/**
 * Modulus. Returns x - y * trunc (x/y).
 */
float __ovld __cnfn fmod(float, float);
float2 __ovld __cnfn fmod(float2, float2);
float3 __ovld __cnfn fmod(float3, float3);
float4 __ovld __cnfn fmod(float4, float4);
float8 __ovld __cnfn fmod(float8, float8);
float16 __ovld __cnfn fmod(float16, float16);
#ifdef cl_khr_fp64
double __ovld __cnfn fmod(double, double);
double2 __ovld __cnfn fmod(double2, double2);
double3 __ovld __cnfn fmod(double3, double3);
double4 __ovld __cnfn fmod(double4, double4);
double8 __ovld __cnfn fmod(double8, double8);
double16 __ovld __cnfn fmod(double16, double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn fmod(half, half);
half2 __ovld __cnfn fmod(half2, half2);
half3 __ovld __cnfn fmod(half3, half3);
half4 __ovld __cnfn fmod(half4, half4);
half8 __ovld __cnfn fmod(half8, half8);
half16 __ovld __cnfn fmod(half16, half16);
#endif //cl_khr_fp16

/**
 * Returns fmin(x - floor (x), 0x1.fffffep-1f ).
 * floor(x) is returned in iptr.
 */
#if defined(__opencl_c_generic_address_space)
float __ovld fract(float, float *);
float2 __ovld fract(float2, float2 *);
float3 __ovld fract(float3, float3 *);
float4 __ovld fract(float4, float4 *);
float8 __ovld fract(float8, float8 *);
float16 __ovld fract(float16, float16 *);
#ifdef cl_khr_fp64
double __ovld fract(double, double *);
double2 __ovld fract(double2, double2 *);
double3 __ovld fract(double3, double3 *);
double4 __ovld fract(double4, double4 *);
double8 __ovld fract(double8, double8 *);
double16 __ovld fract(double16, double16 *);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld fract(half, half *);
half2 __ovld fract(half2, half2 *);
half3 __ovld fract(half3, half3 *);
half4 __ovld fract(half4, half4 *);
half8 __ovld fract(half8, half8 *);
half16 __ovld fract(half16, half16 *);
#endif //cl_khr_fp16
#endif //defined(__opencl_c_generic_address_space)

#if defined(__opencl_c_named_address_space_builtins)
float __ovld fract(float, __global float *);
float2 __ovld fract(float2, __global float2 *);
float3 __ovld fract(float3, __global float3 *);
float4 __ovld fract(float4, __global float4 *);
float8 __ovld fract(float8, __global float8 *);
float16 __ovld fract(float16, __global float16 *);
float __ovld fract(float, __local float *);
float2 __ovld fract(float2, __local float2 *);
float3 __ovld fract(float3, __local float3 *);
float4 __ovld fract(float4, __local float4 *);
float8 __ovld fract(float8, __local float8 *);
float16 __ovld fract(float16, __local float16 *);
float __ovld fract(float, __private float *);
float2 __ovld fract(float2, __private float2 *);
float3 __ovld fract(float3, __private float3 *);
float4 __ovld fract(float4, __private float4 *);
float8 __ovld fract(float8, __private float8 *);
float16 __ovld fract(float16, __private float16 *);
#ifdef cl_khr_fp64
double __ovld fract(double, __global double *);
double2 __ovld fract(double2, __global double2 *);
double3 __ovld fract(double3, __global double3 *);
double4 __ovld fract(double4, __global double4 *);
double8 __ovld fract(double8, __global double8 *);
double16 __ovld fract(double16, __global double16 *);
double __ovld fract(double, __local double *);
double2 __ovld fract(double2, __local double2 *);
double3 __ovld fract(double3, __local double3 *);
double4 __ovld fract(double4, __local double4 *);
double8 __ovld fract(double8, __local double8 *);
double16 __ovld fract(double16, __local double16 *);
double __ovld fract(double, __private double *);
double2 __ovld fract(double2, __private double2 *);
double3 __ovld fract(double3, __private double3 *);
double4 __ovld fract(double4, __private double4 *);
double8 __ovld fract(double8, __private double8 *);
double16 __ovld fract(double16, __private double16 *);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld fract(half, __global half *);
half2 __ovld fract(half2, __global half2 *);
half3 __ovld fract(half3, __global half3 *);
half4 __ovld fract(half4, __global half4 *);
half8 __ovld fract(half8, __global half8 *);
half16 __ovld fract(half16, __global half16 *);
half __ovld fract(half, __local half *);
half2 __ovld fract(half2, __local half2 *);
half3 __ovld fract(half3, __local half3 *);
half4 __ovld fract(half4, __local half4 *);
half8 __ovld fract(half8, __local half8 *);
half16 __ovld fract(half16, __local half16 *);
half __ovld fract(half, __private half *);
half2 __ovld fract(half2, __private half2 *);
half3 __ovld fract(half3, __private half3 *);
half4 __ovld fract(half4, __private half4 *);
half8 __ovld fract(half8, __private half8 *);
half16 __ovld fract(half16, __private half16 *);
#endif //cl_khr_fp16
#endif //defined(__opencl_c_named_address_space_builtins)

/**
 * Extract mantissa and exponent from x. For each
 * component the mantissa returned is a float with
 * magnitude in the interval [1/2, 1) or 0. Each
 * component of x equals mantissa returned * 2^exp.
 */
#if defined(__opencl_c_generic_address_space)
float __ovld frexp(float, int *);
float2 __ovld frexp(float2, int2 *);
float3 __ovld frexp(float3, int3 *);
float4 __ovld frexp(float4, int4 *);
float8 __ovld frexp(float8, int8 *);
float16 __ovld frexp(float16, int16 *);
#ifdef cl_khr_fp64
double __ovld frexp(double, int *);
double2 __ovld frexp(double2, int2 *);
double3 __ovld frexp(double3, int3 *);
double4 __ovld frexp(double4, int4 *);
double8 __ovld frexp(double8, int8 *);
double16 __ovld frexp(double16, int16 *);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld frexp(half, int *);
half2 __ovld frexp(half2, int2 *);
half3 __ovld frexp(half3, int3 *);
half4 __ovld frexp(half4, int4 *);
half8 __ovld frexp(half8, int8 *);
half16 __ovld frexp(half16, int16 *);
#endif //cl_khr_fp16
#endif //defined(__opencl_c_generic_address_space)

#if defined(__opencl_c_named_address_space_builtins)
float __ovld frexp(float, __global int *);
float2 __ovld frexp(float2, __global int2 *);
float3 __ovld frexp(float3, __global int3 *);
float4 __ovld frexp(float4, __global int4 *);
float8 __ovld frexp(float8, __global int8 *);
float16 __ovld frexp(float16, __global int16 *);
float __ovld frexp(float, __local int *);
float2 __ovld frexp(float2, __local int2 *);
float3 __ovld frexp(float3, __local int3 *);
float4 __ovld frexp(float4, __local int4 *);
float8 __ovld frexp(float8, __local int8 *);
float16 __ovld frexp(float16, __local int16 *);
float __ovld frexp(float, __private int *);
float2 __ovld frexp(float2, __private int2 *);
float3 __ovld frexp(float3, __private int3 *);
float4 __ovld frexp(float4, __private int4 *);
float8 __ovld frexp(float8, __private int8 *);
float16 __ovld frexp(float16, __private int16 *);
#ifdef cl_khr_fp64
double __ovld frexp(double, __global int *);
double2 __ovld frexp(double2, __global int2 *);
double3 __ovld frexp(double3, __global int3 *);
double4 __ovld frexp(double4, __global int4 *);
double8 __ovld frexp(double8, __global int8 *);
double16 __ovld frexp(double16, __global int16 *);
double __ovld frexp(double, __local int *);
double2 __ovld frexp(double2, __local int2 *);
double3 __ovld frexp(double3, __local int3 *);
double4 __ovld frexp(double4, __local int4 *);
double8 __ovld frexp(double8, __local int8 *);
double16 __ovld frexp(double16, __local int16 *);
double __ovld frexp(double, __private int *);
double2 __ovld frexp(double2, __private int2 *);
double3 __ovld frexp(double3, __private int3 *);
double4 __ovld frexp(double4, __private int4 *);
double8 __ovld frexp(double8, __private int8 *);
double16 __ovld frexp(double16, __private int16 *);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld frexp(half, __global int *);
half2 __ovld frexp(half2, __global int2 *);
half3 __ovld frexp(half3, __global int3 *);
half4 __ovld frexp(half4, __global int4 *);
half8 __ovld frexp(half8, __global int8 *);
half16 __ovld frexp(half16, __global int16 *);
half __ovld frexp(half, __local int *);
half2 __ovld frexp(half2, __local int2 *);
half3 __ovld frexp(half3, __local int3 *);
half4 __ovld frexp(half4, __local int4 *);
half8 __ovld frexp(half8, __local int8 *);
half16 __ovld frexp(half16, __local int16 *);
half __ovld frexp(half, __private int *);
half2 __ovld frexp(half2, __private int2 *);
half3 __ovld frexp(half3, __private int3 *);
half4 __ovld frexp(half4, __private int4 *);
half8 __ovld frexp(half8, __private int8 *);
half16 __ovld frexp(half16, __private int16 *);
#endif //cl_khr_fp16
#endif //defined(__opencl_c_named_address_space_builtins)

/**
 * Compute the value of the square root of x^2 + y^2
 * without undue overflow or underflow.
 */
float __ovld __cnfn hypot(float, float);
float2 __ovld __cnfn hypot(float2, float2);
float3 __ovld __cnfn hypot(float3, float3);
float4 __ovld __cnfn hypot(float4, float4);
float8 __ovld __cnfn hypot(float8, float8);
float16 __ovld __cnfn hypot(float16, float16);
#ifdef cl_khr_fp64
double __ovld __cnfn hypot(double, double);
double2 __ovld __cnfn hypot(double2, double2);
double3 __ovld __cnfn hypot(double3, double3);
double4 __ovld __cnfn hypot(double4, double4);
double8 __ovld __cnfn hypot(double8, double8);
double16 __ovld __cnfn hypot(double16, double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn hypot(half, half);
half2 __ovld __cnfn hypot(half2, half2);
half3 __ovld __cnfn hypot(half3, half3);
half4 __ovld __cnfn hypot(half4, half4);
half8 __ovld __cnfn hypot(half8, half8);
half16 __ovld __cnfn hypot(half16, half16);
#endif //cl_khr_fp16

/**
 * Return the exponent as an integer value.
 */
int __ovld __cnfn ilogb(float);
int2 __ovld __cnfn ilogb(float2);
int3 __ovld __cnfn ilogb(float3);
int4 __ovld __cnfn ilogb(float4);
int8 __ovld __cnfn ilogb(float8);
int16 __ovld __cnfn ilogb(float16);
#ifdef cl_khr_fp64
int __ovld __cnfn ilogb(double);
int2 __ovld __cnfn ilogb(double2);
int3 __ovld __cnfn ilogb(double3);
int4 __ovld __cnfn ilogb(double4);
int8 __ovld __cnfn ilogb(double8);
int16 __ovld __cnfn ilogb(double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
int __ovld __cnfn ilogb(half);
int2 __ovld __cnfn ilogb(half2);
int3 __ovld __cnfn ilogb(half3);
int4 __ovld __cnfn ilogb(half4);
int8 __ovld __cnfn ilogb(half8);
int16 __ovld __cnfn ilogb(half16);
#endif //cl_khr_fp16

/**
 * Multiply x by 2 to the power n.
 */
float __ovld __cnfn ldexp(float, int);
float2 __ovld __cnfn ldexp(float2, int2);
float3 __ovld __cnfn ldexp(float3, int3);
float4 __ovld __cnfn ldexp(float4, int4);
float8 __ovld __cnfn ldexp(float8, int8);
float16 __ovld __cnfn ldexp(float16, int16);
float2 __ovld __cnfn ldexp(float2, int);
float3 __ovld __cnfn ldexp(float3, int);
float4 __ovld __cnfn ldexp(float4, int);
float8 __ovld __cnfn ldexp(float8, int);
float16 __ovld __cnfn ldexp(float16, int);
#ifdef cl_khr_fp64
double __ovld __cnfn ldexp(double, int);
double2 __ovld __cnfn ldexp(double2, int2);
double3 __ovld __cnfn ldexp(double3, int3);
double4 __ovld __cnfn ldexp(double4, int4);
double8 __ovld __cnfn ldexp(double8, int8);
double16 __ovld __cnfn ldexp(double16, int16);
double2 __ovld __cnfn ldexp(double2, int);
double3 __ovld __cnfn ldexp(double3, int);
double4 __ovld __cnfn ldexp(double4, int);
double8 __ovld __cnfn ldexp(double8, int);
double16 __ovld __cnfn ldexp(double16, int);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn ldexp(half, int);
half2 __ovld __cnfn ldexp(half2, int2);
half3 __ovld __cnfn ldexp(half3, int3);
half4 __ovld __cnfn ldexp(half4, int4);
half8 __ovld __cnfn ldexp(half8, int8);
half16 __ovld __cnfn ldexp(half16, int16);
half2 __ovld __cnfn ldexp(half2, int);
half3 __ovld __cnfn ldexp(half3, int);
half4 __ovld __cnfn ldexp(half4, int);
half8 __ovld __cnfn ldexp(half8, int);
half16 __ovld __cnfn ldexp(half16, int);
#endif //cl_khr_fp16

/**
 * Log gamma function. Returns the natural
 * logarithm of the absolute value of the gamma
 * function. The sign of the gamma function is
 * returned in the signp argument of lgamma_r.
 */
float __ovld __cnfn lgamma(float);
float2 __ovld __cnfn lgamma(float2);
float3 __ovld __cnfn lgamma(float3);
float4 __ovld __cnfn lgamma(float4);
float8 __ovld __cnfn lgamma(float8);
float16 __ovld __cnfn lgamma(float16);
#ifdef cl_khr_fp64
double __ovld __cnfn lgamma(double);
double2 __ovld __cnfn lgamma(double2);
double3 __ovld __cnfn lgamma(double3);
double4 __ovld __cnfn lgamma(double4);
double8 __ovld __cnfn lgamma(double8);
double16 __ovld __cnfn lgamma(double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn lgamma(half);
half2 __ovld __cnfn lgamma(half2);
half3 __ovld __cnfn lgamma(half3);
half4 __ovld __cnfn lgamma(half4);
half8 __ovld __cnfn lgamma(half8);
half16 __ovld __cnfn lgamma(half16);
#endif //cl_khr_fp16

#if defined(__opencl_c_generic_address_space)
float __ovld lgamma_r(float, int *);
float2 __ovld lgamma_r(float2, int2 *);
float3 __ovld lgamma_r(float3, int3 *);
float4 __ovld lgamma_r(float4, int4 *);
float8 __ovld lgamma_r(float8, int8 *);
float16 __ovld lgamma_r(float16, int16 *);
#ifdef cl_khr_fp64
double __ovld lgamma_r(double, int *);
double2 __ovld lgamma_r(double2, int2 *);
double3 __ovld lgamma_r(double3, int3 *);
double4 __ovld lgamma_r(double4, int4 *);
double8 __ovld lgamma_r(double8, int8 *);
double16 __ovld lgamma_r(double16, int16 *);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld lgamma_r(half, int *);
half2 __ovld lgamma_r(half2, int2 *);
half3 __ovld lgamma_r(half3, int3 *);
half4 __ovld lgamma_r(half4, int4 *);
half8 __ovld lgamma_r(half8, int8 *);
half16 __ovld lgamma_r(half16, int16 *);
#endif //cl_khr_fp16
#endif //defined(__opencl_c_generic_address_space)

#if defined(__opencl_c_named_address_space_builtins)
float __ovld lgamma_r(float, __global int *);
float2 __ovld lgamma_r(float2, __global int2 *);
float3 __ovld lgamma_r(float3, __global int3 *);
float4 __ovld lgamma_r(float4, __global int4 *);
float8 __ovld lgamma_r(float8, __global int8 *);
float16 __ovld lgamma_r(float16, __global int16 *);
float __ovld lgamma_r(float, __local int *);
float2 __ovld lgamma_r(float2, __local int2 *);
float3 __ovld lgamma_r(float3, __local int3 *);
float4 __ovld lgamma_r(float4, __local int4 *);
float8 __ovld lgamma_r(float8, __local int8 *);
float16 __ovld lgamma_r(float16, __local int16 *);
float __ovld lgamma_r(float, __private int *);
float2 __ovld lgamma_r(float2, __private int2 *);
float3 __ovld lgamma_r(float3, __private int3 *);
float4 __ovld lgamma_r(float4, __private int4 *);
float8 __ovld lgamma_r(float8, __private int8 *);
float16 __ovld lgamma_r(float16, __private int16 *);
#ifdef cl_khr_fp64
double __ovld lgamma_r(double, __global int *);
double2 __ovld lgamma_r(double2, __global int2 *);
double3 __ovld lgamma_r(double3, __global int3 *);
double4 __ovld lgamma_r(double4, __global int4 *);
double8 __ovld lgamma_r(double8, __global int8 *);
double16 __ovld lgamma_r(double16, __global int16 *);
double __ovld lgamma_r(double, __local int *);
double2 __ovld lgamma_r(double2, __local int2 *);
double3 __ovld lgamma_r(double3, __local int3 *);
double4 __ovld lgamma_r(double4, __local int4 *);
double8 __ovld lgamma_r(double8, __local int8 *);
double16 __ovld lgamma_r(double16, __local int16 *);
double __ovld lgamma_r(double, __private int *);
double2 __ovld lgamma_r(double2, __private int2 *);
double3 __ovld lgamma_r(double3, __private int3 *);
double4 __ovld lgamma_r(double4, __private int4 *);
double8 __ovld lgamma_r(double8, __private int8 *);
double16 __ovld lgamma_r(double16, __private int16 *);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld lgamma_r(half, __global int *);
half2 __ovld lgamma_r(half2, __global int2 *);
half3 __ovld lgamma_r(half3, __global int3 *);
half4 __ovld lgamma_r(half4, __global int4 *);
half8 __ovld lgamma_r(half8, __global int8 *);
half16 __ovld lgamma_r(half16, __global int16 *);
half __ovld lgamma_r(half, __local int *);
half2 __ovld lgamma_r(half2, __local int2 *);
half3 __ovld lgamma_r(half3, __local int3 *);
half4 __ovld lgamma_r(half4, __local int4 *);
half8 __ovld lgamma_r(half8, __local int8 *);
half16 __ovld lgamma_r(half16, __local int16 *);
half __ovld lgamma_r(half, __private int *);
half2 __ovld lgamma_r(half2, __private int2 *);
half3 __ovld lgamma_r(half3, __private int3 *);
half4 __ovld lgamma_r(half4, __private int4 *);
half8 __ovld lgamma_r(half8, __private int8 *);
half16 __ovld lgamma_r(half16, __private int16 *);
#endif //cl_khr_fp16
#endif //defined(__opencl_c_named_address_space_builtins)

/**
 * Compute natural logarithm.
 */
float __ovld __cnfn log(float);
float2 __ovld __cnfn log(float2);
float3 __ovld __cnfn log(float3);
float4 __ovld __cnfn log(float4);
float8 __ovld __cnfn log(float8);
float16 __ovld __cnfn log(float16);
#ifdef cl_khr_fp64
double __ovld __cnfn log(double);
double2 __ovld __cnfn log(double2);
double3 __ovld __cnfn log(double3);
double4 __ovld __cnfn log(double4);
double8 __ovld __cnfn log(double8);
double16 __ovld __cnfn log(double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn log(half);
half2 __ovld __cnfn log(half2);
half3 __ovld __cnfn log(half3);
half4 __ovld __cnfn log(half4);
half8 __ovld __cnfn log(half8);
half16 __ovld __cnfn log(half16);
#endif //cl_khr_fp16

/**
 * Compute a base 2 logarithm.
 */
float __ovld __cnfn log2(float);
float2 __ovld __cnfn log2(float2);
float3 __ovld __cnfn log2(float3);
float4 __ovld __cnfn log2(float4);
float8 __ovld __cnfn log2(float8);
float16 __ovld __cnfn log2(float16);
#ifdef cl_khr_fp64
double __ovld __cnfn log2(double);
double2 __ovld __cnfn log2(double2);
double3 __ovld __cnfn log2(double3);
double4 __ovld __cnfn log2(double4);
double8 __ovld __cnfn log2(double8);
double16 __ovld __cnfn log2(double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn log2(half);
half2 __ovld __cnfn log2(half2);
half3 __ovld __cnfn log2(half3);
half4 __ovld __cnfn log2(half4);
half8 __ovld __cnfn log2(half8);
half16 __ovld __cnfn log2(half16);
#endif //cl_khr_fp16

/**
 * Compute a base 10 logarithm.
 */
float __ovld __cnfn log10(float);
float2 __ovld __cnfn log10(float2);
float3 __ovld __cnfn log10(float3);
float4 __ovld __cnfn log10(float4);
float8 __ovld __cnfn log10(float8);
float16 __ovld __cnfn log10(float16);
#ifdef cl_khr_fp64
double __ovld __cnfn log10(double);
double2 __ovld __cnfn log10(double2);
double3 __ovld __cnfn log10(double3);
double4 __ovld __cnfn log10(double4);
double8 __ovld __cnfn log10(double8);
double16 __ovld __cnfn log10(double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn log10(half);
half2 __ovld __cnfn log10(half2);
half3 __ovld __cnfn log10(half3);
half4 __ovld __cnfn log10(half4);
half8 __ovld __cnfn log10(half8);
half16 __ovld __cnfn log10(half16);
#endif //cl_khr_fp16

/**
 * Compute a base e logarithm of (1.0 + x).
 */
float __ovld __cnfn log1p(float);
float2 __ovld __cnfn log1p(float2);
float3 __ovld __cnfn log1p(float3);
float4 __ovld __cnfn log1p(float4);
float8 __ovld __cnfn log1p(float8);
float16 __ovld __cnfn log1p(float16);
#ifdef cl_khr_fp64
double __ovld __cnfn log1p(double);
double2 __ovld __cnfn log1p(double2);
double3 __ovld __cnfn log1p(double3);
double4 __ovld __cnfn log1p(double4);
double8 __ovld __cnfn log1p(double8);
double16 __ovld __cnfn log1p(double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn log1p(half);
half2 __ovld __cnfn log1p(half2);
half3 __ovld __cnfn log1p(half3);
half4 __ovld __cnfn log1p(half4);
half8 __ovld __cnfn log1p(half8);
half16 __ovld __cnfn log1p(half16);
#endif //cl_khr_fp16

/**
 * Compute the exponent of x, which is the integral
 * part of logr | x |.
 */
float __ovld __cnfn logb(float);
float2 __ovld __cnfn logb(float2);
float3 __ovld __cnfn logb(float3);
float4 __ovld __cnfn logb(float4);
float8 __ovld __cnfn logb(float8);
float16 __ovld __cnfn logb(float16);
#ifdef cl_khr_fp64
double __ovld __cnfn logb(double);
double2 __ovld __cnfn logb(double2);
double3 __ovld __cnfn logb(double3);
double4 __ovld __cnfn logb(double4);
double8 __ovld __cnfn logb(double8);
double16 __ovld __cnfn logb(double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn logb(half);
half2 __ovld __cnfn logb(half2);
half3 __ovld __cnfn logb(half3);
half4 __ovld __cnfn logb(half4);
half8 __ovld __cnfn logb(half8);
half16 __ovld __cnfn logb(half16);
#endif //cl_khr_fp16

/**
 * mad approximates a * b + c. Whether or how the
 * product of a * b is rounded and how supernormal or
 * subnormal intermediate products are handled is not
 * defined. mad is intended to be used where speed is
 * preferred over accuracy.
 */
float __ovld __cnfn mad(float, float, float);
float2 __ovld __cnfn mad(float2, float2, float2);
float3 __ovld __cnfn mad(float3, float3, float3);
float4 __ovld __cnfn mad(float4, float4, float4);
float8 __ovld __cnfn mad(float8, float8, float8);
float16 __ovld __cnfn mad(float16, float16, float16);
#ifdef cl_khr_fp64
double __ovld __cnfn mad(double, double, double);
double2 __ovld __cnfn mad(double2, double2, double2);
double3 __ovld __cnfn mad(double3, double3, double3);
double4 __ovld __cnfn mad(double4, double4, double4);
double8 __ovld __cnfn mad(double8, double8, double8);
double16 __ovld __cnfn mad(double16, double16, double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn mad(half, half, half);
half2 __ovld __cnfn mad(half2, half2, half2);
half3 __ovld __cnfn mad(half3, half3, half3);
half4 __ovld __cnfn mad(half4, half4, half4);
half8 __ovld __cnfn mad(half8, half8, half8);
half16 __ovld __cnfn mad(half16, half16, half16);
#endif //cl_khr_fp16

/**
 * Returns x if | x | > | y |, y if | y | > | x |, otherwise
 * fmax(x, y).
 */
float __ovld __cnfn maxmag(float, float);
float2 __ovld __cnfn maxmag(float2, float2);
float3 __ovld __cnfn maxmag(float3, float3);
float4 __ovld __cnfn maxmag(float4, float4);
float8 __ovld __cnfn maxmag(float8, float8);
float16 __ovld __cnfn maxmag(float16, float16);
#ifdef cl_khr_fp64
double __ovld __cnfn maxmag(double, double);
double2 __ovld __cnfn maxmag(double2, double2);
double3 __ovld __cnfn maxmag(double3, double3);
double4 __ovld __cnfn maxmag(double4, double4);
double8 __ovld __cnfn maxmag(double8, double8);
double16 __ovld __cnfn maxmag(double16, double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn maxmag(half, half);
half2 __ovld __cnfn maxmag(half2, half2);
half3 __ovld __cnfn maxmag(half3, half3);
half4 __ovld __cnfn maxmag(half4, half4);
half8 __ovld __cnfn maxmag(half8, half8);
half16 __ovld __cnfn maxmag(half16, half16);
#endif //cl_khr_fp16

/**
 * Returns x if | x | < | y |, y if | y | < | x |, otherwise
 * fmin(x, y).
 */
float __ovld __cnfn minmag(float, float);
float2 __ovld __cnfn minmag(float2, float2);
float3 __ovld __cnfn minmag(float3, float3);
float4 __ovld __cnfn minmag(float4, float4);
float8 __ovld __cnfn minmag(float8, float8);
float16 __ovld __cnfn minmag(float16, float16);
#ifdef cl_khr_fp64
double __ovld __cnfn minmag(double, double);
double2 __ovld __cnfn minmag(double2, double2);
double3 __ovld __cnfn minmag(double3, double3);
double4 __ovld __cnfn minmag(double4, double4);
double8 __ovld __cnfn minmag(double8, double8);
double16 __ovld __cnfn minmag(double16, double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn minmag(half, half);
half2 __ovld __cnfn minmag(half2, half2);
half3 __ovld __cnfn minmag(half3, half3);
half4 __ovld __cnfn minmag(half4, half4);
half8 __ovld __cnfn minmag(half8, half8);
half16 __ovld __cnfn minmag(half16, half16);
#endif //cl_khr_fp16

/**
 * Decompose a floating-point number. The modf
 * function breaks the argument x into integral and
 * fractional parts, each of which has the same sign as
 * the argument. It stores the integral part in the object
 * pointed to by iptr.
 */
#if defined(__opencl_c_generic_address_space)
float __ovld modf(float, float *);
float2 __ovld modf(float2, float2 *);
float3 __ovld modf(float3, float3 *);
float4 __ovld modf(float4, float4 *);
float8 __ovld modf(float8, float8 *);
float16 __ovld modf(float16, float16 *);
#ifdef cl_khr_fp64
double __ovld modf(double, double *);
double2 __ovld modf(double2, double2 *);
double3 __ovld modf(double3, double3 *);
double4 __ovld modf(double4, double4 *);
double8 __ovld modf(double8, double8 *);
double16 __ovld modf(double16, double16 *);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld modf(half, half *);
half2 __ovld modf(half2, half2 *);
half3 __ovld modf(half3, half3 *);
half4 __ovld modf(half4, half4 *);
half8 __ovld modf(half8, half8 *);
half16 __ovld modf(half16, half16 *);
#endif //cl_khr_fp16
#endif //defined(__opencl_c_generic_address_space)

#if defined(__opencl_c_named_address_space_builtins)
float __ovld modf(float, __global float *);
float2 __ovld modf(float2, __global float2 *);
float3 __ovld modf(float3, __global float3 *);
float4 __ovld modf(float4, __global float4 *);
float8 __ovld modf(float8, __global float8 *);
float16 __ovld modf(float16, __global float16 *);
float __ovld modf(float, __local float *);
float2 __ovld modf(float2, __local float2 *);
float3 __ovld modf(float3, __local float3 *);
float4 __ovld modf(float4, __local float4 *);
float8 __ovld modf(float8, __local float8 *);
float16 __ovld modf(float16, __local float16 *);
float __ovld modf(float, __private float *);
float2 __ovld modf(float2, __private float2 *);
float3 __ovld modf(float3, __private float3 *);
float4 __ovld modf(float4, __private float4 *);
float8 __ovld modf(float8, __private float8 *);
float16 __ovld modf(float16, __private float16 *);
#ifdef cl_khr_fp64
double __ovld modf(double, __global double *);
double2 __ovld modf(double2, __global double2 *);
double3 __ovld modf(double3, __global double3 *);
double4 __ovld modf(double4, __global double4 *);
double8 __ovld modf(double8, __global double8 *);
double16 __ovld modf(double16, __global double16 *);
double __ovld modf(double, __local double *);
double2 __ovld modf(double2, __local double2 *);
double3 __ovld modf(double3, __local double3 *);
double4 __ovld modf(double4, __local double4 *);
double8 __ovld modf(double8, __local double8 *);
double16 __ovld modf(double16, __local double16 *);
double __ovld modf(double, __private double *);
double2 __ovld modf(double2, __private double2 *);
double3 __ovld modf(double3, __private double3 *);
double4 __ovld modf(double4, __private double4 *);
double8 __ovld modf(double8, __private double8 *);
double16 __ovld modf(double16, __private double16 *);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld modf(half, __global half *);
half2 __ovld modf(half2, __global half2 *);
half3 __ovld modf(half3, __global half3 *);
half4 __ovld modf(half4, __global half4 *);
half8 __ovld modf(half8, __global half8 *);
half16 __ovld modf(half16, __global half16 *);
half __ovld modf(half, __local half *);
half2 __ovld modf(half2, __local half2 *);
half3 __ovld modf(half3, __local half3 *);
half4 __ovld modf(half4, __local half4 *);
half8 __ovld modf(half8, __local half8 *);
half16 __ovld modf(half16, __local half16 *);
half __ovld modf(half, __private half *);
half2 __ovld modf(half2, __private half2 *);
half3 __ovld modf(half3, __private half3 *);
half4 __ovld modf(half4, __private half4 *);
half8 __ovld modf(half8, __private half8 *);
half16 __ovld modf(half16, __private half16 *);
#endif //cl_khr_fp16
#endif //defined(__opencl_c_named_address_space_builtins)

/**
 * Returns a quiet NaN. The nancode may be placed
 * in the significand of the resulting NaN.
 */
float __ovld __cnfn nan(uint);
float2 __ovld __cnfn nan(uint2);
float3 __ovld __cnfn nan(uint3);
float4 __ovld __cnfn nan(uint4);
float8 __ovld __cnfn nan(uint8);
float16 __ovld __cnfn nan(uint16);
#ifdef cl_khr_fp64
double __ovld __cnfn nan(ulong);
double2 __ovld __cnfn nan(ulong2);
double3 __ovld __cnfn nan(ulong3);
double4 __ovld __cnfn nan(ulong4);
double8 __ovld __cnfn nan(ulong8);
double16 __ovld __cnfn nan(ulong16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn nan(ushort);
half2 __ovld __cnfn nan(ushort2);
half3 __ovld __cnfn nan(ushort3);
half4 __ovld __cnfn nan(ushort4);
half8 __ovld __cnfn nan(ushort8);
half16 __ovld __cnfn nan(ushort16);
#endif //cl_khr_fp16

/**
 * Computes the next representable single-precision
 * floating-point value following x in the direction of
 * y. Thus, if y is less than x, nextafter() returns the
 * largest representable floating-point number less
 * than x.
 */
float __ovld __cnfn nextafter(float, float);
float2 __ovld __cnfn nextafter(float2, float2);
float3 __ovld __cnfn nextafter(float3, float3);
float4 __ovld __cnfn nextafter(float4, float4);
float8 __ovld __cnfn nextafter(float8, float8);
float16 __ovld __cnfn nextafter(float16, float16);
#ifdef cl_khr_fp64
double __ovld __cnfn nextafter(double, double);
double2 __ovld __cnfn nextafter(double2, double2);
double3 __ovld __cnfn nextafter(double3, double3);
double4 __ovld __cnfn nextafter(double4, double4);
double8 __ovld __cnfn nextafter(double8, double8);
double16 __ovld __cnfn nextafter(double16, double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn nextafter(half, half);
half2 __ovld __cnfn nextafter(half2, half2);
half3 __ovld __cnfn nextafter(half3, half3);
half4 __ovld __cnfn nextafter(half4, half4);
half8 __ovld __cnfn nextafter(half8, half8);
half16 __ovld __cnfn nextafter(half16, half16);
#endif //cl_khr_fp16

/**
 * Compute x to the power y.
 */
float __ovld __cnfn pow(float, float);
float2 __ovld __cnfn pow(float2, float2);
float3 __ovld __cnfn pow(float3, float3);
float4 __ovld __cnfn pow(float4, float4);
float8 __ovld __cnfn pow(float8, float8);
float16 __ovld __cnfn pow(float16, float16);
#ifdef cl_khr_fp64
double __ovld __cnfn pow(double, double);
double2 __ovld __cnfn pow(double2, double2);
double3 __ovld __cnfn pow(double3, double3);
double4 __ovld __cnfn pow(double4, double4);
double8 __ovld __cnfn pow(double8, double8);
double16 __ovld __cnfn pow(double16, double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn pow(half, half);
half2 __ovld __cnfn pow(half2, half2);
half3 __ovld __cnfn pow(half3, half3);
half4 __ovld __cnfn pow(half4, half4);
half8 __ovld __cnfn pow(half8, half8);
half16 __ovld __cnfn pow(half16, half16);
#endif //cl_khr_fp16

/**
 * Compute x to the power y, where y is an integer.
 */
float __ovld __cnfn pown(float, int);
float2 __ovld __cnfn pown(float2, int2);
float3 __ovld __cnfn pown(float3, int3);
float4 __ovld __cnfn pown(float4, int4);
float8 __ovld __cnfn pown(float8, int8);
float16 __ovld __cnfn pown(float16, int16);
#ifdef cl_khr_fp64
double __ovld __cnfn pown(double, int);
double2 __ovld __cnfn pown(double2, int2);
double3 __ovld __cnfn pown(double3, int3);
double4 __ovld __cnfn pown(double4, int4);
double8 __ovld __cnfn pown(double8, int8);
double16 __ovld __cnfn pown(double16, int16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn pown(half, int);
half2 __ovld __cnfn pown(half2, int2);
half3 __ovld __cnfn pown(half3, int3);
half4 __ovld __cnfn pown(half4, int4);
half8 __ovld __cnfn pown(half8, int8);
half16 __ovld __cnfn pown(half16, int16);
#endif //cl_khr_fp16

/**
 * Compute x to the power y, where x is >= 0.
 */
float __ovld __cnfn powr(float, float);
float2 __ovld __cnfn powr(float2, float2);
float3 __ovld __cnfn powr(float3, float3);
float4 __ovld __cnfn powr(float4, float4);
float8 __ovld __cnfn powr(float8, float8);
float16 __ovld __cnfn powr(float16, float16);
#ifdef cl_khr_fp64
double __ovld __cnfn powr(double, double);
double2 __ovld __cnfn powr(double2, double2);
double3 __ovld __cnfn powr(double3, double3);
double4 __ovld __cnfn powr(double4, double4);
double8 __ovld __cnfn powr(double8, double8);
double16 __ovld __cnfn powr(double16, double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn powr(half, half);
half2 __ovld __cnfn powr(half2, half2);
half3 __ovld __cnfn powr(half3, half3);
half4 __ovld __cnfn powr(half4, half4);
half8 __ovld __cnfn powr(half8, half8);
half16 __ovld __cnfn powr(half16, half16);
#endif //cl_khr_fp16

/**
 * Compute the value r such that r = x - n*y, where n
 * is the integer nearest the exact value of x/y. If there
 * are two integers closest to x/y, n shall be the even
 * one. If r is zero, it is given the same sign as x.
 */
float __ovld __cnfn remainder(float, float);
float2 __ovld __cnfn remainder(float2, float2);
float3 __ovld __cnfn remainder(float3, float3);
float4 __ovld __cnfn remainder(float4, float4);
float8 __ovld __cnfn remainder(float8, float8);
float16 __ovld __cnfn remainder(float16, float16);
#ifdef cl_khr_fp64
double __ovld __cnfn remainder(double, double);
double2 __ovld __cnfn remainder(double2, double2);
double3 __ovld __cnfn remainder(double3, double3);
double4 __ovld __cnfn remainder(double4, double4);
double8 __ovld __cnfn remainder(double8, double8);
double16 __ovld __cnfn remainder(double16, double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn remainder(half, half);
half2 __ovld __cnfn remainder(half2, half2);
half3 __ovld __cnfn remainder(half3, half3);
half4 __ovld __cnfn remainder(half4, half4);
half8 __ovld __cnfn remainder(half8, half8);
half16 __ovld __cnfn remainder(half16, half16);
#endif //cl_khr_fp16

/**
 * The remquo function computes the value r such
 * that r = x - n*y, where n is the integer nearest the
 * exact value of x/y. If there are two integers closest
 * to x/y, n shall be the even one. If r is zero, it is
 * given the same sign as x. This is the same value
 * that is returned by the remainder function.
 * remquo also calculates the lower seven bits of the
 * integral quotient x/y, and gives that value the same
 * sign as x/y. It stores this signed value in the object
 * pointed to by quo.
 */
#if defined(__opencl_c_generic_address_space)
float __ovld remquo(float, float, int *);
float2 __ovld remquo(float2, float2, int2 *);
float3 __ovld remquo(float3, float3, int3 *);
float4 __ovld remquo(float4, float4, int4 *);
float8 __ovld remquo(float8, float8, int8 *);
float16 __ovld remquo(float16, float16, int16 *);
#ifdef cl_khr_fp64
double __ovld remquo(double, double, int *);
double2 __ovld remquo(double2, double2, int2 *);
double3 __ovld remquo(double3, double3, int3 *);
double4 __ovld remquo(double4, double4, int4 *);
double8 __ovld remquo(double8, double8, int8 *);
double16 __ovld remquo(double16, double16, int16 *);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld remquo(half, half, int *);
half2 __ovld remquo(half2, half2, int2 *);
half3 __ovld remquo(half3, half3, int3 *);
half4 __ovld remquo(half4, half4, int4 *);
half8 __ovld remquo(half8, half8, int8 *);
half16 __ovld remquo(half16, half16, int16 *);
#endif //cl_khr_fp16
#endif //defined(__opencl_c_generic_address_space)

#if defined(__opencl_c_named_address_space_builtins)
float __ovld remquo(float, float, __global int *);
float2 __ovld remquo(float2, float2, __global int2 *);
float3 __ovld remquo(float3, float3, __global int3 *);
float4 __ovld remquo(float4, float4, __global int4 *);
float8 __ovld remquo(float8, float8, __global int8 *);
float16 __ovld remquo(float16, float16, __global int16 *);
float __ovld remquo(float, float, __local int *);
float2 __ovld remquo(float2, float2, __local int2 *);
float3 __ovld remquo(float3, float3, __local int3 *);
float4 __ovld remquo(float4, float4, __local int4 *);
float8 __ovld remquo(float8, float8, __local int8 *);
float16 __ovld remquo(float16, float16, __local int16 *);
float __ovld remquo(float, float, __private int *);
float2 __ovld remquo(float2, float2, __private int2 *);
float3 __ovld remquo(float3, float3, __private int3 *);
float4 __ovld remquo(float4, float4, __private int4 *);
float8 __ovld remquo(float8, float8, __private int8 *);
float16 __ovld remquo(float16, float16, __private int16 *);
#ifdef cl_khr_fp64
double __ovld remquo(double, double, __global int *);
double2 __ovld remquo(double2, double2, __global int2 *);
double3 __ovld remquo(double3, double3, __global int3 *);
double4 __ovld remquo(double4, double4, __global int4 *);
double8 __ovld remquo(double8, double8, __global int8 *);
double16 __ovld remquo(double16, double16, __global int16 *);
double __ovld remquo(double, double, __local int *);
double2 __ovld remquo(double2, double2, __local int2 *);
double3 __ovld remquo(double3, double3, __local int3 *);
double4 __ovld remquo(double4, double4, __local int4 *);
double8 __ovld remquo(double8, double8, __local int8 *);
double16 __ovld remquo(double16, double16, __local int16 *);
double __ovld remquo(double, double, __private int *);
double2 __ovld remquo(double2, double2, __private int2 *);
double3 __ovld remquo(double3, double3, __private int3 *);
double4 __ovld remquo(double4, double4, __private int4 *);
double8 __ovld remquo(double8, double8, __private int8 *);
double16 __ovld remquo(double16, double16, __private int16 *);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld remquo(half, half, __global int *);
half2 __ovld remquo(half2, half2, __global int2 *);
half3 __ovld remquo(half3, half3, __global int3 *);
half4 __ovld remquo(half4, half4, __global int4 *);
half8 __ovld remquo(half8, half8, __global int8 *);
half16 __ovld remquo(half16, half16, __global int16 *);
half __ovld remquo(half, half, __local int *);
half2 __ovld remquo(half2, half2, __local int2 *);
half3 __ovld remquo(half3, half3, __local int3 *);
half4 __ovld remquo(half4, half4, __local int4 *);
half8 __ovld remquo(half8, half8, __local int8 *);
half16 __ovld remquo(half16, half16, __local int16 *);
half __ovld remquo(half, half, __private int *);
half2 __ovld remquo(half2, half2, __private int2 *);
half3 __ovld remquo(half3, half3, __private int3 *);
half4 __ovld remquo(half4, half4, __private int4 *);
half8 __ovld remquo(half8, half8, __private int8 *);
half16 __ovld remquo(half16, half16, __private int16 *);
#endif //cl_khr_fp16
#endif //defined(__opencl_c_named_address_space_builtins)
/**
 * Round to integral value (using round to nearest
 * even rounding mode) in floating-point format.
 * Refer to section 7.1 for description of rounding
 * modes.
 */
float __ovld __cnfn rint(float);
float2 __ovld __cnfn rint(float2);
float3 __ovld __cnfn rint(float3);
float4 __ovld __cnfn rint(float4);
float8 __ovld __cnfn rint(float8);
float16 __ovld __cnfn rint(float16);
#ifdef cl_khr_fp64
double __ovld __cnfn rint(double);
double2 __ovld __cnfn rint(double2);
double3 __ovld __cnfn rint(double3);
double4 __ovld __cnfn rint(double4);
double8 __ovld __cnfn rint(double8);
double16 __ovld __cnfn rint(double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn rint(half);
half2 __ovld __cnfn rint(half2);
half3 __ovld __cnfn rint(half3);
half4 __ovld __cnfn rint(half4);
half8 __ovld __cnfn rint(half8);
half16 __ovld __cnfn rint(half16);
#endif //cl_khr_fp16

/**
 * Compute x to the power 1/y.
 */
float __ovld __cnfn rootn(float, int);
float2 __ovld __cnfn rootn(float2, int2);
float3 __ovld __cnfn rootn(float3, int3);
float4 __ovld __cnfn rootn(float4, int4);
float8 __ovld __cnfn rootn(float8, int8);
float16 __ovld __cnfn rootn(float16, int16);
#ifdef cl_khr_fp64
double __ovld __cnfn rootn(double, int);
double2 __ovld __cnfn rootn(double2, int2);
double3 __ovld __cnfn rootn(double3, int3);
double4 __ovld __cnfn rootn(double4, int4);
double8 __ovld __cnfn rootn(double8, int8);
double16 __ovld __cnfn rootn(double16, int16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn rootn(half, int);
half2 __ovld __cnfn rootn(half2, int2);
half3 __ovld __cnfn rootn(half3, int3);
half4 __ovld __cnfn rootn(half4, int4);
half8 __ovld __cnfn rootn(half8, int8);
half16 __ovld __cnfn rootn(half16, int16);
#endif //cl_khr_fp16

/**
 * Return the integral value nearest to x rounding
 * halfway cases away from zero, regardless of the
 * current rounding direction.
 */
float __ovld __cnfn round(float);
float2 __ovld __cnfn round(float2);
float3 __ovld __cnfn round(float3);
float4 __ovld __cnfn round(float4);
float8 __ovld __cnfn round(float8);
float16 __ovld __cnfn round(float16);
#ifdef cl_khr_fp64
double __ovld __cnfn round(double);
double2 __ovld __cnfn round(double2);
double3 __ovld __cnfn round(double3);
double4 __ovld __cnfn round(double4);
double8 __ovld __cnfn round(double8);
double16 __ovld __cnfn round(double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn round(half);
half2 __ovld __cnfn round(half2);
half3 __ovld __cnfn round(half3);
half4 __ovld __cnfn round(half4);
half8 __ovld __cnfn round(half8);
half16 __ovld __cnfn round(half16);
#endif //cl_khr_fp16

/**
 * Compute inverse square root.
 */
float __ovld __cnfn rsqrt(float);
float2 __ovld __cnfn rsqrt(float2);
float3 __ovld __cnfn rsqrt(float3);
float4 __ovld __cnfn rsqrt(float4);
float8 __ovld __cnfn rsqrt(float8);
float16 __ovld __cnfn rsqrt(float16);
#ifdef cl_khr_fp64
double __ovld __cnfn rsqrt(double);
double2 __ovld __cnfn rsqrt(double2);
double3 __ovld __cnfn rsqrt(double3);
double4 __ovld __cnfn rsqrt(double4);
double8 __ovld __cnfn rsqrt(double8);
double16 __ovld __cnfn rsqrt(double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn rsqrt(half);
half2 __ovld __cnfn rsqrt(half2);
half3 __ovld __cnfn rsqrt(half3);
half4 __ovld __cnfn rsqrt(half4);
half8 __ovld __cnfn rsqrt(half8);
half16 __ovld __cnfn rsqrt(half16);
#endif //cl_khr_fp16

/**
 * Compute sine.
 */
float __ovld __cnfn sin(float);
float2 __ovld __cnfn sin(float2);
float3 __ovld __cnfn sin(float3);
float4 __ovld __cnfn sin(float4);
float8 __ovld __cnfn sin(float8);
float16 __ovld __cnfn sin(float16);
#ifdef cl_khr_fp64
double __ovld __cnfn sin(double);
double2 __ovld __cnfn sin(double2);
double3 __ovld __cnfn sin(double3);
double4 __ovld __cnfn sin(double4);
double8 __ovld __cnfn sin(double8);
double16 __ovld __cnfn sin(double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn sin(half);
half2 __ovld __cnfn sin(half2);
half3 __ovld __cnfn sin(half3);
half4 __ovld __cnfn sin(half4);
half8 __ovld __cnfn sin(half8);
half16 __ovld __cnfn sin(half16);
#endif //cl_khr_fp16

/**
 * Compute sine and cosine of x. The computed sine
 * is the return value and computed cosine is returned
 * in cosval.
 */
#if defined(__opencl_c_generic_address_space)
float __ovld sincos(float, float *);
float2 __ovld sincos(float2, float2 *);
float3 __ovld sincos(float3, float3 *);
float4 __ovld sincos(float4, float4 *);
float8 __ovld sincos(float8, float8 *);
float16 __ovld sincos(float16, float16 *);
#ifdef cl_khr_fp64
double __ovld sincos(double, double *);
double2 __ovld sincos(double2, double2 *);
double3 __ovld sincos(double3, double3 *);
double4 __ovld sincos(double4, double4 *);
double8 __ovld sincos(double8, double8 *);
double16 __ovld sincos(double16, double16 *);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld sincos(half, half *);
half2 __ovld sincos(half2, half2 *);
half3 __ovld sincos(half3, half3 *);
half4 __ovld sincos(half4, half4 *);
half8 __ovld sincos(half8, half8 *);
half16 __ovld sincos(half16, half16 *);
#endif //cl_khr_fp16
#endif //defined(__opencl_c_generic_address_space)

#if defined(__opencl_c_named_address_space_builtins)
float __ovld sincos(float, __global float *);
float2 __ovld sincos(float2, __global float2 *);
float3 __ovld sincos(float3, __global float3 *);
float4 __ovld sincos(float4, __global float4 *);
float8 __ovld sincos(float8, __global float8 *);
float16 __ovld sincos(float16, __global float16 *);
float __ovld sincos(float, __local float *);
float2 __ovld sincos(float2, __local float2 *);
float3 __ovld sincos(float3, __local float3 *);
float4 __ovld sincos(float4, __local float4 *);
float8 __ovld sincos(float8, __local float8 *);
float16 __ovld sincos(float16, __local float16 *);
float __ovld sincos(float, __private float *);
float2 __ovld sincos(float2, __private float2 *);
float3 __ovld sincos(float3, __private float3 *);
float4 __ovld sincos(float4, __private float4 *);
float8 __ovld sincos(float8, __private float8 *);
float16 __ovld sincos(float16, __private float16 *);
#ifdef cl_khr_fp64
double __ovld sincos(double, __global double *);
double2 __ovld sincos(double2, __global double2 *);
double3 __ovld sincos(double3, __global double3 *);
double4 __ovld sincos(double4, __global double4 *);
double8 __ovld sincos(double8, __global double8 *);
double16 __ovld sincos(double16, __global double16 *);
double __ovld sincos(double, __local double *);
double2 __ovld sincos(double2, __local double2 *);
double3 __ovld sincos(double3, __local double3 *);
double4 __ovld sincos(double4, __local double4 *);
double8 __ovld sincos(double8, __local double8 *);
double16 __ovld sincos(double16, __local double16 *);
double __ovld sincos(double, __private double *);
double2 __ovld sincos(double2, __private double2 *);
double3 __ovld sincos(double3, __private double3 *);
double4 __ovld sincos(double4, __private double4 *);
double8 __ovld sincos(double8, __private double8 *);
double16 __ovld sincos(double16, __private double16 *);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld sincos(half, __global half *);
half2 __ovld sincos(half2, __global half2 *);
half3 __ovld sincos(half3, __global half3 *);
half4 __ovld sincos(half4, __global half4 *);
half8 __ovld sincos(half8, __global half8 *);
half16 __ovld sincos(half16, __global half16 *);
half __ovld sincos(half, __local half *);
half2 __ovld sincos(half2, __local half2 *);
half3 __ovld sincos(half3, __local half3 *);
half4 __ovld sincos(half4, __local half4 *);
half8 __ovld sincos(half8, __local half8 *);
half16 __ovld sincos(half16, __local half16 *);
half __ovld sincos(half, __private half *);
half2 __ovld sincos(half2, __private half2 *);
half3 __ovld sincos(half3, __private half3 *);
half4 __ovld sincos(half4, __private half4 *);
half8 __ovld sincos(half8, __private half8 *);
half16 __ovld sincos(half16, __private half16 *);
#endif //cl_khr_fp16
#endif //defined(__opencl_c_named_address_space_builtins)

/**
 * Compute hyperbolic sine.
 */
float __ovld __cnfn sinh(float);
float2 __ovld __cnfn sinh(float2);
float3 __ovld __cnfn sinh(float3);
float4 __ovld __cnfn sinh(float4);
float8 __ovld __cnfn sinh(float8);
float16 __ovld __cnfn sinh(float16);
#ifdef cl_khr_fp64
double __ovld __cnfn sinh(double);
double2 __ovld __cnfn sinh(double2);
double3 __ovld __cnfn sinh(double3);
double4 __ovld __cnfn sinh(double4);
double8 __ovld __cnfn sinh(double8);
double16 __ovld __cnfn sinh(double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn sinh(half);
half2 __ovld __cnfn sinh(half2);
half3 __ovld __cnfn sinh(half3);
half4 __ovld __cnfn sinh(half4);
half8 __ovld __cnfn sinh(half8);
half16 __ovld __cnfn sinh(half16);
#endif //cl_khr_fp16

/**
 * Compute sin (PI * x).
 */
float __ovld __cnfn sinpi(float);
float2 __ovld __cnfn sinpi(float2);
float3 __ovld __cnfn sinpi(float3);
float4 __ovld __cnfn sinpi(float4);
float8 __ovld __cnfn sinpi(float8);
float16 __ovld __cnfn sinpi(float16);
#ifdef cl_khr_fp64
double __ovld __cnfn sinpi(double);
double2 __ovld __cnfn sinpi(double2);
double3 __ovld __cnfn sinpi(double3);
double4 __ovld __cnfn sinpi(double4);
double8 __ovld __cnfn sinpi(double8);
double16 __ovld __cnfn sinpi(double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn sinpi(half);
half2 __ovld __cnfn sinpi(half2);
half3 __ovld __cnfn sinpi(half3);
half4 __ovld __cnfn sinpi(half4);
half8 __ovld __cnfn sinpi(half8);
half16 __ovld __cnfn sinpi(half16);
#endif //cl_khr_fp16

/**
 * Compute square root.
 */
float __ovld __cnfn sqrt(float);
float2 __ovld __cnfn sqrt(float2);
float3 __ovld __cnfn sqrt(float3);
float4 __ovld __cnfn sqrt(float4);
float8 __ovld __cnfn sqrt(float8);
float16 __ovld __cnfn sqrt(float16);
#ifdef cl_khr_fp64
double __ovld __cnfn sqrt(double);
double2 __ovld __cnfn sqrt(double2);
double3 __ovld __cnfn sqrt(double3);
double4 __ovld __cnfn sqrt(double4);
double8 __ovld __cnfn sqrt(double8);
double16 __ovld __cnfn sqrt(double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn sqrt(half);
half2 __ovld __cnfn sqrt(half2);
half3 __ovld __cnfn sqrt(half3);
half4 __ovld __cnfn sqrt(half4);
half8 __ovld __cnfn sqrt(half8);
half16 __ovld __cnfn sqrt(half16);
#endif //cl_khr_fp16

/**
 * Compute tangent.
 */
float __ovld __cnfn tan(float);
float2 __ovld __cnfn tan(float2);
float3 __ovld __cnfn tan(float3);
float4 __ovld __cnfn tan(float4);
float8 __ovld __cnfn tan(float8);
float16 __ovld __cnfn tan(float16);
#ifdef cl_khr_fp64
double __ovld __cnfn tan(double);
double2 __ovld __cnfn tan(double2);
double3 __ovld __cnfn tan(double3);
double4 __ovld __cnfn tan(double4);
double8 __ovld __cnfn tan(double8);
double16 __ovld __cnfn tan(double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn tan(half);
half2 __ovld __cnfn tan(half2);
half3 __ovld __cnfn tan(half3);
half4 __ovld __cnfn tan(half4);
half8 __ovld __cnfn tan(half8);
half16 __ovld __cnfn tan(half16);
#endif //cl_khr_fp16

/**
 * Compute hyperbolic tangent.
 */
float __ovld __cnfn tanh(float);
float2 __ovld __cnfn tanh(float2);
float3 __ovld __cnfn tanh(float3);
float4 __ovld __cnfn tanh(float4);
float8 __ovld __cnfn tanh(float8);
float16 __ovld __cnfn tanh(float16);
#ifdef cl_khr_fp64
double __ovld __cnfn tanh(double);
double2 __ovld __cnfn tanh(double2);
double3 __ovld __cnfn tanh(double3);
double4 __ovld __cnfn tanh(double4);
double8 __ovld __cnfn tanh(double8);
double16 __ovld __cnfn tanh(double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn tanh(half);
half2 __ovld __cnfn tanh(half2);
half3 __ovld __cnfn tanh(half3);
half4 __ovld __cnfn tanh(half4);
half8 __ovld __cnfn tanh(half8);
half16 __ovld __cnfn tanh(half16);
#endif //cl_khr_fp16

/**
 * Compute tan (PI * x).
 */
float __ovld __cnfn tanpi(float);
float2 __ovld __cnfn tanpi(float2);
float3 __ovld __cnfn tanpi(float3);
float4 __ovld __cnfn tanpi(float4);
float8 __ovld __cnfn tanpi(float8);
float16 __ovld __cnfn tanpi(float16);
#ifdef cl_khr_fp64
double __ovld __cnfn tanpi(double);
double2 __ovld __cnfn tanpi(double2);
double3 __ovld __cnfn tanpi(double3);
double4 __ovld __cnfn tanpi(double4);
double8 __ovld __cnfn tanpi(double8);
double16 __ovld __cnfn tanpi(double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn tanpi(half);
half2 __ovld __cnfn tanpi(half2);
half3 __ovld __cnfn tanpi(half3);
half4 __ovld __cnfn tanpi(half4);
half8 __ovld __cnfn tanpi(half8);
half16 __ovld __cnfn tanpi(half16);
#endif //cl_khr_fp16

/**
 * Compute the gamma function.
 */
float __ovld __cnfn tgamma(float);
float2 __ovld __cnfn tgamma(float2);
float3 __ovld __cnfn tgamma(float3);
float4 __ovld __cnfn tgamma(float4);
float8 __ovld __cnfn tgamma(float8);
float16 __ovld __cnfn tgamma(float16);
#ifdef cl_khr_fp64
double __ovld __cnfn tgamma(double);
double2 __ovld __cnfn tgamma(double2);
double3 __ovld __cnfn tgamma(double3);
double4 __ovld __cnfn tgamma(double4);
double8 __ovld __cnfn tgamma(double8);
double16 __ovld __cnfn tgamma(double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn tgamma(half);
half2 __ovld __cnfn tgamma(half2);
half3 __ovld __cnfn tgamma(half3);
half4 __ovld __cnfn tgamma(half4);
half8 __ovld __cnfn tgamma(half8);
half16 __ovld __cnfn tgamma(half16);
#endif //cl_khr_fp16

/**
 * Round to integral value using the round to zero
 * rounding mode.
 */
float __ovld __cnfn trunc(float);
float2 __ovld __cnfn trunc(float2);
float3 __ovld __cnfn trunc(float3);
float4 __ovld __cnfn trunc(float4);
float8 __ovld __cnfn trunc(float8);
float16 __ovld __cnfn trunc(float16);
#ifdef cl_khr_fp64
double __ovld __cnfn trunc(double);
double2 __ovld __cnfn trunc(double2);
double3 __ovld __cnfn trunc(double3);
double4 __ovld __cnfn trunc(double4);
double8 __ovld __cnfn trunc(double8);
double16 __ovld __cnfn trunc(double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn trunc(half);
half2 __ovld __cnfn trunc(half2);
half3 __ovld __cnfn trunc(half3);
half4 __ovld __cnfn trunc(half4);
half8 __ovld __cnfn trunc(half8);
half16 __ovld __cnfn trunc(half16);
#endif //cl_khr_fp16

/**
 * Compute cosine. x must be in the range -2^16 ... +2^16.
 */
float __ovld __cnfn half_cos(float);
float2 __ovld __cnfn half_cos(float2);
float3 __ovld __cnfn half_cos(float3);
float4 __ovld __cnfn half_cos(float4);
float8 __ovld __cnfn half_cos(float8);
float16 __ovld __cnfn half_cos(float16);

/**
 * Compute x / y.
 */
float __ovld __cnfn half_divide(float, float);
float2 __ovld __cnfn half_divide(float2, float2);
float3 __ovld __cnfn half_divide(float3, float3);
float4 __ovld __cnfn half_divide(float4, float4);
float8 __ovld __cnfn half_divide(float8, float8);
float16 __ovld __cnfn half_divide(float16, float16);

/**
 * Compute the base- e exponential of x.
 */
float __ovld __cnfn half_exp(float);
float2 __ovld __cnfn half_exp(float2);
float3 __ovld __cnfn half_exp(float3);
float4 __ovld __cnfn half_exp(float4);
float8 __ovld __cnfn half_exp(float8);
float16 __ovld __cnfn half_exp(float16);

/**
 * Compute the base- 2 exponential of x.
 */
float __ovld __cnfn half_exp2(float);
float2 __ovld __cnfn half_exp2(float2);
float3 __ovld __cnfn half_exp2(float3);
float4 __ovld __cnfn half_exp2(float4);
float8 __ovld __cnfn half_exp2(float8);
float16 __ovld __cnfn half_exp2(float16);

/**
 * Compute the base- 10 exponential of x.
 */
float __ovld __cnfn half_exp10(float);
float2 __ovld __cnfn half_exp10(float2);
float3 __ovld __cnfn half_exp10(float3);
float4 __ovld __cnfn half_exp10(float4);
float8 __ovld __cnfn half_exp10(float8);
float16 __ovld __cnfn half_exp10(float16);

/**
 * Compute natural logarithm.
 */
float __ovld __cnfn half_log(float);
float2 __ovld __cnfn half_log(float2);
float3 __ovld __cnfn half_log(float3);
float4 __ovld __cnfn half_log(float4);
float8 __ovld __cnfn half_log(float8);
float16 __ovld __cnfn half_log(float16);

/**
 * Compute a base 2 logarithm.
 */
float __ovld __cnfn half_log2(float);
float2 __ovld __cnfn half_log2(float2);
float3 __ovld __cnfn half_log2(float3);
float4 __ovld __cnfn half_log2(float4);
float8 __ovld __cnfn half_log2(float8);
float16 __ovld __cnfn half_log2(float16);

/**
 * Compute a base 10 logarithm.
 */
float __ovld __cnfn half_log10(float);
float2 __ovld __cnfn half_log10(float2);
float3 __ovld __cnfn half_log10(float3);
float4 __ovld __cnfn half_log10(float4);
float8 __ovld __cnfn half_log10(float8);
float16 __ovld __cnfn half_log10(float16);

/**
 * Compute x to the power y, where x is >= 0.
 */
float __ovld __cnfn half_powr(float, float);
float2 __ovld __cnfn half_powr(float2, float2);
float3 __ovld __cnfn half_powr(float3, float3);
float4 __ovld __cnfn half_powr(float4, float4);
float8 __ovld __cnfn half_powr(float8, float8);
float16 __ovld __cnfn half_powr(float16, float16);

/**
 * Compute reciprocal.
 */
float __ovld __cnfn half_recip(float);
float2 __ovld __cnfn half_recip(float2);
float3 __ovld __cnfn half_recip(float3);
float4 __ovld __cnfn half_recip(float4);
float8 __ovld __cnfn half_recip(float8);
float16 __ovld __cnfn half_recip(float16);

/**
 * Compute inverse square root.
 */
float __ovld __cnfn half_rsqrt(float);
float2 __ovld __cnfn half_rsqrt(float2);
float3 __ovld __cnfn half_rsqrt(float3);
float4 __ovld __cnfn half_rsqrt(float4);
float8 __ovld __cnfn half_rsqrt(float8);
float16 __ovld __cnfn half_rsqrt(float16);

/**
 * Compute sine. x must be in the range -2^16 ... +2^16.
 */
float __ovld __cnfn half_sin(float);
float2 __ovld __cnfn half_sin(float2);
float3 __ovld __cnfn half_sin(float3);
float4 __ovld __cnfn half_sin(float4);
float8 __ovld __cnfn half_sin(float8);
float16 __ovld __cnfn half_sin(float16);

/**
 * Compute square root.
 */
float __ovld __cnfn half_sqrt(float);
float2 __ovld __cnfn half_sqrt(float2);
float3 __ovld __cnfn half_sqrt(float3);
float4 __ovld __cnfn half_sqrt(float4);
float8 __ovld __cnfn half_sqrt(float8);
float16 __ovld __cnfn half_sqrt(float16);

/**
 * Compute tangent. x must be in the range -216 ... +216.
 */
float __ovld __cnfn half_tan(float);
float2 __ovld __cnfn half_tan(float2);
float3 __ovld __cnfn half_tan(float3);
float4 __ovld __cnfn half_tan(float4);
float8 __ovld __cnfn half_tan(float8);
float16 __ovld __cnfn half_tan(float16);

/**
 * Compute cosine over an implementation-defined range.
 * The maximum error is implementation-defined.
 */
float __ovld __cnfn native_cos(float);
float2 __ovld __cnfn native_cos(float2);
float3 __ovld __cnfn native_cos(float3);
float4 __ovld __cnfn native_cos(float4);
float8 __ovld __cnfn native_cos(float8);
float16 __ovld __cnfn native_cos(float16);

/**
 * Compute x / y over an implementation-defined range.
 * The maximum error is implementation-defined.
 */
float __ovld __cnfn native_divide(float, float);
float2 __ovld __cnfn native_divide(float2, float2);
float3 __ovld __cnfn native_divide(float3, float3);
float4 __ovld __cnfn native_divide(float4, float4);
float8 __ovld __cnfn native_divide(float8, float8);
float16 __ovld __cnfn native_divide(float16, float16);

/**
 * Compute the base- e exponential of x over an
 * implementation-defined range. The maximum error is
 * implementation-defined.
 */
float __ovld __cnfn native_exp(float);
float2 __ovld __cnfn native_exp(float2);
float3 __ovld __cnfn native_exp(float3);
float4 __ovld __cnfn native_exp(float4);
float8 __ovld __cnfn native_exp(float8);
float16 __ovld __cnfn native_exp(float16);

/**
 * Compute the base- 2 exponential of x over an
 * implementation-defined range. The maximum error is
 * implementation-defined.
 */
float __ovld __cnfn native_exp2(float);
float2 __ovld __cnfn native_exp2(float2);
float3 __ovld __cnfn native_exp2(float3);
float4 __ovld __cnfn native_exp2(float4);
float8 __ovld __cnfn native_exp2(float8);
float16 __ovld __cnfn native_exp2(float16);

/**
 * Compute the base- 10 exponential of x over an
 * implementation-defined range. The maximum error is
 * implementation-defined.
 */
float __ovld __cnfn native_exp10(float);
float2 __ovld __cnfn native_exp10(float2);
float3 __ovld __cnfn native_exp10(float3);
float4 __ovld __cnfn native_exp10(float4);
float8 __ovld __cnfn native_exp10(float8);
float16 __ovld __cnfn native_exp10(float16);

/**
 * Compute natural logarithm over an implementationdefined
 * range. The maximum error is implementation
 * defined.
 */
float __ovld __cnfn native_log(float);
float2 __ovld __cnfn native_log(float2);
float3 __ovld __cnfn native_log(float3);
float4 __ovld __cnfn native_log(float4);
float8 __ovld __cnfn native_log(float8);
float16 __ovld __cnfn native_log(float16);

/**
 * Compute a base 2 logarithm over an implementationdefined
 * range. The maximum error is implementationdefined.
 */
float __ovld __cnfn native_log2(float);
float2 __ovld __cnfn native_log2(float2);
float3 __ovld __cnfn native_log2(float3);
float4 __ovld __cnfn native_log2(float4);
float8 __ovld __cnfn native_log2(float8);
float16 __ovld __cnfn native_log2(float16);

/**
 * Compute a base 10 logarithm over an implementationdefined
 * range. The maximum error is implementationdefined.
 */
float __ovld __cnfn native_log10(float);
float2 __ovld __cnfn native_log10(float2);
float3 __ovld __cnfn native_log10(float3);
float4 __ovld __cnfn native_log10(float4);
float8 __ovld __cnfn native_log10(float8);
float16 __ovld __cnfn native_log10(float16);

/**
 * Compute x to the power y, where x is >= 0. The range of
 * x and y are implementation-defined. The maximum error
 * is implementation-defined.
 */
float __ovld __cnfn native_powr(float, float);
float2 __ovld __cnfn native_powr(float2, float2);
float3 __ovld __cnfn native_powr(float3, float3);
float4 __ovld __cnfn native_powr(float4, float4);
float8 __ovld __cnfn native_powr(float8, float8);
float16 __ovld __cnfn native_powr(float16, float16);

/**
 * Compute reciprocal over an implementation-defined
 * range. The maximum error is implementation-defined.
 */
float __ovld __cnfn native_recip(float);
float2 __ovld __cnfn native_recip(float2);
float3 __ovld __cnfn native_recip(float3);
float4 __ovld __cnfn native_recip(float4);
float8 __ovld __cnfn native_recip(float8);
float16 __ovld __cnfn native_recip(float16);

/**
 * Compute inverse square root over an implementationdefined
 * range. The maximum error is implementationdefined.
 */
float __ovld __cnfn native_rsqrt(float);
float2 __ovld __cnfn native_rsqrt(float2);
float3 __ovld __cnfn native_rsqrt(float3);
float4 __ovld __cnfn native_rsqrt(float4);
float8 __ovld __cnfn native_rsqrt(float8);
float16 __ovld __cnfn native_rsqrt(float16);

/**
 * Compute sine over an implementation-defined range.
 * The maximum error is implementation-defined.
 */
float __ovld __cnfn native_sin(float);
float2 __ovld __cnfn native_sin(float2);
float3 __ovld __cnfn native_sin(float3);
float4 __ovld __cnfn native_sin(float4);
float8 __ovld __cnfn native_sin(float8);
float16 __ovld __cnfn native_sin(float16);

/**
 * Compute square root over an implementation-defined
 * range. The maximum error is implementation-defined.
 */
float __ovld __cnfn native_sqrt(float);
float2 __ovld __cnfn native_sqrt(float2);
float3 __ovld __cnfn native_sqrt(float3);
float4 __ovld __cnfn native_sqrt(float4);
float8 __ovld __cnfn native_sqrt(float8);
float16 __ovld __cnfn native_sqrt(float16);

/**
 * Compute tangent over an implementation-defined range.
 * The maximum error is implementation-defined.
 */
float __ovld __cnfn native_tan(float);
float2 __ovld __cnfn native_tan(float2);
float3 __ovld __cnfn native_tan(float3);
float4 __ovld __cnfn native_tan(float4);
float8 __ovld __cnfn native_tan(float8);
float16 __ovld __cnfn native_tan(float16);

// OpenCL v1.1 s6.11.3, v1.2 s6.12.3, v2.0 s6.13.3 - Integer Functions

/**
 * Returns | x |.
 */
uchar __ovld __cnfn abs(char);
uchar __ovld __cnfn abs(uchar);
uchar2 __ovld __cnfn abs(char2);
uchar2 __ovld __cnfn abs(uchar2);
uchar3 __ovld __cnfn abs(char3);
uchar3 __ovld __cnfn abs(uchar3);
uchar4 __ovld __cnfn abs(char4);
uchar4 __ovld __cnfn abs(uchar4);
uchar8 __ovld __cnfn abs(char8);
uchar8 __ovld __cnfn abs(uchar8);
uchar16 __ovld __cnfn abs(char16);
uchar16 __ovld __cnfn abs(uchar16);
ushort __ovld __cnfn abs(short);
ushort __ovld __cnfn abs(ushort);
ushort2 __ovld __cnfn abs(short2);
ushort2 __ovld __cnfn abs(ushort2);
ushort3 __ovld __cnfn abs(short3);
ushort3 __ovld __cnfn abs(ushort3);
ushort4 __ovld __cnfn abs(short4);
ushort4 __ovld __cnfn abs(ushort4);
ushort8 __ovld __cnfn abs(short8);
ushort8 __ovld __cnfn abs(ushort8);
ushort16 __ovld __cnfn abs(short16);
ushort16 __ovld __cnfn abs(ushort16);
uint __ovld __cnfn abs(int);
uint __ovld __cnfn abs(uint);
uint2 __ovld __cnfn abs(int2);
uint2 __ovld __cnfn abs(uint2);
uint3 __ovld __cnfn abs(int3);
uint3 __ovld __cnfn abs(uint3);
uint4 __ovld __cnfn abs(int4);
uint4 __ovld __cnfn abs(uint4);
uint8 __ovld __cnfn abs(int8);
uint8 __ovld __cnfn abs(uint8);
uint16 __ovld __cnfn abs(int16);
uint16 __ovld __cnfn abs(uint16);
ulong __ovld __cnfn abs(long);
ulong __ovld __cnfn abs(ulong);
ulong2 __ovld __cnfn abs(long2);
ulong2 __ovld __cnfn abs(ulong2);
ulong3 __ovld __cnfn abs(long3);
ulong3 __ovld __cnfn abs(ulong3);
ulong4 __ovld __cnfn abs(long4);
ulong4 __ovld __cnfn abs(ulong4);
ulong8 __ovld __cnfn abs(long8);
ulong8 __ovld __cnfn abs(ulong8);
ulong16 __ovld __cnfn abs(long16);
ulong16 __ovld __cnfn abs(ulong16);

/**
 * Returns | x - y | without modulo overflow.
 */
uchar __ovld __cnfn abs_diff(char, char);
uchar __ovld __cnfn abs_diff(uchar, uchar);
uchar2 __ovld __cnfn abs_diff(char2, char2);
uchar2 __ovld __cnfn abs_diff(uchar2, uchar2);
uchar3 __ovld __cnfn abs_diff(char3, char3);
uchar3 __ovld __cnfn abs_diff(uchar3, uchar3);
uchar4 __ovld __cnfn abs_diff(char4, char4);
uchar4 __ovld __cnfn abs_diff(uchar4, uchar4);
uchar8 __ovld __cnfn abs_diff(char8, char8);
uchar8 __ovld __cnfn abs_diff(uchar8, uchar8);
uchar16 __ovld __cnfn abs_diff(char16, char16);
uchar16 __ovld __cnfn abs_diff(uchar16, uchar16);
ushort __ovld __cnfn abs_diff(short, short);
ushort __ovld __cnfn abs_diff(ushort, ushort);
ushort2 __ovld __cnfn abs_diff(short2, short2);
ushort2 __ovld __cnfn abs_diff(ushort2, ushort2);
ushort3 __ovld __cnfn abs_diff(short3, short3);
ushort3 __ovld __cnfn abs_diff(ushort3, ushort3);
ushort4 __ovld __cnfn abs_diff(short4, short4);
ushort4 __ovld __cnfn abs_diff(ushort4, ushort4);
ushort8 __ovld __cnfn abs_diff(short8, short8);
ushort8 __ovld __cnfn abs_diff(ushort8, ushort8);
ushort16 __ovld __cnfn abs_diff(short16, short16);
ushort16 __ovld __cnfn abs_diff(ushort16, ushort16);
uint __ovld __cnfn abs_diff(int, int);
uint __ovld __cnfn abs_diff(uint, uint);
uint2 __ovld __cnfn abs_diff(int2, int2);
uint2 __ovld __cnfn abs_diff(uint2, uint2);
uint3 __ovld __cnfn abs_diff(int3, int3);
uint3 __ovld __cnfn abs_diff(uint3, uint3);
uint4 __ovld __cnfn abs_diff(int4, int4);
uint4 __ovld __cnfn abs_diff(uint4, uint4);
uint8 __ovld __cnfn abs_diff(int8, int8);
uint8 __ovld __cnfn abs_diff(uint8, uint8);
uint16 __ovld __cnfn abs_diff(int16, int16);
uint16 __ovld __cnfn abs_diff(uint16, uint16);
ulong __ovld __cnfn abs_diff(long, long);
ulong __ovld __cnfn abs_diff(ulong, ulong);
ulong2 __ovld __cnfn abs_diff(long2, long2);
ulong2 __ovld __cnfn abs_diff(ulong2, ulong2);
ulong3 __ovld __cnfn abs_diff(long3, long3);
ulong3 __ovld __cnfn abs_diff(ulong3, ulong3);
ulong4 __ovld __cnfn abs_diff(long4, long4);
ulong4 __ovld __cnfn abs_diff(ulong4, ulong4);
ulong8 __ovld __cnfn abs_diff(long8, long8);
ulong8 __ovld __cnfn abs_diff(ulong8, ulong8);
ulong16 __ovld __cnfn abs_diff(long16, long16);
ulong16 __ovld __cnfn abs_diff(ulong16, ulong16);

/**
 * Returns x + y and saturates the result.
 */
char __ovld __cnfn add_sat(char, char);
uchar __ovld __cnfn add_sat(uchar, uchar);
char2 __ovld __cnfn add_sat(char2, char2);
uchar2 __ovld __cnfn add_sat(uchar2, uchar2);
char3 __ovld __cnfn add_sat(char3, char3);
uchar3 __ovld __cnfn add_sat(uchar3, uchar3);
char4 __ovld __cnfn add_sat(char4, char4);
uchar4 __ovld __cnfn add_sat(uchar4, uchar4);
char8 __ovld __cnfn add_sat(char8, char8);
uchar8 __ovld __cnfn add_sat(uchar8, uchar8);
char16 __ovld __cnfn add_sat(char16, char16);
uchar16 __ovld __cnfn add_sat(uchar16, uchar16);
short __ovld __cnfn add_sat(short, short);
ushort __ovld __cnfn add_sat(ushort, ushort);
short2 __ovld __cnfn add_sat(short2, short2);
ushort2 __ovld __cnfn add_sat(ushort2, ushort2);
short3 __ovld __cnfn add_sat(short3, short3);
ushort3 __ovld __cnfn add_sat(ushort3, ushort3);
short4 __ovld __cnfn add_sat(short4, short4);
ushort4 __ovld __cnfn add_sat(ushort4, ushort4);
short8 __ovld __cnfn add_sat(short8, short8);
ushort8 __ovld __cnfn add_sat(ushort8, ushort8);
short16 __ovld __cnfn add_sat(short16, short16);
ushort16 __ovld __cnfn add_sat(ushort16, ushort16);
int __ovld __cnfn add_sat(int, int);
uint __ovld __cnfn add_sat(uint, uint);
int2 __ovld __cnfn add_sat(int2, int2);
uint2 __ovld __cnfn add_sat(uint2, uint2);
int3 __ovld __cnfn add_sat(int3, int3);
uint3 __ovld __cnfn add_sat(uint3, uint3);
int4 __ovld __cnfn add_sat(int4, int4);
uint4 __ovld __cnfn add_sat(uint4, uint4);
int8 __ovld __cnfn add_sat(int8, int8);
uint8 __ovld __cnfn add_sat(uint8, uint8);
int16 __ovld __cnfn add_sat(int16, int16);
uint16 __ovld __cnfn add_sat(uint16, uint16);
long __ovld __cnfn add_sat(long, long);
ulong __ovld __cnfn add_sat(ulong, ulong);
long2 __ovld __cnfn add_sat(long2, long2);
ulong2 __ovld __cnfn add_sat(ulong2, ulong2);
long3 __ovld __cnfn add_sat(long3, long3);
ulong3 __ovld __cnfn add_sat(ulong3, ulong3);
long4 __ovld __cnfn add_sat(long4, long4);
ulong4 __ovld __cnfn add_sat(ulong4, ulong4);
long8 __ovld __cnfn add_sat(long8, long8);
ulong8 __ovld __cnfn add_sat(ulong8, ulong8);
long16 __ovld __cnfn add_sat(long16, long16);
ulong16 __ovld __cnfn add_sat(ulong16, ulong16);

/**
 * Returns (x + y) >> 1. The intermediate sum does
 * not modulo overflow.
 */
char __ovld __cnfn hadd(char, char);
uchar __ovld __cnfn hadd(uchar, uchar);
char2 __ovld __cnfn hadd(char2, char2);
uchar2 __ovld __cnfn hadd(uchar2, uchar2);
char3 __ovld __cnfn hadd(char3, char3);
uchar3 __ovld __cnfn hadd(uchar3, uchar3);
char4 __ovld __cnfn hadd(char4, char4);
uchar4 __ovld __cnfn hadd(uchar4, uchar4);
char8 __ovld __cnfn hadd(char8, char8);
uchar8 __ovld __cnfn hadd(uchar8, uchar8);
char16 __ovld __cnfn hadd(char16, char16);
uchar16 __ovld __cnfn hadd(uchar16, uchar16);
short __ovld __cnfn hadd(short, short);
ushort __ovld __cnfn hadd(ushort, ushort);
short2 __ovld __cnfn hadd(short2, short2);
ushort2 __ovld __cnfn hadd(ushort2, ushort2);
short3 __ovld __cnfn hadd(short3, short3);
ushort3 __ovld __cnfn hadd(ushort3, ushort3);
short4 __ovld __cnfn hadd(short4, short4);
ushort4 __ovld __cnfn hadd(ushort4, ushort4);
short8 __ovld __cnfn hadd(short8, short8);
ushort8 __ovld __cnfn hadd(ushort8, ushort8);
short16 __ovld __cnfn hadd(short16, short16);
ushort16 __ovld __cnfn hadd(ushort16, ushort16);
int __ovld __cnfn hadd(int, int);
uint __ovld __cnfn hadd(uint, uint);
int2 __ovld __cnfn hadd(int2, int2);
uint2 __ovld __cnfn hadd(uint2, uint2);
int3 __ovld __cnfn hadd(int3, int3);
uint3 __ovld __cnfn hadd(uint3, uint3);
int4 __ovld __cnfn hadd(int4, int4);
uint4 __ovld __cnfn hadd(uint4, uint4);
int8 __ovld __cnfn hadd(int8, int8);
uint8 __ovld __cnfn hadd(uint8, uint8);
int16 __ovld __cnfn hadd(int16, int16);
uint16 __ovld __cnfn hadd(uint16, uint16);
long __ovld __cnfn hadd(long, long);
ulong __ovld __cnfn hadd(ulong, ulong);
long2 __ovld __cnfn hadd(long2, long2);
ulong2 __ovld __cnfn hadd(ulong2, ulong2);
long3 __ovld __cnfn hadd(long3, long3);
ulong3 __ovld __cnfn hadd(ulong3, ulong3);
long4 __ovld __cnfn hadd(long4, long4);
ulong4 __ovld __cnfn hadd(ulong4, ulong4);
long8 __ovld __cnfn hadd(long8, long8);
ulong8 __ovld __cnfn hadd(ulong8, ulong8);
long16 __ovld __cnfn hadd(long16, long16);
ulong16 __ovld __cnfn hadd(ulong16, ulong16);

/**
 * Returns (x + y + 1) >> 1. The intermediate sum
 * does not modulo overflow.
 */
char __ovld __cnfn rhadd(char, char);
uchar __ovld __cnfn rhadd(uchar, uchar);
char2 __ovld __cnfn rhadd(char2, char2);
uchar2 __ovld __cnfn rhadd(uchar2, uchar2);
char3 __ovld __cnfn rhadd(char3, char3);
uchar3 __ovld __cnfn rhadd(uchar3, uchar3);
char4 __ovld __cnfn rhadd(char4, char4);
uchar4 __ovld __cnfn rhadd(uchar4, uchar4);
char8 __ovld __cnfn rhadd(char8, char8);
uchar8 __ovld __cnfn rhadd(uchar8, uchar8);
char16 __ovld __cnfn rhadd(char16, char16);
uchar16 __ovld __cnfn rhadd(uchar16, uchar16);
short __ovld __cnfn rhadd(short, short);
ushort __ovld __cnfn rhadd(ushort, ushort);
short2 __ovld __cnfn rhadd(short2, short2);
ushort2 __ovld __cnfn rhadd(ushort2, ushort2);
short3 __ovld __cnfn rhadd(short3, short3);
ushort3 __ovld __cnfn rhadd(ushort3, ushort3);
short4 __ovld __cnfn rhadd(short4, short4);
ushort4 __ovld __cnfn rhadd(ushort4, ushort4);
short8 __ovld __cnfn rhadd(short8, short8);
ushort8 __ovld __cnfn rhadd(ushort8, ushort8);
short16 __ovld __cnfn rhadd(short16, short16);
ushort16 __ovld __cnfn rhadd(ushort16, ushort16);
int __ovld __cnfn rhadd(int, int);
uint __ovld __cnfn rhadd(uint, uint);
int2 __ovld __cnfn rhadd(int2, int2);
uint2 __ovld __cnfn rhadd(uint2, uint2);
int3 __ovld __cnfn rhadd(int3, int3);
uint3 __ovld __cnfn rhadd(uint3, uint3);
int4 __ovld __cnfn rhadd(int4, int4);
uint4 __ovld __cnfn rhadd(uint4, uint4);
int8 __ovld __cnfn rhadd(int8, int8);
uint8 __ovld __cnfn rhadd(uint8, uint8);
int16 __ovld __cnfn rhadd(int16, int16);
uint16 __ovld __cnfn rhadd(uint16, uint16);
long __ovld __cnfn rhadd(long, long);
ulong __ovld __cnfn rhadd(ulong, ulong);
long2 __ovld __cnfn rhadd(long2, long2);
ulong2 __ovld __cnfn rhadd(ulong2, ulong2);
long3 __ovld __cnfn rhadd(long3, long3);
ulong3 __ovld __cnfn rhadd(ulong3, ulong3);
long4 __ovld __cnfn rhadd(long4, long4);
ulong4 __ovld __cnfn rhadd(ulong4, ulong4);
long8 __ovld __cnfn rhadd(long8, long8);
ulong8 __ovld __cnfn rhadd(ulong8, ulong8);
long16 __ovld __cnfn rhadd(long16, long16);
ulong16 __ovld __cnfn rhadd(ulong16, ulong16);

/**
 * Returns min(max(x, minval), maxval).
 * Results are undefined if minval > maxval.
 */
char __ovld __cnfn clamp(char, char, char);
uchar __ovld __cnfn clamp(uchar, uchar, uchar);
char2 __ovld __cnfn clamp(char2, char2, char2);
uchar2 __ovld __cnfn clamp(uchar2, uchar2, uchar2);
char3 __ovld __cnfn clamp(char3, char3, char3);
uchar3 __ovld __cnfn clamp(uchar3, uchar3, uchar3);
char4 __ovld __cnfn clamp(char4, char4, char4);
uchar4 __ovld __cnfn clamp(uchar4, uchar4, uchar4);
char8 __ovld __cnfn clamp(char8, char8, char8);
uchar8 __ovld __cnfn clamp(uchar8, uchar8, uchar8);
char16 __ovld __cnfn clamp(char16, char16, char16);
uchar16 __ovld __cnfn clamp(uchar16, uchar16, uchar16);
short __ovld __cnfn clamp(short, short, short);
ushort __ovld __cnfn clamp(ushort, ushort, ushort);
short2 __ovld __cnfn clamp(short2, short2, short2);
ushort2 __ovld __cnfn clamp(ushort2, ushort2, ushort2);
short3 __ovld __cnfn clamp(short3, short3, short3);
ushort3 __ovld __cnfn clamp(ushort3, ushort3, ushort3);
short4 __ovld __cnfn clamp(short4, short4, short4);
ushort4 __ovld __cnfn clamp(ushort4, ushort4, ushort4);
short8 __ovld __cnfn clamp(short8, short8, short8);
ushort8 __ovld __cnfn clamp(ushort8, ushort8, ushort8);
short16 __ovld __cnfn clamp(short16, short16, short16);
ushort16 __ovld __cnfn clamp(ushort16, ushort16, ushort16);
int __ovld __cnfn clamp(int, int, int);
uint __ovld __cnfn clamp(uint, uint, uint);
int2 __ovld __cnfn clamp(int2, int2, int2);
uint2 __ovld __cnfn clamp(uint2, uint2, uint2);
int3 __ovld __cnfn clamp(int3, int3, int3);
uint3 __ovld __cnfn clamp(uint3, uint3, uint3);
int4 __ovld __cnfn clamp(int4, int4, int4);
uint4 __ovld __cnfn clamp(uint4, uint4, uint4);
int8 __ovld __cnfn clamp(int8, int8, int8);
uint8 __ovld __cnfn clamp(uint8, uint8, uint8);
int16 __ovld __cnfn clamp(int16, int16, int16);
uint16 __ovld __cnfn clamp(uint16, uint16, uint16);
long __ovld __cnfn clamp(long, long, long);
ulong __ovld __cnfn clamp(ulong, ulong, ulong);
long2 __ovld __cnfn clamp(long2, long2, long2);
ulong2 __ovld __cnfn clamp(ulong2, ulong2, ulong2);
long3 __ovld __cnfn clamp(long3, long3, long3);
ulong3 __ovld __cnfn clamp(ulong3, ulong3, ulong3);
long4 __ovld __cnfn clamp(long4, long4, long4);
ulong4 __ovld __cnfn clamp(ulong4, ulong4, ulong4);
long8 __ovld __cnfn clamp(long8, long8, long8);
ulong8 __ovld __cnfn clamp(ulong8, ulong8, ulong8);
long16 __ovld __cnfn clamp(long16, long16, long16);
ulong16 __ovld __cnfn clamp(ulong16, ulong16, ulong16);
char2 __ovld __cnfn clamp(char2, char, char);
uchar2 __ovld __cnfn clamp(uchar2, uchar, uchar);
char3 __ovld __cnfn clamp(char3, char, char);
uchar3 __ovld __cnfn clamp(uchar3, uchar, uchar);
char4 __ovld __cnfn clamp(char4, char, char);
uchar4 __ovld __cnfn clamp(uchar4, uchar, uchar);
char8 __ovld __cnfn clamp(char8, char, char);
uchar8 __ovld __cnfn clamp(uchar8, uchar, uchar);
char16 __ovld __cnfn clamp(char16, char, char);
uchar16 __ovld __cnfn clamp(uchar16, uchar, uchar);
short2 __ovld __cnfn clamp(short2, short, short);
ushort2 __ovld __cnfn clamp(ushort2, ushort, ushort);
short3 __ovld __cnfn clamp(short3, short, short);
ushort3 __ovld __cnfn clamp(ushort3, ushort, ushort);
short4 __ovld __cnfn clamp(short4, short, short);
ushort4 __ovld __cnfn clamp(ushort4, ushort, ushort);
short8 __ovld __cnfn clamp(short8, short, short);
ushort8 __ovld __cnfn clamp(ushort8, ushort, ushort);
short16 __ovld __cnfn clamp(short16, short, short);
ushort16 __ovld __cnfn clamp(ushort16, ushort, ushort);
int2 __ovld __cnfn clamp(int2, int, int);
uint2 __ovld __cnfn clamp(uint2, uint, uint);
int3 __ovld __cnfn clamp(int3, int, int);
uint3 __ovld __cnfn clamp(uint3, uint, uint);
int4 __ovld __cnfn clamp(int4, int, int);
uint4 __ovld __cnfn clamp(uint4, uint, uint);
int8 __ovld __cnfn clamp(int8, int, int);
uint8 __ovld __cnfn clamp(uint8, uint, uint);
int16 __ovld __cnfn clamp(int16, int, int);
uint16 __ovld __cnfn clamp(uint16, uint, uint);
long2 __ovld __cnfn clamp(long2, long, long);
ulong2 __ovld __cnfn clamp(ulong2, ulong, ulong);
long3 __ovld __cnfn clamp(long3, long, long);
ulong3 __ovld __cnfn clamp(ulong3, ulong, ulong);
long4 __ovld __cnfn clamp(long4, long, long);
ulong4 __ovld __cnfn clamp(ulong4, ulong, ulong);
long8 __ovld __cnfn clamp(long8, long, long);
ulong8 __ovld __cnfn clamp(ulong8, ulong, ulong);
long16 __ovld __cnfn clamp(long16, long, long);
ulong16 __ovld __cnfn clamp(ulong16, ulong, ulong);

/**
 * Returns the number of leading 0-bits in x, starting
 * at the most significant bit position.
 */
char __ovld __cnfn clz(char);
uchar __ovld __cnfn clz(uchar);
char2 __ovld __cnfn clz(char2);
uchar2 __ovld __cnfn clz(uchar2);
char3 __ovld __cnfn clz(char3);
uchar3 __ovld __cnfn clz(uchar3);
char4 __ovld __cnfn clz(char4);
uchar4 __ovld __cnfn clz(uchar4);
char8 __ovld __cnfn clz(char8);
uchar8 __ovld __cnfn clz(uchar8);
char16 __ovld __cnfn clz(char16);
uchar16 __ovld __cnfn clz(uchar16);
short __ovld __cnfn clz(short);
ushort __ovld __cnfn clz(ushort);
short2 __ovld __cnfn clz(short2);
ushort2 __ovld __cnfn clz(ushort2);
short3 __ovld __cnfn clz(short3);
ushort3 __ovld __cnfn clz(ushort3);
short4 __ovld __cnfn clz(short4);
ushort4 __ovld __cnfn clz(ushort4);
short8 __ovld __cnfn clz(short8);
ushort8 __ovld __cnfn clz(ushort8);
short16 __ovld __cnfn clz(short16);
ushort16 __ovld __cnfn clz(ushort16);
int __ovld __cnfn clz(int);
uint __ovld __cnfn clz(uint);
int2 __ovld __cnfn clz(int2);
uint2 __ovld __cnfn clz(uint2);
int3 __ovld __cnfn clz(int3);
uint3 __ovld __cnfn clz(uint3);
int4 __ovld __cnfn clz(int4);
uint4 __ovld __cnfn clz(uint4);
int8 __ovld __cnfn clz(int8);
uint8 __ovld __cnfn clz(uint8);
int16 __ovld __cnfn clz(int16);
uint16 __ovld __cnfn clz(uint16);
long __ovld __cnfn clz(long);
ulong __ovld __cnfn clz(ulong);
long2 __ovld __cnfn clz(long2);
ulong2 __ovld __cnfn clz(ulong2);
long3 __ovld __cnfn clz(long3);
ulong3 __ovld __cnfn clz(ulong3);
long4 __ovld __cnfn clz(long4);
ulong4 __ovld __cnfn clz(ulong4);
long8 __ovld __cnfn clz(long8);
ulong8 __ovld __cnfn clz(ulong8);
long16 __ovld __cnfn clz(long16);
ulong16 __ovld __cnfn clz(ulong16);

/**
 * Returns the count of trailing 0-bits in x. If x is 0,
 * returns the size in bits of the type of x or
 * component type of x, if x is a vector.
 */
#if defined(__OPENCL_CPP_VERSION__) || (__OPENCL_C_VERSION__ >= CL_VERSION_2_0)
char __ovld __cnfn ctz(char);
uchar __ovld __cnfn ctz(uchar);
char2 __ovld __cnfn ctz(char2);
uchar2 __ovld __cnfn ctz(uchar2);
char3 __ovld __cnfn ctz(char3);
uchar3 __ovld __cnfn ctz(uchar3);
char4 __ovld __cnfn ctz(char4);
uchar4 __ovld __cnfn ctz(uchar4);
char8 __ovld __cnfn ctz(char8);
uchar8 __ovld __cnfn ctz(uchar8);
char16 __ovld __cnfn ctz(char16);
uchar16 __ovld __cnfn ctz(uchar16);
short __ovld __cnfn ctz(short);
ushort __ovld __cnfn ctz(ushort);
short2 __ovld __cnfn ctz(short2);
ushort2 __ovld __cnfn ctz(ushort2);
short3 __ovld __cnfn ctz(short3);
ushort3 __ovld __cnfn ctz(ushort3);
short4 __ovld __cnfn ctz(short4);
ushort4 __ovld __cnfn ctz(ushort4);
short8 __ovld __cnfn ctz(short8);
ushort8 __ovld __cnfn ctz(ushort8);
short16 __ovld __cnfn ctz(short16);
ushort16 __ovld __cnfn ctz(ushort16);
int __ovld __cnfn ctz(int);
uint __ovld __cnfn ctz(uint);
int2 __ovld __cnfn ctz(int2);
uint2 __ovld __cnfn ctz(uint2);
int3 __ovld __cnfn ctz(int3);
uint3 __ovld __cnfn ctz(uint3);
int4 __ovld __cnfn ctz(int4);
uint4 __ovld __cnfn ctz(uint4);
int8 __ovld __cnfn ctz(int8);
uint8 __ovld __cnfn ctz(uint8);
int16 __ovld __cnfn ctz(int16);
uint16 __ovld __cnfn ctz(uint16);
long __ovld __cnfn ctz(long);
ulong __ovld __cnfn ctz(ulong);
long2 __ovld __cnfn ctz(long2);
ulong2 __ovld __cnfn ctz(ulong2);
long3 __ovld __cnfn ctz(long3);
ulong3 __ovld __cnfn ctz(ulong3);
long4 __ovld __cnfn ctz(long4);
ulong4 __ovld __cnfn ctz(ulong4);
long8 __ovld __cnfn ctz(long8);
ulong8 __ovld __cnfn ctz(ulong8);
long16 __ovld __cnfn ctz(long16);
ulong16 __ovld __cnfn ctz(ulong16);
#endif //defined(__OPENCL_CPP_VERSION__) || (__OPENCL_C_VERSION__ >= CL_VERSION_2_0)

/**
 * Returns mul_hi(a, b) + c.
 */
char __ovld __cnfn mad_hi(char, char, char);
uchar __ovld __cnfn mad_hi(uchar, uchar, uchar);
char2 __ovld __cnfn mad_hi(char2, char2, char2);
uchar2 __ovld __cnfn mad_hi(uchar2, uchar2, uchar2);
char3 __ovld __cnfn mad_hi(char3, char3, char3);
uchar3 __ovld __cnfn mad_hi(uchar3, uchar3, uchar3);
char4 __ovld __cnfn mad_hi(char4, char4, char4);
uchar4 __ovld __cnfn mad_hi(uchar4, uchar4, uchar4);
char8 __ovld __cnfn mad_hi(char8, char8, char8);
uchar8 __ovld __cnfn mad_hi(uchar8, uchar8, uchar8);
char16 __ovld __cnfn mad_hi(char16, char16, char16);
uchar16 __ovld __cnfn mad_hi(uchar16, uchar16, uchar16);
short __ovld __cnfn mad_hi(short, short, short);
ushort __ovld __cnfn mad_hi(ushort, ushort, ushort);
short2 __ovld __cnfn mad_hi(short2, short2, short2);
ushort2 __ovld __cnfn mad_hi(ushort2, ushort2, ushort2);
short3 __ovld __cnfn mad_hi(short3, short3, short3);
ushort3 __ovld __cnfn mad_hi(ushort3, ushort3, ushort3);
short4 __ovld __cnfn mad_hi(short4, short4, short4);
ushort4 __ovld __cnfn mad_hi(ushort4, ushort4, ushort4);
short8 __ovld __cnfn mad_hi(short8, short8, short8);
ushort8 __ovld __cnfn mad_hi(ushort8, ushort8, ushort8);
short16 __ovld __cnfn mad_hi(short16, short16, short16);
ushort16 __ovld __cnfn mad_hi(ushort16, ushort16, ushort16);
int __ovld __cnfn mad_hi(int, int, int);
uint __ovld __cnfn mad_hi(uint, uint, uint);
int2 __ovld __cnfn mad_hi(int2, int2, int2);
uint2 __ovld __cnfn mad_hi(uint2, uint2, uint2);
int3 __ovld __cnfn mad_hi(int3, int3, int3);
uint3 __ovld __cnfn mad_hi(uint3, uint3, uint3);
int4 __ovld __cnfn mad_hi(int4, int4, int4);
uint4 __ovld __cnfn mad_hi(uint4, uint4, uint4);
int8 __ovld __cnfn mad_hi(int8, int8, int8);
uint8 __ovld __cnfn mad_hi(uint8, uint8, uint8);
int16 __ovld __cnfn mad_hi(int16, int16, int16);
uint16 __ovld __cnfn mad_hi(uint16, uint16, uint16);
long __ovld __cnfn mad_hi(long, long, long);
ulong __ovld __cnfn mad_hi(ulong, ulong, ulong);
long2 __ovld __cnfn mad_hi(long2, long2, long2);
ulong2 __ovld __cnfn mad_hi(ulong2, ulong2, ulong2);
long3 __ovld __cnfn mad_hi(long3, long3, long3);
ulong3 __ovld __cnfn mad_hi(ulong3, ulong3, ulong3);
long4 __ovld __cnfn mad_hi(long4, long4, long4);
ulong4 __ovld __cnfn mad_hi(ulong4, ulong4, ulong4);
long8 __ovld __cnfn mad_hi(long8, long8, long8);
ulong8 __ovld __cnfn mad_hi(ulong8, ulong8, ulong8);
long16 __ovld __cnfn mad_hi(long16, long16, long16);
ulong16 __ovld __cnfn mad_hi(ulong16, ulong16, ulong16);

/**
 * Returns a * b + c and saturates the result.
 */
char __ovld __cnfn mad_sat(char, char, char);
uchar __ovld __cnfn mad_sat(uchar, uchar, uchar);
char2 __ovld __cnfn mad_sat(char2, char2, char2);
uchar2 __ovld __cnfn mad_sat(uchar2, uchar2, uchar2);
char3 __ovld __cnfn mad_sat(char3, char3, char3);
uchar3 __ovld __cnfn mad_sat(uchar3, uchar3, uchar3);
char4 __ovld __cnfn mad_sat(char4, char4, char4);
uchar4 __ovld __cnfn mad_sat(uchar4, uchar4, uchar4);
char8 __ovld __cnfn mad_sat(char8, char8, char8);
uchar8 __ovld __cnfn mad_sat(uchar8, uchar8, uchar8);
char16 __ovld __cnfn mad_sat(char16, char16, char16);
uchar16 __ovld __cnfn mad_sat(uchar16, uchar16, uchar16);
short __ovld __cnfn mad_sat(short, short, short);
ushort __ovld __cnfn mad_sat(ushort, ushort, ushort);
short2 __ovld __cnfn mad_sat(short2, short2, short2);
ushort2 __ovld __cnfn mad_sat(ushort2, ushort2, ushort2);
short3 __ovld __cnfn mad_sat(short3, short3, short3);
ushort3 __ovld __cnfn mad_sat(ushort3, ushort3, ushort3);
short4 __ovld __cnfn mad_sat(short4, short4, short4);
ushort4 __ovld __cnfn mad_sat(ushort4, ushort4, ushort4);
short8 __ovld __cnfn mad_sat(short8, short8, short8);
ushort8 __ovld __cnfn mad_sat(ushort8, ushort8, ushort8);
short16 __ovld __cnfn mad_sat(short16, short16, short16);
ushort16 __ovld __cnfn mad_sat(ushort16, ushort16, ushort16);
int __ovld __cnfn mad_sat(int, int, int);
uint __ovld __cnfn mad_sat(uint, uint, uint);
int2 __ovld __cnfn mad_sat(int2, int2, int2);
uint2 __ovld __cnfn mad_sat(uint2, uint2, uint2);
int3 __ovld __cnfn mad_sat(int3, int3, int3);
uint3 __ovld __cnfn mad_sat(uint3, uint3, uint3);
int4 __ovld __cnfn mad_sat(int4, int4, int4);
uint4 __ovld __cnfn mad_sat(uint4, uint4, uint4);
int8 __ovld __cnfn mad_sat(int8, int8, int8);
uint8 __ovld __cnfn mad_sat(uint8, uint8, uint8);
int16 __ovld __cnfn mad_sat(int16, int16, int16);
uint16 __ovld __cnfn mad_sat(uint16, uint16, uint16);
long __ovld __cnfn mad_sat(long, long, long);
ulong __ovld __cnfn mad_sat(ulong, ulong, ulong);
long2 __ovld __cnfn mad_sat(long2, long2, long2);
ulong2 __ovld __cnfn mad_sat(ulong2, ulong2, ulong2);
long3 __ovld __cnfn mad_sat(long3, long3, long3);
ulong3 __ovld __cnfn mad_sat(ulong3, ulong3, ulong3);
long4 __ovld __cnfn mad_sat(long4, long4, long4);
ulong4 __ovld __cnfn mad_sat(ulong4, ulong4, ulong4);
long8 __ovld __cnfn mad_sat(long8, long8, long8);
ulong8 __ovld __cnfn mad_sat(ulong8, ulong8, ulong8);
long16 __ovld __cnfn mad_sat(long16, long16, long16);
ulong16 __ovld __cnfn mad_sat(ulong16, ulong16, ulong16);

/**
 * Returns y if x < y, otherwise it returns x.
 */
char __ovld __cnfn max(char, char);
uchar __ovld __cnfn max(uchar, uchar);
char2 __ovld __cnfn max(char2, char2);
uchar2 __ovld __cnfn max(uchar2, uchar2);
char3 __ovld __cnfn max(char3, char3);
uchar3 __ovld __cnfn max(uchar3, uchar3);
char4 __ovld __cnfn max(char4, char4);
uchar4 __ovld __cnfn max(uchar4, uchar4);
char8 __ovld __cnfn max(char8, char8);
uchar8 __ovld __cnfn max(uchar8, uchar8);
char16 __ovld __cnfn max(char16, char16);
uchar16 __ovld __cnfn max(uchar16, uchar16);
short __ovld __cnfn max(short, short);
ushort __ovld __cnfn max(ushort, ushort);
short2 __ovld __cnfn max(short2, short2);
ushort2 __ovld __cnfn max(ushort2, ushort2);
short3 __ovld __cnfn max(short3, short3);
ushort3 __ovld __cnfn max(ushort3, ushort3);
short4 __ovld __cnfn max(short4, short4);
ushort4 __ovld __cnfn max(ushort4, ushort4);
short8 __ovld __cnfn max(short8, short8);
ushort8 __ovld __cnfn max(ushort8, ushort8);
short16 __ovld __cnfn max(short16, short16);
ushort16 __ovld __cnfn max(ushort16, ushort16);
int __ovld __cnfn max(int, int);
uint __ovld __cnfn max(uint, uint);
int2 __ovld __cnfn max(int2, int2);
uint2 __ovld __cnfn max(uint2, uint2);
int3 __ovld __cnfn max(int3, int3);
uint3 __ovld __cnfn max(uint3, uint3);
int4 __ovld __cnfn max(int4, int4);
uint4 __ovld __cnfn max(uint4, uint4);
int8 __ovld __cnfn max(int8, int8);
uint8 __ovld __cnfn max(uint8, uint8);
int16 __ovld __cnfn max(int16, int16);
uint16 __ovld __cnfn max(uint16, uint16);
long __ovld __cnfn max(long, long);
ulong __ovld __cnfn max(ulong, ulong);
long2 __ovld __cnfn max(long2, long2);
ulong2 __ovld __cnfn max(ulong2, ulong2);
long3 __ovld __cnfn max(long3, long3);
ulong3 __ovld __cnfn max(ulong3, ulong3);
long4 __ovld __cnfn max(long4, long4);
ulong4 __ovld __cnfn max(ulong4, ulong4);
long8 __ovld __cnfn max(long8, long8);
ulong8 __ovld __cnfn max(ulong8, ulong8);
long16 __ovld __cnfn max(long16, long16);
ulong16 __ovld __cnfn max(ulong16, ulong16);
char2 __ovld __cnfn max(char2, char);
uchar2 __ovld __cnfn max(uchar2, uchar);
char3 __ovld __cnfn max(char3, char);
uchar3 __ovld __cnfn max(uchar3, uchar);
char4 __ovld __cnfn max(char4, char);
uchar4 __ovld __cnfn max(uchar4, uchar);
char8 __ovld __cnfn max(char8, char);
uchar8 __ovld __cnfn max(uchar8, uchar);
char16 __ovld __cnfn max(char16, char);
uchar16 __ovld __cnfn max(uchar16, uchar);
short2 __ovld __cnfn max(short2, short);
ushort2 __ovld __cnfn max(ushort2, ushort);
short3 __ovld __cnfn max(short3, short);
ushort3 __ovld __cnfn max(ushort3, ushort);
short4 __ovld __cnfn max(short4, short);
ushort4 __ovld __cnfn max(ushort4, ushort);
short8 __ovld __cnfn max(short8, short);
ushort8 __ovld __cnfn max(ushort8, ushort);
short16 __ovld __cnfn max(short16, short);
ushort16 __ovld __cnfn max(ushort16, ushort);
int2 __ovld __cnfn max(int2, int);
uint2 __ovld __cnfn max(uint2, uint);
int3 __ovld __cnfn max(int3, int);
uint3 __ovld __cnfn max(uint3, uint);
int4 __ovld __cnfn max(int4, int);
uint4 __ovld __cnfn max(uint4, uint);
int8 __ovld __cnfn max(int8, int);
uint8 __ovld __cnfn max(uint8, uint);
int16 __ovld __cnfn max(int16, int);
uint16 __ovld __cnfn max(uint16, uint);
long2 __ovld __cnfn max(long2, long);
ulong2 __ovld __cnfn max(ulong2, ulong);
long3 __ovld __cnfn max(long3, long);
ulong3 __ovld __cnfn max(ulong3, ulong);
long4 __ovld __cnfn max(long4, long);
ulong4 __ovld __cnfn max(ulong4, ulong);
long8 __ovld __cnfn max(long8, long);
ulong8 __ovld __cnfn max(ulong8, ulong);
long16 __ovld __cnfn max(long16, long);
ulong16 __ovld __cnfn max(ulong16, ulong);

/**
 * Returns y if y < x, otherwise it returns x.
 */
char __ovld __cnfn min(char, char);
uchar __ovld __cnfn min(uchar, uchar);
char2 __ovld __cnfn min(char2, char2);
uchar2 __ovld __cnfn min(uchar2, uchar2);
char3 __ovld __cnfn min(char3, char3);
uchar3 __ovld __cnfn min(uchar3, uchar3);
char4 __ovld __cnfn min(char4, char4);
uchar4 __ovld __cnfn min(uchar4, uchar4);
char8 __ovld __cnfn min(char8, char8);
uchar8 __ovld __cnfn min(uchar8, uchar8);
char16 __ovld __cnfn min(char16, char16);
uchar16 __ovld __cnfn min(uchar16, uchar16);
short __ovld __cnfn min(short, short);
ushort __ovld __cnfn min(ushort, ushort);
short2 __ovld __cnfn min(short2, short2);
ushort2 __ovld __cnfn min(ushort2, ushort2);
short3 __ovld __cnfn min(short3, short3);
ushort3 __ovld __cnfn min(ushort3, ushort3);
short4 __ovld __cnfn min(short4, short4);
ushort4 __ovld __cnfn min(ushort4, ushort4);
short8 __ovld __cnfn min(short8, short8);
ushort8 __ovld __cnfn min(ushort8, ushort8);
short16 __ovld __cnfn min(short16, short16);
ushort16 __ovld __cnfn min(ushort16, ushort16);
int __ovld __cnfn min(int, int);
uint __ovld __cnfn min(uint, uint);
int2 __ovld __cnfn min(int2, int2);
uint2 __ovld __cnfn min(uint2, uint2);
int3 __ovld __cnfn min(int3, int3);
uint3 __ovld __cnfn min(uint3, uint3);
int4 __ovld __cnfn min(int4, int4);
uint4 __ovld __cnfn min(uint4, uint4);
int8 __ovld __cnfn min(int8, int8);
uint8 __ovld __cnfn min(uint8, uint8);
int16 __ovld __cnfn min(int16, int16);
uint16 __ovld __cnfn min(uint16, uint16);
long __ovld __cnfn min(long, long);
ulong __ovld __cnfn min(ulong, ulong);
long2 __ovld __cnfn min(long2, long2);
ulong2 __ovld __cnfn min(ulong2, ulong2);
long3 __ovld __cnfn min(long3, long3);
ulong3 __ovld __cnfn min(ulong3, ulong3);
long4 __ovld __cnfn min(long4, long4);
ulong4 __ovld __cnfn min(ulong4, ulong4);
long8 __ovld __cnfn min(long8, long8);
ulong8 __ovld __cnfn min(ulong8, ulong8);
long16 __ovld __cnfn min(long16, long16);
ulong16 __ovld __cnfn min(ulong16, ulong16);
char2 __ovld __cnfn min(char2, char);
uchar2 __ovld __cnfn min(uchar2, uchar);
char3 __ovld __cnfn min(char3, char);
uchar3 __ovld __cnfn min(uchar3, uchar);
char4 __ovld __cnfn min(char4, char);
uchar4 __ovld __cnfn min(uchar4, uchar);
char8 __ovld __cnfn min(char8, char);
uchar8 __ovld __cnfn min(uchar8, uchar);
char16 __ovld __cnfn min(char16, char);
uchar16 __ovld __cnfn min(uchar16, uchar);
short2 __ovld __cnfn min(short2, short);
ushort2 __ovld __cnfn min(ushort2, ushort);
short3 __ovld __cnfn min(short3, short);
ushort3 __ovld __cnfn min(ushort3, ushort);
short4 __ovld __cnfn min(short4, short);
ushort4 __ovld __cnfn min(ushort4, ushort);
short8 __ovld __cnfn min(short8, short);
ushort8 __ovld __cnfn min(ushort8, ushort);
short16 __ovld __cnfn min(short16, short);
ushort16 __ovld __cnfn min(ushort16, ushort);
int2 __ovld __cnfn min(int2, int);
uint2 __ovld __cnfn min(uint2, uint);
int3 __ovld __cnfn min(int3, int);
uint3 __ovld __cnfn min(uint3, uint);
int4 __ovld __cnfn min(int4, int);
uint4 __ovld __cnfn min(uint4, uint);
int8 __ovld __cnfn min(int8, int);
uint8 __ovld __cnfn min(uint8, uint);
int16 __ovld __cnfn min(int16, int);
uint16 __ovld __cnfn min(uint16, uint);
long2 __ovld __cnfn min(long2, long);
ulong2 __ovld __cnfn min(ulong2, ulong);
long3 __ovld __cnfn min(long3, long);
ulong3 __ovld __cnfn min(ulong3, ulong);
long4 __ovld __cnfn min(long4, long);
ulong4 __ovld __cnfn min(ulong4, ulong);
long8 __ovld __cnfn min(long8, long);
ulong8 __ovld __cnfn min(ulong8, ulong);
long16 __ovld __cnfn min(long16, long);
ulong16 __ovld __cnfn min(ulong16, ulong);

/**
 * Computes x * y and returns the high half of the
 * product of x and y.
 */
char __ovld __cnfn mul_hi(char, char);
uchar __ovld __cnfn mul_hi(uchar, uchar);
char2 __ovld __cnfn mul_hi(char2, char2);
uchar2 __ovld __cnfn mul_hi(uchar2, uchar2);
char3 __ovld __cnfn mul_hi(char3, char3);
uchar3 __ovld __cnfn mul_hi(uchar3, uchar3);
char4 __ovld __cnfn mul_hi(char4, char4);
uchar4 __ovld __cnfn mul_hi(uchar4, uchar4);
char8 __ovld __cnfn mul_hi(char8, char8);
uchar8 __ovld __cnfn mul_hi(uchar8, uchar8);
char16 __ovld __cnfn mul_hi(char16, char16);
uchar16 __ovld __cnfn mul_hi(uchar16, uchar16);
short __ovld __cnfn mul_hi(short, short);
ushort __ovld __cnfn mul_hi(ushort, ushort);
short2 __ovld __cnfn mul_hi(short2, short2);
ushort2 __ovld __cnfn mul_hi(ushort2, ushort2);
short3 __ovld __cnfn mul_hi(short3, short3);
ushort3 __ovld __cnfn mul_hi(ushort3, ushort3);
short4 __ovld __cnfn mul_hi(short4, short4);
ushort4 __ovld __cnfn mul_hi(ushort4, ushort4);
short8 __ovld __cnfn mul_hi(short8, short8);
ushort8 __ovld __cnfn mul_hi(ushort8, ushort8);
short16 __ovld __cnfn mul_hi(short16, short16);
ushort16 __ovld __cnfn mul_hi(ushort16, ushort16);
int __ovld __cnfn mul_hi(int, int);
uint __ovld __cnfn mul_hi(uint, uint);
int2 __ovld __cnfn mul_hi(int2, int2);
uint2 __ovld __cnfn mul_hi(uint2, uint2);
int3 __ovld __cnfn mul_hi(int3, int3);
uint3 __ovld __cnfn mul_hi(uint3, uint3);
int4 __ovld __cnfn mul_hi(int4, int4);
uint4 __ovld __cnfn mul_hi(uint4, uint4);
int8 __ovld __cnfn mul_hi(int8, int8);
uint8 __ovld __cnfn mul_hi(uint8, uint8);
int16 __ovld __cnfn mul_hi(int16, int16);
uint16 __ovld __cnfn mul_hi(uint16, uint16);
long __ovld __cnfn mul_hi(long, long);
ulong __ovld __cnfn mul_hi(ulong, ulong);
long2 __ovld __cnfn mul_hi(long2, long2);
ulong2 __ovld __cnfn mul_hi(ulong2, ulong2);
long3 __ovld __cnfn mul_hi(long3, long3);
ulong3 __ovld __cnfn mul_hi(ulong3, ulong3);
long4 __ovld __cnfn mul_hi(long4, long4);
ulong4 __ovld __cnfn mul_hi(ulong4, ulong4);
long8 __ovld __cnfn mul_hi(long8, long8);
ulong8 __ovld __cnfn mul_hi(ulong8, ulong8);
long16 __ovld __cnfn mul_hi(long16, long16);
ulong16 __ovld __cnfn mul_hi(ulong16, ulong16);

/**
 * For each element in v, the bits are shifted left by
 * the number of bits given by the corresponding
 * element in i (subject to usual shift modulo rules
 * described in section 6.3). Bits shifted off the left
 * side of the element are shifted back in from the
 * right.
 */
char __ovld __cnfn rotate(char, char);
uchar __ovld __cnfn rotate(uchar, uchar);
char2 __ovld __cnfn rotate(char2, char2);
uchar2 __ovld __cnfn rotate(uchar2, uchar2);
char3 __ovld __cnfn rotate(char3, char3);
uchar3 __ovld __cnfn rotate(uchar3, uchar3);
char4 __ovld __cnfn rotate(char4, char4);
uchar4 __ovld __cnfn rotate(uchar4, uchar4);
char8 __ovld __cnfn rotate(char8, char8);
uchar8 __ovld __cnfn rotate(uchar8, uchar8);
char16 __ovld __cnfn rotate(char16, char16);
uchar16 __ovld __cnfn rotate(uchar16, uchar16);
short __ovld __cnfn rotate(short, short);
ushort __ovld __cnfn rotate(ushort, ushort);
short2 __ovld __cnfn rotate(short2, short2);
ushort2 __ovld __cnfn rotate(ushort2, ushort2);
short3 __ovld __cnfn rotate(short3, short3);
ushort3 __ovld __cnfn rotate(ushort3, ushort3);
short4 __ovld __cnfn rotate(short4, short4);
ushort4 __ovld __cnfn rotate(ushort4, ushort4);
short8 __ovld __cnfn rotate(short8, short8);
ushort8 __ovld __cnfn rotate(ushort8, ushort8);
short16 __ovld __cnfn rotate(short16, short16);
ushort16 __ovld __cnfn rotate(ushort16, ushort16);
int __ovld __cnfn rotate(int, int);
uint __ovld __cnfn rotate(uint, uint);
int2 __ovld __cnfn rotate(int2, int2);
uint2 __ovld __cnfn rotate(uint2, uint2);
int3 __ovld __cnfn rotate(int3, int3);
uint3 __ovld __cnfn rotate(uint3, uint3);
int4 __ovld __cnfn rotate(int4, int4);
uint4 __ovld __cnfn rotate(uint4, uint4);
int8 __ovld __cnfn rotate(int8, int8);
uint8 __ovld __cnfn rotate(uint8, uint8);
int16 __ovld __cnfn rotate(int16, int16);
uint16 __ovld __cnfn rotate(uint16, uint16);
long __ovld __cnfn rotate(long, long);
ulong __ovld __cnfn rotate(ulong, ulong);
long2 __ovld __cnfn rotate(long2, long2);
ulong2 __ovld __cnfn rotate(ulong2, ulong2);
long3 __ovld __cnfn rotate(long3, long3);
ulong3 __ovld __cnfn rotate(ulong3, ulong3);
long4 __ovld __cnfn rotate(long4, long4);
ulong4 __ovld __cnfn rotate(ulong4, ulong4);
long8 __ovld __cnfn rotate(long8, long8);
ulong8 __ovld __cnfn rotate(ulong8, ulong8);
long16 __ovld __cnfn rotate(long16, long16);
ulong16 __ovld __cnfn rotate(ulong16, ulong16);

/**
 * Returns x - y and saturates the result.
 */
char __ovld __cnfn sub_sat(char, char);
uchar __ovld __cnfn sub_sat(uchar, uchar);
char2 __ovld __cnfn sub_sat(char2, char2);
uchar2 __ovld __cnfn sub_sat(uchar2, uchar2);
char3 __ovld __cnfn sub_sat(char3, char3);
uchar3 __ovld __cnfn sub_sat(uchar3, uchar3);
char4 __ovld __cnfn sub_sat(char4, char4);
uchar4 __ovld __cnfn sub_sat(uchar4, uchar4);
char8 __ovld __cnfn sub_sat(char8, char8);
uchar8 __ovld __cnfn sub_sat(uchar8, uchar8);
char16 __ovld __cnfn sub_sat(char16, char16);
uchar16 __ovld __cnfn sub_sat(uchar16, uchar16);
short __ovld __cnfn sub_sat(short, short);
ushort __ovld __cnfn sub_sat(ushort, ushort);
short2 __ovld __cnfn sub_sat(short2, short2);
ushort2 __ovld __cnfn sub_sat(ushort2, ushort2);
short3 __ovld __cnfn sub_sat(short3, short3);
ushort3 __ovld __cnfn sub_sat(ushort3, ushort3);
short4 __ovld __cnfn sub_sat(short4, short4);
ushort4 __ovld __cnfn sub_sat(ushort4, ushort4);
short8 __ovld __cnfn sub_sat(short8, short8);
ushort8 __ovld __cnfn sub_sat(ushort8, ushort8);
short16 __ovld __cnfn sub_sat(short16, short16);
ushort16 __ovld __cnfn sub_sat(ushort16, ushort16);
int __ovld __cnfn sub_sat(int, int);
uint __ovld __cnfn sub_sat(uint, uint);
int2 __ovld __cnfn sub_sat(int2, int2);
uint2 __ovld __cnfn sub_sat(uint2, uint2);
int3 __ovld __cnfn sub_sat(int3, int3);
uint3 __ovld __cnfn sub_sat(uint3, uint3);
int4 __ovld __cnfn sub_sat(int4, int4);
uint4 __ovld __cnfn sub_sat(uint4, uint4);
int8 __ovld __cnfn sub_sat(int8, int8);
uint8 __ovld __cnfn sub_sat(uint8, uint8);
int16 __ovld __cnfn sub_sat(int16, int16);
uint16 __ovld __cnfn sub_sat(uint16, uint16);
long __ovld __cnfn sub_sat(long, long);
ulong __ovld __cnfn sub_sat(ulong, ulong);
long2 __ovld __cnfn sub_sat(long2, long2);
ulong2 __ovld __cnfn sub_sat(ulong2, ulong2);
long3 __ovld __cnfn sub_sat(long3, long3);
ulong3 __ovld __cnfn sub_sat(ulong3, ulong3);
long4 __ovld __cnfn sub_sat(long4, long4);
ulong4 __ovld __cnfn sub_sat(ulong4, ulong4);
long8 __ovld __cnfn sub_sat(long8, long8);
ulong8 __ovld __cnfn sub_sat(ulong8, ulong8);
long16 __ovld __cnfn sub_sat(long16, long16);
ulong16 __ovld __cnfn sub_sat(ulong16, ulong16);

/**
 * result[i] = ((short)hi[i] << 8) | lo[i]
 * result[i] = ((ushort)hi[i] << 8) | lo[i]
 */
short __ovld __cnfn upsample(char, uchar);
ushort __ovld __cnfn upsample(uchar, uchar);
short2 __ovld __cnfn upsample(char2, uchar2);
short3 __ovld __cnfn upsample(char3, uchar3);
short4 __ovld __cnfn upsample(char4, uchar4);
short8 __ovld __cnfn upsample(char8, uchar8);
short16 __ovld __cnfn upsample(char16, uchar16);
ushort2 __ovld __cnfn upsample(uchar2, uchar2);
ushort3 __ovld __cnfn upsample(uchar3, uchar3);
ushort4 __ovld __cnfn upsample(uchar4, uchar4);
ushort8 __ovld __cnfn upsample(uchar8, uchar8);
ushort16 __ovld __cnfn upsample(uchar16, uchar16);

/**
 * result[i] = ((int)hi[i] << 16) | lo[i]
 * result[i] = ((uint)hi[i] << 16) | lo[i]
 */
int __ovld __cnfn upsample(short, ushort);
uint __ovld __cnfn upsample(ushort, ushort);
int2 __ovld __cnfn upsample(short2, ushort2);
int3 __ovld __cnfn upsample(short3, ushort3);
int4 __ovld __cnfn upsample(short4, ushort4);
int8 __ovld __cnfn upsample(short8, ushort8);
int16 __ovld __cnfn upsample(short16, ushort16);
uint2 __ovld __cnfn upsample(ushort2, ushort2);
uint3 __ovld __cnfn upsample(ushort3, ushort3);
uint4 __ovld __cnfn upsample(ushort4, ushort4);
uint8 __ovld __cnfn upsample(ushort8, ushort8);
uint16 __ovld __cnfn upsample(ushort16, ushort16);
/**
 * result[i] = ((long)hi[i] << 32) | lo[i]
 * result[i] = ((ulong)hi[i] << 32) | lo[i]
 */
long __ovld __cnfn upsample(int, uint);
ulong __ovld __cnfn upsample(uint, uint);
long2 __ovld __cnfn upsample(int2, uint2);
long3 __ovld __cnfn upsample(int3, uint3);
long4 __ovld __cnfn upsample(int4, uint4);
long8 __ovld __cnfn upsample(int8, uint8);
long16 __ovld __cnfn upsample(int16, uint16);
ulong2 __ovld __cnfn upsample(uint2, uint2);
ulong3 __ovld __cnfn upsample(uint3, uint3);
ulong4 __ovld __cnfn upsample(uint4, uint4);
ulong8 __ovld __cnfn upsample(uint8, uint8);
ulong16 __ovld __cnfn upsample(uint16, uint16);

/*
 * popcount(x): returns the number of set bit in x
 */
#if defined(__OPENCL_CPP_VERSION__) || (__OPENCL_C_VERSION__ >= CL_VERSION_1_2)
char __ovld __cnfn popcount(char);
uchar __ovld __cnfn popcount(uchar);
char2 __ovld __cnfn popcount(char2);
uchar2 __ovld __cnfn popcount(uchar2);
char3 __ovld __cnfn popcount(char3);
uchar3 __ovld __cnfn popcount(uchar3);
char4 __ovld __cnfn popcount(char4);
uchar4 __ovld __cnfn popcount(uchar4);
char8 __ovld __cnfn popcount(char8);
uchar8 __ovld __cnfn popcount(uchar8);
char16 __ovld __cnfn popcount(char16);
uchar16 __ovld __cnfn popcount(uchar16);
short __ovld __cnfn popcount(short);
ushort __ovld __cnfn popcount(ushort);
short2 __ovld __cnfn popcount(short2);
ushort2 __ovld __cnfn popcount(ushort2);
short3 __ovld __cnfn popcount(short3);
ushort3 __ovld __cnfn popcount(ushort3);
short4 __ovld __cnfn popcount(short4);
ushort4 __ovld __cnfn popcount(ushort4);
short8 __ovld __cnfn popcount(short8);
ushort8 __ovld __cnfn popcount(ushort8);
short16 __ovld __cnfn popcount(short16);
ushort16 __ovld __cnfn popcount(ushort16);
int __ovld __cnfn popcount(int);
uint __ovld __cnfn popcount(uint);
int2 __ovld __cnfn popcount(int2);
uint2 __ovld __cnfn popcount(uint2);
int3 __ovld __cnfn popcount(int3);
uint3 __ovld __cnfn popcount(uint3);
int4 __ovld __cnfn popcount(int4);
uint4 __ovld __cnfn popcount(uint4);
int8 __ovld __cnfn popcount(int8);
uint8 __ovld __cnfn popcount(uint8);
int16 __ovld __cnfn popcount(int16);
uint16 __ovld __cnfn popcount(uint16);
long __ovld __cnfn popcount(long);
ulong __ovld __cnfn popcount(ulong);
long2 __ovld __cnfn popcount(long2);
ulong2 __ovld __cnfn popcount(ulong2);
long3 __ovld __cnfn popcount(long3);
ulong3 __ovld __cnfn popcount(ulong3);
long4 __ovld __cnfn popcount(long4);
ulong4 __ovld __cnfn popcount(ulong4);
long8 __ovld __cnfn popcount(long8);
ulong8 __ovld __cnfn popcount(ulong8);
long16 __ovld __cnfn popcount(long16);
ulong16 __ovld __cnfn popcount(ulong16);
#endif // defined(__OPENCL_CPP_VERSION__) || (__OPENCL_C_VERSION__ >= CL_VERSION_1_2)

/**
 * Multiply two 24-bit integer values x and y and add
 * the 32-bit integer result to the 32-bit integer z.
 * Refer to definition of mul24 to see how the 24-bit
 * integer multiplication is performed.
 */
int __ovld __cnfn mad24(int, int, int);
uint __ovld __cnfn mad24(uint, uint, uint);
int2 __ovld __cnfn mad24(int2, int2, int2);
uint2 __ovld __cnfn mad24(uint2, uint2, uint2);
int3 __ovld __cnfn mad24(int3, int3, int3);
uint3 __ovld __cnfn mad24(uint3, uint3, uint3);
int4 __ovld __cnfn mad24(int4, int4, int4);
uint4 __ovld __cnfn mad24(uint4, uint4, uint4);
int8 __ovld __cnfn mad24(int8, int8, int8);
uint8 __ovld __cnfn mad24(uint8, uint8, uint8);
int16 __ovld __cnfn mad24(int16, int16, int16);
uint16 __ovld __cnfn mad24(uint16, uint16, uint16);

/**
 * Multiply two 24-bit integer values x and y. x and y
 * are 32-bit integers but only the low 24-bits are used
 * to perform the multiplication. mul24 should only
 * be used when values in x and y are in the range [-
 * 2^23, 2^23-1] if x and y are signed integers and in the
 * range [0, 2^24-1] if x and y are unsigned integers. If
 * x and y are not in this range, the multiplication
 * result is implementation-defined.
 */
int __ovld __cnfn mul24(int, int);
uint __ovld __cnfn mul24(uint, uint);
int2 __ovld __cnfn mul24(int2, int2);
uint2 __ovld __cnfn mul24(uint2, uint2);
int3 __ovld __cnfn mul24(int3, int3);
uint3 __ovld __cnfn mul24(uint3, uint3);
int4 __ovld __cnfn mul24(int4, int4);
uint4 __ovld __cnfn mul24(uint4, uint4);
int8 __ovld __cnfn mul24(int8, int8);
uint8 __ovld __cnfn mul24(uint8, uint8);
int16 __ovld __cnfn mul24(int16, int16);
uint16 __ovld __cnfn mul24(uint16, uint16);

// OpenCL v1.1 s6.11.4, v1.2 s6.12.4, v2.0 s6.13.4 - Common Functions

/**
 * Returns fmin(fmax(x, minval), maxval).
 * Results are undefined if minval > maxval.
 */
float __ovld __cnfn clamp(float, float, float);
float2 __ovld __cnfn clamp(float2, float2, float2);
float3 __ovld __cnfn clamp(float3, float3, float3);
float4 __ovld __cnfn clamp(float4, float4, float4);
float8 __ovld __cnfn clamp(float8, float8, float8);
float16 __ovld __cnfn clamp(float16, float16, float16);
float2 __ovld __cnfn clamp(float2, float, float);
float3 __ovld __cnfn clamp(float3, float, float);
float4 __ovld __cnfn clamp(float4, float, float);
float8 __ovld __cnfn clamp(float8, float, float);
float16 __ovld __cnfn clamp(float16, float, float);
#ifdef cl_khr_fp64
double __ovld __cnfn clamp(double, double, double);
double2 __ovld __cnfn clamp(double2, double2, double2);
double3 __ovld __cnfn clamp(double3, double3, double3);
double4 __ovld __cnfn clamp(double4, double4, double4);
double8 __ovld __cnfn clamp(double8, double8, double8);
double16 __ovld __cnfn clamp(double16, double16, double16);
double2 __ovld __cnfn clamp(double2, double, double);
double3 __ovld __cnfn clamp(double3, double, double);
double4 __ovld __cnfn clamp(double4, double, double);
double8 __ovld __cnfn clamp(double8, double, double);
double16 __ovld __cnfn clamp(double16, double, double);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn clamp(half, half, half);
half2 __ovld __cnfn clamp(half2, half2, half2);
half3 __ovld __cnfn clamp(half3, half3, half3);
half4 __ovld __cnfn clamp(half4, half4, half4);
half8 __ovld __cnfn clamp(half8, half8, half8);
half16 __ovld __cnfn clamp(half16, half16, half16);
half2 __ovld __cnfn clamp(half2, half, half);
half3 __ovld __cnfn clamp(half3, half, half);
half4 __ovld __cnfn clamp(half4, half, half);
half8 __ovld __cnfn clamp(half8, half, half);
half16 __ovld __cnfn clamp(half16, half, half);
#endif //cl_khr_fp16

/**
 * Converts radians to degrees, i.e. (180 / PI) *
 * radians.
 */
float __ovld __cnfn degrees(float);
float2 __ovld __cnfn degrees(float2);
float3 __ovld __cnfn degrees(float3);
float4 __ovld __cnfn degrees(float4);
float8 __ovld __cnfn degrees(float8);
float16 __ovld __cnfn degrees(float16);
#ifdef cl_khr_fp64
double __ovld __cnfn degrees(double);
double2 __ovld __cnfn degrees(double2);
double3 __ovld __cnfn degrees(double3);
double4 __ovld __cnfn degrees(double4);
double8 __ovld __cnfn degrees(double8);
double16 __ovld __cnfn degrees(double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn degrees(half);
half2 __ovld __cnfn degrees(half2);
half3 __ovld __cnfn degrees(half3);
half4 __ovld __cnfn degrees(half4);
half8 __ovld __cnfn degrees(half8);
half16 __ovld __cnfn degrees(half16);
#endif //cl_khr_fp16

/**
 * Returns y if x < y, otherwise it returns x. If x and y
 * are infinite or NaN, the return values are undefined.
 */
float __ovld __cnfn max(float, float);
float2 __ovld __cnfn max(float2, float2);
float3 __ovld __cnfn max(float3, float3);
float4 __ovld __cnfn max(float4, float4);
float8 __ovld __cnfn max(float8, float8);
float16 __ovld __cnfn max(float16, float16);
float2 __ovld __cnfn max(float2, float);
float3 __ovld __cnfn max(float3, float);
float4 __ovld __cnfn max(float4, float);
float8 __ovld __cnfn max(float8, float);
float16 __ovld __cnfn max(float16, float);
#ifdef cl_khr_fp64
double __ovld __cnfn max(double, double);
double2 __ovld __cnfn max(double2, double2);
double3 __ovld __cnfn max(double3, double3);
double4 __ovld __cnfn max(double4, double4);
double8 __ovld __cnfn max(double8, double8);
double16 __ovld __cnfn max(double16, double16);
double2 __ovld __cnfn max(double2, double);
double3 __ovld __cnfn max(double3, double);
double4 __ovld __cnfn max(double4, double);
double8 __ovld __cnfn max(double8, double);
double16 __ovld __cnfn max(double16, double);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn max(half, half);
half2 __ovld __cnfn max(half2, half2);
half3 __ovld __cnfn max(half3, half3);
half4 __ovld __cnfn max(half4, half4);
half8 __ovld __cnfn max(half8, half8);
half16 __ovld __cnfn max(half16, half16);
half2 __ovld __cnfn max(half2, half);
half3 __ovld __cnfn max(half3, half);
half4 __ovld __cnfn max(half4, half);
half8 __ovld __cnfn max(half8, half);
half16 __ovld __cnfn max(half16, half);
#endif //cl_khr_fp16

/**
 * Returns y if y < x, otherwise it returns x. If x and y
 * are infinite or NaN, the return values are undefined.
 */
float __ovld __cnfn min(float, float);
float2 __ovld __cnfn min(float2, float2);
float3 __ovld __cnfn min(float3, float3);
float4 __ovld __cnfn min(float4, float4);
float8 __ovld __cnfn min(float8, float8);
float16 __ovld __cnfn min(float16, float16);
float2 __ovld __cnfn min(float2, float);
float3 __ovld __cnfn min(float3, float);
float4 __ovld __cnfn min(float4, float);
float8 __ovld __cnfn min(float8, float);
float16 __ovld __cnfn min(float16, float);
#ifdef cl_khr_fp64
double __ovld __cnfn min(double, double);
double2 __ovld __cnfn min(double2, double2);
double3 __ovld __cnfn min(double3, double3);
double4 __ovld __cnfn min(double4, double4);
double8 __ovld __cnfn min(double8, double8);
double16 __ovld __cnfn min(double16, double16);
double2 __ovld __cnfn min(double2, double);
double3 __ovld __cnfn min(double3, double);
double4 __ovld __cnfn min(double4, double);
double8 __ovld __cnfn min(double8, double);
double16 __ovld __cnfn min(double16, double);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn min(half, half);
half2 __ovld __cnfn min(half2, half2);
half3 __ovld __cnfn min(half3, half3);
half4 __ovld __cnfn min(half4, half4);
half8 __ovld __cnfn min(half8, half8);
half16 __ovld __cnfn min(half16, half16);
half2 __ovld __cnfn min(half2, half);
half3 __ovld __cnfn min(half3, half);
half4 __ovld __cnfn min(half4, half);
half8 __ovld __cnfn min(half8, half);
half16 __ovld __cnfn min(half16, half);
#endif //cl_khr_fp16

/**
 * Returns the linear blend of x & y implemented as:
 * x + (y - x) * a
 * a must be a value in the range 0.0 ... 1.0. If a is not
 * in the range 0.0 ... 1.0, the return values are
 * undefined.
 */
float __ovld __cnfn mix(float, float, float);
float2 __ovld __cnfn mix(float2, float2, float2);
float3 __ovld __cnfn mix(float3, float3, float3);
float4 __ovld __cnfn mix(float4, float4, float4);
float8 __ovld __cnfn mix(float8, float8, float8);
float16 __ovld __cnfn mix(float16, float16, float16);
float2 __ovld __cnfn mix(float2, float2, float);
float3 __ovld __cnfn mix(float3, float3, float);
float4 __ovld __cnfn mix(float4, float4, float);
float8 __ovld __cnfn mix(float8, float8, float);
float16 __ovld __cnfn mix(float16, float16, float);
#ifdef cl_khr_fp64
double __ovld __cnfn mix(double, double, double);
double2 __ovld __cnfn mix(double2, double2, double2);
double3 __ovld __cnfn mix(double3, double3, double3);
double4 __ovld __cnfn mix(double4, double4, double4);
double8 __ovld __cnfn mix(double8, double8, double8);
double16 __ovld __cnfn mix(double16, double16, double16);
double2 __ovld __cnfn mix(double2, double2, double);
double3 __ovld __cnfn mix(double3, double3, double);
double4 __ovld __cnfn mix(double4, double4, double);
double8 __ovld __cnfn mix(double8, double8, double);
double16 __ovld __cnfn mix(double16, double16, double);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn mix(half, half, half);
half2 __ovld __cnfn mix(half2, half2, half2);
half3 __ovld __cnfn mix(half3, half3, half3);
half4 __ovld __cnfn mix(half4, half4, half4);
half8 __ovld __cnfn mix(half8, half8, half8);
half16 __ovld __cnfn mix(half16, half16, half16);
half2 __ovld __cnfn mix(half2, half2, half);
half3 __ovld __cnfn mix(half3, half3, half);
half4 __ovld __cnfn mix(half4, half4, half);
half8 __ovld __cnfn mix(half8, half8, half);
half16 __ovld __cnfn mix(half16, half16, half);
#endif //cl_khr_fp16

/**
 * Converts degrees to radians, i.e. (PI / 180) *
 * degrees.
 */
float __ovld __cnfn radians(float);
float2 __ovld __cnfn radians(float2);
float3 __ovld __cnfn radians(float3);
float4 __ovld __cnfn radians(float4);
float8 __ovld __cnfn radians(float8);
float16 __ovld __cnfn radians(float16);
#ifdef cl_khr_fp64
double __ovld __cnfn radians(double);
double2 __ovld __cnfn radians(double2);
double3 __ovld __cnfn radians(double3);
double4 __ovld __cnfn radians(double4);
double8 __ovld __cnfn radians(double8);
double16 __ovld __cnfn radians(double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn radians(half);
half2 __ovld __cnfn radians(half2);
half3 __ovld __cnfn radians(half3);
half4 __ovld __cnfn radians(half4);
half8 __ovld __cnfn radians(half8);
half16 __ovld __cnfn radians(half16);
#endif //cl_khr_fp16

/**
 * Returns 0.0 if x < edge, otherwise it returns 1.0.
 */
float __ovld __cnfn step(float, float);
float2 __ovld __cnfn step(float2, float2);
float3 __ovld __cnfn step(float3, float3);
float4 __ovld __cnfn step(float4, float4);
float8 __ovld __cnfn step(float8, float8);
float16 __ovld __cnfn step(float16, float16);
float2 __ovld __cnfn step(float, float2);
float3 __ovld __cnfn step(float, float3);
float4 __ovld __cnfn step(float, float4);
float8 __ovld __cnfn step(float, float8);
float16 __ovld __cnfn step(float, float16);
#ifdef cl_khr_fp64
double __ovld __cnfn step(double, double);
double2 __ovld __cnfn step(double2, double2);
double3 __ovld __cnfn step(double3, double3);
double4 __ovld __cnfn step(double4, double4);
double8 __ovld __cnfn step(double8, double8);
double16 __ovld __cnfn step(double16, double16);
double2 __ovld __cnfn step(double, double2);
double3 __ovld __cnfn step(double, double3);
double4 __ovld __cnfn step(double, double4);
double8 __ovld __cnfn step(double, double8);
double16 __ovld __cnfn step(double, double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn step(half, half);
half2 __ovld __cnfn step(half2, half2);
half3 __ovld __cnfn step(half3, half3);
half4 __ovld __cnfn step(half4, half4);
half8 __ovld __cnfn step(half8, half8);
half16 __ovld __cnfn step(half16, half16);
half2 __ovld __cnfn step(half, half2);
half3 __ovld __cnfn step(half, half3);
half4 __ovld __cnfn step(half, half4);
half8 __ovld __cnfn step(half, half8);
half16 __ovld __cnfn step(half, half16);
#endif //cl_khr_fp16

/**
 * Returns 0.0 if x <= edge0 and 1.0 if x >= edge1 and
 * performs smooth Hermite interpolation between 0
 * and 1when edge0 < x < edge1. This is useful in
 * cases where you would want a threshold function
 * with a smooth transition.
 * This is equivalent to:
 * gentype t;
 * t = clamp ((x - edge0) / (edge1 - edge0), 0, 1);
 * return t * t * (3 - 2 * t);
 * Results are undefined if edge0 >= edge1 or if x,
 * edge0 or edge1 is a NaN.
 */
float __ovld __cnfn smoothstep(float, float, float);
float2 __ovld __cnfn smoothstep(float2, float2, float2);
float3 __ovld __cnfn smoothstep(float3, float3, float3);
float4 __ovld __cnfn smoothstep(float4, float4, float4);
float8 __ovld __cnfn smoothstep(float8, float8, float8);
float16 __ovld __cnfn smoothstep(float16, float16, float16);
float2 __ovld __cnfn smoothstep(float, float, float2);
float3 __ovld __cnfn smoothstep(float, float, float3);
float4 __ovld __cnfn smoothstep(float, float, float4);
float8 __ovld __cnfn smoothstep(float, float, float8);
float16 __ovld __cnfn smoothstep(float, float, float16);
#ifdef cl_khr_fp64
double __ovld __cnfn smoothstep(double, double, double);
double2 __ovld __cnfn smoothstep(double2, double2, double2);
double3 __ovld __cnfn smoothstep(double3, double3, double3);
double4 __ovld __cnfn smoothstep(double4, double4, double4);
double8 __ovld __cnfn smoothstep(double8, double8, double8);
double16 __ovld __cnfn smoothstep(double16, double16, double16);
double2 __ovld __cnfn smoothstep(double, double, double2);
double3 __ovld __cnfn smoothstep(double, double, double3);
double4 __ovld __cnfn smoothstep(double, double, double4);
double8 __ovld __cnfn smoothstep(double, double, double8);
double16 __ovld __cnfn smoothstep(double, double, double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn smoothstep(half, half, half);
half2 __ovld __cnfn smoothstep(half2, half2, half2);
half3 __ovld __cnfn smoothstep(half3, half3, half3);
half4 __ovld __cnfn smoothstep(half4, half4, half4);
half8 __ovld __cnfn smoothstep(half8, half8, half8);
half16 __ovld __cnfn smoothstep(half16, half16, half16);
half2 __ovld __cnfn smoothstep(half, half, half2);
half3 __ovld __cnfn smoothstep(half, half, half3);
half4 __ovld __cnfn smoothstep(half, half, half4);
half8 __ovld __cnfn smoothstep(half, half, half8);
half16 __ovld __cnfn smoothstep(half, half, half16);
#endif //cl_khr_fp16

/**
 * Returns 1.0 if x > 0, -0.0 if x = -0.0, +0.0 if x =
 * +0.0, or -1.0 if x < 0. Returns 0.0 if x is a NaN.
 */
float __ovld __cnfn sign(float);
float2 __ovld __cnfn sign(float2);
float3 __ovld __cnfn sign(float3);
float4 __ovld __cnfn sign(float4);
float8 __ovld __cnfn sign(float8);
float16 __ovld __cnfn sign(float16);
#ifdef cl_khr_fp64
double __ovld __cnfn sign(double);
double2 __ovld __cnfn sign(double2);
double3 __ovld __cnfn sign(double3);
double4 __ovld __cnfn sign(double4);
double8 __ovld __cnfn sign(double8);
double16 __ovld __cnfn sign(double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn sign(half);
half2 __ovld __cnfn sign(half2);
half3 __ovld __cnfn sign(half3);
half4 __ovld __cnfn sign(half4);
half8 __ovld __cnfn sign(half8);
half16 __ovld __cnfn sign(half16);
#endif //cl_khr_fp16

// OpenCL v1.1 s6.11.5, v1.2 s6.12.5, v2.0 s6.13.5 - Geometric Functions

/**
 * Returns the cross product of p0.xyz and p1.xyz. The
 * w component of float4 result returned will be 0.0.
 */
float4 __ovld __cnfn cross(float4, float4);
float3 __ovld __cnfn cross(float3, float3);
#ifdef cl_khr_fp64
double4 __ovld __cnfn cross(double4, double4);
double3 __ovld __cnfn cross(double3, double3);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half4 __ovld __cnfn cross(half4, half4);
half3 __ovld __cnfn cross(half3, half3);
#endif //cl_khr_fp16

/**
 * Compute dot product.
 */
float __ovld __cnfn dot(float, float);
float __ovld __cnfn dot(float2, float2);
float __ovld __cnfn dot(float3, float3);
float __ovld __cnfn dot(float4, float4);
#ifdef cl_khr_fp64
double __ovld __cnfn dot(double, double);
double __ovld __cnfn dot(double2, double2);
double __ovld __cnfn dot(double3, double3);
double __ovld __cnfn dot(double4, double4);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn dot(half, half);
half __ovld __cnfn dot(half2, half2);
half __ovld __cnfn dot(half3, half3);
half __ovld __cnfn dot(half4, half4);
#endif //cl_khr_fp16

/**
 * Returns the distance between p0 and p1. This is
 * calculated as length(p0 - p1).
 */
float __ovld __cnfn distance(float, float);
float __ovld __cnfn distance(float2, float2);
float __ovld __cnfn distance(float3, float3);
float __ovld __cnfn distance(float4, float4);
#ifdef cl_khr_fp64
double __ovld __cnfn distance(double, double);
double __ovld __cnfn distance(double2, double2);
double __ovld __cnfn distance(double3, double3);
double __ovld __cnfn distance(double4, double4);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn distance(half, half);
half __ovld __cnfn distance(half2, half2);
half __ovld __cnfn distance(half3, half3);
half __ovld __cnfn distance(half4, half4);
#endif //cl_khr_fp16

/**
 * Return the length of vector p, i.e.,
 * sqrt(p.x2 + p.y 2 + ...)
 */
float __ovld __cnfn length(float);
float __ovld __cnfn length(float2);
float __ovld __cnfn length(float3);
float __ovld __cnfn length(float4);
#ifdef cl_khr_fp64
double __ovld __cnfn length(double);
double __ovld __cnfn length(double2);
double __ovld __cnfn length(double3);
double __ovld __cnfn length(double4);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn length(half);
half __ovld __cnfn length(half2);
half __ovld __cnfn length(half3);
half __ovld __cnfn length(half4);
#endif //cl_khr_fp16

/**
 * Returns a vector in the same direction as p but with a
 * length of 1.
 */
float __ovld __cnfn normalize(float);
float2 __ovld __cnfn normalize(float2);
float3 __ovld __cnfn normalize(float3);
float4 __ovld __cnfn normalize(float4);
#ifdef cl_khr_fp64
double __ovld __cnfn normalize(double);
double2 __ovld __cnfn normalize(double2);
double3 __ovld __cnfn normalize(double3);
double4 __ovld __cnfn normalize(double4);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn normalize(half);
half2 __ovld __cnfn normalize(half2);
half3 __ovld __cnfn normalize(half3);
half4 __ovld __cnfn normalize(half4);
#endif //cl_khr_fp16

/**
 * Returns fast_length(p0 - p1).
 */
float __ovld __cnfn fast_distance(float, float);
float __ovld __cnfn fast_distance(float2, float2);
float __ovld __cnfn fast_distance(float3, float3);
float __ovld __cnfn fast_distance(float4, float4);

/**
 * Returns the length of vector p computed as:
 * half_sqrt(p.x2 + p.y2 + ...)
 */
float __ovld __cnfn fast_length(float);
float __ovld __cnfn fast_length(float2);
float __ovld __cnfn fast_length(float3);
float __ovld __cnfn fast_length(float4);

/**
 * Returns a vector in the same direction as p but with a
 * length of 1. fast_normalize is computed as:
 * p * half_rsqrt (p.x^2 + p.y^2 + ... )
 * The result shall be within 8192 ulps error from the
 * infinitely precise result of
 * if (all(p == 0.0f))
 * result = p;
 * else
 * result = p / sqrt (p.x^2 + p.y^2 + ...);
 * with the following exceptions:
 * 1) If the sum of squares is greater than FLT_MAX
 * then the value of the floating-point values in the
 * result vector are undefined.
 * 2) If the sum of squares is less than FLT_MIN then
 * the implementation may return back p.
 * 3) If the device is in "denorms are flushed to zero"
 * mode, individual operand elements with magnitude
 * less than sqrt(FLT_MIN) may be flushed to zero
 * before proceeding with the calculation.
 */
float __ovld __cnfn fast_normalize(float);
float2 __ovld __cnfn fast_normalize(float2);
float3 __ovld __cnfn fast_normalize(float3);
float4 __ovld __cnfn fast_normalize(float4);

// OpenCL v1.1 s6.11.6, v1.2 s6.12.6, v2.0 s6.13.6 - Relational Functions

/**
 * intn isequal (floatn x, floatn y)
 * Returns the component-wise compare of x == y.
 */
int __ovld __cnfn isequal(float, float);
int2 __ovld __cnfn isequal(float2, float2);
int3 __ovld __cnfn isequal(float3, float3);
int4 __ovld __cnfn isequal(float4, float4);
int8 __ovld __cnfn isequal(float8, float8);
int16 __ovld __cnfn isequal(float16, float16);
#ifdef cl_khr_fp64
int __ovld __cnfn isequal(double, double);
long2 __ovld __cnfn isequal(double2, double2);
long3 __ovld __cnfn isequal(double3, double3);
long4 __ovld __cnfn isequal(double4, double4);
long8 __ovld __cnfn isequal(double8, double8);
long16 __ovld __cnfn isequal(double16, double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
int __ovld __cnfn isequal(half, half);
short2 __ovld __cnfn isequal(half2, half2);
short3 __ovld __cnfn isequal(half3, half3);
short4 __ovld __cnfn isequal(half4, half4);
short8 __ovld __cnfn isequal(half8, half8);
short16 __ovld __cnfn isequal(half16, half16);
#endif //cl_khr_fp16

/**
 * Returns the component-wise compare of x != y.
 */
int __ovld __cnfn isnotequal(float, float);
int2 __ovld __cnfn isnotequal(float2, float2);
int3 __ovld __cnfn isnotequal(float3, float3);
int4 __ovld __cnfn isnotequal(float4, float4);
int8 __ovld __cnfn isnotequal(float8, float8);
int16 __ovld __cnfn isnotequal(float16, float16);
#ifdef cl_khr_fp64
int __ovld __cnfn isnotequal(double, double);
long2 __ovld __cnfn isnotequal(double2, double2);
long3 __ovld __cnfn isnotequal(double3, double3);
long4 __ovld __cnfn isnotequal(double4, double4);
long8 __ovld __cnfn isnotequal(double8, double8);
long16 __ovld __cnfn isnotequal(double16, double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
int __ovld __cnfn isnotequal(half, half);
short2 __ovld __cnfn isnotequal(half2, half2);
short3 __ovld __cnfn isnotequal(half3, half3);
short4 __ovld __cnfn isnotequal(half4, half4);
short8 __ovld __cnfn isnotequal(half8, half8);
short16 __ovld __cnfn isnotequal(half16, half16);
#endif //cl_khr_fp16

/**
 * Returns the component-wise compare of x > y.
 */
int __ovld __cnfn isgreater(float, float);
int2 __ovld __cnfn isgreater(float2, float2);
int3 __ovld __cnfn isgreater(float3, float3);
int4 __ovld __cnfn isgreater(float4, float4);
int8 __ovld __cnfn isgreater(float8, float8);
int16 __ovld __cnfn isgreater(float16, float16);
#ifdef cl_khr_fp64
int __ovld __cnfn isgreater(double, double);
long2 __ovld __cnfn isgreater(double2, double2);
long3 __ovld __cnfn isgreater(double3, double3);
long4 __ovld __cnfn isgreater(double4, double4);
long8 __ovld __cnfn isgreater(double8, double8);
long16 __ovld __cnfn isgreater(double16, double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
int __ovld __cnfn isgreater(half, half);
short2 __ovld __cnfn isgreater(half2, half2);
short3 __ovld __cnfn isgreater(half3, half3);
short4 __ovld __cnfn isgreater(half4, half4);
short8 __ovld __cnfn isgreater(half8, half8);
short16 __ovld __cnfn isgreater(half16, half16);
#endif //cl_khr_fp16

/**
 * Returns the component-wise compare of x >= y.
 */
int __ovld __cnfn isgreaterequal(float, float);
int2 __ovld __cnfn isgreaterequal(float2, float2);
int3 __ovld __cnfn isgreaterequal(float3, float3);
int4 __ovld __cnfn isgreaterequal(float4, float4);
int8 __ovld __cnfn isgreaterequal(float8, float8);
int16 __ovld __cnfn isgreaterequal(float16, float16);
#ifdef cl_khr_fp64
int __ovld __cnfn isgreaterequal(double, double);
long2 __ovld __cnfn isgreaterequal(double2, double2);
long3 __ovld __cnfn isgreaterequal(double3, double3);
long4 __ovld __cnfn isgreaterequal(double4, double4);
long8 __ovld __cnfn isgreaterequal(double8, double8);
long16 __ovld __cnfn isgreaterequal(double16, double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
int __ovld __cnfn isgreaterequal(half, half);
short2 __ovld __cnfn isgreaterequal(half2, half2);
short3 __ovld __cnfn isgreaterequal(half3, half3);
short4 __ovld __cnfn isgreaterequal(half4, half4);
short8 __ovld __cnfn isgreaterequal(half8, half8);
short16 __ovld __cnfn isgreaterequal(half16, half16);
#endif //cl_khr_fp16

/**
 * Returns the component-wise compare of x < y.
 */
int __ovld __cnfn isless(float, float);
int2 __ovld __cnfn isless(float2, float2);
int3 __ovld __cnfn isless(float3, float3);
int4 __ovld __cnfn isless(float4, float4);
int8 __ovld __cnfn isless(float8, float8);
int16 __ovld __cnfn isless(float16, float16);
#ifdef cl_khr_fp64
int __ovld __cnfn isless(double, double);
long2 __ovld __cnfn isless(double2, double2);
long3 __ovld __cnfn isless(double3, double3);
long4 __ovld __cnfn isless(double4, double4);
long8 __ovld __cnfn isless(double8, double8);
long16 __ovld __cnfn isless(double16, double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
int __ovld __cnfn isless(half, half);
short2 __ovld __cnfn isless(half2, half2);
short3 __ovld __cnfn isless(half3, half3);
short4 __ovld __cnfn isless(half4, half4);
short8 __ovld __cnfn isless(half8, half8);
short16 __ovld __cnfn isless(half16, half16);
#endif //cl_khr_fp16

/**
 * Returns the component-wise compare of x <= y.
 */
int __ovld __cnfn islessequal(float, float);
int2 __ovld __cnfn islessequal(float2, float2);
int3 __ovld __cnfn islessequal(float3, float3);
int4 __ovld __cnfn islessequal(float4, float4);
int8 __ovld __cnfn islessequal(float8, float8);
int16 __ovld __cnfn islessequal(float16, float16);
#ifdef cl_khr_fp64
int __ovld __cnfn islessequal(double, double);
long2 __ovld __cnfn islessequal(double2, double2);
long3 __ovld __cnfn islessequal(double3, double3);
long4 __ovld __cnfn islessequal(double4, double4);
long8 __ovld __cnfn islessequal(double8, double8);
long16 __ovld __cnfn islessequal(double16, double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
int __ovld __cnfn islessequal(half, half);
short2 __ovld __cnfn islessequal(half2, half2);
short3 __ovld __cnfn islessequal(half3, half3);
short4 __ovld __cnfn islessequal(half4, half4);
short8 __ovld __cnfn islessequal(half8, half8);
short16 __ovld __cnfn islessequal(half16, half16);
#endif //cl_khr_fp16

/**
 * Returns the component-wise compare of
 * (x < y) || (x > y) .
 */
int __ovld __cnfn islessgreater(float, float);
int2 __ovld __cnfn islessgreater(float2, float2);
int3 __ovld __cnfn islessgreater(float3, float3);
int4 __ovld __cnfn islessgreater(float4, float4);
int8 __ovld __cnfn islessgreater(float8, float8);
int16 __ovld __cnfn islessgreater(float16, float16);
#ifdef cl_khr_fp64
int __ovld __cnfn islessgreater(double, double);
long2 __ovld __cnfn islessgreater(double2, double2);
long3 __ovld __cnfn islessgreater(double3, double3);
long4 __ovld __cnfn islessgreater(double4, double4);
long8 __ovld __cnfn islessgreater(double8, double8);
long16 __ovld __cnfn islessgreater(double16, double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
int __ovld __cnfn islessgreater(half, half);
short2 __ovld __cnfn islessgreater(half2, half2);
short3 __ovld __cnfn islessgreater(half3, half3);
short4 __ovld __cnfn islessgreater(half4, half4);
short8 __ovld __cnfn islessgreater(half8, half8);
short16 __ovld __cnfn islessgreater(half16, half16);
#endif //cl_khr_fp16

/**
 * Test for finite value.
 */
int __ovld __cnfn isfinite(float);
int2 __ovld __cnfn isfinite(float2);
int3 __ovld __cnfn isfinite(float3);
int4 __ovld __cnfn isfinite(float4);
int8 __ovld __cnfn isfinite(float8);
int16 __ovld __cnfn isfinite(float16);
#ifdef cl_khr_fp64
int __ovld __cnfn isfinite(double);
long2 __ovld __cnfn isfinite(double2);
long3 __ovld __cnfn isfinite(double3);
long4 __ovld __cnfn isfinite(double4);
long8 __ovld __cnfn isfinite(double8);
long16 __ovld __cnfn isfinite(double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
int __ovld __cnfn isfinite(half);
short2 __ovld __cnfn isfinite(half2);
short3 __ovld __cnfn isfinite(half3);
short4 __ovld __cnfn isfinite(half4);
short8 __ovld __cnfn isfinite(half8);
short16 __ovld __cnfn isfinite(half16);
#endif //cl_khr_fp16

/**
 * Test for infinity value (+ve or -ve) .
 */
int __ovld __cnfn isinf(float);
int2 __ovld __cnfn isinf(float2);
int3 __ovld __cnfn isinf(float3);
int4 __ovld __cnfn isinf(float4);
int8 __ovld __cnfn isinf(float8);
int16 __ovld __cnfn isinf(float16);
#ifdef cl_khr_fp64
int __ovld __cnfn isinf(double);
long2 __ovld __cnfn isinf(double2);
long3 __ovld __cnfn isinf(double3);
long4 __ovld __cnfn isinf(double4);
long8 __ovld __cnfn isinf(double8);
long16 __ovld __cnfn isinf(double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
int __ovld __cnfn isinf(half);
short2 __ovld __cnfn isinf(half2);
short3 __ovld __cnfn isinf(half3);
short4 __ovld __cnfn isinf(half4);
short8 __ovld __cnfn isinf(half8);
short16 __ovld __cnfn isinf(half16);
#endif //cl_khr_fp16

/**
 * Test for a NaN.
 */
int __ovld __cnfn isnan(float);
int2 __ovld __cnfn isnan(float2);
int3 __ovld __cnfn isnan(float3);
int4 __ovld __cnfn isnan(float4);
int8 __ovld __cnfn isnan(float8);
int16 __ovld __cnfn isnan(float16);
#ifdef cl_khr_fp64
int __ovld __cnfn isnan(double);
long2 __ovld __cnfn isnan(double2);
long3 __ovld __cnfn isnan(double3);
long4 __ovld __cnfn isnan(double4);
long8 __ovld __cnfn isnan(double8);
long16 __ovld __cnfn isnan(double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
int __ovld __cnfn isnan(half);
short2 __ovld __cnfn isnan(half2);
short3 __ovld __cnfn isnan(half3);
short4 __ovld __cnfn isnan(half4);
short8 __ovld __cnfn isnan(half8);
short16 __ovld __cnfn isnan(half16);
#endif //cl_khr_fp16

/**
 * Test for a normal value.
 */
int __ovld __cnfn isnormal(float);
int2 __ovld __cnfn isnormal(float2);
int3 __ovld __cnfn isnormal(float3);
int4 __ovld __cnfn isnormal(float4);
int8 __ovld __cnfn isnormal(float8);
int16 __ovld __cnfn isnormal(float16);
#ifdef cl_khr_fp64
int __ovld __cnfn isnormal(double);
long2 __ovld __cnfn isnormal(double2);
long3 __ovld __cnfn isnormal(double3);
long4 __ovld __cnfn isnormal(double4);
long8 __ovld __cnfn isnormal(double8);
long16 __ovld __cnfn isnormal(double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
int __ovld __cnfn isnormal(half);
short2 __ovld __cnfn isnormal(half2);
short3 __ovld __cnfn isnormal(half3);
short4 __ovld __cnfn isnormal(half4);
short8 __ovld __cnfn isnormal(half8);
short16 __ovld __cnfn isnormal(half16);
#endif //cl_khr_fp16

/**
 * Test if arguments are ordered. isordered() takes
 * arguments x and y, and returns the result
 * isequal(x, x) && isequal(y, y).
 */
int __ovld __cnfn isordered(float, float);
int2 __ovld __cnfn isordered(float2, float2);
int3 __ovld __cnfn isordered(float3, float3);
int4 __ovld __cnfn isordered(float4, float4);
int8 __ovld __cnfn isordered(float8, float8);
int16 __ovld __cnfn isordered(float16, float16);
#ifdef cl_khr_fp64
int __ovld __cnfn isordered(double, double);
long2 __ovld __cnfn isordered(double2, double2);
long3 __ovld __cnfn isordered(double3, double3);
long4 __ovld __cnfn isordered(double4, double4);
long8 __ovld __cnfn isordered(double8, double8);
long16 __ovld __cnfn isordered(double16, double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
int __ovld __cnfn isordered(half, half);
short2 __ovld __cnfn isordered(half2, half2);
short3 __ovld __cnfn isordered(half3, half3);
short4 __ovld __cnfn isordered(half4, half4);
short8 __ovld __cnfn isordered(half8, half8);
short16 __ovld __cnfn isordered(half16, half16);
#endif //cl_khr_fp16

/**
 * Test if arguments are unordered. isunordered()
 * takes arguments x and y, returning non-zero if x or y
 * is NaN, and zero otherwise.
 */
int __ovld __cnfn isunordered(float, float);
int2 __ovld __cnfn isunordered(float2, float2);
int3 __ovld __cnfn isunordered(float3, float3);
int4 __ovld __cnfn isunordered(float4, float4);
int8 __ovld __cnfn isunordered(float8, float8);
int16 __ovld __cnfn isunordered(float16, float16);
#ifdef cl_khr_fp64
int __ovld __cnfn isunordered(double, double);
long2 __ovld __cnfn isunordered(double2, double2);
long3 __ovld __cnfn isunordered(double3, double3);
long4 __ovld __cnfn isunordered(double4, double4);
long8 __ovld __cnfn isunordered(double8, double8);
long16 __ovld __cnfn isunordered(double16, double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
int __ovld __cnfn isunordered(half, half);
short2 __ovld __cnfn isunordered(half2, half2);
short3 __ovld __cnfn isunordered(half3, half3);
short4 __ovld __cnfn isunordered(half4, half4);
short8 __ovld __cnfn isunordered(half8, half8);
short16 __ovld __cnfn isunordered(half16, half16);
#endif //cl_khr_fp16

/**
 * Test for sign bit. The scalar version of the function
 * returns a 1 if the sign bit in the float is set else returns
 * 0. The vector version of the function returns the
 * following for each component in floatn: a -1 if the
 * sign bit in the float is set else returns 0.
 */
int __ovld __cnfn signbit(float);
int2 __ovld __cnfn signbit(float2);
int3 __ovld __cnfn signbit(float3);
int4 __ovld __cnfn signbit(float4);
int8 __ovld __cnfn signbit(float8);
int16 __ovld __cnfn signbit(float16);
#ifdef cl_khr_fp64
int __ovld __cnfn signbit(double);
long2 __ovld __cnfn signbit(double2);
long3 __ovld __cnfn signbit(double3);
long4 __ovld __cnfn signbit(double4);
long8 __ovld __cnfn signbit(double8);
long16 __ovld __cnfn signbit(double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
int __ovld __cnfn signbit(half);
short2 __ovld __cnfn signbit(half2);
short3 __ovld __cnfn signbit(half3);
short4 __ovld __cnfn signbit(half4);
short8 __ovld __cnfn signbit(half8);
short16 __ovld __cnfn signbit(half16);
#endif //cl_khr_fp16

/**
 * Returns 1 if the most significant bit in any component
 * of x is set; otherwise returns 0.
 */
int __ovld __cnfn any(char);
int __ovld __cnfn any(char2);
int __ovld __cnfn any(char3);
int __ovld __cnfn any(char4);
int __ovld __cnfn any(char8);
int __ovld __cnfn any(char16);
int __ovld __cnfn any(short);
int __ovld __cnfn any(short2);
int __ovld __cnfn any(short3);
int __ovld __cnfn any(short4);
int __ovld __cnfn any(short8);
int __ovld __cnfn any(short16);
int __ovld __cnfn any(int);
int __ovld __cnfn any(int2);
int __ovld __cnfn any(int3);
int __ovld __cnfn any(int4);
int __ovld __cnfn any(int8);
int __ovld __cnfn any(int16);
int __ovld __cnfn any(long);
int __ovld __cnfn any(long2);
int __ovld __cnfn any(long3);
int __ovld __cnfn any(long4);
int __ovld __cnfn any(long8);
int __ovld __cnfn any(long16);

/**
 * Returns 1 if the most significant bit in all components
 * of x is set; otherwise returns 0.
 */
int __ovld __cnfn all(char);
int __ovld __cnfn all(char2);
int __ovld __cnfn all(char3);
int __ovld __cnfn all(char4);
int __ovld __cnfn all(char8);
int __ovld __cnfn all(char16);
int __ovld __cnfn all(short);
int __ovld __cnfn all(short2);
int __ovld __cnfn all(short3);
int __ovld __cnfn all(short4);
int __ovld __cnfn all(short8);
int __ovld __cnfn all(short16);
int __ovld __cnfn all(int);
int __ovld __cnfn all(int2);
int __ovld __cnfn all(int3);
int __ovld __cnfn all(int4);
int __ovld __cnfn all(int8);
int __ovld __cnfn all(int16);
int __ovld __cnfn all(long);
int __ovld __cnfn all(long2);
int __ovld __cnfn all(long3);
int __ovld __cnfn all(long4);
int __ovld __cnfn all(long8);
int __ovld __cnfn all(long16);

/**
 * Each bit of the result is the corresponding bit of a if
 * the corresponding bit of c is 0. Otherwise it is the
 * corresponding bit of b.
 */
char __ovld __cnfn bitselect(char, char, char);
uchar __ovld __cnfn bitselect(uchar, uchar, uchar);
char2 __ovld __cnfn bitselect(char2, char2, char2);
uchar2 __ovld __cnfn bitselect(uchar2, uchar2, uchar2);
char3 __ovld __cnfn bitselect(char3, char3, char3);
uchar3 __ovld __cnfn bitselect(uchar3, uchar3, uchar3);
char4 __ovld __cnfn bitselect(char4, char4, char4);
uchar4 __ovld __cnfn bitselect(uchar4, uchar4, uchar4);
char8 __ovld __cnfn bitselect(char8, char8, char8);
uchar8 __ovld __cnfn bitselect(uchar8, uchar8, uchar8);
char16 __ovld __cnfn bitselect(char16, char16, char16);
uchar16 __ovld __cnfn bitselect(uchar16, uchar16, uchar16);
short __ovld __cnfn bitselect(short, short, short);
ushort __ovld __cnfn bitselect(ushort, ushort, ushort);
short2 __ovld __cnfn bitselect(short2, short2, short2);
ushort2 __ovld __cnfn bitselect(ushort2, ushort2, ushort2);
short3 __ovld __cnfn bitselect(short3, short3, short3);
ushort3 __ovld __cnfn bitselect(ushort3, ushort3, ushort3);
short4 __ovld __cnfn bitselect(short4, short4, short4);
ushort4 __ovld __cnfn bitselect(ushort4, ushort4, ushort4);
short8 __ovld __cnfn bitselect(short8, short8, short8);
ushort8 __ovld __cnfn bitselect(ushort8, ushort8, ushort8);
short16 __ovld __cnfn bitselect(short16, short16, short16);
ushort16 __ovld __cnfn bitselect(ushort16, ushort16, ushort16);
int __ovld __cnfn bitselect(int, int, int);
uint __ovld __cnfn bitselect(uint, uint, uint);
int2 __ovld __cnfn bitselect(int2, int2, int2);
uint2 __ovld __cnfn bitselect(uint2, uint2, uint2);
int3 __ovld __cnfn bitselect(int3, int3, int3);
uint3 __ovld __cnfn bitselect(uint3, uint3, uint3);
int4 __ovld __cnfn bitselect(int4, int4, int4);
uint4 __ovld __cnfn bitselect(uint4, uint4, uint4);
int8 __ovld __cnfn bitselect(int8, int8, int8);
uint8 __ovld __cnfn bitselect(uint8, uint8, uint8);
int16 __ovld __cnfn bitselect(int16, int16, int16);
uint16 __ovld __cnfn bitselect(uint16, uint16, uint16);
long __ovld __cnfn bitselect(long, long, long);
ulong __ovld __cnfn bitselect(ulong, ulong, ulong);
long2 __ovld __cnfn bitselect(long2, long2, long2);
ulong2 __ovld __cnfn bitselect(ulong2, ulong2, ulong2);
long3 __ovld __cnfn bitselect(long3, long3, long3);
ulong3 __ovld __cnfn bitselect(ulong3, ulong3, ulong3);
long4 __ovld __cnfn bitselect(long4, long4, long4);
ulong4 __ovld __cnfn bitselect(ulong4, ulong4, ulong4);
long8 __ovld __cnfn bitselect(long8, long8, long8);
ulong8 __ovld __cnfn bitselect(ulong8, ulong8, ulong8);
long16 __ovld __cnfn bitselect(long16, long16, long16);
ulong16 __ovld __cnfn bitselect(ulong16, ulong16, ulong16);
float __ovld __cnfn bitselect(float, float, float);
float2 __ovld __cnfn bitselect(float2, float2, float2);
float3 __ovld __cnfn bitselect(float3, float3, float3);
float4 __ovld __cnfn bitselect(float4, float4, float4);
float8 __ovld __cnfn bitselect(float8, float8, float8);
float16 __ovld __cnfn bitselect(float16, float16, float16);
#ifdef cl_khr_fp64
double __ovld __cnfn bitselect(double, double, double);
double2 __ovld __cnfn bitselect(double2, double2, double2);
double3 __ovld __cnfn bitselect(double3, double3, double3);
double4 __ovld __cnfn bitselect(double4, double4, double4);
double8 __ovld __cnfn bitselect(double8, double8, double8);
double16 __ovld __cnfn bitselect(double16, double16, double16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn bitselect(half, half, half);
half2 __ovld __cnfn bitselect(half2, half2, half2);
half3 __ovld __cnfn bitselect(half3, half3, half3);
half4 __ovld __cnfn bitselect(half4, half4, half4);
half8 __ovld __cnfn bitselect(half8, half8, half8);
half16 __ovld __cnfn bitselect(half16, half16, half16);
#endif //cl_khr_fp16

/**
 * For each component of a vector type,
 * result[i] = if MSB of c[i] is set ? b[i] : a[i].
 * For a scalar type, result = c ? b : a.
 * b and a must have the same type.
 * c must have the same number of elements and bits as a.
 */
char __ovld __cnfn select(char, char, char);
uchar __ovld __cnfn select(uchar, uchar, char);
char2 __ovld __cnfn select(char2, char2, char2);
uchar2 __ovld __cnfn select(uchar2, uchar2, char2);
char3 __ovld __cnfn select(char3, char3, char3);
uchar3 __ovld __cnfn select(uchar3, uchar3, char3);
char4 __ovld __cnfn select(char4, char4, char4);
uchar4 __ovld __cnfn select(uchar4, uchar4, char4);
char8 __ovld __cnfn select(char8, char8, char8);
uchar8 __ovld __cnfn select(uchar8, uchar8, char8);
char16 __ovld __cnfn select(char16, char16, char16);
uchar16 __ovld __cnfn select(uchar16, uchar16, char16);

short __ovld __cnfn select(short, short, short);
ushort __ovld __cnfn select(ushort, ushort, short);
short2 __ovld __cnfn select(short2, short2, short2);
ushort2 __ovld __cnfn select(ushort2, ushort2, short2);
short3 __ovld __cnfn select(short3, short3, short3);
ushort3 __ovld __cnfn select(ushort3, ushort3, short3);
short4 __ovld __cnfn select(short4, short4, short4);
ushort4 __ovld __cnfn select(ushort4, ushort4, short4);
short8 __ovld __cnfn select(short8, short8, short8);
ushort8 __ovld __cnfn select(ushort8, ushort8, short8);
short16 __ovld __cnfn select(short16, short16, short16);
ushort16 __ovld __cnfn select(ushort16, ushort16, short16);

int __ovld __cnfn select(int, int, int);
uint __ovld __cnfn select(uint, uint, int);
int2 __ovld __cnfn select(int2, int2, int2);
uint2 __ovld __cnfn select(uint2, uint2, int2);
int3 __ovld __cnfn select(int3, int3, int3);
uint3 __ovld __cnfn select(uint3, uint3, int3);
int4 __ovld __cnfn select(int4, int4, int4);
uint4 __ovld __cnfn select(uint4, uint4, int4);
int8 __ovld __cnfn select(int8, int8, int8);
uint8 __ovld __cnfn select(uint8, uint8, int8);
int16 __ovld __cnfn select(int16, int16, int16);
uint16 __ovld __cnfn select(uint16, uint16, int16);
float __ovld __cnfn select(float, float, int);
float2 __ovld __cnfn select(float2, float2, int2);
float3 __ovld __cnfn select(float3, float3, int3);
float4 __ovld __cnfn select(float4, float4, int4);
float8 __ovld __cnfn select(float8, float8, int8);
float16 __ovld __cnfn select(float16, float16, int16);

long __ovld __cnfn select(long, long, long);
ulong __ovld __cnfn select(ulong, ulong, long);
long2 __ovld __cnfn select(long2, long2, long2);
ulong2 __ovld __cnfn select(ulong2, ulong2, long2);
long3 __ovld __cnfn select(long3, long3, long3);
ulong3 __ovld __cnfn select(ulong3, ulong3, long3);
long4 __ovld __cnfn select(long4, long4, long4);
ulong4 __ovld __cnfn select(ulong4, ulong4, long4);
long8 __ovld __cnfn select(long8, long8, long8);
ulong8 __ovld __cnfn select(ulong8, ulong8, long8);
long16 __ovld __cnfn select(long16, long16, long16);
ulong16 __ovld __cnfn select(ulong16, ulong16, long16);

char __ovld __cnfn select(char, char, uchar);
uchar __ovld __cnfn select(uchar, uchar, uchar);
char2 __ovld __cnfn select(char2, char2, uchar2);
uchar2 __ovld __cnfn select(uchar2, uchar2, uchar2);
char3 __ovld __cnfn select(char3, char3, uchar3);
uchar3 __ovld __cnfn select(uchar3, uchar3, uchar3);
char4 __ovld __cnfn select(char4, char4, uchar4);
uchar4 __ovld __cnfn select(uchar4, uchar4, uchar4);
char8 __ovld __cnfn select(char8, char8, uchar8);
uchar8 __ovld __cnfn select(uchar8, uchar8, uchar8);
char16 __ovld __cnfn select(char16, char16, uchar16);
uchar16 __ovld __cnfn select(uchar16, uchar16, uchar16);

short __ovld __cnfn select(short, short, ushort);
ushort __ovld __cnfn select(ushort, ushort, ushort);
short2 __ovld __cnfn select(short2, short2, ushort2);
ushort2 __ovld __cnfn select(ushort2, ushort2, ushort2);
short3 __ovld __cnfn select(short3, short3, ushort3);
ushort3 __ovld __cnfn select(ushort3, ushort3, ushort3);
short4 __ovld __cnfn select(short4, short4, ushort4);
ushort4 __ovld __cnfn select(ushort4, ushort4, ushort4);
short8 __ovld __cnfn select(short8, short8, ushort8);
ushort8 __ovld __cnfn select(ushort8, ushort8, ushort8);
short16 __ovld __cnfn select(short16, short16, ushort16);
ushort16 __ovld __cnfn select(ushort16, ushort16, ushort16);

int __ovld __cnfn select(int, int, uint);
uint __ovld __cnfn select(uint, uint, uint);
int2 __ovld __cnfn select(int2, int2, uint2);
uint2 __ovld __cnfn select(uint2, uint2, uint2);
int3 __ovld __cnfn select(int3, int3, uint3);
uint3 __ovld __cnfn select(uint3, uint3, uint3);
int4 __ovld __cnfn select(int4, int4, uint4);
uint4 __ovld __cnfn select(uint4, uint4, uint4);
int8 __ovld __cnfn select(int8, int8, uint8);
uint8 __ovld __cnfn select(uint8, uint8, uint8);
int16 __ovld __cnfn select(int16, int16, uint16);
uint16 __ovld __cnfn select(uint16, uint16, uint16);
float __ovld __cnfn select(float, float, uint);
float2 __ovld __cnfn select(float2, float2, uint2);
float3 __ovld __cnfn select(float3, float3, uint3);
float4 __ovld __cnfn select(float4, float4, uint4);
float8 __ovld __cnfn select(float8, float8, uint8);
float16 __ovld __cnfn select(float16, float16, uint16);

long __ovld __cnfn select(long, long, ulong);
ulong __ovld __cnfn select(ulong, ulong, ulong);
long2 __ovld __cnfn select(long2, long2, ulong2);
ulong2 __ovld __cnfn select(ulong2, ulong2, ulong2);
long3 __ovld __cnfn select(long3, long3, ulong3);
ulong3 __ovld __cnfn select(ulong3, ulong3, ulong3);
long4 __ovld __cnfn select(long4, long4, ulong4);
ulong4 __ovld __cnfn select(ulong4, ulong4, ulong4);
long8 __ovld __cnfn select(long8, long8, ulong8);
ulong8 __ovld __cnfn select(ulong8, ulong8, ulong8);
long16 __ovld __cnfn select(long16, long16, ulong16);
ulong16 __ovld __cnfn select(ulong16, ulong16, ulong16);

#ifdef cl_khr_fp64
double __ovld __cnfn select(double, double, long);
double2 __ovld __cnfn select(double2, double2, long2);
double3 __ovld __cnfn select(double3, double3, long3);
double4 __ovld __cnfn select(double4, double4, long4);
double8 __ovld __cnfn select(double8, double8, long8);
double16 __ovld __cnfn select(double16, double16, long16);
double __ovld __cnfn select(double, double, ulong);
double2 __ovld __cnfn select(double2, double2, ulong2);
double3 __ovld __cnfn select(double3, double3, ulong3);
double4 __ovld __cnfn select(double4, double4, ulong4);
double8 __ovld __cnfn select(double8, double8, ulong8);
double16 __ovld __cnfn select(double16, double16, ulong16);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
half __ovld __cnfn select(half, half, short);
half2 __ovld __cnfn select(half2, half2, short2);
half3 __ovld __cnfn select(half3, half3, short3);
half4 __ovld __cnfn select(half4, half4, short4);
half8 __ovld __cnfn select(half8, half8, short8);
half16 __ovld __cnfn select(half16, half16, short16);
half __ovld __cnfn select(half, half, ushort);
half2 __ovld __cnfn select(half2, half2, ushort2);
half3 __ovld __cnfn select(half3, half3, ushort3);
half4 __ovld __cnfn select(half4, half4, ushort4);
half8 __ovld __cnfn select(half8, half8, ushort8);
half16 __ovld __cnfn select(half16, half16, ushort16);
#endif //cl_khr_fp16

// OpenCL v1.1 s6.11.7, v1.2 s6.12.7, v2.0 s6.13.7 - Vector Data Load and Store Functions
// OpenCL extensions v1.1 s9.6.6, v1.2 s9.5.6, v2.0 s9.4.6 - Vector Data Load and Store Functions for Half Type
/**
 * Use generic type gentype to indicate the built-in data types
 * char, uchar, short, ushort, int, uint, long, ulong, float,
 * double or half.
 *
 * vloadn return sizeof (gentypen) bytes of data read from address (p + (offset * n)).
 *
 * vstoren write sizeof (gentypen) bytes given by data to address (p + (offset * n)).
 *
 * The address computed as (p + (offset * n)) must be
 * 8-bit aligned if gentype is char, uchar;
 * 16-bit aligned if gentype is short, ushort, half;
 * 32-bit aligned if gentype is int, uint, float;
 * 64-bit aligned if gentype is long, ulong, double.
 */

char2 __ovld __purefn vload2(size_t, const __constant char *);
uchar2 __ovld __purefn vload2(size_t, const __constant uchar *);
short2 __ovld __purefn vload2(size_t, const __constant short *);
ushort2 __ovld __purefn vload2(size_t, const __constant ushort *);
int2 __ovld __purefn vload2(size_t, const __constant int *);
uint2 __ovld __purefn vload2(size_t, const __constant uint *);
long2 __ovld __purefn vload2(size_t, const __constant long *);
ulong2 __ovld __purefn vload2(size_t, const __constant ulong *);
float2 __ovld __purefn vload2(size_t, const __constant float *);
char3 __ovld __purefn vload3(size_t, const __constant char *);
uchar3 __ovld __purefn vload3(size_t, const __constant uchar *);
short3 __ovld __purefn vload3(size_t, const __constant short *);
ushort3 __ovld __purefn vload3(size_t, const __constant ushort *);
int3 __ovld __purefn vload3(size_t, const __constant int *);
uint3 __ovld __purefn vload3(size_t, const __constant uint *);
long3 __ovld __purefn vload3(size_t, const __constant long *);
ulong3 __ovld __purefn vload3(size_t, const __constant ulong *);
float3 __ovld __purefn vload3(size_t, const __constant float *);
char4 __ovld __purefn vload4(size_t, const __constant char *);
uchar4 __ovld __purefn vload4(size_t, const __constant uchar *);
short4 __ovld __purefn vload4(size_t, const __constant short *);
ushort4 __ovld __purefn vload4(size_t, const __constant ushort *);
int4 __ovld __purefn vload4(size_t, const __constant int *);
uint4 __ovld __purefn vload4(size_t, const __constant uint *);
long4 __ovld __purefn vload4(size_t, const __constant long *);
ulong4 __ovld __purefn vload4(size_t, const __constant ulong *);
float4 __ovld __purefn vload4(size_t, const __constant float *);
char8 __ovld __purefn vload8(size_t, const __constant char *);
uchar8 __ovld __purefn vload8(size_t, const __constant uchar *);
short8 __ovld __purefn vload8(size_t, const __constant short *);
ushort8 __ovld __purefn vload8(size_t, const __constant ushort *);
int8 __ovld __purefn vload8(size_t, const __constant int *);
uint8 __ovld __purefn vload8(size_t, const __constant uint *);
long8 __ovld __purefn vload8(size_t, const __constant long *);
ulong8 __ovld __purefn vload8(size_t, const __constant ulong *);
float8 __ovld __purefn vload8(size_t, const __constant float *);
char16 __ovld __purefn vload16(size_t, const __constant char *);
uchar16 __ovld __purefn vload16(size_t, const __constant uchar *);
short16 __ovld __purefn vload16(size_t, const __constant short *);
ushort16 __ovld __purefn vload16(size_t, const __constant ushort *);
int16 __ovld __purefn vload16(size_t, const __constant int *);
uint16 __ovld __purefn vload16(size_t, const __constant uint *);
long16 __ovld __purefn vload16(size_t, const __constant long *);
ulong16 __ovld __purefn vload16(size_t, const __constant ulong *);
float16 __ovld __purefn vload16(size_t, const __constant float *);
#ifdef cl_khr_fp64
double2 __ovld __purefn vload2(size_t, const __constant double *);
double3 __ovld __purefn vload3(size_t, const __constant double *);
double4 __ovld __purefn vload4(size_t, const __constant double *);
double8 __ovld __purefn vload8(size_t, const __constant double *);
double16 __ovld __purefn vload16(size_t, const __constant double *);
#endif //cl_khr_fp64

#ifdef cl_khr_fp16
half2 __ovld __purefn vload2(size_t, const __constant half *);
half3 __ovld __purefn vload3(size_t, const __constant half *);
half4 __ovld __purefn vload4(size_t, const __constant half *);
half8 __ovld __purefn vload8(size_t, const __constant half *);
half16 __ovld __purefn vload16(size_t, const __constant half *);
#endif //cl_khr_fp16

#if defined(__opencl_c_generic_address_space)
char2 __ovld __purefn vload2(size_t, const char *);
uchar2 __ovld __purefn vload2(size_t, const uchar *);
short2 __ovld __purefn vload2(size_t, const short *);
ushort2 __ovld __purefn vload2(size_t, const ushort *);
int2 __ovld __purefn vload2(size_t, const int *);
uint2 __ovld __purefn vload2(size_t, const uint *);
long2 __ovld __purefn vload2(size_t, const long *);
ulong2 __ovld __purefn vload2(size_t, const ulong *);
float2 __ovld __purefn vload2(size_t, const float *);
char3 __ovld __purefn vload3(size_t, const char *);
uchar3 __ovld __purefn vload3(size_t, const uchar *);
short3 __ovld __purefn vload3(size_t, const short *);
ushort3 __ovld __purefn vload3(size_t, const ushort *);
int3 __ovld __purefn vload3(size_t, const int *);
uint3 __ovld __purefn vload3(size_t, const uint *);
long3 __ovld __purefn vload3(size_t, const long *);
ulong3 __ovld __purefn vload3(size_t, const ulong *);
float3 __ovld __purefn vload3(size_t, const float *);
char4 __ovld __purefn vload4(size_t, const char *);
uchar4 __ovld __purefn vload4(size_t, const uchar *);
short4 __ovld __purefn vload4(size_t, const short *);
ushort4 __ovld __purefn vload4(size_t, const ushort *);
int4 __ovld __purefn vload4(size_t, const int *);
uint4 __ovld __purefn vload4(size_t, const uint *);
long4 __ovld __purefn vload4(size_t, const long *);
ulong4 __ovld __purefn vload4(size_t, const ulong *);
float4 __ovld __purefn vload4(size_t, const float *);
char8 __ovld __purefn vload8(size_t, const char *);
uchar8 __ovld __purefn vload8(size_t, const uchar *);
short8 __ovld __purefn vload8(size_t, const short *);
ushort8 __ovld __purefn vload8(size_t, const ushort *);
int8 __ovld __purefn vload8(size_t, const int *);
uint8 __ovld __purefn vload8(size_t, const uint *);
long8 __ovld __purefn vload8(size_t, const long *);
ulong8 __ovld __purefn vload8(size_t, const ulong *);
float8 __ovld __purefn vload8(size_t, const float *);
char16 __ovld __purefn vload16(size_t, const char *);
uchar16 __ovld __purefn vload16(size_t, const uchar *);
short16 __ovld __purefn vload16(size_t, const short *);
ushort16 __ovld __purefn vload16(size_t, const ushort *);
int16 __ovld __purefn vload16(size_t, const int *);
uint16 __ovld __purefn vload16(size_t, const uint *);
long16 __ovld __purefn vload16(size_t, const long *);
ulong16 __ovld __purefn vload16(size_t, const ulong *);
float16 __ovld __purefn vload16(size_t, const float *);

#ifdef cl_khr_fp64
double2 __ovld __purefn vload2(size_t, const double *);
double3 __ovld __purefn vload3(size_t, const double *);
double4 __ovld __purefn vload4(size_t, const double *);
double8 __ovld __purefn vload8(size_t, const double *);
double16 __ovld __purefn vload16(size_t, const double *);
#endif //cl_khr_fp64

#ifdef cl_khr_fp16
half2 __ovld __purefn vload2(size_t, const half *);
half3 __ovld __purefn vload3(size_t, const half *);
half4 __ovld __purefn vload4(size_t, const half *);
half8 __ovld __purefn vload8(size_t, const half *);
half16 __ovld __purefn vload16(size_t, const half *);
#endif //cl_khr_fp16
#endif //defined(__opencl_c_generic_address_space)

#if defined(__opencl_c_named_address_space_builtins)
char2 __ovld __purefn vload2(size_t, const __global char *);
uchar2 __ovld __purefn vload2(size_t, const __global uchar *);
short2 __ovld __purefn vload2(size_t, const __global short *);
ushort2 __ovld __purefn vload2(size_t, const __global ushort *);
int2 __ovld __purefn vload2(size_t, const __global int *);
uint2 __ovld __purefn vload2(size_t, const __global uint *);
long2 __ovld __purefn vload2(size_t, const __global long *);
ulong2 __ovld __purefn vload2(size_t, const __global ulong *);
float2 __ovld __purefn vload2(size_t, const __global float *);
char3 __ovld __purefn vload3(size_t, const __global char *);
uchar3 __ovld __purefn vload3(size_t, const __global uchar *);
short3 __ovld __purefn vload3(size_t, const __global short *);
ushort3 __ovld __purefn vload3(size_t, const __global ushort *);
int3 __ovld __purefn vload3(size_t, const __global int *);
uint3 __ovld __purefn vload3(size_t, const __global uint *);
long3 __ovld __purefn vload3(size_t, const __global long *);
ulong3 __ovld __purefn vload3(size_t, const __global ulong *);
float3 __ovld __purefn vload3(size_t, const __global float *);
char4 __ovld __purefn vload4(size_t, const __global char *);
uchar4 __ovld __purefn vload4(size_t, const __global uchar *);
short4 __ovld __purefn vload4(size_t, const __global short *);
ushort4 __ovld __purefn vload4(size_t, const __global ushort *);
int4 __ovld __purefn vload4(size_t, const __global int *);
uint4 __ovld __purefn vload4(size_t, const __global uint *);
long4 __ovld __purefn vload4(size_t, const __global long *);
ulong4 __ovld __purefn vload4(size_t, const __global ulong *);
float4 __ovld __purefn vload4(size_t, const __global float *);
char8 __ovld __purefn vload8(size_t, const __global char *);
uchar8 __ovld __purefn vload8(size_t, const __global uchar *);
short8 __ovld __purefn vload8(size_t, const __global short *);
ushort8 __ovld __purefn vload8(size_t, const __global ushort *);
int8 __ovld __purefn vload8(size_t, const __global int *);
uint8 __ovld __purefn vload8(size_t, const __global uint *);
long8 __ovld __purefn vload8(size_t, const __global long *);
ulong8 __ovld __purefn vload8(size_t, const __global ulong *);
float8 __ovld __purefn vload8(size_t, const __global float *);
char16 __ovld __purefn vload16(size_t, const __global char *);
uchar16 __ovld __purefn vload16(size_t, const __global uchar *);
short16 __ovld __purefn vload16(size_t, const __global short *);
ushort16 __ovld __purefn vload16(size_t, const __global ushort *);
int16 __ovld __purefn vload16(size_t, const __global int *);
uint16 __ovld __purefn vload16(size_t, const __global uint *);
long16 __ovld __purefn vload16(size_t, const __global long *);
ulong16 __ovld __purefn vload16(size_t, const __global ulong *);
float16 __ovld __purefn vload16(size_t, const __global float *);
char2 __ovld __purefn vload2(size_t, const __local char *);
uchar2 __ovld __purefn vload2(size_t, const __local uchar *);
short2 __ovld __purefn vload2(size_t, const __local short *);
ushort2 __ovld __purefn vload2(size_t, const __local ushort *);
int2 __ovld __purefn vload2(size_t, const __local int *);
uint2 __ovld __purefn vload2(size_t, const __local uint *);
long2 __ovld __purefn vload2(size_t, const __local long *);
ulong2 __ovld __purefn vload2(size_t, const __local ulong *);
float2 __ovld __purefn vload2(size_t, const __local float *);
char3 __ovld __purefn vload3(size_t, const __local char *);
uchar3 __ovld __purefn vload3(size_t, const __local uchar *);
short3 __ovld __purefn vload3(size_t, const __local short *);
ushort3 __ovld __purefn vload3(size_t, const __local ushort *);
int3 __ovld __purefn vload3(size_t, const __local int *);
uint3 __ovld __purefn vload3(size_t, const __local uint *);
long3 __ovld __purefn vload3(size_t, const __local long *);
ulong3 __ovld __purefn vload3(size_t, const __local ulong *);
float3 __ovld __purefn vload3(size_t, const __local float *);
char4 __ovld __purefn vload4(size_t, const __local char *);
uchar4 __ovld __purefn vload4(size_t, const __local uchar *);
short4 __ovld __purefn vload4(size_t, const __local short *);
ushort4 __ovld __purefn vload4(size_t, const __local ushort *);
int4 __ovld __purefn vload4(size_t, const __local int *);
uint4 __ovld __purefn vload4(size_t, const __local uint *);
long4 __ovld __purefn vload4(size_t, const __local long *);
ulong4 __ovld __purefn vload4(size_t, const __local ulong *);
float4 __ovld __purefn vload4(size_t, const __local float *);
char8 __ovld __purefn vload8(size_t, const __local char *);
uchar8 __ovld __purefn vload8(size_t, const __local uchar *);
short8 __ovld __purefn vload8(size_t, const __local short *);
ushort8 __ovld __purefn vload8(size_t, const __local ushort *);
int8 __ovld __purefn vload8(size_t, const __local int *);
uint8 __ovld __purefn vload8(size_t, const __local uint *);
long8 __ovld __purefn vload8(size_t, const __local long *);
ulong8 __ovld __purefn vload8(size_t, const __local ulong *);
float8 __ovld __purefn vload8(size_t, const __local float *);
char16 __ovld __purefn vload16(size_t, const __local char *);
uchar16 __ovld __purefn vload16(size_t, const __local uchar *);
short16 __ovld __purefn vload16(size_t, const __local short *);
ushort16 __ovld __purefn vload16(size_t, const __local ushort *);
int16 __ovld __purefn vload16(size_t, const __local int *);
uint16 __ovld __purefn vload16(size_t, const __local uint *);
long16 __ovld __purefn vload16(size_t, const __local long *);
ulong16 __ovld __purefn vload16(size_t, const __local ulong *);
float16 __ovld __purefn vload16(size_t, const __local float *);
char2 __ovld __purefn vload2(size_t, const __private char *);
uchar2 __ovld __purefn vload2(size_t, const __private uchar *);
short2 __ovld __purefn vload2(size_t, const __private short *);
ushort2 __ovld __purefn vload2(size_t, const __private ushort *);
int2 __ovld __purefn vload2(size_t, const __private int *);
uint2 __ovld __purefn vload2(size_t, const __private uint *);
long2 __ovld __purefn vload2(size_t, const __private long *);
ulong2 __ovld __purefn vload2(size_t, const __private ulong *);
float2 __ovld __purefn vload2(size_t, const __private float *);
char3 __ovld __purefn vload3(size_t, const __private char *);
uchar3 __ovld __purefn vload3(size_t, const __private uchar *);
short3 __ovld __purefn vload3(size_t, const __private short *);
ushort3 __ovld __purefn vload3(size_t, const __private ushort *);
int3 __ovld __purefn vload3(size_t, const __private int *);
uint3 __ovld __purefn vload3(size_t, const __private uint *);
long3 __ovld __purefn vload3(size_t, const __private long *);
ulong3 __ovld __purefn vload3(size_t, const __private ulong *);
float3 __ovld __purefn vload3(size_t, const __private float *);
char4 __ovld __purefn vload4(size_t, const __private char *);
uchar4 __ovld __purefn vload4(size_t, const __private uchar *);
short4 __ovld __purefn vload4(size_t, const __private short *);
ushort4 __ovld __purefn vload4(size_t, const __private ushort *);
int4 __ovld __purefn vload4(size_t, const __private int *);
uint4 __ovld __purefn vload4(size_t, const __private uint *);
long4 __ovld __purefn vload4(size_t, const __private long *);
ulong4 __ovld __purefn vload4(size_t, const __private ulong *);
float4 __ovld __purefn vload4(size_t, const __private float *);
char8 __ovld __purefn vload8(size_t, const __private char *);
uchar8 __ovld __purefn vload8(size_t, const __private uchar *);
short8 __ovld __purefn vload8(size_t, const __private short *);
ushort8 __ovld __purefn vload8(size_t, const __private ushort *);
int8 __ovld __purefn vload8(size_t, const __private int *);
uint8 __ovld __purefn vload8(size_t, const __private uint *);
long8 __ovld __purefn vload8(size_t, const __private long *);
ulong8 __ovld __purefn vload8(size_t, const __private ulong *);
float8 __ovld __purefn vload8(size_t, const __private float *);
char16 __ovld __purefn vload16(size_t, const __private char *);
uchar16 __ovld __purefn vload16(size_t, const __private uchar *);
short16 __ovld __purefn vload16(size_t, const __private short *);
ushort16 __ovld __purefn vload16(size_t, const __private ushort *);
int16 __ovld __purefn vload16(size_t, const __private int *);
uint16 __ovld __purefn vload16(size_t, const __private uint *);
long16 __ovld __purefn vload16(size_t, const __private long *);
ulong16 __ovld __purefn vload16(size_t, const __private ulong *);
float16 __ovld __purefn vload16(size_t, const __private float *);

#ifdef cl_khr_fp64
double2 __ovld __purefn vload2(size_t, const __global double *);
double3 __ovld __purefn vload3(size_t, const __global double *);
double4 __ovld __purefn vload4(size_t, const __global double *);
double8 __ovld __purefn vload8(size_t, const __global double *);
double16 __ovld __purefn vload16(size_t, const __global double *);
double2 __ovld __purefn vload2(size_t, const __local double *);
double3 __ovld __purefn vload3(size_t, const __local double *);
double4 __ovld __purefn vload4(size_t, const __local double *);
double8 __ovld __purefn vload8(size_t, const __local double *);
double16 __ovld __purefn vload16(size_t, const __local double *);
double2 __ovld __purefn vload2(size_t, const __private double *);
double3 __ovld __purefn vload3(size_t, const __private double *);
double4 __ovld __purefn vload4(size_t, const __private double *);
double8 __ovld __purefn vload8(size_t, const __private double *);
double16 __ovld __purefn vload16(size_t, const __private double *);
#endif //cl_khr_fp64

#ifdef cl_khr_fp16
half2 __ovld __purefn vload2(size_t, const __global half *);
half3 __ovld __purefn vload3(size_t, const __global half *);
half4 __ovld __purefn vload4(size_t, const __global half *);
half8 __ovld __purefn vload8(size_t, const __global half *);
half16 __ovld __purefn vload16(size_t, const __global half *);
half2 __ovld __purefn vload2(size_t, const __local half *);
half3 __ovld __purefn vload3(size_t, const __local half *);
half4 __ovld __purefn vload4(size_t, const __local half *);
half8 __ovld __purefn vload8(size_t, const __local half *);
half16 __ovld __purefn vload16(size_t, const __local half *);
half2 __ovld __purefn vload2(size_t, const __private half *);
half3 __ovld __purefn vload3(size_t, const __private half *);
half4 __ovld __purefn vload4(size_t, const __private half *);
half8 __ovld __purefn vload8(size_t, const __private half *);
half16 __ovld __purefn vload16(size_t, const __private half *);
#endif //cl_khr_fp16
#endif //defined(__opencl_c_named_address_space_builtins)

#if defined(__opencl_c_generic_address_space)
void __ovld vstore2(char2, size_t, char *);
void __ovld vstore2(uchar2, size_t, uchar *);
void __ovld vstore2(short2, size_t, short *);
void __ovld vstore2(ushort2, size_t, ushort *);
void __ovld vstore2(int2, size_t, int *);
void __ovld vstore2(uint2, size_t, uint *);
void __ovld vstore2(long2, size_t, long *);
void __ovld vstore2(ulong2, size_t, ulong *);
void __ovld vstore2(float2, size_t, float *);
void __ovld vstore3(char3, size_t, char *);
void __ovld vstore3(uchar3, size_t, uchar *);
void __ovld vstore3(short3, size_t, short *);
void __ovld vstore3(ushort3, size_t, ushort *);
void __ovld vstore3(int3, size_t, int *);
void __ovld vstore3(uint3, size_t, uint *);
void __ovld vstore3(long3, size_t, long *);
void __ovld vstore3(ulong3, size_t, ulong *);
void __ovld vstore3(float3, size_t, float *);
void __ovld vstore4(char4, size_t, char *);
void __ovld vstore4(uchar4, size_t, uchar *);
void __ovld vstore4(short4, size_t, short *);
void __ovld vstore4(ushort4, size_t, ushort *);
void __ovld vstore4(int4, size_t, int *);
void __ovld vstore4(uint4, size_t, uint *);
void __ovld vstore4(long4, size_t, long *);
void __ovld vstore4(ulong4, size_t, ulong *);
void __ovld vstore4(float4, size_t, float *);
void __ovld vstore8(char8, size_t, char *);
void __ovld vstore8(uchar8, size_t, uchar *);
void __ovld vstore8(short8, size_t, short *);
void __ovld vstore8(ushort8, size_t, ushort *);
void __ovld vstore8(int8, size_t, int *);
void __ovld vstore8(uint8, size_t, uint *);
void __ovld vstore8(long8, size_t, long *);
void __ovld vstore8(ulong8, size_t, ulong *);
void __ovld vstore8(float8, size_t, float *);
void __ovld vstore16(char16, size_t, char *);
void __ovld vstore16(uchar16, size_t, uchar *);
void __ovld vstore16(short16, size_t, short *);
void __ovld vstore16(ushort16, size_t, ushort *);
void __ovld vstore16(int16, size_t, int *);
void __ovld vstore16(uint16, size_t, uint *);
void __ovld vstore16(long16, size_t, long *);
void __ovld vstore16(ulong16, size_t, ulong *);
void __ovld vstore16(float16, size_t, float *);
#ifdef cl_khr_fp64
void __ovld vstore2(double2, size_t, double *);
void __ovld vstore3(double3, size_t, double *);
void __ovld vstore4(double4, size_t, double *);
void __ovld vstore8(double8, size_t, double *);
void __ovld vstore16(double16, size_t, double *);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
void __ovld vstore2(half2, size_t, half *);
void __ovld vstore3(half3, size_t, half *);
void __ovld vstore4(half4, size_t, half *);
void __ovld vstore8(half8, size_t, half *);
void __ovld vstore16(half16, size_t, half *);
#endif //cl_khr_fp16
#endif //defined(__opencl_c_generic_address_space)

#if defined(__opencl_c_named_address_space_builtins)
void __ovld vstore2(char2, size_t, __global char *);
void __ovld vstore2(uchar2, size_t, __global uchar *);
void __ovld vstore2(short2, size_t, __global short *);
void __ovld vstore2(ushort2, size_t, __global ushort *);
void __ovld vstore2(int2, size_t, __global int *);
void __ovld vstore2(uint2, size_t, __global uint *);
void __ovld vstore2(long2, size_t, __global long *);
void __ovld vstore2(ulong2, size_t, __global ulong *);
void __ovld vstore2(float2, size_t, __global float *);
void __ovld vstore3(char3, size_t, __global char *);
void __ovld vstore3(uchar3, size_t, __global uchar *);
void __ovld vstore3(short3, size_t, __global short *);
void __ovld vstore3(ushort3, size_t, __global ushort *);
void __ovld vstore3(int3, size_t, __global int *);
void __ovld vstore3(uint3, size_t, __global uint *);
void __ovld vstore3(long3, size_t, __global long *);
void __ovld vstore3(ulong3, size_t, __global ulong *);
void __ovld vstore3(float3, size_t, __global float *);
void __ovld vstore4(char4, size_t, __global char *);
void __ovld vstore4(uchar4, size_t, __global uchar *);
void __ovld vstore4(short4, size_t, __global short *);
void __ovld vstore4(ushort4, size_t, __global ushort *);
void __ovld vstore4(int4, size_t, __global int *);
void __ovld vstore4(uint4, size_t, __global uint *);
void __ovld vstore4(long4, size_t, __global long *);
void __ovld vstore4(ulong4, size_t, __global ulong *);
void __ovld vstore4(float4, size_t, __global float *);
void __ovld vstore8(char8, size_t, __global char *);
void __ovld vstore8(uchar8, size_t, __global uchar *);
void __ovld vstore8(short8, size_t, __global short *);
void __ovld vstore8(ushort8, size_t, __global ushort *);
void __ovld vstore8(int8, size_t, __global int *);
void __ovld vstore8(uint8, size_t, __global uint *);
void __ovld vstore8(long8, size_t, __global long *);
void __ovld vstore8(ulong8, size_t, __global ulong *);
void __ovld vstore8(float8, size_t, __global float *);
void __ovld vstore16(char16, size_t, __global char *);
void __ovld vstore16(uchar16, size_t, __global uchar *);
void __ovld vstore16(short16, size_t, __global short *);
void __ovld vstore16(ushort16, size_t, __global ushort *);
void __ovld vstore16(int16, size_t, __global int *);
void __ovld vstore16(uint16, size_t, __global uint *);
void __ovld vstore16(long16, size_t, __global long *);
void __ovld vstore16(ulong16, size_t, __global ulong *);
void __ovld vstore16(float16, size_t, __global float *);
void __ovld vstore2(char2, size_t, __local char *);
void __ovld vstore2(uchar2, size_t, __local uchar *);
void __ovld vstore2(short2, size_t, __local short *);
void __ovld vstore2(ushort2, size_t, __local ushort *);
void __ovld vstore2(int2, size_t, __local int *);
void __ovld vstore2(uint2, size_t, __local uint *);
void __ovld vstore2(long2, size_t, __local long *);
void __ovld vstore2(ulong2, size_t, __local ulong *);
void __ovld vstore2(float2, size_t, __local float *);
void __ovld vstore3(char3, size_t, __local char *);
void __ovld vstore3(uchar3, size_t, __local uchar *);
void __ovld vstore3(short3, size_t, __local short *);
void __ovld vstore3(ushort3, size_t, __local ushort *);
void __ovld vstore3(int3, size_t, __local int *);
void __ovld vstore3(uint3, size_t, __local uint *);
void __ovld vstore3(long3, size_t, __local long *);
void __ovld vstore3(ulong3, size_t, __local ulong *);
void __ovld vstore3(float3, size_t, __local float *);
void __ovld vstore4(char4, size_t, __local char *);
void __ovld vstore4(uchar4, size_t, __local uchar *);
void __ovld vstore4(short4, size_t, __local short *);
void __ovld vstore4(ushort4, size_t, __local ushort *);
void __ovld vstore4(int4, size_t, __local int *);
void __ovld vstore4(uint4, size_t, __local uint *);
void __ovld vstore4(long4, size_t, __local long *);
void __ovld vstore4(ulong4, size_t, __local ulong *);
void __ovld vstore4(float4, size_t, __local float *);
void __ovld vstore8(char8, size_t, __local char *);
void __ovld vstore8(uchar8, size_t, __local uchar *);
void __ovld vstore8(short8, size_t, __local short *);
void __ovld vstore8(ushort8, size_t, __local ushort *);
void __ovld vstore8(int8, size_t, __local int *);
void __ovld vstore8(uint8, size_t, __local uint *);
void __ovld vstore8(long8, size_t, __local long *);
void __ovld vstore8(ulong8, size_t, __local ulong *);
void __ovld vstore8(float8, size_t, __local float *);
void __ovld vstore16(char16, size_t, __local char *);
void __ovld vstore16(uchar16, size_t, __local uchar *);
void __ovld vstore16(short16, size_t, __local short *);
void __ovld vstore16(ushort16, size_t, __local ushort *);
void __ovld vstore16(int16, size_t, __local int *);
void __ovld vstore16(uint16, size_t, __local uint *);
void __ovld vstore16(long16, size_t, __local long *);
void __ovld vstore16(ulong16, size_t, __local ulong *);
void __ovld vstore16(float16, size_t, __local float *);
void __ovld vstore2(char2, size_t, __private char *);
void __ovld vstore2(uchar2, size_t, __private uchar *);
void __ovld vstore2(short2, size_t, __private short *);
void __ovld vstore2(ushort2, size_t, __private ushort *);
void __ovld vstore2(int2, size_t, __private int *);
void __ovld vstore2(uint2, size_t, __private uint *);
void __ovld vstore2(long2, size_t, __private long *);
void __ovld vstore2(ulong2, size_t, __private ulong *);
void __ovld vstore2(float2, size_t, __private float *);
void __ovld vstore3(char3, size_t, __private char *);
void __ovld vstore3(uchar3, size_t, __private uchar *);
void __ovld vstore3(short3, size_t, __private short *);
void __ovld vstore3(ushort3, size_t, __private ushort *);
void __ovld vstore3(int3, size_t, __private int *);
void __ovld vstore3(uint3, size_t, __private uint *);
void __ovld vstore3(long3, size_t, __private long *);
void __ovld vstore3(ulong3, size_t, __private ulong *);
void __ovld vstore3(float3, size_t, __private float *);
void __ovld vstore4(char4, size_t, __private char *);
void __ovld vstore4(uchar4, size_t, __private uchar *);
void __ovld vstore4(short4, size_t, __private short *);
void __ovld vstore4(ushort4, size_t, __private ushort *);
void __ovld vstore4(int4, size_t, __private int *);
void __ovld vstore4(uint4, size_t, __private uint *);
void __ovld vstore4(long4, size_t, __private long *);
void __ovld vstore4(ulong4, size_t, __private ulong *);
void __ovld vstore4(float4, size_t, __private float *);
void __ovld vstore8(char8, size_t, __private char *);
void __ovld vstore8(uchar8, size_t, __private uchar *);
void __ovld vstore8(short8, size_t, __private short *);
void __ovld vstore8(ushort8, size_t, __private ushort *);
void __ovld vstore8(int8, size_t, __private int *);
void __ovld vstore8(uint8, size_t, __private uint *);
void __ovld vstore8(long8, size_t, __private long *);
void __ovld vstore8(ulong8, size_t, __private ulong *);
void __ovld vstore8(float8, size_t, __private float *);
void __ovld vstore16(char16, size_t, __private char *);
void __ovld vstore16(uchar16, size_t, __private uchar *);
void __ovld vstore16(short16, size_t, __private short *);
void __ovld vstore16(ushort16, size_t, __private ushort *);
void __ovld vstore16(int16, size_t, __private int *);
void __ovld vstore16(uint16, size_t, __private uint *);
void __ovld vstore16(long16, size_t, __private long *);
void __ovld vstore16(ulong16, size_t, __private ulong *);
void __ovld vstore16(float16, size_t, __private float *);
#ifdef cl_khr_fp64
void __ovld vstore2(double2, size_t, __global double *);
void __ovld vstore3(double3, size_t, __global double *);
void __ovld vstore4(double4, size_t, __global double *);
void __ovld vstore8(double8, size_t, __global double *);
void __ovld vstore16(double16, size_t, __global double *);
void __ovld vstore2(double2, size_t, __local double *);
void __ovld vstore3(double3, size_t, __local double *);
void __ovld vstore4(double4, size_t, __local double *);
void __ovld vstore8(double8, size_t, __local double *);
void __ovld vstore16(double16, size_t, __local double *);
void __ovld vstore2(double2, size_t, __private double *);
void __ovld vstore3(double3, size_t, __private double *);
void __ovld vstore4(double4, size_t, __private double *);
void __ovld vstore8(double8, size_t, __private double *);
void __ovld vstore16(double16, size_t, __private double *);
#endif //cl_khr_fp64
#ifdef cl_khr_fp16
void __ovld vstore2(half2, size_t, __global half *);
void __ovld vstore3(half3, size_t, __global half *);
void __ovld vstore4(half4, size_t, __global half *);
void __ovld vstore8(half8, size_t, __global half *);
void __ovld vstore16(half16, size_t, __global half *);
void __ovld vstore2(half2, size_t, __local half *);
void __ovld vstore3(half3, size_t, __local half *);
void __ovld vstore4(half4, size_t, __local half *);
void __ovld vstore8(half8, size_t, __local half *);
void __ovld vstore16(half16, size_t, __local half *);
void __ovld vstore2(half2, size_t, __private half *);
void __ovld vstore3(half3, size_t, __private half *);
void __ovld vstore4(half4, size_t, __private half *);
void __ovld vstore8(half8, size_t, __private half *);
void __ovld vstore16(half16, size_t, __private half *);
#endif //cl_khr_fp16
#endif //defined(__opencl_c_named_address_space_builtins)

/**
 * Read sizeof (half) bytes of data from address
 * (p + offset). The data read is interpreted as a
 * half value. The half value is converted to a
 * float value and the float value is returned.
 * The read address computed as (p + offset)
 * must be 16-bit aligned.
 */
float __ovld __purefn vload_half(size_t, const __constant half *);
#if defined(__opencl_c_generic_address_space)
float __ovld __purefn vload_half(size_t, const half *);
#endif //defined(__opencl_c_generic_address_space)

#if defined(__opencl_c_named_address_space_builtins)
float __ovld __purefn vload_half(size_t, const __global half *);
float __ovld __purefn vload_half(size_t, const __local half *);
float __ovld __purefn vload_half(size_t, const __private half *);
#endif //defined(__opencl_c_named_address_space_builtins)

/**
 * Read sizeof (halfn) bytes of data from address
 * (p + (offset * n)). The data read is interpreted
 * as a halfn value. The halfn value read is
 * converted to a floatn value and the floatn
 * value is returned. The read address computed
 * as (p + (offset * n)) must be 16-bit aligned.
 */
float2 __ovld __purefn vload_half2(size_t, const __constant half *);
float3 __ovld __purefn vload_half3(size_t, const __constant half *);
float4 __ovld __purefn vload_half4(size_t, const __constant half *);
float8 __ovld __purefn vload_half8(size_t, const __constant half *);
float16 __ovld __purefn vload_half16(size_t, const __constant half *);
#if defined(__opencl_c_generic_address_space)
float2 __ovld __purefn vload_half2(size_t, const half *);
float3 __ovld __purefn vload_half3(size_t, const half *);
float4 __ovld __purefn vload_half4(size_t, const half *);
float8 __ovld __purefn vload_half8(size_t, const half *);
float16 __ovld __purefn vload_half16(size_t, const half *);
#endif //defined(__opencl_c_generic_address_space)

#if defined(__opencl_c_named_address_space_builtins)
float2 __ovld __purefn vload_half2(size_t, const __global half *);
float3 __ovld __purefn vload_half3(size_t, const __global half *);
float4 __ovld __purefn vload_half4(size_t, const __global half *);
float8 __ovld __purefn vload_half8(size_t, const __global half *);
float16 __ovld __purefn vload_half16(size_t, const __global half *);
float2 __ovld __purefn vload_half2(size_t, const __local half *);
float3 __ovld __purefn vload_half3(size_t, const __local half *);
float4 __ovld __purefn vload_half4(size_t, const __local half *);
float8 __ovld __purefn vload_half8(size_t, const __local half *);
float16 __ovld __purefn vload_half16(size_t, const __local half *);
float2 __ovld __purefn vload_half2(size_t, const __private half *);
float3 __ovld __purefn vload_half3(size_t, const __private half *);
float4 __ovld __purefn vload_half4(size_t, const __private half *);
float8 __ovld __purefn vload_half8(size_t, const __private half *);
float16 __ovld __purefn vload_half16(size_t, const __private half *);
#endif //defined(__opencl_c_named_address_space_builtins)

/**
 * The float value given by data is first
 * converted to a half value using the appropriate
 * rounding mode. The half value is then written
 * to address computed as (p + offset). The
 * address computed as (p + offset) must be 16-
 * bit aligned.
 * vstore_half use the current rounding mode.
 * The default current rounding mode is round to
 * nearest even.
 */
#if defined(__opencl_c_generic_address_space)
void __ovld vstore_half(float, size_t, half *);
void __ovld vstore_half_rte(float, size_t, half *);
void __ovld vstore_half_rtz(float, size_t, half *);
void __ovld vstore_half_rtp(float, size_t, half *);
void __ovld vstore_half_rtn(float, size_t, half *);
#ifdef cl_khr_fp64
void __ovld vstore_half(double, size_t, half *);
void __ovld vstore_half_rte(double, size_t, half *);
void __ovld vstore_half_rtz(double, size_t, half *);
void __ovld vstore_half_rtp(double, size_t, half *);
void __ovld vstore_half_rtn(double, size_t, half *);
#endif //cl_khr_fp64
#endif //defined(__opencl_c_generic_address_space)

#if defined(__opencl_c_named_address_space_builtins)
void __ovld vstore_half(float, size_t, __global half *);
void __ovld vstore_half_rte(float, size_t, __global half *);
void __ovld vstore_half_rtz(float, size_t, __global half *);
void __ovld vstore_half_rtp(float, size_t, __global half *);
void __ovld vstore_half_rtn(float, size_t, __global half *);
void __ovld vstore_half(float, size_t, __local half *);
void __ovld vstore_half_rte(float, size_t, __local half *);
void __ovld vstore_half_rtz(float, size_t, __local half *);
void __ovld vstore_half_rtp(float, size_t, __local half *);
void __ovld vstore_half_rtn(float, size_t, __local half *);
void __ovld vstore_half(float, size_t, __private half *);
void __ovld vstore_half_rte(float, size_t, __private half *);
void __ovld vstore_half_rtz(float, size_t, __private half *);
void __ovld vstore_half_rtp(float, size_t, __private half *);
void __ovld vstore_half_rtn(float, size_t, __private half *);
#ifdef cl_khr_fp64
void __ovld vstore_half(double, size_t, __global half *);
void __ovld vstore_half_rte(double, size_t, __global half *);
void __ovld vstore_half_rtz(double, size_t, __global half *);
void __ovld vstore_half_rtp(double, size_t, __global half *);
void __ovld vstore_half_rtn(double, size_t, __global half *);
void __ovld vstore_half(double, size_t, __local half *);
void __ovld vstore_half_rte(double, size_t, __local half *);
void __ovld vstore_half_rtz(double, size_t, __local half *);
void __ovld vstore_half_rtp(double, size_t, __local half *);
void __ovld vstore_half_rtn(double, size_t, __local half *);
void __ovld vstore_half(double, size_t, __private half *);
void __ovld vstore_half_rte(double, size_t, __private half *);
void __ovld vstore_half_rtz(double, size_t, __private half *);
void __ovld vstore_half_rtp(double, size_t, __private half *);
void __ovld vstore_half_rtn(double, size_t, __private half *);
#endif //cl_khr_fp64
#endif //defined(__opencl_c_named_address_space_builtins)

/**
 * The floatn value given by data is converted to
 * a halfn value using the appropriate rounding
 * mode. The halfn value is then written to
 * address computed as (p + (offset * n)). The
 * address computed as (p + (offset * n)) must be
 * 16-bit aligned.
 * vstore_halfn uses the current rounding mode.
 * The default current rounding mode is round to
 * nearest even.
 */
#if defined(__opencl_c_generic_address_space)
void __ovld vstore_half2(float2, size_t, half *);
void __ovld vstore_half3(float3, size_t, half *);
void __ovld vstore_half4(float4, size_t, half *);
void __ovld vstore_half8(float8, size_t, half *);
void __ovld vstore_half16(float16, size_t, half *);
void __ovld vstore_half2_rte(float2, size_t, half *);
void __ovld vstore_half3_rte(float3, size_t, half *);
void __ovld vstore_half4_rte(float4, size_t, half *);
void __ovld vstore_half8_rte(float8, size_t, half *);
void __ovld vstore_half16_rte(float16, size_t, half *);
void __ovld vstore_half2_rtz(float2, size_t, half *);
void __ovld vstore_half3_rtz(float3, size_t, half *);
void __ovld vstore_half4_rtz(float4, size_t, half *);
void __ovld vstore_half8_rtz(float8, size_t, half *);
void __ovld vstore_half16_rtz(float16, size_t, half *);
void __ovld vstore_half2_rtp(float2, size_t, half *);
void __ovld vstore_half3_rtp(float3, size_t, half *);
void __ovld vstore_half4_rtp(float4, size_t, half *);
void __ovld vstore_half8_rtp(float8, size_t, half *);
void __ovld vstore_half16_rtp(float16, size_t, half *);
void __ovld vstore_half2_rtn(float2, size_t, half *);
void __ovld vstore_half3_rtn(float3, size_t, half *);
void __ovld vstore_half4_rtn(float4, size_t, half *);
void __ovld vstore_half8_rtn(float8, size_t, half *);
void __ovld vstore_half16_rtn(float16, size_t, half *);
#ifdef cl_khr_fp64
void __ovld vstore_half2(double2, size_t, half *);
void __ovld vstore_half3(double3, size_t, half *);
void __ovld vstore_half4(double4, size_t, half *);
void __ovld vstore_half8(double8, size_t, half *);
void __ovld vstore_half16(double16, size_t, half *);
void __ovld vstore_half2_rte(double2, size_t, half *);
void __ovld vstore_half3_rte(double3, size_t, half *);
void __ovld vstore_half4_rte(double4, size_t, half *);
void __ovld vstore_half8_rte(double8, size_t, half *);
void __ovld vstore_half16_rte(double16, size_t, half *);
void __ovld vstore_half2_rtz(double2, size_t, half *);
void __ovld vstore_half3_rtz(double3, size_t, half *);
void __ovld vstore_half4_rtz(double4, size_t, half *);
void __ovld vstore_half8_rtz(double8, size_t, half *);
void __ovld vstore_half16_rtz(double16, size_t, half *);
void __ovld vstore_half2_rtp(double2, size_t, half *);
void __ovld vstore_half3_rtp(double3, size_t, half *);
void __ovld vstore_half4_rtp(double4, size_t, half *);
void __ovld vstore_half8_rtp(double8, size_t, half *);
void __ovld vstore_half16_rtp(double16, size_t, half *);
void __ovld vstore_half2_rtn(double2, size_t, half *);
void __ovld vstore_half3_rtn(double3, size_t, half *);
void __ovld vstore_half4_rtn(double4, size_t, half *);
void __ovld vstore_half8_rtn(double8, size_t, half *);
void __ovld vstore_half16_rtn(double16, size_t, half *);
#endif //cl_khr_fp64
#endif //defined(__opencl_c_generic_address_space)

#if defined(__opencl_c_named_address_space_builtins)
void __ovld vstore_half2(float2, size_t, __global half *);
void __ovld vstore_half3(float3, size_t, __global half *);
void __ovld vstore_half4(float4, size_t, __global half *);
void __ovld vstore_half8(float8, size_t, __global half *);
void __ovld vstore_half16(float16, size_t, __global half *);
void __ovld vstore_half2_rte(float2, size_t, __global half *);
void __ovld vstore_half3_rte(float3, size_t, __global half *);
void __ovld vstore_half4_rte(float4, size_t, __global half *);
void __ovld vstore_half8_rte(float8, size_t, __global half *);
void __ovld vstore_half16_rte(float16, size_t, __global half *);
void __ovld vstore_half2_rtz(float2, size_t, __global half *);
void __ovld vstore_half3_rtz(float3, size_t, __global half *);
void __ovld vstore_half4_rtz(float4, size_t, __global half *);
void __ovld vstore_half8_rtz(float8, size_t, __global half *);
void __ovld vstore_half16_rtz(float16, size_t, __global half *);
void __ovld vstore_half2_rtp(float2, size_t, __global half *);
void __ovld vstore_half3_rtp(float3, size_t, __global half *);
void __ovld vstore_half4_rtp(float4, size_t, __global half *);
void __ovld vstore_half8_rtp(float8, size_t, __global half *);
void __ovld vstore_half16_rtp(float16, size_t, __global half *);
void __ovld vstore_half2_