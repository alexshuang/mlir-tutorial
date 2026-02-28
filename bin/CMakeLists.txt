get_property(mlir_libs GLOBAL PROPERTY MLIR_DIALECT_LIBS)
get_property(mlir_conversion_libs GLOBAL PROPERTY MLIR_CONVERSION_LIBS)
get_property(toy_libs GLOBAL PROPERTY TOY_LIBS)

add_llvm_executable(toy-opt toy-opt.cpp)

llvm_update_compile_flags(toy-opt)

target_link_libraries(toy-opt PRIVATE
  ${mlir_libs}
  ${mlir_conversion_libs}
  ${toy_libs}
  MLIRIR
  MLIROptLib
  MLIRPass
  MLIRTransforms
)

mlir_check_all_link_libraries(toy-opt)