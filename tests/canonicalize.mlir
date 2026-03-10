// # RUN: toy-opt --split-input-file %s --canonicalize

func.func @test_toy_canonicalize(%arg0: !toy.ptr<i32>) -> (tensor<3x3x!toy.ptr<i32>>) {
  // CHECK: toy.splat
  %0 = toy.splat %arg0 : !toy.ptr<i32> -> tensor<1x3x!toy.ptr<i32>>
  // CHECK-NOT: toy.broadcast
  %1 = toy.broadcast %0 : tensor<1x3x!toy.ptr<i32>> -> tensor<3x3x!toy.ptr<i32>>
  return %1 : tensor<3x3x!toy.ptr<i32>>
}

// ---

func.func @test_toy_canonicalize2(%arg0: !toy.ptr<i32>) -> (tensor<3x3x3x!toy.ptr<i32>>) {
  // CHECK: toy.splat
  %0 = toy.splat %arg0 : !toy.ptr<i32> -> tensor<1x3x1x!toy.ptr<i32>>
  // CHECK-NOT: toy.broadcast
  %1 = toy.broadcast %0 : tensor<1x3x1x!toy.ptr<i32>> -> tensor<3x3x1x!toy.ptr<i32>>
  %2 = toy.broadcast %1 : tensor<3x3x1x!toy.ptr<i32>> -> tensor<3x3x3x!toy.ptr<i32>>
  return %2 : tensor<3x3x3x!toy.ptr<i32>>
}

// ---

func.func @test_toy_canonicalize3(%arg0: tensor<1x3x1x!toy.ptr<i32>>) -> (tensor<3x3x3x!toy.ptr<i32>>) {
  // CHECK: toy.broadcast
  %1 = toy.broadcast %arg0 : tensor<1x3x1x!toy.ptr<i32>> -> tensor<3x3x1x!toy.ptr<i32>>
  // CHECK-NOT: toy.broadcast
  %2 = toy.broadcast %1 : tensor<3x3x1x!toy.ptr<i32>> -> tensor<3x3x3x!toy.ptr<i32>>
  return %2 : tensor<3x3x3x!toy.ptr<i32>>
}