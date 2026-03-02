// # RUN: toy-opt %s --split-input-file --canonicalize | FileCheck %s

func.func @test_toy_addptr_fold() -> (tensor<3x!toy.ptr<i32>>) {
  // CHECK: toy.constant [2, 3, 4]
  %0 = toy.constant [1, 2, 3] : tensor<3x!toy.ptr<i32>>
  %cst_1 = arith.constant 1 : i32
  // CHECK-NOT: toy.addptr
  %1 = toy.addptr %0, %cst_1 : tensor<3x!toy.ptr<i32>>, i32 -> tensor<3x!toy.ptr<i32>>
  return %1 : tensor<3x!toy.ptr<i32>>
}

// ---

func.func @test_toy_addptr_fold2() -> (tensor<3x!toy.ptr<i32>>) {
  // CHECK-NOT: toy.constant
  %0 = arith.constant dense<[1, 2, 3]> : tensor<3xi32>
  %1 = toy.from_tensor %0 : tensor<3xi32> -> tensor<3x!toy.ptr<i32>>
  %cst_1 = arith.constant 1 : i32
  // CHECK: toy.addptr
  %2 = toy.addptr %1, %cst_1 : tensor<3x!toy.ptr<i32>>, i32 -> tensor<3x!toy.ptr<i32>>
  return %2 : tensor<3x!toy.ptr<i32>>
}
