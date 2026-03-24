#ifndef TOY_TO_TOYCPU_CONVERSION
#define TOY_TO_TOYCPU_CONVERSION

include "mlir/Pass/PassBase.td"

def ConvertToyToToyCPU : Pass<"convert-toy-to-toycpu"> {
    let summary = "Lower Toy dialect to strand MLIR dialects";
    let description = [{
        Lower Toy dialect to strand MLIR dialects, e.g. toy.addptr -> arith.addi.
    }];
    let dependentDialects = [
        "mlir::affine::AffineDialect",
        "mlir::func::FuncDialect",
        "mlir::toy::ToyDialect",
        "mlir::arith::ArithDialect",
        "mlir::tensor::TensorDialect",
        "mlir::memref::MemRefDialect"
    ];
}

#endif
