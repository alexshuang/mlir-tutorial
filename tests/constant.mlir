// # RUN: toy-opt --split-input-file %s

func.func @test_toy_constant() -> (tensor<3x!toy.ptr<i32>>) {
  %0 = toy.constant [1, 2, 3] : tensor<3x!toy.ptr<i32>>
  return %0 : tensor<3x!toy.ptr<i32>>
}

// ---

func.func @test_toy_constant2() -> (tensor<3x!toy.ptr<f32>>) {
  %0 = toy.constant [1., 2., 3.] : tensor<3x!toy.ptr<f32>>
  return %0 : tensor<3x!toy.ptr<f32>>
}
