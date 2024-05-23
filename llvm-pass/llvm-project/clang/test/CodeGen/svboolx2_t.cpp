// NOTE: Assertions have been autogenerated by utils/update_cc_test_checks.py
// RUN: %clang_cc1 -triple aarch64-none-linux-gnu -target-feature +sve -target-feature +bf16 -S -emit-llvm -o - %s | FileCheck %s

// CHECK-LABEL: @_Z3foo10svboolx2_t(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[ARG_ADDR:%.*]] = alloca <vscale x 32 x i1>, align 2
// CHECK-NEXT:    store <vscale x 32 x i1> [[ARG:%.*]], ptr [[ARG_ADDR]], align 2
// CHECK-NEXT:    [[TMP0:%.*]] = load <vscale x 32 x i1>, ptr [[ARG_ADDR]], align 2
// CHECK-NEXT:    ret <vscale x 32 x i1> [[TMP0]]
//
__clang_svboolx2_t foo(__clang_svboolx2_t arg) { return arg; }

__clang_svboolx2_t bar();
// CHECK-LABEL: @_Z4foo2v(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[CALL:%.*]] = call <vscale x 32 x i1> @_Z3barv()
// CHECK-NEXT:    ret <vscale x 32 x i1> [[CALL]]
//
__clang_svboolx2_t foo2() { return bar(); }

__clang_svboolx2_t bar2(__clang_svboolx2_t);
// CHECK-LABEL: @_Z4foo310svboolx2_t(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[ARG_ADDR:%.*]] = alloca <vscale x 32 x i1>, align 2
// CHECK-NEXT:    store <vscale x 32 x i1> [[ARG:%.*]], ptr [[ARG_ADDR]], align 2
// CHECK-NEXT:    [[TMP0:%.*]] = load <vscale x 32 x i1>, ptr [[ARG_ADDR]], align 2
// CHECK-NEXT:    [[CALL:%.*]] = call <vscale x 32 x i1> @_Z4bar210svboolx2_t(<vscale x 32 x i1> [[TMP0]])
// CHECK-NEXT:    ret <vscale x 32 x i1> [[CALL]]
//
__clang_svboolx2_t foo3(__clang_svboolx2_t arg) { return bar2(arg); }

