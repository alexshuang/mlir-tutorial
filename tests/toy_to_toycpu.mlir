// # RUN: toy-opt --split-input-file --convert-toy-to-toycpu %s | FileCheck %s

func.func @test_toy_addptr(%arg0: !toy.ptr<i32>, %arg1: i32) -> (!toy.ptr<i32>) {
  // CHECK-NOT: toy.addptr
  // CHECK: arith.addi
  %1 = toy.addptr %arg0, %arg1 : !toy.ptr<i32>, i32
  return %1 : !toy.ptr<i32>
}

// ---

func.func @test_toy_addptr_tensor(%arg0: tensor<2x!toy.ptr<i32>>, %arg1: tensor<2xi32>) -> (tensor<2x!toy.ptr<i32>>) {
  // CHECK-NOT: toy.addptr
  // CHECK: arith.addi
  %1 = toy.addptr %arg0, %arg1 : tensor<2x!toy.ptr<i32>>, tensor<2xi32>
  return %1 : tensor<2x!toy.ptr<i32>>
}

// ---

func.func @convert_constant() -> (tensor<3x!toy.ptr<f32>>) {
  // CHECK-NOT: toy.constant
  // CHECK: arith.constant
  %0 = toy.constant [1., 2., 3.] : tensor<3x!toy.ptr<f32>>
  // CHECK-NOT: !toy.ptr<f32>
  return %0 : tensor<3x!toy.ptr<f32>>
}

// ---

func.func @convert_broadcast() -> (tensor<3x3x!toy.ptr<i32>>) {
  // CHECK-NOT: toy.constant
  %cst = toy.constant [1, 2, 3] : tensor<1x3x!toy.ptr<i32>>
  // CHECK: toy.from_tensor
  %1 = toy.broadcast %cst : tensor<1x3x!toy.ptr<i32>> -> tensor<3x3x!toy.ptr<i32>>
  return %1 : tensor<3x3x!toy.ptr<i32>>
}

// ---

func.func @convert_splat(%arg0: !toy.ptr<i32>) -> (tensor<1x3x!toy.ptr<i32>>) {
  // CHECK: toy.splat
  %0 = toy.splat %arg0 : !toy.ptr<i32> -> tensor<1x3x!toy.ptr<i32>>
  return %0 : tensor<1x3x!toy.ptr<i32>>
}
