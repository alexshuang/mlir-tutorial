// # RUN: toy-opt %s
func.func @test_toy_ptr_type(%arg0: !toy.ptr<i8>) -> (!toy.ptr<i8>) {
  return %arg0 : !toy.ptr<i8>
}
