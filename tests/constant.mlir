// # RUN: toy-opt %s

func.func @test_toy_constant() -> (tensor<3x!toy.ptr<i32>>) {
  %0 = toy.constant [1, 2, 3] : tensor<3x!toy.ptr<i32>>
  return %0 : tensor<3x!toy.ptr<i32>>
}
