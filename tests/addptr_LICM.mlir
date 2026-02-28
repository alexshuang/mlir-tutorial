func.func @test_toy_addptr_LICM(%arg0: !toy.ptr<i32>) -> (!toy.ptr<i32>) {
  %cst_1 = arith.constant 1 : i32
  %sum_0 = toy.addptr %arg0, %cst_1 : !toy.ptr<i32>, i32 -> !toy.ptr<i32>
  %sum = affine.for %i = 0 to 4 iter_args(%sum_iter = %sum_0) -> !toy.ptr<i32> {
    %sum_1 = toy.addptr %sum_0, %cst_1 : !toy.ptr<i32>, i32 -> !toy.ptr<i32>
    affine.yield %sum_0 : !toy.ptr<i32>
  }
  return %sum : !toy.ptr<i32>
}

// # RUN: toy-opt %s --loop-invariant-code-motion | FileCheck %s
// CHECK: toy.addptr
// CHECK: toy.addptr
// CHECK: affine.for
// CHECK-NOT: toy.addptr
