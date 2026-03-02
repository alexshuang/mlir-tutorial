// # RUN: toy-opt %s

func.func @test_toy_from_tensor() -> (tensor<3x!toy.ptr<i32>>) {
  %0 = arith.constant dense<[1, 2, 3]> : tensor<3xi32>
  %1 = toy.from_tensor %0 : tensor<3xi32> -> tensor<3x!toy.ptr<i32>>
  return %1 : tensor<3x!toy.ptr<i32>>
}
