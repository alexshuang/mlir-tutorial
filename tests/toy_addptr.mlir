// # RUN: toy-opt %s
func.func @test_toy_addptr(%arg0: !toy.ptr<i32>, %arg1: i32) -> (!toy.ptr<i32>) {
  %1 = toy.addptr %arg0, %arg1 : !toy.ptr<i32>, i32 -> !toy.ptr<i32>
  return %1 : !toy.ptr<i32>
}
